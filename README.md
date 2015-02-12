=============
IMPRESSIONIST
=============

### AUTHOR
----------

Doug Ilijev (dci89)
doug.ilijev@utexas.edu


### CREDITS
-----------

CS 384G - Graduate Computer Graphics
Don Fussell

Submitted: 9/25/2013


=============
BUILD AND RUN
=============

To build, one of the following:

    make
    make all
    make impressionist

To run:

    ./impressionist


========
FEATURES
========

First I will describe the required features, and then the bells and whistles I added.


----------------
--- REQUIRED ---
----------------

Description of required features implemented is below.


### Brushes
-----------

All brushes respond to Alpha setting - 1.00 is opaque, 0.00 is fully transparent.

Point
    controlled by:
        Size

Line
    controlled by:
        Line Width
        Line Length
        Line Angle - 0 degrees is horizontal

Scattered Brushes (Point, Line, Circle)
    scatter controlled by:
        Scatter Density - How many brush strokes should be taken for each click or move of the brush.
        Scatter Radius - How widely spread the brush strokes should be from the center of the click.
            The center of each stroke will be within the specified radius.


### Setting the Length and Angle
--------------------------------

You can use the sliders to set the length and angle of the line brush and scattered line brush.
You can also right click and drag on the painting to set the length and angle.
    A red line will be drawn to show the vector the settings will represent.
You can use the drop-down to select the ability to choose brush direction from the cursor direction.
    This will cause brush strokes made by the line brushes to follow the direction of the cursor.

    
### Filter Kernel
-----------------

You can use the filter kernel dialog to apply a kernel to either the source image or the painting.
    Select the source from the drop down.
    Fields are provided for the divisor and offset.
    Kernel will be centered around the middle pixel.

You can preview the result of applying the convolution before applying it,
or cancel it to restore the image to what it was before.



--------------------------
--- BELLS AND WHISTLES ---
--------------------------


### Guides
----------

A cursor is displayed on the source image to show you where you are taking color from in the painting.
The cursor is a 5x5 square of pixels which are the inverse of the color on the source image.

When you right click and drag to make a vector for length and angle of the line brush, the drag vector
is displayed on the painting in red.


### Brushes
-----------

Enabled line antialiasing to make the line brush appear better.

Triangle Brush
    Draws equilateral triangles oriented according to the set angle.
    controlled by
        Size - the distance of the triangle's vertices from the target point
        Angle - the direction of one of the points of the triangle.

AutoPaint
    I attempted to implement an auto-paint function from the brush dialog.
    I couldn't seem to get it to draw on the painting canvas, but when you click the button,
    the autopainted image shows up in the original picture.
    If you move the mouse over the image, things reset.

