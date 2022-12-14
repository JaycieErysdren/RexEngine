# Rex Engine

A multi-purpose multi-target 2D and 3D game engine, written in C and C++.

The engine is made up of modules that aid in interfacing with various libraries and hardware that vary for various platform targets.

At the moment it's primarily focused towards DOS, using software rendering for the 2D and 3D aspects. A Linux version is possible to compile, but the graphics modules are still using 8-bit color so it won't work properly on any modern system. Windows support is pending.

## Media

![A screenshot of a 3D game engine demonstrating raycaster rendering.](./.github/rayc00.png)
![A screenshot of a 3D game engine demonstrating portal-sector rendering.](./.github/sector00.png)
![A screenshot of a 3D game engine demonstrating voxels.](./.github/voxel00.png)
![A screenshot of a 3D game engine demonstrating voxels.](./.github/voxel01.png)

## Building

Can be easily built with the bash and batch scripts in the main directory of this repository.

## External Dependencies

[SDL2](https://www.libsdl.org/)

[DJGPP](https://www.delorie.com/djgpp/)


The following thirdparty packages are already included in this repository:


[mempoolite](https://github.com/jefyt/mempoolite/)

[cJSON](https://github.com/DaveGamble/cJSON)

## Special Thanks

[Kaitai Struct](http://kaitai.io/)

## Contact

[Discord](https://discord.gg/5MwE3xMcdN)

## License

ANTI-CAPITALIST SOFTWARE LICENSE (v 1.4)

Copyright © 2022-2023 Erysdren Media

This is anti-capitalist software, released for free use by individuals
and organizations that do not operate by capitalist principles.

Permission is hereby granted, free of charge, to any person or
organization (the "User") obtaining a copy of this software and
associated documentation files (the "Software"), to use, copy, modify,
merge, distribute, and/or sell copies of the Software, subject to the
following conditions:

  1. The above copyright notice and this permission notice shall be
  included in all copies or modified versions of the Software.

  2. The User is one of the following:
    a. An individual person, laboring for themselves
    b. A non-profit organization
    c. An educational institution
    d. An organization that seeks shared profit for all of its members,
    and allows non-members to set the cost of their labor

  3. If the User is an organization with owners, then all owners are
  workers and all workers are owners with equal equity and/or equal vote.

  4. If the User is an organization, then the User is not law enforcement
  or military, or working for or under either.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT EXPRESS OR IMPLIED WARRANTY OF
ANY KIND, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
