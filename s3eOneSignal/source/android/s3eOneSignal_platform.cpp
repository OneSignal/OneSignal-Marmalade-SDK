/*
 * android-specific implementation of the s3eOneSignal extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eOneSignal_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

#define S3E_CURRENT_EXT			ONESIGNAL
#define S3E_DEVICE_ONESIGNAL	S3E_EXT_ONESIGNAL_HASH

static void NotificationReceivedCallback(JNIEnv* env, jobject _this, jstring m_Message, jstring m_AdditionalData, bool m_isActive);
static void TagsReceivedCallback(JNIEnv* env, jobject _this, jstring m_Tags);
static void IdsAvailableCallback(JNIEnv* env, jobject _this, jstring m_PlayerID, jstring m_PushToken);

static jobject g_Obj;
static jmethodID g_OneSignalInitialize;
static jmethodID g_OneSignalSendTag;
static jmethodID g_OneSignalGetTags;
static jmethodID g_OneSignalDeleteTag;
static jmethodID g_OneSignalGetIdsAvailable;
static jmethodID g_GameThriveGetIdsAvailable;
static jmethodID g_OneSignalOnPause;
static jmethodID g_OneSignalOnResume;
static jmethodID g_OneSignalEnableVibrate;
static jmethodID g_OneSignalEnableSound;

s3eResult s3eOneSignalInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;
	
	const JNINativeMethod nativeMethodDefs[] =
	{
        {"NotificationReceivedCallback", "(Ljava/lang/String;Ljava/lang/String;Z)V", (void *)&NotificationReceivedCallback},
		{"TagsReceivedCallback", "(Ljava/lang/String;)V", (void *)&TagsReceivedCallback},
		{"IdsAvailableCallback", "(Ljava/lang/String;Ljava/lang/String;)V", (void *)&IdsAvailableCallback},
        {"IdsAvailableCallback_GameThrive", "(Ljava/lang/String;Ljava/lang/String;)V", (void *)&IdsAvailableCallback},
	};

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("s3eOneSignal");
    if (!cls)
        goto fail;
	
	env->RegisterNatives(cls, nativeMethodDefs, sizeof(nativeMethodDefs)/sizeof(nativeMethodDefs[0]));

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_OneSignalInitialize = env->GetMethodID(cls, "OneSignalInitialize", "(Ljava/lang/String;Ljava/lang/String;Z)V");
    if (!g_OneSignalInitialize)
        goto fail;

    g_OneSignalSendTag = env->GetMethodID(cls, "OneSignalSendTag", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_OneSignalSendTag)
        goto fail;

    g_OneSignalGetTags = env->GetMethodID(cls, "OneSignalGetTags", "()V");
    if (!g_OneSignalGetTags)
        goto fail;

    g_OneSignalDeleteTag = env->GetMethodID(cls, "OneSignalDeleteTag", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_OneSignalDeleteTag)
        goto fail;

    g_OneSignalGetIdsAvailable = env->GetMethodID(cls, "OneSignalGetIdsAvailable", "()V");
    if (!g_OneSignalGetIdsAvailable)
        goto fail;

    g_GameThriveGetIdsAvailable = env->GetMethodID(cls, "GameThriveGetIdsAvailable", "()V");
    if (!g_GameThriveGetIdsAvailable)
        goto fail;
		
	g_OneSignalOnPause = env->GetMethodID(cls, "OneSignalOnPause", "()V");
    if (!g_OneSignalOnPause)
        goto fail;
	
	g_OneSignalOnResume = env->GetMethodID(cls, "OneSignalOnResume", "()V");
    if (!g_OneSignalOnResume)
        goto fail;

    g_OneSignalEnableVibrate = env->GetMethodID(cls, "OneSignalEnableVibrate", "(Z)V");
    if (!g_OneSignalEnableVibrate)
        goto fail;

    g_OneSignalEnableSound = env->GetMethodID(cls, "OneSignalEnableSound", "(Z)V");
    if (!g_OneSignalEnableSound)
        goto fail;

    IwTrace(ONESIGNAL, ("ONESIGNAL init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(s3eOneSignal, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void s3eOneSignalTerminate_platform()
{
    // Add any platform-specific termination code here
}

void OneSignalInitialize_platform(const char* appId, const char* googleProjectNumber, OneSignalNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring appId_jstr = env->NewStringUTF(appId);
    jstring googleProjectNumber_jstr = env->NewStringUTF(googleProjectNumber);
    
    // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, NOTIFICATION_RECEIVED, (s3eCallback)callbackFn, NULL, false);
    
    env->CallVoidMethod(g_Obj, g_OneSignalInitialize, appId_jstr, googleProjectNumber_jstr, autoRegister);
}

void GameThriveInitialize_platform(const char* appId, const char* googleProjectNumber, GameThriveNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring appId_jstr = env->NewStringUTF(appId);
    jstring googleProjectNumber_jstr = env->NewStringUTF(googleProjectNumber);
	
	// Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, NOTIFICATION_RECEIVED, (s3eCallback)callbackFn, NULL, false);
	
    env->CallVoidMethod(g_Obj, g_OneSignalInitialize, appId_jstr, googleProjectNumber_jstr, autoRegister);
}

void NotificationReceivedCallback(JNIEnv* env, jobject _this, jstring m_Message, jstring m_AdditionalData, bool m_isActive) {
	const char* nativeMessageStr = NULL;
	const char* nativeAdditionalDataStr = NULL;
	
	if (m_Message != NULL)
		nativeMessageStr = env->GetStringUTFChars(m_Message, 0);
	if (m_AdditionalData != NULL)
		nativeAdditionalDataStr = env->GetStringUTFChars(m_AdditionalData, 0);
	
	OneSignalNotificationReceivedResult result;
	result.m_Message = nativeMessageStr;
	result.m_AdditionalData = nativeAdditionalDataStr;
	result.m_isActive = m_isActive;
	
	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
						   S3E_ONESIGNAL_CALLBACK_NOTIFICATION_RECEIVED,
						   &result,
						   sizeof(result),
						   NULL,
						   S3E_FALSE); // FALSE, not a one shot callback.
	
	// TODO: Fix very very small memory leak(1 KiBs or less).
	// 		 The 2 lines below fixes the leak however then the const char* pointer is invalid to the callback receiver.
	//env->ReleaseStringUTFChars(m_Message, nativeMessageStr);
	//env->ReleaseStringUTFChars(m_AdditionalData, nativeAdditionalDataStr);
}

void TagsReceivedCallback(JNIEnv* env, jobject _this, jstring m_Tags) {
	const char* nativeTagsStr = env->GetStringUTFChars(m_Tags, 0);
	
	OneSignalTagsReceivedResult result;
	result.m_Tags = nativeTagsStr;
	
	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
							S3E_ONESIGNAL_CALLBACK_TAGS_RECEIVED,
							&result,
							sizeof(result),
							NULL,
							S3E_TRUE); // TRUE, a one shot callback.
	
	// TODO: Fix very very small memory leak(1 KiBs or less).
	// 		 The line below fixes the leak however then the const char* pointer is invalid to the callback receiver.
	//env->ReleaseStringUTFChars(m_Tags, nativeTagsStr);
}

void IdsAvailableCallback(JNIEnv* env, jobject _this, jstring m_UserID, jstring m_PushToken) {
	const char* nativeUserIDStr = env->GetStringUTFChars(m_UserID, 0);
	const char* nativePushTokenStr = env->GetStringUTFChars(m_PushToken, 0);
	
	OneSignalIdsAvailableResult result;
	result.m_UserID = nativeUserIDStr;
	result.m_PushToken = nativePushTokenStr;
	
	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
							S3E_ONESIGNAL_CALLBACK_IDS_AVAILABLE,
							&result,
							sizeof(result),
							NULL,
							S3E_FALSE); // FALSE, not a one shot callback.
	
	// TODO: Fix very very small memory leak(1 KiBs or less).
	// 		 The 2 lines below fixes the leak however then the const char* pointer is invalid to the callback receiver.
	//env->ReleaseStringUTFChars(m_UserID, nativeUserIDStr);
	//env->ReleaseStringUTFChars(m_PushToken, nativePushTokenStr);
}

void IdsAvailableCallback_GameThrive(JNIEnv* env, jobject _this, jstring m_PlayerID, jstring m_PushToken) {
    const char* nativePlayerIDStr = env->GetStringUTFChars(m_PlayerID, 0);
    const char* nativePushTokenStr = env->GetStringUTFChars(m_PushToken, 0);
    
    GameThriveIdsAvailableResult result;
    result.m_PlayerID = nativePlayerIDStr;
    result.m_PushToken = nativePushTokenStr;
    
    s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
                            S3E_GAMETHRIVE_CALLBACK_IDS_AVAILABLE,
                            &result,
                            sizeof(result),
                            NULL,
                            S3E_FALSE); // FALSE, not a one shot callback.
    
    // TODO: Fix very very small memory leak(1 KiBs or less).
    //       The 2 lines below fixes the leak however then the const char* pointer is invalid to the callback receiver.
    //env->ReleaseStringUTFChars(m_PlayerID, nativePlayerIDStr);
    //env->ReleaseStringUTFChars(m_PushToken, nativePushTokenStr);
}

void OneSignalSendTag_platform(const char* key, const char* value)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring key_jstr = env->NewStringUTF(key);
    jstring value_jstr = env->NewStringUTF(value);
	
    env->CallVoidMethod(g_Obj, g_OneSignalSendTag, key_jstr, value_jstr);
}

void OneSignalGetTags_platform(OneSignalTagsReceivedCallbackFn callbackFn)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
	
	// Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, TAGS_RECEIVED, (s3eCallback)callbackFn, NULL, false);
	
    env->CallVoidMethod(g_Obj, g_OneSignalGetTags);
}

void GameThriveGetTags_platform(GameThriveTagsReceivedCallbackFn callbackFn)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    
    // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, TAGS_RECEIVED, (s3eCallback)callbackFn, NULL, false);
    
    env->CallVoidMethod(g_Obj, g_OneSignalGetTags);
}

void OneSignalDeleteTag_platform(const char* key)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring key_jstr = env->NewStringUTF(key);
    env->CallVoidMethod(g_Obj, g_OneSignalDeleteTag, key_jstr);
}

void OneSignalSendPurchase_platform(const double* amount)
{
   // Deperecated, automaticly track purchases
}

void OneSignalGetIdsAvailable_platform(OneSignalIdsAvailableCallbackFn callbackFn)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
	
		// Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, IDS_AVAILABLE, (s3eCallback)callbackFn, NULL, false);
	
    env->CallVoidMethod(g_Obj, g_OneSignalGetIdsAvailable);
}

void GameThriveGetIdsAvailable_platform(GameThriveIdsAvailableCallbackFn callbackFn)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    
        // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, IDS_AVAILABLE, (s3eCallback)callbackFn, NULL, false);
    
    env->CallVoidMethod(g_Obj, g_GameThriveGetIdsAvailable);
}


void OneSignalOnPause_platform() {
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_OneSignalOnPause);
}

void OneSignalOnResume_platform() {
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_OneSignalOnResume);
}

void OneSignalRegisterForPushNotifications_platform() {
	// On Android we always auto register for push notifications.
}

void OneSignalEnableVibrate_platform(s3eBool enable) {
	JNIEnv* env = s3eEdkJNIGetEnv();
    
    if (enable == S3E_FALSE)
    	env->CallVoidMethod(g_Obj, g_OneSignalEnableVibrate, JNI_FALSE);
    else
    	env->CallVoidMethod(g_Obj, g_OneSignalEnableVibrate, JNI_TRUE);
}

void OneSignalEnableSound_platform(s3eBool enable) {
	JNIEnv* env = s3eEdkJNIGetEnv();
    
    if (enable == S3E_FALSE)
    	env->CallVoidMethod(g_Obj, g_OneSignalEnableSound, JNI_FALSE);
    else
    	env->CallVoidMethod(g_Obj, g_OneSignalEnableSound, JNI_TRUE);
}