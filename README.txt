This was a work in progress Hex editor. I've since stopped working on it
but think it's a vaulable learning experience. It was intended as an 
editor designed to write bytes directly to a file, but when I saw
that it could inspect its own RAM, I became distracted and that's the state
I left it in.

Ordinary procedure. 
run "make" from this directory.
The executable will be called "0xedit"
use "./0xedit" to run.

The controls are as follows:
ctl+Q to quit.
The editor begins in Hexadecimal mode.  
Use the arrow keys to navigate the cursor.
the "," and "." keys set the view 1 unit backwards and forwards.

The cursor will wrap if it leaves either end of a line.
If the cursor leaves the top or bottom of the screen,
the view will be scrolled to accomodate.

In hexadecimal mode you can only type 0123456789ABCDEF.

You can change mode with ctl+[A,B,F,H,O]
A - ASCII (Not implemented)
B - Binary 
F - Quaternary
O - Octal (Not implemented)
H - Hexadecimal

In each mode you can only type their respective characters.

ctl+D Toggles the debug menu which shows additional info about what's going on.

ctl+S Toggles an incomplete "Save" mode.

shift+[left, right arrow keys] increases or decreases the
number of groups per line.

shift+[upn down arrow keys] increase or decrease the number of bytes per group
Though this feeature seems to no longer work on newer versions of OSX.
