=== Specular Light shading using point light sample application ===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	OpenGLES 2.0, OpenGLES 3.0, Light shading, Point Light, Specular light, per vertex light shading technique.

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


Specular light per vertex light shading technique application on Android.

== Description ==
This sample application demonstrate Specular light shading with per vertex light shading technique.
Specular light is responsible for generating the shininess on the surface of an object. Unlike diffuse light which uses the incident ray and surface normal to find the intensity of light, specular light uses reflected ray and the direction from the viewer to find the intensity of light.
Note: This sample is implemented using point lighting.

== Installation ==
Importing sample source in Android platform:-
1. Go to New-> Project -> Android Project from Existing Code. Click Next
2. Specify the path of the project. Click Finish
Note: The path of the project should be the directory name which contains Android.xml.

== Changelog ==
1. Implementaton of Specular shading with point light. - 8/2/2014