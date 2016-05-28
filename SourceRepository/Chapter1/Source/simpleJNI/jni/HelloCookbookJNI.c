#include "include/HelloCookbookJNI.h"
 
JNIEXPORT jstring JNICALL Java_com_cookbookgles_NativeDevActivity_getMessage
          (JNIEnv *env, jobject thisObj) {
   return (*env)->NewStringUTF(env, "Hello from Cookbook native code.");
}