/*
 * File to manage Notifications
 */


/**
 * @file CIO_EVENT_H_.h
 * @brief CIO CIO_EVENT_H_ 
 *
 * Supplies API for GENERATION OF EVENTS.
 */

#ifndef CIO_EVENT_H_
#define CIO_EVENT_H_

#include <kaa/platform/ext_user_callback.h>
#include "gen/kaa_open_wrt_command_event_class_family_definitions.h"
#include "gen/kaa_open_wrt_command_event_class_family.h"
#include "gen/kaa_cio_event_class_family_definitions.h"
#include "gen/kaa_cio_event_class_family.h"
#include <kaa/kaa_event.h>


#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    kaa_string_t* cmd_id;
    kaa_string_t* result;
    kaa_string_t* execution_response;

} cio_command_event_response_t;

unsigned int cio_notification_response(void *context, char* result,char* response,char* id);
//unsigned int cio_notification_response(void *context, cio_command_event_response_t *data);
kaa_error_t kaa_on_event_listeners(void *context, const kaa_endpoint_id listeners[], size_t listeners_count);
kaa_error_t kaa_on_event_listeners_failed(void *context);
kaa_error_t kaa_on_attached(void *context, const char *user_external_id, const char *endpoint_access_token);
kaa_error_t kaa_on_detached(void *context, const char *endpoint_access_token);
kaa_error_t kaa_on_attach_success(void *context);
kaa_error_t kaa_on_attach_failed(void *context, user_verifier_error_code_t error_code, const char *reason);

void *get_client(void);

#define KAA_DEMO_UNUSED(x) (void)(x);



#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif /* CIO_EVENT_H_ */
