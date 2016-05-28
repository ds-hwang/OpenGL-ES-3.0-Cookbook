#ifndef _Included_com_cookbook_JNIActivity
#define _Included_com_cookbook_JNIActivity
#include <jni.h>

JNIEXPORT jstring JNICALL Java_com_cookbookgles_NativeDevActivity_getMessage
  (JNIEnv *, jobject);

#endif
