#include "jni/JniHelper.h"
#include <string.h>
#include <android/log.h>
#include <jni.h>

#include "CCBluetooth.h"

#define LOG_TAG "Java_org_cocos2dx_lib_bluetooth_BluetoothManager"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define JAVAVM    cocos2d::JniHelper::getJavaVM()

#define  CLASS_NAME "org/cocos2dx/lib/bluetooth/BluetoothManager"

using namespace std;
using namespace cocos2d;

void *_delegate;

extern "C" {

	void setDelegateJni(void *delegate)
	{
		_delegate = delegate;
	}

	void startJni(const char *peerID, const char *message)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "start", "(Ljava/lang/String;Ljava/lang/String;)V")) {
			jstring arg1 = t.env->NewStringUTF(peerID);
			jstring arg2 = t.env->NewStringUTF(message);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, arg1, arg2);
			t.env->DeleteLocalRef(arg1);
			t.env->DeleteLocalRef(arg2);
		}
	}

	void stopJni()
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stop", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
	}

    // from BluetoothManager to CCBluetoothDelegate
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_bluetooth_BluetoothManager_nativeCalledFromBluetoothManager(JNIEnv *env, jobject thiz, jint result, jint status, jstring error, jstring peerID, jstring message) {
        if(_delegate) {
            const char *arg1 = env->GetStringUTFChars(error, NULL); 
            const char *arg2 = env->GetStringUTFChars(peerID, NULL); 
            const char *arg3 = env->GetStringUTFChars(message, NULL); 

            bluetooth_plugin::CCBluetoothDelegate *bluetoorhDelegate = (bluetooth_plugin::CCBluetoothDelegate*)_delegate;
            bluetoorhDelegate->onResult(result, status, arg1, arg2, arg3);
        }
    }
}

