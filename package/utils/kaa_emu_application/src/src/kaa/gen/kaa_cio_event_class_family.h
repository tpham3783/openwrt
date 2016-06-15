/**
 *  Copyright 2014-2016 CyberVision, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


# ifndef KAA_CIO_EVENT_CLASS_FAMILY_H
# define KAA_CIO_EVENT_CLASS_FAMILY_H

# include "kaa_cio_event_class_family_definitions.h" 
# include "../kaa_event.h"
# include "../kaa_error.h"

# ifdef __cplusplus
extern "C" {
# endif


/**
 * @brief Listener of alert_event events.
 */
typedef void (* on_kaa_cio_event_class_family_alert_event)(void *context, kaa_cio_event_class_family_alert_event_t *event, kaa_endpoint_id_p source);

/**
 * @brief Listener of reboot_event events.
 */
typedef void (* on_kaa_cio_event_class_family_reboot_event)(void *context, kaa_cio_event_class_family_reboot_event_t *event, kaa_endpoint_id_p source);

/**
 * @brief Listener of device_check_in_event events.
 */
typedef void (* on_kaa_cio_event_class_family_device_check_in_event)(void *context, kaa_cio_event_class_family_device_check_in_event_t *event, kaa_endpoint_id_p source);


/**
 * @brief Set listener for alert_event events.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       listener    Listener callback.
 * @param[in]       context     Listener's context.
 * @return Error code.
 */
kaa_error_t kaa_event_manager_set_kaa_cio_event_class_family_alert_event_listener(kaa_event_manager_t *self, on_kaa_cio_event_class_family_alert_event listener, void *context);

/**
 * @brief Set listener for reboot_event events.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       listener    Listener callback.
 * @param[in]       context     Listener's context.
 * @return Error code.
 */
kaa_error_t kaa_event_manager_set_kaa_cio_event_class_family_reboot_event_listener(kaa_event_manager_t *self, on_kaa_cio_event_class_family_reboot_event listener, void *context);

/**
 * @brief Set listener for device_check_in_event events.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       listener    Listener callback.
 * @param[in]       context     Listener's context.
 * @return Error code.
 */
kaa_error_t kaa_event_manager_set_kaa_cio_event_class_family_device_check_in_event_listener(kaa_event_manager_t *self, on_kaa_cio_event_class_family_device_check_in_event listener, void *context);


/**
 * @brief Send event of type alert_event.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       event       Pointer to the event object.
 * @param[in]       target      The target endpoint of the event (null-terminated string). The size of
 *                              the target parameter should be equal to @link KAA_ENDPOINT_ID_LENGTH @endlink .
 *                              If @code NULL @endcode event will be broadcasted.
 * 
 * @return Error code.
 */
kaa_error_t kaa_event_manager_send_kaa_cio_event_class_family_alert_event(kaa_event_manager_t *self, kaa_cio_event_class_family_alert_event_t *event, kaa_endpoint_id_p target);

/**
 * @brief Send event of type reboot_event.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       event       Pointer to the event object.
 * @param[in]       target      The target endpoint of the event (null-terminated string). The size of
 *                              the target parameter should be equal to @link KAA_ENDPOINT_ID_LENGTH @endlink .
 *                              If @code NULL @endcode event will be broadcasted.
 * 
 * @return Error code.
 */
kaa_error_t kaa_event_manager_send_kaa_cio_event_class_family_reboot_event(kaa_event_manager_t *self, kaa_cio_event_class_family_reboot_event_t *event, kaa_endpoint_id_p target);

/**
 * @brief Send event of type device_check_in_event.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       event       Pointer to the event object.
 * @param[in]       target      The target endpoint of the event (null-terminated string). The size of
 *                              the target parameter should be equal to @link KAA_ENDPOINT_ID_LENGTH @endlink .
 *                              If @code NULL @endcode event will be broadcasted.
 * 
 * @return Error code.
 */
kaa_error_t kaa_event_manager_send_kaa_cio_event_class_family_device_check_in_event(kaa_event_manager_t *self, kaa_cio_event_class_family_device_check_in_event_t *event, kaa_endpoint_id_p target);


/**
 * @brief Add event of type alert_event to the events block.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       event       Pointer to the event object.
 * @param[in]       target      Target of the event (null-terminated string). If NULL - event will be broadcasted.
 * @param[in]       trx_id      Event block id.
 *
 * @return Error code.
 */
kaa_error_t kaa_event_manager_add_kaa_cio_event_class_family_alert_event_event_to_block(kaa_event_manager_t *self, kaa_cio_event_class_family_alert_event_t *event, kaa_endpoint_id_p target, kaa_event_block_id trx_id);

/**
 * @brief Add event of type reboot_event to the events block.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       event       Pointer to the event object.
 * @param[in]       target      Target of the event (null-terminated string). If NULL - event will be broadcasted.
 * @param[in]       trx_id      Event block id.
 *
 * @return Error code.
 */
kaa_error_t kaa_event_manager_add_kaa_cio_event_class_family_reboot_event_event_to_block(kaa_event_manager_t *self, kaa_cio_event_class_family_reboot_event_t *event, kaa_endpoint_id_p target, kaa_event_block_id trx_id);

/**
 * @brief Add event of type device_check_in_event to the events block.
 * 
 * @param[in]       self        Valid pointer to event manager.
 * @param[in]       event       Pointer to the event object.
 * @param[in]       target      Target of the event (null-terminated string). If NULL - event will be broadcasted.
 * @param[in]       trx_id      Event block id.
 *
 * @return Error code.
 */
kaa_error_t kaa_event_manager_add_kaa_cio_event_class_family_device_check_in_event_event_to_block(kaa_event_manager_t *self, kaa_cio_event_class_family_device_check_in_event_t *event, kaa_endpoint_id_p target, kaa_event_block_id trx_id);

# ifdef __cplusplus
}      /* extern "C" */
# endif

# endif // KAA_CIO_EVENT_CLASS_FAMILY_H