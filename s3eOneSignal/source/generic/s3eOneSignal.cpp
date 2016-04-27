/*
Generic implementation of the s3eOneSignal extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "s3eOneSignal_internal.h"
#include "s3eDevice.h"
#include "IwDebug.h"

int GameThriveSystemPaused(void* systemData, void* userData) {
	OneSignalOnPause_platform();
	return S3E_RESULT_SUCCESS;
}

int OneSignalSystemPaused(void* systemData, void* userData) {
	OneSignalOnPause_platform();
	return S3E_RESULT_SUCCESS;
}

int GameThriveSystemResume(void* systemData, void* userData) {
	OneSignalOnResume_platform();
	return S3E_RESULT_SUCCESS;
}

int OneSignalSystemResume(void* systemData, void* userData) {
	OneSignalOnResume_platform();
	return S3E_RESULT_SUCCESS;
}

s3eResult s3eOneSignalInit()
{
    //Add any generic initialisation code here
    return s3eOneSignalInit_platform();
}

void s3eOneSignalTerminate()
{
    //Add any generic termination code here
    s3eOneSignalTerminate_platform();
}

void GameThriveInitialize(const char* appId, const char* googleProjectNumber, GameThriveNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister)
{
	GameThriveInitialize_platform(appId, googleProjectNumber, callbackFn, autoRegister);
}

void OneSignalInitialize(const char* appId, const char* googleProjectNumber, OneSignalNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister)
{
	OneSignalInitialize_platform(appId, googleProjectNumber, callbackFn, autoRegister);
}

void GameThriveSendTag(const char* key, const char* value)
{
	OneSignalSendTag_platform(key, value);
}

void OneSignalSendTag(const char* key, const char* value)
{
	OneSignalSendTag_platform(key, value);
}

void GameThriveGetTags(GameThriveTagsReceivedCallbackFn callbackFn)
{
	GameThriveGetTags_platform(callbackFn);
}

void OneSignalGetTags(OneSignalTagsReceivedCallbackFn callbackFn)
{
	OneSignalGetTags_platform(callbackFn);
}

void GameThriveDeleteTag(const char* key)
{
	OneSignalDeleteTag_platform(key);
}

void OneSignalDeleteTag(const char* key)
{
	OneSignalDeleteTag_platform(key);
}

void GameThriveSendPurchase(const double* amount)
{
	OneSignalSendPurchase_platform(amount);
}

void OneSignalSendPurchase(const double* amount)
{
	OneSignalSendPurchase_platform(amount);
}

void GameThriveGetIdsAvailable(GameThriveIdsAvailableCallbackFn callbackFn)
{
	GameThriveGetIdsAvailable_platform(callbackFn);
}

void OneSignalGetIdsAvailable(OneSignalIdsAvailableCallbackFn callbackFn)
{
	OneSignalGetIdsAvailable_platform(callbackFn);
}

void GameThriveRegisterForPushNotifications() {
	OneSignalRegisterForPushNotifications_platform();
}

void OneSignalRegisterForPushNotifications() {
	OneSignalRegisterForPushNotifications_platform();
}

void GameThriveEnableVibrate(s3eBool enable) {
	OneSignalEnableVibrate_platform(enable);
}

void OneSignalEnableVibrate(s3eBool enable) {
	OneSignalEnableVibrate_platform(enable);
}

void GameThriveEnableSound(s3eBool enable) {
	OneSignalEnableSound_platform(enable);
}

void OneSignalEnableSound(s3eBool enable) {
	OneSignalEnableSound_platform(enable);
}

void OneSignalEnableInAppAlertNotification(s3eBool enable) {
	OneSignalEnableInAppAlertNotification_platform(enable);
}

void OneSignalEnableNotificationsWhenActive(s3eBool enable) {
	OneSignalEnableNotificationsWhenActive_platform(enable);
}

void OneSignalSetSubscription(s3eBool enable) {
	OneSignalSetSubscription_platform(enable);
}

void OneSignalPostNotification(const char* jsonStr)
{
	OneSignalPostNotification_platform(jsonStr);
}

void OneSignalPostNotificationWithCallback(const char* jsonStr, OneSignalPostNotificationCallbackFn callbackSuccessFn, OneSignalPostNotificationCallbackFn callbackFailureFn)
{
	OneSignalPostNotificationWithCallback_platform(jsonStr, callbackSuccessFn, callbackFailureFn);
}
