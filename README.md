
TTY-CLOCK & TTY-TIMER
=====================

Terminal-based clock and timer utilities with large ASCII digit output.
Built with ncurses. Minimal. Fast. Zero fluff.

------------------------------------------------------------
TTY-CLOCK
------------------------------------------------------------

Usage:
```
    tty-clock [options]
```
Options:
```
    -s            Show seconds
    -S            Screensaver mode
    -x            Draw a box around the clock
    -c            Center in terminal
    -C [0-7]      Set color (0=black .. 7=white)
    -b            Use bold/bright color
    -t            Use 12-hour format (AM/PM)
    -u            Use UTC time instead of local
    -f format     Custom date format (strftime)
    -D            Hide date
    -B            Blink colon
    -r            Rebound/bouncing mode
    -T tty        Display on a specific TTY (e.g. /dev/tty1)
    -n            Do not quit on keypress
    -d delay      Delay between updates in seconds (default: 1)
    -a nsdelay    Additional delay in nanoseconds (default: 0)
    -v            Version info
    -i            Program info
    -h            Help
```
Key Bindings:
```
    q            Quit
    s            Toggle seconds
    t            Toggle 12/24-hour
    c            Toggle center mode
    x            Toggle box
    b            Toggle bold
    r            Toggle rebound
    0–7          Change color
    Arrows       Move clock (if not centered)
```
Examples:
```
    tty-clock -s -c -C 2
    tty-clock -S -n -r -C 4 -b
```
------------------------------------------------------------
TTY-TIMER
------------------------------------------------------------

Usage:
   ```
    tty-timer [options]
```
Options:
   ```
    -s            Show seconds
    -x            Draw a box
    -c            Center in terminal
    -C [0-7]      Set color
    -b            Use bold/bright color
    -D            Hide date
    -B            Blink colon
    -d delay      Delay in seconds (default: 1)
    -a nsdelay    Additional nanosecond delay (default: 0)
    -v            Version info
    -i            Program info
    -h            Help
```
Key Bindings:
 ```
    q            Quit
    r            Reset timer to 00:00:00
    s            Toggle seconds
    c            Toggle center mode
    x            Toggle box
    b            Toggle bold
    0–7          Change color
    Arrows       Move timer (if not centered)
```
Examples:

``` 
    tty-timer -c -b -C 3
    tty-timer -x -C 1 -s
```
------------------------------------------------------------
REQUIREMENTS
------------------------------------------------------------

- A POSIX-compatible terminal (Linux, BSD, macOS)
- ncurses or ncursesw development headers
- Minimum terminal size: 35x7 characters
- ANSI color support recommended

Install deps (example for Debian/Ubuntu):
```
    sudo apt install libncurses5-dev
```
Compile:
 ```
    make tty-clock
    make tty-timer
    sudo make install
```