/*
java implementation of the s3eOneSignal extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;

import android.content.Context;

import org.json.JSONException;
import org.json.JSONObject;

import com.onesignal.OneSignal;
import com.onesignal.OneSignal.GetTagsHandler;
import com.onesignal.OneSignal.IdsAvailableHandler;
import com.onesignal.OneSignal.NotificationOpenedHandler;

class s3eOneSignal implements NotificationOpenedHandler {
	private boolean inited = false;

    public void OneSignalInitialize(String appId, String googleProjectNumber, boolean autoRegister) {
		if (!inited) {
			OneSignal.sdkType = "marmalade";
			OneSignal.init((Context)LoaderActivity.m_Activity, googleProjectNumber, appId, this);
			inited = true;
		}
    }
	
	@Override
	public void notificationOpened(String message, JSONObject additionalData, boolean isActive) {
		String extraData = null;
		if (additionalData != null)
			extraData = additionalData.toString();
		
		NotificationReceivedCallback(message, extraData, isActive);
	}
	
	
    public void OneSignalSendTag(String key, String value) {
		OneSignal.sendTag(key, value);
    }
	
    public void OneSignalGetTags() {
		OneSignal.getTags(new GetTagsHandler() {
			@Override
			public void tagsAvailable(JSONObject tags) {
				TagsReceivedCallback(tags.toString());
			}
		});
    }
	
    public void OneSignalDeleteTag(String key) {
        OneSignal.deleteTag(key);
    }
	
    public void OneSignalGetIdsAvailable() {
        OneSignal.idsAvailable(new IdsAvailableHandler() {
			@Override
			public void idsAvailable(String userId, String registrationId) {
				IdsAvailableCallback(userId, registrationId);
			}
		});
    }

    public void GameThriveGetIdsAvailable() {
        OneSignal.idsAvailable(new IdsAvailableHandler() {
			@Override
			public void idsAvailable(String playerId, String registrationId) {
				IdsAvailableCallback_GameThrive(playerId, registrationId);
			}
		});
    }
	
	public void OneSignalOnPause() {
		OneSignal.onPaused();
	}
	
	public void OneSignalOnResume() {
		OneSignal.onResumed();
	}

	public void OneSignalEnableVibrate(boolean enable) {
		OneSignal.enableVibrate(enable);
	}

	public void OneSignalEnableSound(boolean enable) {
		OneSignal.enableSound(enable);
	}

	public void OneSignalEnableInAppAlertNotification(boolean enable) {
		OneSignal.enableInAppAlertNotification(enable);
	}

	public void OneSignalEnableNotificationsWhenActive(boolean enable) {
		OneSignal.enableNotificationsWhenActive(enable);
	}

	public void OneSignalSetSubscription(boolean enable) {
		OneSignal.setSubscription(enable);
	}
	
	public void OneSignalPostNotification(String jsonStr) {
		OneSignal.postNotification(jsonStr, null);
    }
	
	public native void NotificationReceivedCallback(String message, String additionalData, boolean isActive);
	public native void IdsAvailableCallback(String userID, String pushToken);
	public native void IdsAvailableCallback_GameThrive(String playerID, String pushToken);
	public native void TagsReceivedCallback(String tags);
}
