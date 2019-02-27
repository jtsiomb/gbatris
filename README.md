GBAtris
=======

![shot](http://nuclear.mutantstargoat.com/sw/gbatris/img/gbatris2-thumb.jpg)

About
-----
gbatris is a classic-style tetris game for the Nintendo GameBoy Advance.

I always liked the original GameBoy tetris, and not any of the "improved"
versions that where released for the GBA and the Nintendo DS. So I made gbatris
to be as close as possible to the original GameBoy tetris.

  - website: http://nuclear.mutantstargoat.com/sw/gbatris
  - source repository: https://github.com/jtsiomb/gbatris

gbatris is based on, and shares some code with, my previous `termtris` project,
which is a tetris for ANSI/VT220-compatible terminals on UNIX systems:
http://nuclear.mutantstargoat.com/sw/termtris

Controls
--------
  - *D-pad left/right* moves the block horizontally.
  - *D-pad down* makes the block fall faster.
  - *B* button, or *D-pad up* drops the block immediately.
  - *A* button rotates the block.
  - *START* pauses the game.
  - *SELECT* starts a new game.
  - pressing either trigger button toggles music on/off.

License
-------
Copyright (C) 2019 John Tsiombikas <nuclear@member.fsf.org>

This program is free software. Feel free to use, modify, and/or redistribute it
under the terms of the GNU General Public License version 3, or at your option,
any later version published by the Free Software Foundation. See COPYING for
details.

The background music is a short loop from Greig Stewart's Korobeinki
composition and performance on a Laser Harp.
Copyright (C) 2012 Greig Stewart <greig@thereminhero.com>
Get the full version and support the artist on bandcamp:
https://thereminhero.bandcamp.com/album/lasers-theremin-nes-gameboy

Download
--------
Current release of gbatris is v1.0, and is available as a gziped tarball or a
zip archive. Both archives are identical, and contain source code, data files,
and a pre-compiled GameBoy Advance ROM image:
  - http://nuclear.mutantstargoat.com/sw/gbatris/gbatris-1.0.tar.gz
  - http://nuclear.mutantstargoat.com/sw/gbatris/gbatris-1.0.zip

Alternatively you may clone the latest version of the source code, directly from
the git repository:

    git clone https://github.com/jtsiomb/gbatris

Build
-----
To build gbatris you need the devkitarm GBA toolchain from: https://devkitpro.org

No libraries are used other than the GCC C runtime support libraries; just the C
compiler, binutils, and link scripts. So if you already have a freestanding ARM
cross-compiler (target `arm-none-eabi`), you can probably just grab the
appropriate link script, and spec file from the devkitarm archives, and use them
to build gbatris.

Additionally, the `img2tiles` tool, which is built as part of the gbatris build
process, depends on *libpng* and *zlib*.
