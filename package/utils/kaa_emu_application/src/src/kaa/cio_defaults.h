/*
 * Defaults
 */

/**
 * @file cio defaults
 * @brief Default defines and macros
 *
 * Supplies Defaults
 */

#ifndef CIO_DEFAULTS_H_
#define CIO_DEFAULTS_H_

#define DEBUG (1)

/*
 * Hard-coded Kaa log entry body.
 */

#define KAA_DEBUG "KAA_DEBUG"
#define KAA_ERROR "KAA_ERROR"
#define KAA_FATAL "KAA_FATAL"
#define KAA_INFO "KAA_INFO"
#define KAA_TRACE "KAA_TRACE"
#define KAA_WARN "KAA_WARN"

/*
 * Strategy-specific configuration parameters used by Kaa log collection feature.
 */
#define LOG_UPLOAD_THRESHOLD                 1 /* Count of collected logs needed to initiate log upload */
#define LOG_GENERATION_FREQUENCY             1 /* In seconds */
#define LOG_STORAGE_SIZE                     10000 /* The amount of space allocated for a log storage, in bytes */
#define NUM_LOGS_TO_KEEP                     50    /* The minimum amount of logs to be present in a log storage, in percents */
#define LOG_BUF_SZ                           128    /* Log buffer size in bytes */

#define KAA_LOGRECORD_SIZE                   64

#define URL_SIZE_IN_BYTES                    128  /* Size of the url for firmware download */
#define CHEKSUM_SIZE_IN_BYTES                32 /*No of bytes of checksum to be verified */
#define BIN_DESTINATION_PATH_SIZE            128
#define CONFIG_DESTINATION_PATH_SIZE         64

#define IMEI_LENGTH                          15
#define AT_RESPONSE_LENGTH                   256
#define SAFE_SLEEP_TIME_SEC                  15
#define MAX_LENGTH_OF_SYSTEM_CMD_RESPONSE    256
#define DEVICE_TYPE_LEN                      16 

#define LOCAL_DIRECTORY     "/tmp/kaa"
#define FW_BIN_PATH         "/tmp/kaa/Firmware.bin"
#define CONFIG_FILE_PATH    "/tmp/kaa/CONFIG.tar.gz"
#define MD5SUM_FILE_PATH    "/tmp/kaa/md5.txt"
#define IMEI_FILE_PATH      "/tmp/kaa/imei.txt"


#define CURL_ERROR_MESSAGE_SIZE                 1280

#define FIRMWARE_UPDATE     7010
#define CONFIG_UPDATE       7011

#define CONFIG_UPLOAD       7012
#define SYSTEM_LOG_UPLOAD   7013
#define KERNAL_LOG_UPLOAD   7014

#define READ_DATA           7000
#define CHECK_FOR_FILE      7001

/******FIREWALL SETTING MACROS******************/
#define DHCP_RENEW           0x0001
#define ALLOW_PING           0x0002
#define ALLOW_IGMP           0x0004
#define ALLOW_DHCPv6         0x0008
#define ALLOW_MLD            0x0010
#define ALLOW_ICMPv6_INPUT   0x0020
#define ALLOW_ICMPv6_FWD     0x0040
 /*********************************************/

/*
 * CIO Portal Command Identifiers.
*/

#define UPDATE_FW           1100
#define PERFORM_RESET       1101
#define RESTORE_CFG         1102
#define BACKUP_CFG          1103
#define GET_SYS_LOG         1104
#define GET_KERNEL_LOG      1105
#define EXECUTE_AT_CMD      1106
#define GET_FIREWALL        1107
#define SET_FIREWALL        1108
#define PERFORM_REBOOT      1109

#define dprint(TAG, ...) \
            do { if (DEBUG) fprintf(stderr, "[CIO] : " TAG, ##__VA_ARGS__); } while (0)
            
#define dprint_v2(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)

#define RETURN_IF_ERROR(error, message) \
    if ((error)) { \
        printf(message ", error code %d\n", (error)); \
        return (error); \
    }                                            
#endif /* CIO_DEFAULTS_H_ */            


