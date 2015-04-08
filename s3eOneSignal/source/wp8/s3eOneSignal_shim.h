/*
C++ Shim implementation of the s3eOneSignal extension's.

Not intended for modifications

These functions are called by C# to access native interface and from C++ to
access managed interface.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */

#include "s3eOneSignal_internal_wp8.h"

namespace s3eOneSignalExtension
{

// char* <-> Platform::String^ wrappers
static inline Platform::String^ UTF8ToString(const char* utf8str) {
    return s3e_native::s3eAPIDataTypeWrapper::CStringToPlatformString(const_cast<char*>(utf8str));
}

static inline void StringToUTF8(char* utf8str, size_t size, Platform::String^ str) {
    s3e_native::s3eAPIDataTypeWrapper::PlatformStringToCString(const_cast<char*>(utf8str), size, str);
}


// This is the interface which native code can use to talk to managed code.
[Windows::Foundation::Metadata::WebHostHidden]
public interface class IManagedS3Es3eOneSignalAPI : public s3e_native::IManagedS3EAPI {
    // We shouldn't populate these defacto.  We should only add functions here as and when
    // necessary.
    void OneSignalInitialize_managed(Platform::String^ appId);
    void OneSignalSendTag_managed(Platform::String^ key, Platform::String^ value);
    void OneSignalGetTags_managed();
    void OneSignalDeleteTag_managed(Platform::String^ key);
    void OneSignalSendPurchase_managed(double amount);
    void OneSignalGetIdsAvailable_managed();
	void GameThriveGetIdsAvailable_managed();
};// This is the class which managed code can use to talk to native code.

[Windows::Foundation::Metadata::WebHostHidden]
public ref class s3eOneSignalNativeInterface sealed : public s3e_native::INativeS3EAPI {
    // We shouldn't populate these defacto.  We should only add functions here as and when
    // necessary.
public:
    s3eOneSignalNativeInterface() {}

	void CLR_TO_MARM_NotificationReceivedCallback(Platform::String^ additionalData, bool isActive) {
		char* buff;
		
		if (!additionalData->IsEmpty()) {
			buff = new char[4096];
			StringToUTF8(buff, 4096, additionalData);
		}
		else
			buff = 0;

		NotificationReceivedCallback(buff, isActive);
	}

	void CLR_TO_MARM_TagsReceivedCallback(Platform::String^ tags) {
		char* buff;
		
		if (!tags->IsEmpty()) {
			buff = new char[4096];
			StringToUTF8(buff, 4096, tags);
		}
		else
			buff = 0;

		TagsReceivedCallback(buff);
	}

	void CLR_TO_MARM_IdsAvailableCallback(Platform::String^ userId, Platform::String^ channelUri) {
		char* userIdBuff = new char[37];
		StringToUTF8(userIdBuff, 37, userId);

		char* channelUriBuff;
		if (!channelUri->IsEmpty()) {
			channelUriBuff = new char[2048];
			StringToUTF8(channelUriBuff, 2048, channelUri);
		}
		else
			channelUriBuff = 0;

		IdsAvailableCallback(userIdBuff, channelUriBuff);
	}

	void CLR_TO_MARM_IdsAvailableCallback_GameThrive(Platform::String^ playerId, Platform::String^ channelUri) {
		char* playerIdBuff = new char[37];
		StringToUTF8(playerIdBuff, 37, playerId);

		char* channelUriBuff;
		if (!channelUri->IsEmpty()) {
			channelUriBuff = new char[2048];
			StringToUTF8(channelUriBuff, 2048, channelUri);
		}
		else
			channelUriBuff = 0;

		IdsAvailableCallback_GameThrive(playerIdBuff, channelUriBuff);
	}
};


[Windows::Foundation::Metadata::WebHostHidden]
public ref class s3eOneSignalShim sealed {
public:

    s3eOneSignalShim();

    // We use two stage construction to avoid the use of exceptions.  We
    // don't want to ever register an API more than once (as it's a static
    // below).  If we did this initialization in the ctr then we'd either
    // have to fail silently or throw an exception.
     bool Init(IManagedS3Es3eOneSignalAPI^ managedAPI);

    // Unregisters the static interface.
     bool Terminate();

    // Need static getters as we can't have public members in the class.
    static IManagedS3Es3eOneSignalAPI^ GetInterface();
private:

    ~s3eOneSignalShim();

    // This is the unavoidable point where we need a static.  There's
    // no choice as we're converting to a C-API.
    static IManagedS3Es3eOneSignalAPI^ s_ManagedInterface;
};

}
