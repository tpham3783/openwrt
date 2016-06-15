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

# include <inttypes.h>
# include <string.h>
# include "../platform/stdio.h"
# include "kaa_cio_event_class_family_definitions.h"
# include "../avro_src/avro/io.h"
# include "../avro_src/encoding.h"
# include "../utilities/kaa_mem.h"

/*
 * AUTO-GENERATED CODE
 */



static void kaa_cio_event_class_family_alert_event_destroy(void *data)
{
    if (data) {
        kaa_cio_event_class_family_alert_event_t *record = (kaa_cio_event_class_family_alert_event_t *)data;

        kaa_string_destroy(record->device_id);
        kaa_string_destroy(record->timestamp);
        kaa_string_destroy(record->alert_name);
        kaa_string_destroy(record->trigger_value);
        kaa_string_destroy(record->description);
        kaa_string_destroy(record->phone_number);
        kaa_string_destroy(record->param1);
        kaa_string_destroy(record->param2);
        kaa_data_destroy(record);
    }
}

static void kaa_cio_event_class_family_alert_event_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        kaa_cio_event_class_family_alert_event_t *record = (kaa_cio_event_class_family_alert_event_t *)data;

        kaa_string_serialize(writer, record->device_id);
        kaa_string_serialize(writer, record->timestamp);
        kaa_string_serialize(writer, record->alert_name);
        kaa_string_serialize(writer, record->trigger_value);
        kaa_string_serialize(writer, record->description);
        kaa_string_serialize(writer, record->phone_number);
        kaa_string_serialize(writer, record->param1);
        kaa_string_serialize(writer, record->param2);
    }
}

static size_t kaa_cio_event_class_family_alert_event_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        kaa_cio_event_class_family_alert_event_t *record = (kaa_cio_event_class_family_alert_event_t *)data;

        record_size += kaa_string_get_size(record->device_id);
        record_size += kaa_string_get_size(record->timestamp);
        record_size += kaa_string_get_size(record->alert_name);
        record_size += kaa_string_get_size(record->trigger_value);
        record_size += kaa_string_get_size(record->description);
        record_size += kaa_string_get_size(record->phone_number);
        record_size += kaa_string_get_size(record->param1);
        record_size += kaa_string_get_size(record->param2);

        return record_size;
    }

    return 0;
}

kaa_cio_event_class_family_alert_event_t *kaa_cio_event_class_family_alert_event_create(void)
{
    kaa_cio_event_class_family_alert_event_t *record = 
            (kaa_cio_event_class_family_alert_event_t *)KAA_CALLOC(1, sizeof(kaa_cio_event_class_family_alert_event_t));

    if (record) {
        record->serialize = kaa_cio_event_class_family_alert_event_serialize;
        record->get_size = kaa_cio_event_class_family_alert_event_get_size;
        record->destroy = kaa_cio_event_class_family_alert_event_destroy;
    }

    return record;
}

kaa_cio_event_class_family_alert_event_t *kaa_cio_event_class_family_alert_event_deserialize(avro_reader_t reader)
{
    kaa_cio_event_class_family_alert_event_t *record = 
            (kaa_cio_event_class_family_alert_event_t *)KAA_MALLOC(sizeof(kaa_cio_event_class_family_alert_event_t));

    if (record) {
        record->serialize = kaa_cio_event_class_family_alert_event_serialize;
        record->get_size = kaa_cio_event_class_family_alert_event_get_size;
        record->destroy = kaa_cio_event_class_family_alert_event_destroy;

        record->device_id = kaa_string_deserialize(reader);
        record->timestamp = kaa_string_deserialize(reader);
        record->alert_name = kaa_string_deserialize(reader);
        record->trigger_value = kaa_string_deserialize(reader);
        record->description = kaa_string_deserialize(reader);
        record->phone_number = kaa_string_deserialize(reader);
        record->param1 = kaa_string_deserialize(reader);
        record->param2 = kaa_string_deserialize(reader);
    }

    return record;
}



static void kaa_cio_event_class_family_reboot_event_destroy(void *data)
{
    if (data) {
        kaa_cio_event_class_family_reboot_event_t *record = (kaa_cio_event_class_family_reboot_event_t *)data;

        kaa_string_destroy(record->device_id);
        kaa_string_destroy(record->timestamp);
        kaa_string_destroy(record->param1);
        kaa_string_destroy(record->param2);
        kaa_data_destroy(record);
    }
}

static void kaa_cio_event_class_family_reboot_event_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        kaa_cio_event_class_family_reboot_event_t *record = (kaa_cio_event_class_family_reboot_event_t *)data;

        kaa_string_serialize(writer, record->device_id);
        kaa_string_serialize(writer, record->timestamp);
        kaa_string_serialize(writer, record->param1);
        kaa_string_serialize(writer, record->param2);
    }
}

static size_t kaa_cio_event_class_family_reboot_event_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        kaa_cio_event_class_family_reboot_event_t *record = (kaa_cio_event_class_family_reboot_event_t *)data;

        record_size += kaa_string_get_size(record->device_id);
        record_size += kaa_string_get_size(record->timestamp);
        record_size += kaa_string_get_size(record->param1);
        record_size += kaa_string_get_size(record->param2);

        return record_size;
    }

    return 0;
}

kaa_cio_event_class_family_reboot_event_t *kaa_cio_event_class_family_reboot_event_create(void)
{
    kaa_cio_event_class_family_reboot_event_t *record = 
            (kaa_cio_event_class_family_reboot_event_t *)KAA_CALLOC(1, sizeof(kaa_cio_event_class_family_reboot_event_t));

    if (record) {
        record->serialize = kaa_cio_event_class_family_reboot_event_serialize;
        record->get_size = kaa_cio_event_class_family_reboot_event_get_size;
        record->destroy = kaa_cio_event_class_family_reboot_event_destroy;
    }

    return record;
}

kaa_cio_event_class_family_reboot_event_t *kaa_cio_event_class_family_reboot_event_deserialize(avro_reader_t reader)
{
    kaa_cio_event_class_family_reboot_event_t *record = 
            (kaa_cio_event_class_family_reboot_event_t *)KAA_MALLOC(sizeof(kaa_cio_event_class_family_reboot_event_t));

    if (record) {
        record->serialize = kaa_cio_event_class_family_reboot_event_serialize;
        record->get_size = kaa_cio_event_class_family_reboot_event_get_size;
        record->destroy = kaa_cio_event_class_family_reboot_event_destroy;

        record->device_id = kaa_string_deserialize(reader);
        record->timestamp = kaa_string_deserialize(reader);
        record->param1 = kaa_string_deserialize(reader);
        record->param2 = kaa_string_deserialize(reader);
    }

    return record;
}



static void kaa_cio_event_class_family_device_check_in_event_destroy(void *data)
{
    if (data) {
        kaa_cio_event_class_family_device_check_in_event_t *record = (kaa_cio_event_class_family_device_check_in_event_t *)data;

        kaa_string_destroy(record->device_id);
        kaa_string_destroy(record->wwan_ip_address);
        kaa_string_destroy(record->wwan_connect_status);
        kaa_string_destroy(record->wan_ip_address);
        kaa_string_destroy(record->wan_connect_status);
        kaa_string_destroy(record->lan_ip_address);
        kaa_string_destroy(record->lan_connect_status);
        kaa_string_destroy(record->firmware_version);
        kaa_string_destroy(record->system_up_time);
        kaa_string_destroy(record->wwan_data_usage);
        kaa_string_destroy(record->mode);
        kaa_string_destroy(record->product_model_number);
        kaa_string_destroy(record->icc_id_number);
        kaa_string_destroy(record->snr);
        kaa_string_destroy(record->signal_strength);
        kaa_string_destroy(record->apn);
        kaa_string_destroy(record->last_updated_time);
        kaa_string_destroy(record->phone_number);
        kaa_string_destroy(record->param1);
        kaa_string_destroy(record->param2);
        kaa_data_destroy(record);
    }
}

static void kaa_cio_event_class_family_device_check_in_event_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        kaa_cio_event_class_family_device_check_in_event_t *record = (kaa_cio_event_class_family_device_check_in_event_t *)data;

        kaa_string_serialize(writer, record->device_id);
        kaa_string_serialize(writer, record->wwan_ip_address);
        kaa_string_serialize(writer, record->wwan_connect_status);
        kaa_string_serialize(writer, record->wan_ip_address);
        kaa_string_serialize(writer, record->wan_connect_status);
        kaa_string_serialize(writer, record->lan_ip_address);
        kaa_string_serialize(writer, record->lan_connect_status);
        kaa_string_serialize(writer, record->firmware_version);
        kaa_string_serialize(writer, record->system_up_time);
        kaa_string_serialize(writer, record->wwan_data_usage);
        kaa_string_serialize(writer, record->mode);
        kaa_string_serialize(writer, record->product_model_number);
        kaa_string_serialize(writer, record->icc_id_number);
        kaa_string_serialize(writer, record->snr);
        kaa_string_serialize(writer, record->signal_strength);
        kaa_string_serialize(writer, record->apn);
        kaa_string_serialize(writer, record->last_updated_time);
        kaa_string_serialize(writer, record->phone_number);
        kaa_string_serialize(writer, record->param1);
        kaa_string_serialize(writer, record->param2);
    }
}

static size_t kaa_cio_event_class_family_device_check_in_event_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        kaa_cio_event_class_family_device_check_in_event_t *record = (kaa_cio_event_class_family_device_check_in_event_t *)data;

        record_size += kaa_string_get_size(record->device_id);
        record_size += kaa_string_get_size(record->wwan_ip_address);
        record_size += kaa_string_get_size(record->wwan_connect_status);
        record_size += kaa_string_get_size(record->wan_ip_address);
        record_size += kaa_string_get_size(record->wan_connect_status);
        record_size += kaa_string_get_size(record->lan_ip_address);
        record_size += kaa_string_get_size(record->lan_connect_status);
        record_size += kaa_string_get_size(record->firmware_version);
        record_size += kaa_string_get_size(record->system_up_time);
        record_size += kaa_string_get_size(record->wwan_data_usage);
        record_size += kaa_string_get_size(record->mode);
        record_size += kaa_string_get_size(record->product_model_number);
        record_size += kaa_string_get_size(record->icc_id_number);
        record_size += kaa_string_get_size(record->snr);
        record_size += kaa_string_get_size(record->signal_strength);
        record_size += kaa_string_get_size(record->apn);
        record_size += kaa_string_get_size(record->last_updated_time);
        record_size += kaa_string_get_size(record->phone_number);
        record_size += kaa_string_get_size(record->param1);
        record_size += kaa_string_get_size(record->param2);

        return record_size;
    }

    return 0;
}

kaa_cio_event_class_family_device_check_in_event_t *kaa_cio_event_class_family_device_check_in_event_create(void)
{
    kaa_cio_event_class_family_device_check_in_event_t *record = 
            (kaa_cio_event_class_family_device_check_in_event_t *)KAA_CALLOC(1, sizeof(kaa_cio_event_class_family_device_check_in_event_t));

    if (record) {
        record->serialize = kaa_cio_event_class_family_device_check_in_event_serialize;
        record->get_size = kaa_cio_event_class_family_device_check_in_event_get_size;
        record->destroy = kaa_cio_event_class_family_device_check_in_event_destroy;
    }

    return record;
}

kaa_cio_event_class_family_device_check_in_event_t *kaa_cio_event_class_family_device_check_in_event_deserialize(avro_reader_t reader)
{
    kaa_cio_event_class_family_device_check_in_event_t *record = 
            (kaa_cio_event_class_family_device_check_in_event_t *)KAA_MALLOC(sizeof(kaa_cio_event_class_family_device_check_in_event_t));

    if (record) {
        record->serialize = kaa_cio_event_class_family_device_check_in_event_serialize;
        record->get_size = kaa_cio_event_class_family_device_check_in_event_get_size;
        record->destroy = kaa_cio_event_class_family_device_check_in_event_destroy;

        record->device_id = kaa_string_deserialize(reader);
        record->wwan_ip_address = kaa_string_deserialize(reader);
        record->wwan_connect_status = kaa_string_deserialize(reader);
        record->wan_ip_address = kaa_string_deserialize(reader);
        record->wan_connect_status = kaa_string_deserialize(reader);
        record->lan_ip_address = kaa_string_deserialize(reader);
        record->lan_connect_status = kaa_string_deserialize(reader);
        record->firmware_version = kaa_string_deserialize(reader);
        record->system_up_time = kaa_string_deserialize(reader);
        record->wwan_data_usage = kaa_string_deserialize(reader);
        record->mode = kaa_string_deserialize(reader);
        record->product_model_number = kaa_string_deserialize(reader);
        record->icc_id_number = kaa_string_deserialize(reader);
        record->snr = kaa_string_deserialize(reader);
        record->signal_strength = kaa_string_deserialize(reader);
        record->apn = kaa_string_deserialize(reader);
        record->last_updated_time = kaa_string_deserialize(reader);
        record->phone_number = kaa_string_deserialize(reader);
        record->param1 = kaa_string_deserialize(reader);
        record->param2 = kaa_string_deserialize(reader);
    }

    return record;
}

