package com.cookbookgles;

import android.os.Bundle;
import android.widget.TextView;
import android.app.Activity;

public class NativeDevActivity extends Activity {

   static {
     //Comment #1
     // "jniNativeDev.dll" in Windows.
     // "jniNativeDev.so" in Unixes
     System.loadLibrary("jniNativeDev");
   }
 
   //Comment #2
   // Native method that returns a Java String
   // to be displayed on the TextView
   public native String getMessage();
 
   @Override
   public void onCreate(Bundle savedInstanceState) {
     super.onCreate(savedInstanceState);
      
     //Comment #3
     // Create a TextView widget.
     TextView textView = new TextView(this);
      
     //Comment #4
     // Retrieve the text from native method
     // getMessage() and set as text to be displayed.
     textView.setText(getMessage());
     setContentView(textView);
   }   
}
