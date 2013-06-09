easymodeling
============

Modeling tool for Box2D

##Preview

Edit shape

![editshape](../../../homepage/blob/gh-pages/emodeling/editshape.jpg?raw=true)

Edit fixture

![editfixture](../../../homepage/blob/gh-pages/emodeling/editfixture.jpg?raw=true)

Edit joint

![editjoint](../../../homepage/blob/gh-pages/emodeling/editjoint.jpg?raw=true)

Output to codes

![codes](../../../homepage/blob/gh-pages/emodeling/love2d.jpg?raw=true)

##About
EasyModeling was inspired by [R.U.B.E Box2d editor](https://www.iforce2d.net/rube/).

This editor used to create Box2D's body, fixture, joint and adjust their attributes. 

##Sources

[easymodeling](https://github.com/xzrunner/easymodeling)


[libdrag2d](https://github.com/xzrunner/drag2d)	[libshape](https://github.com/xzrunner/easyshape)

##Binaries
[easymodeling.exe](platform/msvc2008/easymodeling/x86/Release/easymodeling.exe?raw=true)

maybe you need these dll files: [freetype6.dll](http://cn.dll-files.com/freetype6.dll.html) [zlib1.dll](http://cn.dll-files.com/zlib1.dll.html)

##How to use

###1. Load images

Press left panel's "+" button to add images.

![load images](../../../homepage/blob/gh-pages/emodeling/ht1.jpg?raw=true)

###2. Edit shape

Double click library's image will save the boundary file on disk, so no need to edit again when loading the same image next time.

Also you can drag the image to stage, double click to edit it. This will saved at the final file.

![edit shape](../../../homepage/blob/gh-pages/emodeling/ht2.jpg?raw=true)

###3. Adjust the position and angle of actors in stage panel

Mouse left drag to move and mouse right drag to rotate.

![Adjust](../../../homepage/blob/gh-pages/emodeling/ht3.jpg?raw=true)

###4. Set attributes though left-low panel

To set b2World, b2Body, b2Fixturem and b2Joint's value.

![attributes](../../../homepage/blob/gh-pages/emodeling/ht4.jpg?raw=true)

###5. Edit joint

Select 2 bodies and press ok to create joint

![edit joint](../../../homepage/blob/gh-pages/emodeling/ht5.jpg?raw=true)

###6. Press *Ctrl + Enter* to run it at preview panel

![preview](../../../homepage/blob/gh-pages/emodeling/ht6.jpg?raw=true)

###7. Select menu *Codes* output the modeling to codes

![preview](../../../homepage/blob/gh-pages/emodeling/codes.jpg?raw=true)
