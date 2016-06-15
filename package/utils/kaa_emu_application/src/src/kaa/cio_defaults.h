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

#include "../platform/time.h"

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
 * Definitions for Returns 
 */

#define KAA_SUCCESS "Success"
#define KAA_FAILED "Failed" 

#define SUCCESS     (0)
#define FAILED      (1)
#define ENABLE_CRITICAL_COMMANDS (1)
/*
 * Strategy-specific configuration parameters used by Kaa log collection feature.
 */
#define LOG_UPLOAD_THRESHOLD                 1 /* Count of collected logs needed to initiate log upload */
#define LOG_GENERATION_FREQUENCY             5 /* In seconds */
#define LOG_STORAGE_SIZE                     10000 /* The amount of space allocated for a log storage, in bytes */
#define NUM_LOGS_TO_KEEP                     50    /* The minimum amount of logs to be present in a log storage, in percents */
#define LOG_BUF_SZ                           128    /* Log buffer size in bytes */

#define KAA_LOGRECORD_SIZE                   128
#define EVENT_RESPONSE_SIZE                  256
#define SYSTEM_UPTIME_SIZE                   15
#define URL_SIZE_IN_BYTES                    128  /* Size of the url for firmware download */
#define CHEKSUM_SIZE_IN_BYTES                32 /*No of bytes of checksum to be verified */
#define BIN_DESTINATION_PATH_SIZE            128
#define CONFIG_DESTINATION_PATH_SIZE         64

#define IMEI_LENGTH                          15
#define AT_RESPONSE_LENGTH                   256
#define SAFE_SLEEP_TIME_SEC                  15
#define MAX_LENGTH_OF_SYSTEM_CMD_RESPONSE    256
#define DEVICE_TYPE_LEN                      16 
#define MIN_MEM_BLOCK_SIZE					 32

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

/***********EVENT Generation****************/

#define KAA_USER_ID            "hansolo"
#define KAA_USER_ACCESS_TOKEN  "04219096209415263812"
#define REQUEST_FQN            "com.afl.cio.event.CommandEventResponse"



 /*********************************************/


 /***********CIO Monitoring****************/

#define MONITORING_CMD_LENGTH            256
#define MONITORING_POWER_ON             0700
#define MONITORING_CONTUNIOUS_RUN       0701



 /*********************************************/


/*********************AT.C********************/

#define GERAN_ONLY 		12 //GSM Digital Cellular Systems (GERAN only)
#define UTRAN_ONLY 		22 //UTRAN only
#define NW_3GPP   		25 //3GPP Systems (GERAN and UTRAN and E-UTRAN) (factory default)
#define EUTRAN_ONLY 	28 //E-UTRAN only
#define GERAN_UTRAN		29 //GERAN and UTRAN
#define GERAN_EUTRAN 	30 //GERAN and E-UTRAN
#define UTRAN_EUTRAN	31 //UTRAN and E-UTRAN
#define SIGNAL_STRENGTH_CUTOFF_COEFFICIENT	(-113) //UTRAN and E-UTRAN
#define MAX_AT_CMD_LENGTH	160

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
#define CUSTOM_COMMAND      1109


/*
 * CIO Portal Sub Command Identifiers
 */

#define SOFT_REBOOT         300
#define GET_DEVICE_STATUS   301

/*
 * Interface types 
 */ 

#define WWAN                (0x0001)
#define WAN                 (0x0002)
#define LAN                 (0x0004) 
 
/*
 * Timer variable
 */  
 
static kaa_time_t last_checkin_time; 
static kaa_time_t last_interface_status_check_time; 


#define CIO_CHECKIN_TIMEOUT     							3600 /* Device Checkin Timeout in Sec*/
#define CIO_INTERFACE_STATUS_CHECK_TIMEOUT     				120 /* Device Interface status Check Timeout in Sec*/

/*
 * Forward Functions 
 */  
 
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


