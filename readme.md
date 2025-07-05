## VFPU Simple Rendering Code Sample

This sample shows how to use the VFPU to render pixels directly to the screen.

An array of 8×4 RGBA colors is loaded into VFPU registers using Allegrex general-purpose loads.
The 8 columns (4 values each) are then swizzled upward (`Y→X, Z→Y, W→Z, X→W`).
After swizzling, the lowest values of each column are rotated into the next one, creating a vertical loop across all columns.

Once transformed, the colors are converted from 8888 format to RGB565 using VFPU instructions, and finally written directly to VRAM.


*m-c/d*
