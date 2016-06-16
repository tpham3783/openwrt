/*
 * CIO Event Generation  file
 * 
*/

#include <stdint.h>

#include "cio_fw_update.h"

#include <kaa/kaa_error.h>
 #include <kaa/kaa_profile.h>
#include <kaa/kaa_context.h>
 #include <kaa/kaa_common.h>
#include <kaa/platform/kaa_client.h>
#include "cio_event.h"
#include "gen/kaa_profile_definitions.h"
#include <kaa/utilities/kaa_log.h>
#include <kaa/kaa_logging.h>
#include <kaa/platform-impl/common/ext_log_upload_strategies.h>

 #include "cio_defaults.h"



/*
 * Callback-s which receive result of event listener requests.
 */

kaa_error_t kaa_on_event_listeners(void *context, const kaa_endpoint_id listeners[], size_t listeners_count)
{
    KAA_DEMO_UNUSED(context);
    KAA_DEMO_UNUSED(listeners);

    printf("%zu event listeners received\n", listeners_count);

 

    return KAA_ERR_NONE;
}

kaa_error_t kaa_on_event_listeners_failed(void *context)
{
    KAA_DEMO_UNUSED(context);
    printf("CIO Event event listeners not found\n");
    return KAA_ERR_NONE;
}

/*
 * Callback-s which receive endpoint attach status.
 */
kaa_error_t kaa_on_attached(void *context, const char *user_external_id, const char *endpoint_access_token)
{
    KAA_DEMO_UNUSED(context);
    printf("CIO Event attached to user %s, access token %s\n", user_external_id, endpoint_access_token);
    return KAA_ERR_NONE;
}


kaa_error_t kaa_on_detached(void *context, const char *endpoint_access_token)
{
    KAA_DEMO_UNUSED(context);
    printf("CIO Event detached from user access token %s\n", endpoint_access_token);
    return KAA_ERR_NONE;
}

kaa_error_t kaa_on_attach_success(void *context)
{
    KAA_DEMO_UNUSED(context);

    printf("CIO Event attach success\n");

    const char *fqns[] = { REQUEST_FQN
                           };

    kaa_event_listeners_callback_t listeners_callback = { NULL
                                                        , &kaa_on_event_listeners
                                                        , &kaa_on_event_listeners_failed };

    kaa_error_t error_code = kaa_event_manager_find_event_listeners(
        kaa_client_get_context(get_client())->event_manager, fqns, 1, &listeners_callback);
    if (error_code) {
        KAA_LOG_ERROR(kaa_client_get_context(get_client())->logger, error_code, "Failed to find event listeners");
    }
 
    return error_code;
}

kaa_error_t kaa_on_attach_failed(void *context, user_verifier_error_code_t error_code, const char *reason)
{
    KAA_DEMO_UNUSED(context);

    printf("CIO Event attach failed: error %d, reason '%s'\n", error_code, (reason ? reason : "null"));

   // is_shutdown = true;
    return KAA_ERR_NONE;
}


 //unsigned int cio_notification_response(void *context, cio_command_event_response_t *data)
unsigned int cio_notification_response(void *context, char* result,char* response,char* id)
 {
    dprint("Firing Event[%s][%s][%s]\n",id,result,response);
  
     /**Event****/
    kaa_error_t error_code;
     kaa_endpoint_id_p result_id;
    char kaa_endpnt_id[32]={};
 

    kaa_open_wrt_command_event_class_family_command_event_response_t * notificaton_tx_event =
    kaa_open_wrt_command_event_class_family_command_event_response_create();

    notificaton_tx_event->command_id = kaa_string_copy_create(id);
    notificaton_tx_event->result = kaa_string_copy_create(result);
    notificaton_tx_event->execution_response = kaa_string_copy_create(response);


    //Creating and sending the event block which consists of 1 events
    kaa_event_block_id trx_id = 0;
    error_code = kaa_event_create_transaction(kaa_client_get_context(get_client())->event_manager, &trx_id);
    KAA_RETURN_IF_ERR(error_code);

   
    // error_code = kaa_profile_manager_get_endpoint_id(kaa_client_get_context(get_client())->user_manager,result_id);
    // sprintf(kaa_endpnt_id,"%s",result_id);
    // dprint("Endpoint Id : %s.............................................................\n",kaa_endpnt_id);

    // if(error_code < 1){
    //     printf("Entererd.ttdj.....................\n" );
    //     error_code = kaa_event_manager_send_kaa_open_wrt_command_event_class_family_command_event_response(
    //     kaa_client_get_context(get_client())->event_manager,notificaton_tx_event,NULL );
    //     KAA_RETURN_IF_ERR(error_code);
    // } else {

    //     printf("Entererd......................\n" );
       
    //     error_code = kaa_event_manager_send_kaa_open_wrt_command_event_class_family_command_event_response(
    //     kaa_client_get_context(get_client())->event_manager,notificaton_tx_event,NULL);
    //     KAA_RETURN_IF_ERR(error_code);
    // }

        error_code = kaa_event_manager_send_kaa_open_wrt_command_event_class_family_command_event_response(
        kaa_client_get_context(get_client())->event_manager,notificaton_tx_event,NULL);
        KAA_RETURN_IF_ERR(error_code);

   
     if(notificaton_tx_event) 
     {
        
        kaa_string_destroy(notificaton_tx_event->command_id);
        kaa_string_destroy(notificaton_tx_event->result);
        kaa_string_destroy(notificaton_tx_event->execution_response);
        kaa_data_destroy(notificaton_tx_event);
    }
   
    // error_code = kaa_event_finish_transaction(kaa_client_get_context(get_client())->event_manager, trx_id);
    // KAA_RETURN_IF_ERR(error_code); 
    process_request(response);
    return (0);
    
 }


