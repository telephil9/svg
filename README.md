svg
====
A native SVG file viewer for Plan9.  
When using the -9 command line flag, the image is dumped to standard output in plan9 image format.

![svg](screenshot.png)

svg relies on the nanosvg library: (https://github.com/memononen/nanosvg).  
math functions are imported from OpenBSD libm: (http://openbsd.org)

Usage:
------
Install with the usual ``mk install``  
Run ``svg [-9] file.svg``

License:
-----------
- svg: MIT license
- nanosvg: zlib license
- libposix: BSD license
