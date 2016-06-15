/*
 * Connected IO - Accel Frontline Limted
 * EMU Router Application
 * Main Application Entry Point
 * 
 */

#include <stdint.h>
#include <time.h>

#include "cio_fw_update.h"

#include <kaa/kaa_error.h>
#include <kaa/kaa_context.h>
#include <kaa/platform/kaa_client.h>
#include <kaa/utilities/kaa_log.h>
#include <kaa/kaa_logging.h>
#include <kaa/platform-impl/common/ext_log_upload_strategies.h>


#include "kaa_notification_manager.h"
#include "gen/kaa_notification_definitions.h"

#include "gen/kaa_profile_definitions.h"
#include "cio_defaults.h"
#include "cio_event.h"
#include "cio_monitoring.h"
#include "cio_notification_manager.h"

/*
 * Forward declarations.
 */
extern kaa_error_t ext_limited_log_storage_create(void **log_storage_context_p
                          , kaa_logger_t *logger
                          , size_t size
                          , size_t percent);


static kaa_client_t *kaa_client = NULL;

static void *log_storage_context         = NULL;
static void *log_upload_strategy_context = NULL;

//kaa_log_bucket_constraints_t bucket_sizes = {1,1};

/* Specify log bucket size constraints */
kaa_log_bucket_constraints_t bucket_sizes = {
     .max_bucket_size       = 512,  /* Bucket size in bytes */
     .max_bucket_log_count  = 1,    /* Maximum log count in one bucket */
};

static size_t log_record_counter = 0;

unsigned int  previous_interface_states = 0;
char device_details[LOG_BUF_SZ] = {};
char device_type[DEVICE_TYPE_LEN] = {};

/**
 * Log records
 */
char emulatedImei[16] = {};
 
char log_record_id[KAA_LOGRECORD_SIZE] = {}; 
char log_record_message[EVENT_RESPONSE_SIZE] = {};

static void emu_device_timer_function(void *context)
{
    
    
       
    kaa_time_t checkin_interval = KAA_TIME() - last_checkin_time;

    if (checkin_interval >= CIO_CHECKIN_TIMEOUT) {
         dprint("Checkin.....\n\n");
        last_checkin_time = KAA_TIME();
        device_check_in_event();
    }
    

    kaa_time_t interface_status_check = KAA_TIME() - last_interface_status_check_time;

    if (interface_status_check >= CIO_INTERFACE_STATUS_CHECK_TIMEOUT) {
         dprint("interface status check Polling.....\n\n");
        last_interface_status_check_time = KAA_TIME();
        check_device_alerts();
    }

   
}

void on_notification(void *context, uint64_t *topic_id, kaa_notification_t *notification)
{
    
    (void)context;  
    int ret = 0;  
    char log_record_tag[KAA_LOGRECORD_SIZE] = {}; 
    static char r_result[KAA_LOGRECORD_SIZE] = {}; 
    static char r_cmdID[KAA_LOGRECORD_SIZE] = {}; 
    static char r_response[EVENT_RESPONSE_SIZE] = {}; 
        
    memset(log_record_id,0,sizeof(KAA_LOGRECORD_SIZE));
    memset(log_record_tag,0,sizeof(KAA_LOGRECORD_SIZE));

    strcpy(log_record_id,notification->command_id->data);
    strcpy(log_record_tag, notification->command->data);

    dprint("Notification Id:%s; Command: %s\n",(char*)log_record_id,(char*)log_record_tag);
    
    memset(log_record_message,'\0',sizeof(log_record_message));
    ret = parse_notification(notification, log_record_message);

    sprintf(r_result,"%s",KAA_SUCCESS);
    sprintf(r_response,"%s",log_record_message);
    sprintf(r_cmdID,"%s",log_record_id);

    cio_notification_response(context,r_result,r_response,r_cmdID);

    //system_health_update();
    
}

void on_topics_received(void *context, kaa_list_t *topics)
{
    dprint("Topic list was updated\n");
    if (!topics || !kaa_list_get_size(topics)) {
        dprint("Topic list is empty");
        return;
    }

    kaa_list_node_t *it = kaa_list_begin(topics);
    while (it) {
        kaa_topic_t *topic = (kaa_topic_t *)kaa_list_get_data(it);
        if (topic != NULL) {
            it = kaa_list_next(it);
        } 
    }
}



void send_cio_notification(void)
{
   //cio_notification_response(NULL, NULL); 
}

void *get_client(void)
{
    return kaa_client;
}

int main(int argc, char *argv[])
{


    char pid[LOG_BUF_SZ]={};
    char cTime[LOG_BUF_SZ]={};
    char cmd[LOG_BUF_SZ]={};

    last_checkin_time = 0;
    last_interface_status_check_time = 0;

    dprint("CIO Portal Application V1.3 (c) 2016\n");

#ifdef EMULATOR
    dprint("EMULATOR\n");
#else
    dprint("DEVICE\n");
    system_powerON();
#endif

    /**
     * Initialize Kaa client.
     */
    kaa_error_t error_code = kaa_client_create(&kaa_client, NULL);
    RETURN_IF_ERROR(error_code, "Failed create Kaa client");

    // Updating the timestamp
    updtae_sys_time();
    get_time(cTime);
  
    dprint("EpocTime:%s.....................................\n",cTime );

    /**
    * Profile data
    */ 
    
    kaa_profile_t *profile = kaa_profile_device_profile_create();

#ifdef EMULATOR
    
    if (argc > 1) {
        strcpy(emulatedImei,argv[1]);
    } else {
        strcpy(emulatedImei,"101010101010101");
    }
    profile->device_version = kaa_string_move_create("v0.0.4", NULL);
    profile->device_id = kaa_string_move_create(emulatedImei, NULL);
    profile->device_type = kaa_string_move_create("Telit", NULL);
    profile->timestamp = kaa_string_move_create(cTime, NULL);
    dprint("IMEI : %s\n",emulatedImei);
#else
    get_firmware_version(device_details);
    profile->device_version = kaa_string_move_create(device_details, NULL);
    profile->device_id = kaa_string_move_create(get_device_imei(), NULL);
    profile->device_type = kaa_string_move_create(get_device_type(&device_type), NULL);
    profile->timestamp = kaa_string_move_create(cTime, NULL);
#endif     
     
    error_code = kaa_profile_manager_update_profile(kaa_client_get_context(kaa_client)->profile_manager, profile);
    RETURN_IF_ERROR(error_code, "Failed to update profile."); 
     
    profile->destroy(profile);

   
    
    
    /**
     * Notification Creation
     **/

    kaa_topic_listener_t topic_listener = { &on_topics_received, kaa_client };
    kaa_notification_listener_t notification_listener = { &on_notification, kaa_client };
    uint32_t topic_listener_id = 0;
    uint32_t notification_listener_id = 0;

    error_code = kaa_add_topic_list_listener(kaa_client_get_context(kaa_client)->notification_manager
                                           , &topic_listener
                                           , &topic_listener_id);
    RETURN_IF_ERROR(error_code, "Failed add topic listener");
    
    
    error_code = kaa_add_notification_listener(kaa_client_get_context(kaa_client)->notification_manager
                                             , &notification_listener
                                             , &notification_listener_id);
    RETURN_IF_ERROR(error_code, "Failed add notification listener");


    // /**Reboot Event****/
 

     error_code = kaa_user_manager_default_attach_to_user(kaa_client_get_context(kaa_client)->user_manager
                                                       , KAA_USER_ID
                                                       , KAA_USER_ACCESS_TOKEN);

   

    /**
     * Start Kaa client main loop.
     */
    error_code = kaa_client_start(kaa_client, &emu_device_timer_function, (void *)kaa_client, LOG_GENERATION_FREQUENCY);
    RETURN_IF_ERROR(error_code, "Failed to start Kaa main loop");
    
    /**
     * Destroy Kaa client.
     */
    kaa_client_destroy(kaa_client);

    dprint("CIO Portal Application Stopped\n");

    return error_code;
}

