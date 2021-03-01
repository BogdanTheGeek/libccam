# libccam
A library that turns C code into gcode.

Inspired by [OpenSCAD](https://www.openscad.org/) and [Tool Path Language](https://tplang.org/).

## Motivation
I want to machine stuff on my CNC mill and lathe without having to use clunky UIs or spend time learing well all the buttons are in a "free as in beer" program, just for that program to move the machining features behind a paywall (im looking at you Fision 720).

The available open source CAM packages are bloated with UI code which makes it nearly impossible for everyone but the core developers to understand.

### Why C?
C is one of the oldest languages still around, yet it is one of the simplest to learn from a syntax point of view. There are only 32 keywords that you have to remember, most of which are not used 99% of the time.

Most importantly, writing toolpaths in a real programming language means that you can use all the tricks you use for data with shapes, like `for` loops for patterns, conditionals, functions that draw custom shapes etc.

Writing everything in C also means that it is easy to port to other platforms or use the code for plugins.

Finally, I love C!

`But, Pointers???? - you ask`

Here is you 5 minute explanation of pointers (don't worry you will not need pointers to use libccam):

Varibles are put somewhere in the memory of a computer when they are defined like this `int a = 5;`. A pointer just holds the address of that variable so that you can use the value inside without copying it, which would be slow. It's really that simple. Where most people strugle and get frustrated with pointers is when they forget when and where to put the `*`, `&`, `.` or `->`. Don't worry about it, just look it up online when you need it, most of the time the compiler will tell you if you used the wrong one and which one would be correct. 


## Project Goals
- Easy way to generate gcode without a bloated UI.
- Be able to do most machining opperations that you would want
- Help you learn gcode.
- Easy to understand code. 
- Learn some cool C.

## Features
 - [X] Traveling at rapid feedrate (G0)
 - [X] Moving at curring feedrate (G1)
 - [X] Setting rapid/feed speed
 - [X] Relative and Absolute moves (G90/G91)
 - [X] Arcs and circles (G2/G3)
 - [X] Pause, Stop and Hold the program with `pause()`, `stop()` and `wait()`
 - [X] Profile cuts with aray of points/arcs with `profile()`
 - [X] Facing at any angle with `face()`
 - [X] Boring with `bore()`
 - [X] Drilling and pecking (G83) with `drill()`, `peck()` and `advanced_peck()`
 - [ ] Pockets
 - [ ] Adaptive tool paths (AKA high efficiency tool paths)
 - [ ] Tool change
 - [ ] [grbl](https://github.com/grbl/grbl) support
 - [ ] Lathe support

## Instal
Just download the repo to your computer or copy `libccam.*`, `main.c` and `generate` to your project.

Make sure you have GCC installed.

Run `./generate` in that folder to convert `main.c` to gcode (output saved in `out.nc`).

### Optional
Install [entr](http://eradman.com/entrproject/) and run:
```
ls | entr -c ./generate
```
to have the file automatically recompiled every time main.c (or any file for that matter) is changed.

Use `Ctrl+C` to quit.

Install [CAMotics](https://camotics.org/) and open `out.nc` as a new project in order to simulate your gcode.
Pressing the refresh icon will reload the gcode file.
