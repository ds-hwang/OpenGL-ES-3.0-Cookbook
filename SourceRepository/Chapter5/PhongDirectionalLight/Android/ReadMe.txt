=== Gouraud Shading ===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	Gouraud shading model, phong reflection model, per fragment shading technique

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


Direction light with Gouraud shading.

== Description ==
The directional light is a special case of the Point Light. Here, the direction of the light falling on the object is considered as non-varying. That means, the direction of all the light rays are parallel. In directional light, the light source is considered infinitely far from the model on which it is supposed to fall. Sometimes it is better to assume the light direction to be parallel during 3D scene rendering process, it is an optimal way to achieve nearly the same effect as Point Light if the distance between source point and model is appreciably larger.


== Installation ==
Importing sample source in Android platform:-
1. Go to New-> Project -> Android Project from Existing Code. Click Next
2. Specify the path of the project. Click Finish
Note: The path of the project should be the directory name which contains Android.xml.

== Changelog ==
1. Gouraud shading implemenation - 14/3/2014