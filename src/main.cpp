#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include <windows.h>
#include <processenv.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int AlreadyRuns(LPCWSTR className);
int RegisterShortcuts(HWND hwnd);
UINT GetKey(std::string key);

std::map<int, std::string> hotkeys{};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#ifndef NDEBUG
    if (AllocConsole())
    {
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    }
#endif
    std::cout << "Test" << std::endl;

    const wchar_t CLASS_NAME[]  = L"NoShortcutterWindow";

    CreateMutex(NULL, TRUE, L"ShortcutterRunning");
    switch (GetLastError())
    {
        case ERROR_SUCCESS:
            break;
        case ERROR_ALREADY_EXISTS:
            return AlreadyRuns(CLASS_NAME);
        default:
            return 0;
    }

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowExW(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Shortcutter",                 // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_HIDE);

    if (RegisterShortcuts(hwnd))
        return 0;

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    case WM_HOTKEY:
    {
        std::cout << wParam << ": " << hotkeys[wParam] << std::endl;

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        CreateProcessA(NULL, (char *)hotkeys[wParam].c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        return 0;
    }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int AlreadyRuns(LPCWSTR className)
{
    auto other = FindWindowW(className, L"Shortcutter");
    auto args = GetCommandLineW();
    int argc;
    auto argv = CommandLineToArgvW(args, &argc);

    if (!argv[1])
        return 0;
    if (wcscmp(argv[1], L"stop") == 0)
        SendMessageW(other, WM_DESTROY, 0, 0);
    return 0;
}

int RegisterShortcuts(HWND hwnd)
{
    char appData[256];
    GetEnvironmentVariableA("AppData", appData, 256);
    std::cout << appData << std::endl;
    std::string dir(appData);
    dir += "/Shortcutter";

    CreateDirectoryA(dir.c_str(), NULL);
    if (GetLastError() == ERROR_PATH_NOT_FOUND)
    {
        std::cout << "No Directory" << std::endl;
        return 1;
    }

    dir += "/Shortcuts.txt";
    std::fstream file;
    file.open(dir, std::ios::in);
    if (!file.is_open())
    {
        std::cout << "No File" << std::endl;
        return 1;
    }

    std::string line;
    for (int i = 1; std::getline(file, line); ++i)
    {
        if (line[0] == '#')
            continue;
        auto ind = line.find(' ');
        auto shortcut = line.substr(0, ind);
        auto command = line.substr(ind + 1);
        UINT mod = MOD_NOREPEAT;
        UINT k = 0;
        while (shortcut.length() > 0)
        {
            ind = shortcut.find('+');
            std::string key = shortcut.substr(0, ind);
            shortcut = shortcut.substr(ind + 1);

            if (key == "alt")
                mod |= MOD_ALT;
            else if (key == "ctrl")
                mod |= MOD_CONTROL;
            else if (key == "shift")
                mod |= MOD_SHIFT;
            else if (key == "win")
                mod |= MOD_WIN;
            else
            {
                k = GetKey(key);
                break;
            }
        }
        RegisterHotKey(hwnd, i, mod, k);
        hotkeys[i] = command;
    }
    return 0;
}

UINT GetKey(std::string key)
{
    if (key.length() == 1)
    {
        if (isalpha(key[0]))
            return tolower(key[0]) - 'A' + 0x41;
        if (isdigit(key[0]))
            return key[0] - '0' + 0x30;
    }
    if (key == "lmb")
        return VK_LBUTTON;
    if (key == "rmb")
        return VK_RBUTTON;
    if (key == "mmb")
        return VK_MBUTTON;
    if (key == "m1")
        return VK_XBUTTON1;
    if (key == "m2")
        return VK_XBUTTON2;
    if (key == "back")
        return VK_BACK;
    if (key == "tab")
        return VK_TAB;
    if (key == "enter")
        return VK_RETURN;
    if (key == "esc")
        return VK_ESCAPE;
    if (key == "space")
        return VK_SPACE;
    if (key == "pgup")
        return VK_PRIOR;
    if (key == "pgdown")
        return VK_NEXT;
    if (key == "end")
        return VK_END;
    if (key == "home")
        return VK_HOME;
    if (key == "left")
        return VK_LEFT;
    if (key == "up")
        return VK_UP;
    if (key == "right")
        return VK_RIGHT;
    if (key == "down")
        return VK_DOWN;
    if (key == "ins")
        return VK_INSERT;
    if (key == "del")
        return VK_DELETE;
    if (key == "num0")
        return VK_NUMPAD0;
    if (key == "num1")
        return VK_NUMPAD1;
    if (key == "num2")
        return VK_NUMPAD2;
    if (key == "num3")
        return VK_NUMPAD3;
    if (key == "num4")
        return VK_NUMPAD4;
    if (key == "num5")
        return VK_NUMPAD5;
    if (key == "num6")
        return VK_NUMPAD6;
    if (key == "num7")
        return VK_NUMPAD7;
    if (key == "num8")
        return VK_NUMPAD8;
    if (key == "num9")
        return VK_NUMPAD9;
    if (key == "mul")
        return VK_MULTIPLY;
    if (key == "add")
        return VK_ADD;
    if (key == "sub")
        return VK_SUBTRACT;
    if (key == "dec")
        return VK_DECIMAL;
    if (key == "div")
        return VK_DIVIDE;
    if (key == "f1")
        return VK_F1;
    if (key == "f2")
        return VK_F2;
    if (key == "f3")
        return VK_F3;
    if (key == "f4")
        return VK_F4;
    if (key == "f5")
        return VK_F5;
    if (key == "f6")
        return VK_F6;
    if (key == "f7")
        return VK_F7;
    if (key == "f8")
        return VK_F8;
    if (key == "f9")
        return VK_F9;
    if (key == "f10")
        return VK_F10;
    if (key == "f11")
        return VK_F11;
    if (key == "f12")
        return VK_F12;
    if (key == "scrl")
        return VK_SCROLL;
    return 0;
}