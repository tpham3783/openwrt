/*
 * File to manage monitoring activites
 */

/**
 * @file cio_monitoring.h
 * @brief CIO monitoring parser
 *
 * Supplies API for parsing,processing and generating alerts for system related health.
 */

#ifndef CIO_MONITORING_H_
#define CIO_MONITORING_H_

#include "kaa_notification_manager.h"
#include "gen/kaa_notification_definitions.h"


#ifdef __cplusplus
extern "C" {
#endif

unsigned int system_health_update(void);
unsigned int system_powerON(void);

unsigned int updtae_sys_time(void);

unsigned int device_check_in_event(void);
unsigned int device_reboot_event(void);
unsigned int device_alert_event(char* alert_name,char* trigger_value,char* description);
unsigned int check_device_alerts(void);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif /* CIO_MONITORING_H_ */
