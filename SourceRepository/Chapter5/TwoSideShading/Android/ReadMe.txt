=== NDK Sample application ===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	OpenGLES first application, OpenGLES for iOS

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


Two side faces shading recipe.

== Description ==
This technique avoids rendering of polygon faces which are facing to the backside, it’s not always desirable to clip the back faces, objects which are not completely enclosed are generally rendered with back faces. Sometimes it makes sense to view those back faces with different colors, this will help the geometry shape to define characteristic which might not be visible with same color on both sides of the faces (back and front).

== Installation ==
Open the project by double clicking the .xcodeproj file.

== Changelog ==
1. NDK sample application implemented - 8/2/2014