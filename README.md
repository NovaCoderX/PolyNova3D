# PolyNova3D
Software based 3D Engine (legacy upload for Amiga development)


Overview:
This is not a game, it is just a simple tech demo to validate OpenGL 
performance on 68K (PiStorm) as a viable option for simple games.
A very long time ago (30 years!), I was into 3D coding and started with 
Qbasic on DOS building 'Dungeon Master' style engines with primitive lighting.
I then build a series of my own 3D engines based on a DOS 3D engine (AST 3D)
using C++ with realtime lighting, texture mapping and animation.  
I had some of my old source code sitting around on floppy discs so I thought
I'd try and port a version of PolyNova3D to 68K before it was lost forever.
I choose a very basic version of PolyNova3D to port because I knew how
demanding it would be to run it without any hardware acceleration.
As PolyNova3D is all about the realtime lighting, I needed to target at
least 16bit color depth and also needed something to render the triangles.
PolyNova3D used to run on Windows and use DirectX for triangle rendering 
so for this 68K port I decided to port an updated version of 
Mesa OpenGL and then add it to SDL (which is what we are using here).
PolyNova3D is a pure software 3D engine and because this port of Mesa lacks
any hardware acceleration, this is a pure software implementation of 3D.
The idea behind PolyNova3D was that it could be used as the foundation for 
building your own simple 3D demos (a basic 3D construction kit).
If you are interested about how it all works, examine the scene file
using a text editor.

Requirements:
1) PiStorm/WinUAE.
2) 4MB of RAM.

Configuration:
You do not need to install any OpenGL, Mesa, Warp or SDL libraries.
Have a look at the polynova.ini for various settings, also check the ToolTypes
for further options.
You can change the colors of the lights etc by editing the scene file using a
text editor.  Be warned that there isn't much in the way of error checking so
you will just crash out if you break something in the scene file.
The log file will output the average FPS on exit.

W = Forward
S = Backwards
A = Strafe left
D = Strafe right
F1,F2,F3,F4 = Toggle the ceiling lights
Escape = Quit

Links:
https://eab.abime.net/showthread.php?p=1702670#post1702670 (PolyNova3D thread on EAB)
