/*
 * Internal header for the s3eOneSignal extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef S3EONESIGNAL_INTERNAL_H
#define S3EONESIGNAL_INTERNAL_H

#include "s3eTypes.h"
#include "s3eOneSignal.h"
#include "s3eOneSignal_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult s3eOneSignalInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult s3eOneSignalInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void s3eOneSignalTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void s3eOneSignalTerminate_platform();
void GameThriveInitialize_platform(const char* appId, const char* googleProjectNumber, GameThriveNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister);
void OneSignalInitialize_platform(const char* appId, const char* googleProjectNumber, OneSignalNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister);

void OneSignalSendTag_platform(const char* key, const char* value);
void GameThriveGetTags_platform(GameThriveTagsReceivedCallbackFn callbackFn);
void OneSignalGetTags_platform(OneSignalTagsReceivedCallbackFn callbackFn);
void OneSignalDeleteTag_platform(const char* key);

void OneSignalSendPurchase_platform(const double* amount);

void GameThriveGetIdsAvailable_platform(GameThriveIdsAvailableCallbackFn callbackFn);
void OneSignalGetIdsAvailable_platform(OneSignalIdsAvailableCallbackFn callbackFn);

int OneSignalSystemPaused_platform(void* systemData, void* userData);
int OneSignalSystemResume_platform(void* systemData, void* userData);

void OneSignalOnPause_platform();
void OneSignalOnResume_platform();

void OneSignalRegisterForPushNotifications_platform();

void OneSignalEnableVibrate_platform(s3eBool enable);
void OneSignalEnableSound_platform(s3eBool enable);

void OneSignalEnableInAppAlertNotification_platform(s3eBool enable);
void OneSignalEnableNotificationsWhenActive_platform(s3eBool enable);

void OneSignalSetSubscription_platform(s3eBool enable);

void OneSignalPostNotification_platform(const char* key);


#endif /* !S3EONESIGNAL_INTERNAL_H */