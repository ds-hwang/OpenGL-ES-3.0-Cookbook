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


Point light using Gouraud shading.

== Description ==The Point light comes from a fixed position in the 3D space. The position of the light and vertices of the object on which it is falling is used to calculate the direction of the light. Each vertex can have different direction of light depending upon its position from the light source as shown in the below image.

== Installation ==
Importing sample source in Android platform:-
1. Go to New-> Project -> Android Project from Existing Code. Click Next
2. Specify the path of the project. Click Finish
Note: The path of the project should be the directory name which contains Android.xml.

== Changelog ==
1. Gouraud shading implemenation - 14/3/2014