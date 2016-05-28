###########################################

Sample code Chapter 1

###########################################

#### REQUIREMENTS #######
1. JDK: http://www.oracle.com/technetwork/java/javase/downloads/index.html
2. Eclipse: http://www.eclipse.org/
3. Android ADT plugin & SDK for Eclipse: http://developer.android.com/sdk/index.html
4. Android NDK: http://developer.android.com/tools/sdk/ndk/index.html
5. Cygwin: http://www.cygwin.com/install.html [For Windows user only]


#### REQUIREMENTS #######
Please refer to Chapter Appendix for "Software Requirement for OpenGLES 3.0 - Android"

#### IMPORTING PROJECT #######

You can import project in to existing Ecllipse following way:-

1.New-> Project -> Android Project from Existing Code. Click next
2.Specify the path of the project. Click Finish
  Note: The path of project should be the directory name which contains ‘Android.xml’
  
  
###### RUNNING THE EXAMPLES #########

1. Open Cygwin command line terminal. 
   Go to current folder path and execute ‘ndk-build’. 
   This command with the help of ‘Android.mk’ compile the files and generate the shared 
   library called ‘JNINativeDev.so’ in ‘CookbookNativeDev\libs\armeabi’ folder path.
2. Compile and execute the project in Eclispe. If you are facing any troubles during course of compilation,
   try clean and build the project. You can run the output on AVD or attaching Android device.