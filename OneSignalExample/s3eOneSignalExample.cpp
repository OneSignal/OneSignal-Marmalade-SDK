/**
 * @page OneSignal Test Application
 *
 * The test application demonstrates how to use the OneSignal push notification extension.
 *
 * MKB:
 * @include OneSignalExample.mkb
 *
 * Source:
 * @include OneSignalExample.cpp
 */

#include "s3eOneSignal.h"
#include "s3eDevice.h"
#include "s3ePointer.h"
#include "s3eSurface.h"
#include "s3eTimer.h"
#include "IwDebug.h"

static void HandleReceivedNotification(OneSignalNotificationReceivedResult* result, void* userData) {
	char buffer[1024];
	sprintf(buffer, "NOTIFICATION RECIEVED CALLBACK MESSAGE: %s", result->m_Message);
	s3eDebugOutputString(buffer);
	
	if (result->m_AdditionalData != NULL) {
		char buffer2[1024];
		sprintf(buffer2, "NOTIFICATION RECIEVED CALLBACK ADDITIONALDATA: %s", result->m_AdditionalData);
		s3eDebugOutputString(buffer2);
	}
}

S3E_MAIN_DECL int main() {
	OneSignalInitialize("b2f7f966-d8cc-11e4-bed1-df8f05be55ba", "703322744261", (OneSignalNotificationReceivedCallbackFn)HandleReceivedNotification, true);
	s3eDeviceRegister(S3E_DEVICE_PAUSE, &OneSignalSystemPaused, NULL);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE, &OneSignalSystemResume, NULL);
	
    while (!s3eDeviceCheckQuitRequest()) {
        s3eDeviceYield(0);
        s3eDebugPrint(0, 10, "OneSignalExample", 0);
		s3eDebugPrint(0, 30, "See logcat or Xcode console for debug info.", 0);

        s3eSurfaceShow();
    }
    return 0;
}
