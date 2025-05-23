/****************************************************************************
 * modules/include/bluetooth/bluetooth.h
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file bluetooth.h
 * @author Sony Semiconductor Solutions Corporation
 * @brief Bluetooth common header for SDK on Spresense.
 * @details This header file includes bluetooth common definition between
 *          API and HAL I/F.
 *           - Error code
 *           - Length of parameter
 *           - Status
 */

#ifndef __MODULES_INCLUDE_BLUETOOTH_BLUETOOTH_H
#define __MODULES_INCLUDE_BLUETOOTH_BLUETOOTH_H

#define BLUETOOTH_DEBUG

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sdk/config.h>

#include <debug.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef BLUETOOTH_DEBUG
#include <stdio.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef BLUETOOTH_DEBUG
#undef _err
#define _err(format, ...)	printf(format, ##__VA_ARGS__)
#undef _info
#define _info(format, ...)	printf(format, ##__VA_ARGS__)
#endif

/**
 *@name BT success code
 *@{
 */
#define BT_SUCCESS 0
/** @} */

/**
 *@name BT fail code
 *@{
 */
#define BT_FAIL    -1
/** @} */

/**
 *@name BT Address Length
 *@{
 */
#define BT_ADDR_LEN 6
/** @} */

/**
 *@name BT Name Length
 *@{
 */
#define BT_NAME_LEN 28
/** @} */

/**
 *@name BT UUID Length
 *@{
 */
#define BT_UUID128_LEN 16
/** @} */

/**
 *@name Flags bits in advertising data
 *@{
 */

/** LE Limited Discoverable Mode */

#define BLE_ADV_FLAGS_LIMITED_DISC_MODE      (0x01)

/** LE General Discoverable Mode */

#define BLE_ADV_FLAGS_GENERAL_DISC_MODE      (0x02)

/** BR/EDR not supported */

#define BLE_ADV_FLAGS_BR_EDR_NOT_SUPPORTED   (0x04)

/** Simultaneous LE and BR/EDR capable(Controller) */

#define BLE_ADV_FLAGS_LE_BR_EDR_CONTROLLER   (0x08)

/** Simultaneous LE and BR/EDR capable(Host) */

#define BLE_ADV_FLAGS_LE_BR_EDR_HOST         (0x10)

/** @} */

/**
 *@name The length of UUID string
 *@{
 */

/** The length of 128-bit UUID string */

#define BLE_UUID_128BIT_STRING_LENGTH  (36)

/** The length of 16-bit UUID string */

#define BLE_UUID_16BIT_STRING_LENGTH   (4)

/** The minimal buffer size for 128-bit UUID string */

#define BLE_UUID_128BIT_STRING_BUFSIZE (BLE_UUID_128BIT_STRING_LENGTH + 1)

/** The minimal buffer size for 16-bit UUID string */

#define BLE_UUID_16BIT_STRING_BUFSIZE  (BLE_UUID_16BIT_STRING_LENGTH + 1)

/** @} */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/**
 * @struct BT_ADDR
 * @brief BT address types
 */
typedef struct
{
	uint8_t address[BT_ADDR_LEN]; /**< Bluetooth device address(6 byte) */
} BT_ADDR;

/**
 * @struct BT_UUID
 * @brief 128-bit UUID types
 */
typedef struct
{
	uint8_t uuid128[BT_UUID128_LEN]; /**< Bluetooth UUID(16 byte) */
} BT_UUID;

/**
 * @enum BT_CMD_STATUS
 * @brief BT hci command status
 */
typedef enum
{
	BT_COMMAND_STATUS_SUCCESS = 0,            /**< Command success */
	BT_COMMAND_STATUS_IN_PROGRESS,            /**< Command in progress */
	BT_COMMAND_STATUS_ALREADY_CONNECTED,      /**< Already connected */
	BT_COMMAND_STATUS_NOT_CONNECTED,          /**< Not connected */
	BT_COMMAND_STATUS_BAD_HANDLE,             /**< Bad handle */
	BT_COMMAND_STATUS_WRONG_STATE,            /**< Wrong state */
	BT_COMMAND_STATUS_INVALID_ARGS,           /**< Invalid argument */
	BT_COMMAND_STATUS_FAILED,                 /**< Command failed */
	BT_COMMAND_STATUS_UNKNOWN_GROUP,          /**< Unknown group */
	BT_COMMAND_STATUS_UNKNOWN_COMMAND,        /**< Unknown command */
	BT_COMMAND_STATUS_CLIENT_NOT_REGISTERED,  /**< HCI client not registered */
	BT_COMMAND_STATUS_OUT_OF_MEMORY,          /**< Out of memory */
	BT_COMMAND_STATUS_DISALLOWED              /**< Command not allowed */
} BT_CMD_STATUS;

/**
 * @enum BT_PAIR_STATUS
 * @brief BT pairing result
 */
typedef enum
{
	BT_PAIR_SUCCESS,                        /**< Pairing success */
	BT_PAIR_PASSKEY_ENTRY_FAILURE,          /**< Passkey entry failure  */
	BT_PAIR_OOB_FAILURE,                    /**< Out of band failure */
	BT_PAIR_PAIRING_AUTHENTICATION_FAILURE, /**< Authentication failure */
	BT_PAIR_CONFIRM_VALUE_FAILURE,          /**< Confirmation value failure */
	BT_PAIR_PAIRING_NOT_SUPPORTED,          /**< Pairing not supported */
	BT_PAIR_ENCRYPTION_KEY_SIZE_FAILURE,    /**< Encryption key size failure */
	BT_PAIR_INVALID_COMMAND,                /**< Invalid pairing command */
	BT_PAIR_PAIRING_FAILURE_UNKNOWN,        /**< Unknown pairing failure */
	BT_PAIR_REPEATED_ATTEMPTS,              /**< Repeat attempts */
	BT_PAIR_INTERNAL_PAIRING_ERROR,         /**< Internal error */
	BT_PAIR_UNKNOWN_IO_CAPABILITIES,        /**< Unknown I/O capability */
	BT_PAIR_SMP_INITIALIZATION_FAILURE,     /**< SMP initialization failure */
	BT_PAIR_CONFIRMATION_FAILRUE,           /**< Confirmation failure */
	BT_PAIR_SMP_BUSY,                       /**< SMP busy */
	BT_PAIR_ENCRYPTION_FAILURE,             /**< Encryption failure */
	BT_PAIR_BONDING_STARTED,                /**< Bonding started */
	BT_PAIR_RESPONSE_TIMEOUT,               /**< Response timeout */
	BT_PAIR_GENERIC_FAILURE,                /**< Generic failure */
	BT_PAIR_CONNECTION_TIMEOUT,             /**< Connection timeout */
} BT_PAIR_STATUS;

/**
 * @enum BT_CONNECT_FAIL_REASON_ID
 * @brief BT profile connection result
 */
typedef enum
{
	BT_CONNECT_NO_SERVICE = 0,  /**< No service */
	BT_CONNECT_NO_DEVICE,       /**< No device */
	BT_CONNECT_TIMEOUT,         /**< Connection timeout */
	BT_CONNECT_OTHER,           /**< Other reason */
} BT_CONNECT_FAIL_REASON_ID;

/**
 * @enum BT_VISIBILITY
 * @brief BT visibility ID
 */
typedef enum
{
  BT_VIS_NO_DISCOVERY_NO_CONNECTABLE = 0, /**< No discoverable and no connectable */
  BT_VIS_DISCOVERY_NO_CONNECTABLE    = 1, /**< Discoverable but no connectable */
  BT_VIS_NO_DISCOVERY_CONNECTABLE    = 2, /**< No discoverable but connectable */
  BT_VIS_DISCOVERY_CONNECTABLE       = 3  /**< Discoverable and connectable */
} BT_VISIBILITY;

/**
 * @enum BLE_APPEARANCE
 * @brief BLE appearance ID
 */
typedef enum
{
  BLE_APPEARANCE_UNKNOWN                          = 0,
  BLE_APPEARANCE_GENERIC_PHONE                    = 64,
  BLE_APPEARANCE_GENERIC_COMPUTER                 = 128,
  BLE_APPEARANCE_GENERIC_WATCH                    = 192,
  BLE_APPEARANCE_WATCH_SPORTS                     = 193,
  BLE_APPEARANCE_GENERIC_CLOCK                    = 256,
  BLE_APPEARANCE_GENERIC_DISPLAY                  = 320,
  BLE_APPEARANCE_GENERIC_REMOTE_CONTROL           = 384,
  BLE_APPEARANCE_GENERIC_EYE_GLASSES              = 448,
  BLE_APPEARANCE_GENERIC_TAG                      = 512,
  BLE_APPEARANCE_GENERIC_KEYRING                  = 576,
  BLE_APPEARANCE_GENERIC_MEDIA_PLAYER             = 640,
  BLE_APPEARANCE_GENERIC_BARCODE_SCANNER          = 704,
  BLE_APPEARANCE_GENERIC_THERMOMETER              = 768,
  BLE_APPEARANCE_THERMOMETER_EAR                  = 769,
  BLE_APPEARANCE_GENERIC_HEART_RATE_SENSOR        = 832,
  BLE_APPEARANCE_HEART_RATE_BELT                  = 833,
  BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE           = 896,
  BLE_APPEARANCE_BLOOD_PRESSURE_ARM               = 897,
  BLE_APPEARANCE_BLOOD_PRESSURE_WRIST             = 898,
  BLE_APPEARANCE_GENERIC_HID_DEVICE               = 960,
  BLE_APPEARANCE_HID_KEYBOARD                     = 961,
  BLE_APPEARANCE_HID_MOUSE                        = 962,
  BLE_APPEARANCE_HID_JOYSTICK                     = 963,
  BLE_APPEARANCE_HID_GAMEPAD                      = 964,
  BLE_APPEARANCE_HID_DIGITIZER_TABLET             = 965,
  BLE_APPEARANCE_HID_CARD_READER                  = 966,
  BLE_APPEARANCE_HID_DIGITAL_PEN                  = 967,
  BLE_APPEARANCE_HID_BARCODE_SCANNER              = 968,
  BLE_APPEARANCE_GENERIC_GLUCOSE_METER            = 1024,
  BLE_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR   = 1088,
  BLE_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE   = 1089,
  BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_SHOE   = 1090,
  BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_HIP    = 1091,
  BLE_APPEARANCE_GENERIC_CYCLING                  = 1152,
  BLE_APPEARANCE_CYCLING_COMPUTER                 = 1153,
  BLE_APPEARANCE_CYCLING_SPEED_SENSOR             = 1154,
  BLE_APPEARANCE_CYCLING_CADENCE_SENSOR           = 1155,
  BLE_APPEARANCE_CYCLING_POWER_SENSOR             = 1156,
  BLE_APPEARANCE_CYCLING_SPEED_AND_CADENCE_SENSOR = 1157,
} BLE_APPEARANCE;

/**
 * @enum BLE_GAP_IO_CAP
 * @brief BLE IO capability
 */
typedef enum {
	BLE_GAP_IO_CAP_DISPLAY_ONLY = 0,   /**< Display Only */
	BLE_GAP_IO_CAP_DISPLAY_YESNO,      /**< Display and Yes/No entry */
	BLE_GAP_IO_CAP_KEYBOARD_ONLY,      /**< Keyboard Only */
	BLE_GAP_IO_CAP_NO_INPUT_NO_OUTPUT, /**< No Input No Output */
	BLE_GAP_IO_CAP_KEYBOARD_DISPLAY,   /**< Keyboard and Display */
} BLE_GAP_IO_CAP;

/**
 * @enum BLE_AD_TYPE
 * @brief BLE data type of advertising data
 */

typedef enum {
  /** BLE address type */

  BLE_AD_TYPE_ADDRESS_TYPE                   = 0x00,

  /** Flags for discoverability. */

  BLE_AD_TYPE_FLAGS                          = 0x01,

  /** Partial list of 16 bit service UUIDs. */

  BLE_AD_TYPE_16BIT_SERVICE_UUID_INCOMPLETE  = 0x02,

  /** Complete list of 16 bit service UUIDs. */

  BLE_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE    = 0x03,

  /** Partial list of 32 bit service UUIDs. */

  BLE_AD_TYPE_32BIT_SERVICE_UUID_INCOMPLETE  = 0x04,

  /** Complete list of 32 bit service UUIDs. */

  BLE_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE    = 0x05,

  /** Partial list of 128 bit service UUIDs. */

  BLE_AD_TYPE_128BIT_SERVICE_UUID_INCOMPLETE = 0x06,

  /** Complete list of 128 bit service UUIDs. */

  BLE_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE   = 0x07,

  /** Short local device name. */

  BLE_AD_TYPE_SHORT_LOCAL_NAME               = 0x08,

  /** Complete local device name. */

  BLE_AD_TYPE_COMPLETE_LOCAL_NAME            = 0x09,

  /** Transmit power level. */

  BLE_AD_TYPE_TX_POWER_LEVEL                 = 0x0A,

  /** Class of device. */

  BLE_AD_TYPE_CLASS_OF_DEVICE                = 0x0D,

  /** Simple Pairing Hash C. */

  BLE_AD_TYPE_SIMPLE_PAIRING_HASH_C          = 0x0E,

  /** Simple Pairing Randomizer R. */

  BLE_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R    = 0x0F,

  /** Security Manager TK Value. */

  BLE_AD_TYPE_SECURITY_MANAGER_TK_VALUE      = 0x10,

  /** Security Manager Out Of Band Flags. */

  BLE_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS     = 0x11,

  /** Slave Connection Interval Range. */

  BLE_AD_TYPE_CONNECTION_INTERVAL_RANGE      = 0x12,

  /** List of 16-bit Service Solicitation UUIDs. */

  BLE_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT  = 0x14,

  /** List of 128-bit Service Solicitation UUIDs. */

  BLE_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT = 0x15,

  /** Service Data - 16-bit UUID. */

  BLE_AD_TYPE_SERVICE_DATA                   = 0x16,

  /** Public Target Address. */

  BLE_AD_TYPE_PUBLIC_TARGET_ADDRESS          = 0x17,

  /** Random Target Address. */

  BLE_AD_TYPE_RANDOM_TARGET_ADDRESS          = 0x18,

  /** Appearance. */

  BLE_AD_TYPE_APPEARANCE                     = 0x19,

  /** Advertising Interval. */

  BLE_AD_TYPE_ADVERTISING_INTERVAL           = 0x1A,

  /** LE Bluetooth Device Address. */

  BLE_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS    = 0x1B,

  /** LE Role. */

  BLE_AD_TYPE_LE_ROLE                        = 0x1C,

  /** Simple Pairing Hash C-256. */

  BLE_AD_TYPE_SIMPLE_PAIRING_HASH_C256       = 0x1D,

  /** Simple Pairing Randomizer R-256. */

  BLE_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256 = 0x1E,

  /** Service Data - 32-bit UUID. */

  BLE_AD_TYPE_SERVICE_DATA_32BIT_UUID        = 0x20,

  /** Service Data - 128-bit UUID. */

  BLE_AD_TYPE_SERVICE_DATA_128BIT_UUID       = 0x21,

  /** LE Secure Connections Confirmation Value */

  BLE_AD_TYPE_LESC_CONFIRMATION_VALUE        = 0x22,

  /** LE Secure Connections Random Value */

  BLE_AD_TYPE_LESC_RANDOM_VALUE              = 0x23,

  /** URI */

  BLE_AD_TYPE_URI                            = 0x24,

  /** 3D Information Data. */

  BLE_AD_TYPE_3D_INFORMATION_DATA            = 0x3D,

  /** Manufacturer Specific Data. */

  BLE_AD_TYPE_MANUFACTURER_SPECIFIC_DATA     = 0xFF,
} BLE_AD_TYPE;

/**
 * @enum BLE_ADDRESS_TYPE
 * @brief BLE address type
 */

typedef enum
{
  /** BLE public address */

  BLE_ADDRTYPE_PUBLIC = 0x00,

  /** BLE random static address */

  BLE_ADDRTYPE_RAND_STATIC,

  /** BLE random private resolvable address */

  BLE_ADDRTYPE_RAND_PRIV_RESOLVABLE,

  /** BLE random private non-resolvable address */

  BLE_ADDRTYPE_RAND_PRIV_NONRESOLVABLE,
} BLE_ADDRESS_TYPE;

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#endif /* __MODULES_INCLUDE_BLUETOOTH_BLUETOOTH_H */
