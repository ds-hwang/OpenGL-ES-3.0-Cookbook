=== Uniform blocks/Interface blocks===
Contributors: 		Parminder Singh  
Author:     		Parminder Singh
Email:           	parminder_ce@yahoo.co.in
Tags:            	uniform,glGetUniformBlockIndex ,GL_UNIFORM_BUFFER , glGetActiveUniformBlockiv, glUniformBlockBinding,glBindBufferBase , glMapBufferRange  
performance in opengles

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


Introduction to Rendering Primitives in OpenGLES.

== Description ==
The interface block helps in grouping the uniform variables into one logical bunch. This is very useful in grouping the related variables in the shader programing. The interface block gives an opportunity to share the uniform data among multiple programs at once.  This allows setting multiple uniform variables in one go, which can be used many times.


== Installation ==
Importing sample source in Android platform:-
1. Go to New-> Project -> Android Project from Existing Code. Click Next
2. Specify the path of the project. Click Finish
Note: The path of the project should be the directory name which contains Android.xml.


== Changelog ==
