Labyrinth-3D
============

Labyrinth 3D is the traditional [Labyrinth game](http://en.wikipedia.org/wiki/Labyrinth_(game) implemented in C++ (OpenGL 3.3+)

**Computer Graphics course project -- Fall 2013**

<iframe class="youtube-player" type="text/html" width="640" height="360" src="http://www.youtube.com/embed/ruv4CX0pg1E?autoplay=1" frameborder="0"></iframe>



check out the video

[![Game Video](http://img.youtube.com/vi/ruv4CX0pg1E/0.jpg)](http://www.youtube.com/watch?v=ruv4CX0pg1E)


Levels
-------
The game offers 6 different levels with increasing difficulty, more levels can be added.

you can build your levels by replacing one of the textfiles with the extension .map (i.e. map0.map). The first line of the map file contains 2 space separated integers, the first integer it the map width and the second is the map height measured in numbers of tiles.

+ `'#'` wall tile
+ `'.'` empty tile
+ `'S'` ball start position
+ `'T'` taget hole
+ `'O'` hole'



Game Control 
------------
###Camera control 
+ `Up`   : raise camera vertically away from the board
+ `Down` : move camera vertically towards the board
+ `'A'`  : move the camera to the left 
+ `'D'`  : move the camera to the right
+ `'W'`  : move the camera forward
+ `'S'`  : move the camera backward

###Board control
tilt the board by dragging the mouse by any angle.

[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/leonardo7/labyrinth-3d/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
