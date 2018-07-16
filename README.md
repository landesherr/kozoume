# kozoume
A GameBoy emulator that aims to be barely functional

## Basic info
kozoume is developed in C99, with graphical rendering (and sound, someday!) using SDL2.

The primary target is Unix-like platforms, though kozoume runs swell on Windows via Cygwin.

## Current status
* CPU emulation is solid. Blarggs `cpu_instrs` passes 100%.
* Commerical games can be booted, and seem to run just fine. MBC1/2/3/5 work for saving, but no RTC support yet.
* Small graphical glitches are still common in some titles.
* Input is still provided only via hard-coded keyboard mappings, but expect that to change soon.
* Audio is still unimplemented.

## Compiling
Currently, the Makefile leaves much to be desired. If you have SDL2 installed to somewhere other than `/usr/local/lib/`, you'll have to hand-edit the Makefile to reflect this.

That aside, it's pretty simple - run `make` in the `src` folder, assuming you have `gcc` or some other modern compiler aliased to `gcc`.

## The philosophy of kozoume
kozoume is not intended to work well, it is merely intended to work

The main principle that drives the development of this emulator is "Do what seems fun and convenient at the time, best practices be damned."
