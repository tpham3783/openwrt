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
static size_t log_record_counter = 0;

int8_t update_log = 0;
char device_details[LOG_BUF_SZ] = {};
char device_type[DEVICE_TYPE_LEN] = {};

/**
 * Log records
 */
 
char log_record_id[KAA_LOGRECORD_SIZE] = {}; 
char log_record_message[KAA_LOGRECORD_SIZE*2] = {};

  
static void emu_device_add_log_record(void *context)
{
    if (update_log == 0) {
        return;
    }
    log_record_counter++;
    kaa_user_log_record_t *log_record = kaa_logging_device_log_create();
    if (!log_record) {
        dprint("Failed to create log record, error code %d\n", KAA_ERR_NOMEM);
        return;
    }
    
    log_record->command_id = kaa_string_move_create(log_record_id, NULL);
    
    if (strstr(log_record_message, "Failed")) {
        log_record->result = kaa_string_move_create("Failed", NULL);
    } else {
        log_record->result = kaa_string_move_create("Success", NULL);
    }

    log_record->message1 = kaa_string_move_create(log_record_message, NULL);
    log_record->message2 = kaa_string_move_create("NA", NULL);

    dprint("Going to add %zuth; id: (%s) ;msg (%s)\n", log_record_counter, (char*)log_record_id,(char*)log_record_message);

    kaa_error_t error_code = kaa_logging_add_record(kaa_client_get_context(kaa_client)->log_collector, log_record);
    if (error_code) {
        dprint("Failed to add log record, error code %d\n", error_code);
    }

    if(strcmp(log_record_message,"Success:Firmware_Update")==0){

        dprint("Initiating Firmware Update\n");
        sys_upgrade(FIRMWARE_UPDATE);

    }else if(strcmp(log_record_message,"Success:Config_Update")==0){

        dprint("Initiating Config Update\n");
        sys_upgrade(CONFIG_UPDATE);

    }else if(strcmp(log_record_message,"Initiating_Reboot") == 0){

        dprint("Initiating Reboot\n");
        //system("reboot");

    }

    log_record->destroy(log_record);

    update_log = 0;
}

void on_notification(void *context, uint64_t *topic_id, kaa_notification_t *notification)
{
    
    (void)context;    
    char log_record_tag[KAA_LOGRECORD_SIZE] = {}; 
    
    memset(log_record_id,0,sizeof(KAA_LOGRECORD_SIZE));
    memset(log_record_tag,0,sizeof(KAA_LOGRECORD_SIZE));

    strcpy(log_record_id,notification->command_id->data);
    strcpy(log_record_tag, notification->command->data);
    
    dprint("Notification Id:%s; Command: %s\n",(char*)log_record_id,(char*)log_record_tag);
    
    update_log = parse_notification(notification, log_record_message);
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

int main(/*int argc, char *argv[]*/)
{
    dprint("CIO Portal Application V1.3 (c) 2016\n");

#ifdef EMULATOR
    dprint("EMULATOR\n");
#else
    dprint("DEVICE\n");
#endif

    /**
     * Initialize Kaa client.
     */
    kaa_error_t error_code = kaa_client_create(&kaa_client, NULL);
    RETURN_IF_ERROR(error_code, "Failed create Kaa client");

    /**
    * Profile data
    */ 
    
    kaa_profile_t *profile = kaa_profile_device_profile_create();

#ifdef EMULATOR
    profile->device_version = kaa_string_move_create("'OpenWrt'_'v0.0.4'.2_'r21'_'ramips/mt7620'_'ER1000T-EU'", NULL);
    profile->device_id = kaa_string_move_create("111222333444555", NULL);
    profile->device_type = kaa_string_move_create("ER1000T-EU", NULL);
#else
    profile->device_version = kaa_string_move_create(get_device_details(&device_details), NULL);
    profile->device_id = kaa_string_move_create(get_device_imei(), NULL);
    profile->device_type = kaa_string_move_create(get_device_type(&device_type), NULL);
#endif     
     
    error_code = kaa_profile_manager_update_profile(kaa_client_get_context(kaa_client)->profile_manager, profile);
    RETURN_IF_ERROR(error_code, "Failed to update profile."); 
     
    profile->destroy(profile);

    /**
    * Log Appender
    */ 
    error_code = ext_limited_log_storage_create(&log_storage_context, kaa_client_get_context(kaa_client)->logger, LOG_STORAGE_SIZE, NUM_LOGS_TO_KEEP);
    RETURN_IF_ERROR(error_code, "Failed to create limited log storage");
    
    error_code = ext_log_upload_strategy_create(kaa_client_get_context(kaa_client), &log_upload_strategy_context, KAA_LOG_UPLOAD_VOLUME_STRATEGY);
    RETURN_IF_ERROR(error_code, "Failed to create log upload strategy");

    error_code = ext_log_upload_strategy_set_threshold_count(log_upload_strategy_context, LOG_UPLOAD_THRESHOLD);
    RETURN_IF_ERROR(error_code, "Failed to set threshold log record count");

    error_code = kaa_logging_init(kaa_client_get_context(kaa_client)->log_collector
                                , log_storage_context
                                , log_upload_strategy_context);
    RETURN_IF_ERROR(error_code, "Failed to init Kaa log collector");
    
    
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

    /**
     * Start Kaa client main loop.
     */
    error_code = kaa_client_start(kaa_client, &emu_device_add_log_record, (void *)kaa_client, LOG_GENERATION_FREQUENCY);
    RETURN_IF_ERROR(error_code, "Failed to start Kaa main loop");
    
    /**
     * Destroy Kaa client.
     */
    kaa_client_destroy(kaa_client);

    dprint("CIO Portal Application Stopped\n");

    return error_code;
}

