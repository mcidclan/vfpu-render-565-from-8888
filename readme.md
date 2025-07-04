## Code Sample VFPU Simple Rendering

This sample demonstrates the use of the VFPU to render pixels directly to the screen.
8 colors are loaded into the VFPU from Allegrex registers into 2 columns of 4 elements each.
These columns are then swizzled upward, and their lowest values are swapped.
Finally, both columns are converted to the RGB565 color format and written directly to VRAM by the VFPU itself.

*m-c/d*
