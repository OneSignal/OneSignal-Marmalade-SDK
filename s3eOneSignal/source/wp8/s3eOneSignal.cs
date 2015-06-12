/*
C# implementation of the s3eOneSignal extension.

Add win8-specific functionality here.

These functions are called via Shim class from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
using System.Windows.Controls;
using Microsoft.Phone.Controls;
using System.Windows.Media;

using s3e_native;
using s3eOneSignalExtension;

using Newtonsoft.Json.Linq;

using OneSignalSDK;
using System.Windows;

namespace s3eOneSignalManaged
{
    public class s3eOneSignalManaged: IManagedS3Es3eOneSignalAPI, IManagedS3EEDKAPI
    {
        public bool ExtRegister(IS3EAPIManager apiManager, object mainPage)
        {
            try
            {
                // Keep a reference to the API Manager in order to call other
                // APIs.
                m_APIManager = apiManager;
                m_MainPage = mainPage as PhoneApplicationPage;

                m_MainPanel = VisualTreeHelper.GetChild(m_MainPage, 0) as Panel;

                // Add the managed API to the API Manager
                if (!m_APIManager.RegisterManagedAPI("s3eOneSignal", this))
                    throw new System.Exception("Can't register Managed API");

                // Add the native API to the API manager.  Note that we don't own the native
                // interface, the API Manager does.  We want the two notions of managed and
                // native interface to be separate as there may be cases where we only want
                // one not the other.  It's only a matter of convenience that we create both
                // APIs in this ctr
                m_Native = new s3eOneSignalNativeInterface();
                if (!m_APIManager.RegisterNativeAPI("s3eOneSignal", m_Native))
                    throw new System.Exception("Can't register Native API");

                // Create a Shim so we can pass the Managed interface down to native
                m_Shim = new s3eOneSignalShim();

                // Pass the managed interface down
                m_Shim.Init(this);
            }
            catch (System.Exception e)
            {
                m_APIManager = null;
                m_Shim = null;
                m_Native = null;
                System.Windows.MessageBox.Show("Failed to register s3eOneSignal : " + e.Message);
                
                return false;
            }
            
            return true;
        }
        IS3EAPIManager m_APIManager = null;
        PhoneApplicationPage m_MainPage = null;
        // hint: add UI Elements as children of m_MainPanel
        Panel m_MainPanel = null;
        s3eOneSignalShim m_Shim = null;
        s3eOneSignalNativeInterface m_Native = null;

        // managed platform functionality
		
        public void OneSignalInitialize_managed(string appId)
        {
            OneSignal.Init(appId, (message, additionalData, isActive) => {
                m_Native.CLR_TO_MARM_NotificationReceivedCallback((message == null) ? "" : message,
                                                                  (additionalData == null) ? "" : JObject.FromObject(additionalData).ToString(),
                                                                  isActive);
            });
        }

        public void OneSignalSendTag_managed(string key, string value)
        {
            OneSignal.SendTag(key, value);
        }

        public void OneSignalGetTags_managed()
        {
            OneSignal.GetTags((tags) => {
                m_Native.CLR_TO_MARM_TagsReceivedCallback((tags == null) ? "" : JObject.FromObject(tags).ToString());
            });
        }

        public void OneSignalDeleteTag_managed(string key)
        {
            OneSignal.DeleteTag(key);
        }

        public void OneSignalSendPurchase_managed(double amount)
        {
            OneSignal.SendPurchase(amount);
        }

        public void OneSignalGetIdsAvailable_managed()
        {
            OneSignal.GetIdsAvailable((userID, channelUri) => {
                m_Native.CLR_TO_MARM_IdsAvailableCallback(userID, (channelUri == null) ? "" : channelUri);
            });
        }

        public void GameThriveGetIdsAvailable_managed()
        {
            OneSignal.GetIdsAvailable((playerID, channelUri) => {
                m_Native.CLR_TO_MARM_IdsAvailableCallback_GameThrive(playerID, (channelUri == null) ? "" : channelUri);
            });
        }
    }
}
