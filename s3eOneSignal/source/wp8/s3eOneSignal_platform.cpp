/*
 * wp8-specific implementation of the s3eOneSignal extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include <crtdbg.h>

#include "s3eEdk.h"

#define _CRT_PERROR_DEFINED
#include <wrl/client.h>
#include <wrl/module.h>
#undef _CRT_PERROR_DEFINED

#include "s3eOneSignal_internal.h"
#include "s3eOneSignal_internal_wp8.h"

#include "s3eOneSignal_shim.h"
#include "IwDebug.h"

#define S3E_CURRENT_EXT			ONESIGNAL
#define S3E_DEVICE_ONESIGNAL	S3E_EXT_ONESIGNAL_HASH

using namespace s3eOneSignalExtension;

s3eResult s3eOneSignalInit_platform()
{
    if (s3eOneSignalShim::GetInterface() == nullptr)
    {
        IwTrace(ONESIGNAL, ("No IManagedS3Es3eOneSignalAPI interface. Managed part of s3eOneSignal extension failed to register."));
        return S3E_RESULT_ERROR;
    }

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void s3eOneSignalTerminate_platform()
{
    // Add any platform-specific termination code here
}

void OneSignalInitialize_platform(const char* appId, const char* googleProjectNumber, OneSignalNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister)
{
    Platform::String^ appId_string = UTF8ToString(appId);

	// Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, NOTIFICATION_RECEIVED, (s3eCallback)callbackFn, NULL, false);

    s3eOneSignalShim::GetInterface()->OneSignalInitialize_managed(appId_string);
}

void GameThriveInitialize_platform(const char* appId, const char* googleProjectNumber, GameThriveNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister)
{
	Platform::String^ appId_string = UTF8ToString(appId);

	// Register Callback
	EDK_CALLBACK_REG(ONESIGNAL, NOTIFICATION_RECEIVED, (s3eCallback)callbackFn, NULL, false);

	s3eOneSignalShim::GetInterface()->OneSignalInitialize_managed(appId_string);
}

void OneSignalSendTag_platform(const char* key, const char* value)
{
    Platform::String^ key_string = UTF8ToString(key);
    Platform::String^ value_string = UTF8ToString(value);
    s3eOneSignalShim::GetInterface()->OneSignalSendTag_managed(key_string, value_string);
}

void OneSignalGetTags_platform(OneSignalTagsReceivedCallbackFn callbackFn) {

	// Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, TAGS_RECEIVED, (s3eCallback)callbackFn, NULL, false);

    s3eOneSignalShim::GetInterface()->OneSignalGetTags_managed();
}

void GameThriveGetTags_platform(GameThriveTagsReceivedCallbackFn callbackFn) {

	// Register Callback
	EDK_CALLBACK_REG(ONESIGNAL, TAGS_RECEIVED, (s3eCallback)callbackFn, NULL, false);

	s3eOneSignalShim::GetInterface()->OneSignalGetTags_managed();
}

void OneSignalDeleteTag_platform(const char* key)
{
    Platform::String^ key_string = UTF8ToString(key);
    s3eOneSignalShim::GetInterface()->OneSignalDeleteTag_managed(key_string);
}

void OneSignalSendPurchase_platform(const double* amount)
{
    s3eOneSignalShim::GetInterface()->OneSignalSendPurchase_managed(*amount);
}

void OneSignalGetIdsAvailable_platform(OneSignalIdsAvailableCallbackFn callbackFn) {
	// Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, IDS_AVAILABLE, (s3eCallback)callbackFn, NULL, false);

    s3eOneSignalShim::GetInterface()->OneSignalGetIdsAvailable_managed();
}

void GameThriveGetIdsAvailable_platform(GameThriveIdsAvailableCallbackFn callbackFn) {
	// Register Callback
	EDK_CALLBACK_REG(ONESIGNAL, IDS_AVAILABLE, (s3eCallback)callbackFn, NULL, false);

	s3eOneSignalShim::GetInterface()->GameThriveGetIdsAvailable_managed();
}

// Windows Phone Native SDK does this on its own
void OneSignalOnPause_platform() {
}
void OneSignalOnResume_platform() {
}

// Windows Phone Native SDK always registers for push
void OneSignalRegisterForPushNotifications_platform() {
}

void OneSignalEnableVibrate_platform(s3eBool enable) {
	// For Android Only
}

void OneSignalEnableSound_platform(s3eBool enable) {
	// For Android Only
}


// The following 3 functions are called from the shim which was called from the .NET CLR
void NotificationReceivedCallback(const char* additionalData, bool isActive) {

	OneSignalNotificationReceivedResult result;
	result.m_Message = NULL;
	result.m_AdditionalData = additionalData;
	result.m_isActive = isActive;

	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
							S3E_ONESIGNAL_CALLBACK_NOTIFICATION_RECEIVED,
							&result,
							sizeof(result),
							NULL,
							S3E_FALSE); // FALSE, not a one shot callback.
}
void TagsReceivedCallback(const char* tags) {
	OneSignalTagsReceivedResult result;
	result.m_Tags = tags;
	
	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
							S3E_ONESIGNAL_CALLBACK_TAGS_RECEIVED,
							&result,
							sizeof(result),
							NULL,
							S3E_TRUE); // TRUE, a one shot callback.
}

void IdsAvailableCallback(const char* userID, const char* pushToken) {
	OneSignalIdsAvailableResult result;
	result.m_UserID = userID;
	result.m_PushToken = pushToken;
	
	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
							S3E_ONESIGNAL_CALLBACK_IDS_AVAILABLE,
							&result,
							sizeof(result),
							NULL,
							S3E_FALSE); // FALSE, not a one shot callback.
}

void IdsAvailableCallback_GameThrive(const char* playerID, const char* pushToken) {
	GameThriveIdsAvailableResult result;
	result.m_PlayerID = playerID;
	result.m_PushToken = pushToken;

	s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
		S3E_GAMETHRIVE_CALLBACK_IDS_AVAILABLE,
		&result,
		sizeof(result),
		NULL,
		S3E_FALSE); // FALSE, not a one shot callback.
}