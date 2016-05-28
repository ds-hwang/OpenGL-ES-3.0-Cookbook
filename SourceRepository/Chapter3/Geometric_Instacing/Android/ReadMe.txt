=== Geometry Instancing ===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	glDrawElementsInstanced, glVertexAttribDivisor , VAO, VBO

==Software Requirements==
1. Java Development Kit (JDK): JDK 7u25
url:	http://www.oracle.com/technetwork/java/javase/downloads/index.html

2. Eclipse: Eclispe Standard 4.3.1
url:	http://www.eclipse.org/

3. ADT plugin & SDK for Eclipse: 
url:	http://developer.android.com/sdk/index.html

4. Android NDK: 
url:	http://developer.android.com/tools/sdk/ndk/index.html

5. Cygwin: 
url:	http://www.cygwin.com/install.html , Only Windows users are required to download Cygwin.


Introduction to Geometry Instancing in OpenGLES.

== Description ==
The geometry instancing allows rendering multiple instances of a same object in a single rendering API call. These multiple instances differ in their generic attributes such as transformation matrices, color, scale, etc. This feature is very useful for implementing particle systems, crowd simulation, rendering of jungle trees, etc. As compared to the traditional way of rendering multiple objects in which multiple rendering API’s are called, this technique is very efficient.  This reduces the overhead of CPU intervention for making several rendering calls to the OpenGL ES rendering engine. 

== Installation ==
Importing sample source in Android platform:-
1. Go to New-> Project -> Android Project from Existing Code. Click Next
2. Specify the path of the project. Click Finish
Note: The path of the project should be the directory name which contains Android.xml.


== Changelog ==
