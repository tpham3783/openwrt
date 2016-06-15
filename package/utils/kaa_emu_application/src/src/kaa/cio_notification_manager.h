/*
 * File to manage Notifications
 */

/**
 * @file cio_notification_manager.h
 * @brief CIO notification parser
 *
 * Supplies API for parsing and processing notifications.
 */

#ifndef CIO_NOTIFICATION_MANAGER_H_
#define CIO_NOTIFICATION_MANAGER_H_

#include "kaa_notification_manager.h"
#include "gen/kaa_notification_definitions.h"


#ifdef __cplusplus
extern "C" {
#endif



int parse_notification(kaa_notification_t *notification, char* status);
int data_backup(char *url, unsigned int Upload_data, char *status);
int frm_update_check(char *url_rx, char *ckecksum_rx, int conf_rx, char *status);
int config_restore(char *url, char *checksum, char *status);
unsigned int set_firewall_settings(char *firewall_setting,char *hostname,char *status);
unsigned int read_firewall_settings(char *status);
unsigned int execute_system_command(char *cmd,char *response,char *status);
int process_request(char* log_record_message);

unsigned int get_current_status(char* status);


#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif /* CIO_NOTIFICATION_MANAGER_H_ */
