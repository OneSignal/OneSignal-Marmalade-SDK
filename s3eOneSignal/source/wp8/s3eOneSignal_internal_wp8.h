#ifndef S3EONESIGNAL_INTERNAL_WP8_H
#define S3EONESIGNAL_INTERNAL_WP8_H

void NotificationReceivedCallback(const char* message, const char* additionalData, bool isActive);
void TagsReceivedCallback(const char* tags);
void IdsAvailableCallback(const char* userID, const char* pushToken);
void IdsAvailableCallback_GameThrive(const char* playerID, const char* pushToken);

#endif /* S3EONESIGNAL_INTERNAL_WP8_H */