/*
 * iphone-specific implementation of the s3eOneSignal extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eOneSignal_internal.h"
#import "OneSignal.h"
#import <objc/runtime.h>
#import <UIKit/UIKit.h>

#define S3E_CURRENT_EXT			ONESIGNAL
#define S3E_DEVICE_ONESIGNAL	S3E_EXT_ONESIGNAL_HASH

#include "s3eEdkError.h"

OneSignal* oneSignal;

NSString* CreateNSString(const char* string) {
    return [NSString stringWithUTF8String: string ? string : ""];
}

static char* CopyToCString(NSString* string) {
	if (string) {
		const char* pStr = [string UTF8String];
		char* pCopy = (char*) s3eEdkMallocOS(strlen(pStr) + 1);
		if (pCopy) {
			strcpy(pCopy, pStr);
			return pCopy;
		}
	}

	return NULL;
}

@implementation UIApplication(GameThriveMarmaladePush)

BOOL marmInitDone = false;

NSString* mAlertMessage;
NSDictionary* mAdditionalData;
BOOL mIsActive;

void initOneSignalObject(NSDictionary* launchOptions, const char* appId, BOOL autoRegister) {
    if (oneSignal == nil) {
        [OneSignal setValue:@"marmalade" forKey:@"mSDKType"];

        NSString* appIdStr = (appId ? [NSString stringWithUTF8String: appId] : nil);
        
        oneSignal = [[OneSignal alloc] initWithLaunchOptions:launchOptions appId:appIdStr handleNotification:^(NSString* message, NSDictionary* additionalData, BOOL isActive) {
            mAlertMessage = message;
            // Need to deep copy as we will be going accoss threads.
            mAdditionalData = (NSDictionary *)CFPropertyListCreateDeepCopy(kCFAllocatorDefault, (CFDictionaryRef)additionalData, kCFPropertyListMutableContainers);
            mIsActive = isActive;
            
            if (marmInitDone)
                processNotificationOpened();
        } autoRegister:autoRegister];
    }
}


static void switchMethods(Class inClass, SEL oldSel, SEL newSel, IMP impl, const char* sig)
{
    class_addMethod(inClass, newSel, impl, sig);
    method_exchangeImplementations(class_getInstanceMethod(inClass, oldSel), class_getInstanceMethod(inClass, newSel));
}

+ (void)load {
    method_exchangeImplementations(class_getInstanceMethod(self, @selector(setDelegate:)), class_getInstanceMethod(self, @selector(setOneSignalMarmaladeDelegate:)));
}

- (void) setOneSignalMarmaladeDelegate:(id<UIApplicationDelegate>)delegate {
    static Class delegateClass = nil;
    
    if(delegateClass == [delegate class]) {
      [self setOneSignalMarmaladeDelegate:delegate];
      return;
    }
    
    delegateClass = [delegate class];
    
    switchMethods(delegateClass, @selector(application:didFinishLaunchingWithOptions:),
                  @selector(application:selectorDidFinishLaunchingWithOptions:), (IMP)didFinishLaunchingWithOptions_OneSignalLocal, "v@:::");
    
    [self setOneSignalMarmaladeDelegate:delegate];
}

BOOL didFinishLaunchingWithOptions_OneSignalLocal(id self, SEL _cmd, id application, id launchOptions) {
    BOOL result = YES;
    
    if ([self respondsToSelector:@selector(application:selectorDidFinishLaunchingWithOptions:)]) {
        BOOL openedFromNotification = ([launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey] != nil);
        if (openedFromNotification)
            initOneSignalObject(launchOptions, nil, true);

        if (![self application:application selectorDidFinishLaunchingWithOptions:launchOptions])
          result = NO;
    }
    else {
        [self applicationDidFinishLaunching:application];
        result = YES;
    }
    
	return result;
}

const char* dictionaryToJsonChar(NSDictionary* dictionaryToConvert) {
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:dictionaryToConvert options:0 error:nil];
    NSString* jsonRequestData = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    
    return [jsonRequestData UTF8String];
}

void processNotificationOpened() {
    OneSignalNotificationReceivedResult result;
    result.m_Message = CopyToCString(mAlertMessage);
    
    if (mAdditionalData)
        result.m_AdditionalData = dictionaryToJsonChar(mAdditionalData);
    
    result.m_isActive = mIsActive;
    
    s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
                           S3E_ONESIGNAL_CALLBACK_NOTIFICATION_RECEIVED,
                           &result,
                           sizeof(result),
                           NULL,
                           S3E_FALSE); // FALSE, not a one shot callback.
}

@end


s3eResult s3eOneSignalInit_platform() {
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void s3eOneSignalTerminate_platform() {
    // Add any platform-specific termination code here
}

void GameThriveInitialize_platform(const char* appId, const char* googleProjectNumber, GameThriveNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister) {
    // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, NOTIFICATION_RECEIVED, (s3eCallback)callbackFn, NULL, false);
    
    initOneSignalObject(nil, appId, autoRegister);
    marmInitDone = true;
    
    if (mAdditionalData)
        processNotificationOpened();
}

void OneSignalInitialize_platform(const char* appId, const char* googleProjectNumber, OneSignalNotificationReceivedCallbackFn callbackFn, s3eBool autoRegister) {
    // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, NOTIFICATION_RECEIVED, (s3eCallback)callbackFn, NULL, false);
    
    initOneSignalObject(nil, appId, autoRegister);
    marmInitDone = true;
    
    if (mAdditionalData)
        processNotificationOpened();
}


void OneSignalSendTag_platform(const char* key, const char* value) {
    [oneSignal sendTag:CreateNSString(key) value:CreateNSString(value)];
}

void GameThriveGetTags_platform(GameThriveTagsReceivedCallbackFn callbackFn) {
    // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, TAGS_RECEIVED, (s3eCallback)callbackFn, NULL, false);
    
    [oneSignal getTags:^(NSDictionary* tags) {
        GameThriveTagsReceivedResult result;
        result.m_Tags = dictionaryToJsonChar(tags);
        
        s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
                               S3E_GAMETHRIVE_CALLBACK_TAGS_RECEIVED,
                               &result,
                               sizeof(result),
                               NULL,
                               S3E_TRUE); // TRUE, a one shot callback.
    }];
}

void OneSignalGetTags_platform(OneSignalTagsReceivedCallbackFn callbackFn) {
    // Register Callback
    EDK_CALLBACK_REG(ONESIGNAL, TAGS_RECEIVED, (s3eCallback)callbackFn, NULL, false);
    
    [oneSignal getTags:^(NSDictionary* tags) {
        OneSignalTagsReceivedResult result;
        result.m_Tags = dictionaryToJsonChar(tags);
        
        s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
                               S3E_ONESIGNAL_CALLBACK_TAGS_RECEIVED,
                               &result,
                               sizeof(result),
                               NULL,
                               S3E_TRUE); // TRUE, a one shot callback.
    }];
}


void OneSignalDeleteTag_platform(const char* key) {
    [oneSignal deleteTag:CreateNSString(key)];
}

void OneSignalSendPurchase_platform(const double* amount) {
}

void GameThriveGetIdsAvailable_platform(GameThriveIdsAvailableCallbackFn callbackFn) {
    EDK_CALLBACK_REG(ONESIGNAL, IDS_AVAILABLE, (s3eCallback)callbackFn, NULL, false);
    
    [oneSignal IdsAvailable:^(NSString* playerId, NSString* pushToken) {
        GameThriveIdsAvailableResult result;
        result.m_PlayerID = CopyToCString(playerId);
        result.m_PushToken = CopyToCString(pushToken);
        
        s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
                               S3E_GAMETHRIVE_CALLBACK_IDS_AVAILABLE,
                               &result,
                               sizeof(result),
                               NULL,
                               S3E_FALSE); // FALSE, not a one shot callback.
    }];
}

void OneSignalGetIdsAvailable_platform(OneSignalIdsAvailableCallbackFn callbackFn) {
    EDK_CALLBACK_REG(ONESIGNAL, IDS_AVAILABLE, (s3eCallback)callbackFn, NULL, false);
    
    [oneSignal IdsAvailable:^(NSString* userId, NSString* pushToken) {
        OneSignalIdsAvailableResult result;
        result.m_UserID = CopyToCString(userId);
        result.m_PushToken = CopyToCString(pushToken);
        
        s3eEdkCallbacksEnqueue(S3E_DEVICE_ONESIGNAL,
                               S3E_ONESIGNAL_CALLBACK_IDS_AVAILABLE,
                               &result,
                               sizeof(result),
                               NULL,
                               S3E_FALSE); // FALSE, not a one shot callback.
    }];
}


void OneSignalOnResume_platform() {
  // Only Applies to Android
}

void OneSignalOnPause_platform() {
  // Only Applies to Android
}

void OneSignalRegisterForPushNotifications_platform() {
  [oneSignal registerForPushNotifications];
}


void OneSignalEnableVibrate_platform(s3eBool enable) {
  // Only Applies to Android
}

void OneSignalEnableSound_platform(s3eBool enable) {
  // Only Applies to Android
}


void OneSignalEnableInAppAlertNotification_platform(s3eBool enable) {
  [oneSignal enableInAppAlertNotification:enable];
}

void OneSignalEnableNotificationsWhenActive_platform(s3eBool enable) {
 // Only Applies to Android
}

void OneSignalSetSubscription_platform(s3eBool enable) {
  [oneSignal setSubscription:enable];
}

void OneSignalPostNotification_platform(const char* jsonData) {
  [oneSignal postNotificationWithJsonString:CreateNSString(jsonData)];
}