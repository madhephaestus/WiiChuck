/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#ifndef WIZFI250_DEFINITIONS_H_
#define WIZFI250_DEFINITIONS_H_

#define FS(x) (__FlashStringHelper*)(x)

#define RET_OK   0
#define RET_NOK  1
#define RET_NOTI 2

// Firmware version string length
#define WL_FW_VER_LENGTH	26
// Maximum size of a SSID
#define WL_SSID_MAX_LENGTH 32
// Size of a MAC-address or BSSID
#define WL_MAC_ADDR_LENGTH 6
// Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH 4
// Maxmium number of socket
#define	MAX_SOCK_NUM		1


// Length of passphrase. Valid lengths are 8-63.
#define WL_WPA_KEY_MAX_LENGTH 63
// Length of key in bytes. Valid values are 5 and 13.
#define WL_WEP_KEY_MAX_LENGTH 13
// Length of auth in bytes.
#define WL_AUTH_MAX_LENGTH	9
// Length of a User ID in bytes.
#define WL_USER_ID_MAX_LENGTH 20
// Length of a User password in bytes.
#define WL_USER_PW_MAX_LENGTH 20

// Maximum size of a SSID list
#define NETWORKS_LIST_MAXNUM	10
//Maximum number of attempts to establish wifi connection
#define MAX_ATTEMPT_CONNECTION	10


#endif /* WIZFI250_DEFINITIONS_H_ */
