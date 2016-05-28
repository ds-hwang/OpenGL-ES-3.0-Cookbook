=== GLPI Framework ===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	glPushMatix, glPopMatrix, Push matrix, pop matrix

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


Use of Push and Pop matrix in OpenGLES.

== Description ==
There is a stack of matrices for each of the matrix modes. In GL_MODELVIEW mode, the stack depth is at least 8. In the other modes, GL_COLOR, GL_PROJECTION, and GL_TEXTURE, the depth is at least 2. The current matrix in any mode is the matrix on the top of the stack for that mode.

PushMatrix pushes the current matrix stack down by one, duplicating the current matrix. That is, after a glPushMatrix call, the matrix on top of the stack is identical to the one below it.

PopMatrix pops the current matrix stack, replacing the current matrix with the one below it on the stack.

Initially, each of the stacks contains one matrix, an identity matrix.

== Installation ==
Importing sample source in Android platform:-
1. Go to New-> Project -> Android Project from Existing Code. Click Next
2. Specify the path of the project. Click Finish
Note: The path of the project should be the directory name which contains Android.xml.


== Changelog ==
1. Sample application for Push and pop matrix. - 19/2/2014