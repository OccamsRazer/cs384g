===========
Ray Tracing
===========

Jared Croy (jac5659)
jared.croy@gmail.com

Submitted: 3/5/2015
I will be using one grace day for this assignment

=============
BUILD AND RUN
=============

To build

    make

To run:

    ./bin/ray


========
FEATURES
========

----------------
--- REQUIRED ---
----------------

1. Fill in the triangle intersection code so that your ray tracer can display triangle meshes.
2. Implemented the Whitted illumination model
3. Implemented Phong interpolation of normals on triangle meshes.
4. Implemented regular super-sampling anti-aliasing. A slider in the GUI
   allows you to chose between 1, 4, 9, and 16 samples. The command line will alow for more.
5. Threaded ray tracing code. Defaults to the max possible on the machine.
6. Implemented cube mapping as a user-selectable option. The default for any scene is no cube mapping.
7. Implemented an oct-tree data structures to accelerate intersection calculations.


NOTES
    There is a weird artifact while rendering with multiple threads. Basically
    it stops displaying the last dozen or so rows of pixels. To force a 
    refresh enable the debug window and click within the rendered  image. The
    missing lines will magically appear.

--------------------------
--- BELLS AND WHISTLES ---
--------------------------

Texture Mapping
    Texture mapping seems to be working for the cube and square example scenes.

