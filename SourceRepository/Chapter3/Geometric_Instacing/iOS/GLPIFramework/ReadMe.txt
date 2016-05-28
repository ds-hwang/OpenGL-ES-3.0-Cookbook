=== Geometry Instancing ===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	glDrawElementsInstanced, glVertexAttribDivisor , VAO, VBO
Requires at least: 	Xcode 6 or greater, Mac running OS X version 10.9.3 or later, 
			OS X version 10.9 and iOS 8.
Tested on: 		Xcode 6.0 and greater versions.



Introduction to Geometry Instancing in OpenGLES.

== Description ==
The geometry instancing allows rendering multiple instances of a same object in a single rendering API call. These multiple instances differ in their generic attributes such as transformation matrices, color, scale, etc. This feature is very useful for implementing particle systems, crowd simulation, rendering of jungle trees, etc. As compared to the traditional way of rendering multiple objects in which multiple rendering API’s are called, this technique is very efficient.  This reduces the overhead of CPU intervention for making several rendering calls to the OpenGL ES rendering engine. 


== Installation ==
Open the project by double clicking the .xcodeproj file.

== Changelog ==
