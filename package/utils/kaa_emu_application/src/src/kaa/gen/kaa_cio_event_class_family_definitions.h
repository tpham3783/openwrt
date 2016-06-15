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

# ifndef KAA_CIO_EVENT_CLASS_FAMILY_DEFINITIONS_H_
# define KAA_CIO_EVENT_CLASS_FAMILY_DEFINITIONS_H_

# include "../kaa_common_schema.h"
# include "../collections/kaa_list.h"

# ifdef __cplusplus
extern "C" {
# endif



typedef struct {
    kaa_string_t * device_id;
    kaa_string_t * timestamp;
    kaa_string_t * alert_name;
    kaa_string_t * trigger_value;
    kaa_string_t * description;
    kaa_string_t * phone_number;
    kaa_string_t * param1;
    kaa_string_t * param2;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} kaa_cio_event_class_family_alert_event_t;

kaa_cio_event_class_family_alert_event_t *kaa_cio_event_class_family_alert_event_create(void);
kaa_cio_event_class_family_alert_event_t *kaa_cio_event_class_family_alert_event_deserialize(avro_reader_t reader);



typedef struct {
    kaa_string_t * device_id;
    kaa_string_t * timestamp;
    kaa_string_t * param1;
    kaa_string_t * param2;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} kaa_cio_event_class_family_reboot_event_t;

kaa_cio_event_class_family_reboot_event_t *kaa_cio_event_class_family_reboot_event_create(void);
kaa_cio_event_class_family_reboot_event_t *kaa_cio_event_class_family_reboot_event_deserialize(avro_reader_t reader);



typedef struct {
    kaa_string_t * device_id;
    kaa_string_t * wwan_ip_address;
    kaa_string_t * wwan_connect_status;
    kaa_string_t * wan_ip_address;
    kaa_string_t * wan_connect_status;
    kaa_string_t * lan_ip_address;
    kaa_string_t * lan_connect_status;
    kaa_string_t * firmware_version;
    kaa_string_t * system_up_time;
    kaa_string_t * wwan_data_usage;
    kaa_string_t * mode;
    kaa_string_t * product_model_number;
    kaa_string_t * icc_id_number;
    kaa_string_t * snr;
    kaa_string_t * signal_strength;
    kaa_string_t * apn;
    kaa_string_t * last_updated_time;
    kaa_string_t * phone_number;
    kaa_string_t * param1;
    kaa_string_t * param2;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} kaa_cio_event_class_family_device_check_in_event_t;

kaa_cio_event_class_family_device_check_in_event_t *kaa_cio_event_class_family_device_check_in_event_create(void);
kaa_cio_event_class_family_device_check_in_event_t *kaa_cio_event_class_family_device_check_in_event_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif