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


NDK Sample application on Android.

== Description ==
This sample application demonstrate the use of Android NDK in Java applications. NDK is used for development of application in c/c++, it uses JNI for interfacing Android java framework application. 

In this example we will experiment with how to use C/C++ code in Android Java application. 
Broadly there are two module of the app 
a. Java based Mobile Application interface – this will use Android api to create a TextView control on the mobile screen.

b. C/C++ base Native code library – here we will use native code where we export a function/method in a library(.so  which is similar to .dll), this exported function will be used by the Mobile Application interface to display the contents that is returned by this function into mobile screen’s TextView control.


== Installation ==
Open the project by double clicking the .xcodeproj file.

== Changelog ==
1. NDK sample application implemented - 8/2/2014