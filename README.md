# Shortcutter
Create global shortcuts in Windows.

## How to use

### First time setup
- Create **the file** `%AppData%/Shortcutter/Shortcuts.txt`
- In this file you can specify your shortcuts

### Adding shortcuts
- Each line (excluding comments) in **the file** specifies a shortcut
- Each shortcut consists of modifiers separated by `+` (no spaces), `+`, single key, ` ` (space) and command
- Lines starting with `#` are ignored
- When you add shortcut you must restart the program so that it takes effect

#### Example
```
# Toggle audio output device (powershell script run via vbscript (beacause of a bug where the terminal window appears))
ctrl+alt+num0 C:\Windows\System32\wscript.exe C:\VBScripts\hidden-ps.vbs X:\PSScripts\AudioSwap.ps1
```
Only the second line will be registered.

### Stopping program
- Use `stop` as the first argument
```shell
> shortcutter stop
```

## List of modifiers
- alt
- ctrl
- shift
- win

## List of keys
- a
- b
- c
- d
- e
- f
- g
- h
- i
- j
- k
- l
- m
- n
- o
- p
- q
- r
- s
- t
- u
- v
- w
- x
- y
- z
- 0
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- lmb (left mouse button)
- rmb (right mouse button)
- mmb (middle mouse button)
- m1 (mouse button 1)
- m2 (mouse button 2)
- back (backspace)
- tab
- enter
- esc
- space
- pgup
- pgdown
- end
- home
- left
- up
- right
- down
- ins
- del
- num0
- num1
- num2
- num3
- num4
- num5
- num6
- num7
- num8
- num9
- mul (*)
- add (+)
- sub (-)
- dec (the decimal delimiter on numpad)
- div (/)
- f1
- f2
- f3
- f4
- f5
- f6
- f7
- f8
- f9
- f10
- f11
- f12
- scrl (scroll lock)

(not all of the keys have been tested, some may not work (mouse))
