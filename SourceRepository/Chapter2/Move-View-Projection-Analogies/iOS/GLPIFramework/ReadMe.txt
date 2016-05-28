=== Managing tranformation using Push/Pop matrix operation in OpenGLES===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	glPushMatrix, glPopMatrix, Push Matrix, Pop Matrix
Requires at least: 	Xcode 6 or greater, Mac running OS X version 10.9.3 or later, 
			OS X version 10.9 and iOS 8.
Tested on: 		Xcode 6.0 and greater versions.



Use of Push and Pop matrix in OpenGLES.

== Description ==
There is a stack of matrices for each of the matrix modes. In GL_MODELVIEW mode, the stack depth is at least 8. In the other modes, GL_COLOR, GL_PROJECTION, and GL_TEXTURE, the depth is at least 2. The current matrix in any mode is the matrix on the top of the stack for that mode.

PushMatrix pushes the current matrix stack down by one, duplicating the current matrix. That is, after a glPushMatrix call, the matrix on top of the stack is identical to the one below it.

PopMatrix pops the current matrix stack, replacing the current matrix with the one below it on the stack.

Initially, each of the stacks contains one matrix, an identity matrix.


== Installation ==
Open the project by double clicking the .xcodeproj file.

== Changelog ==
1. Sample application for Push and pop matrix. - 19/2/2014