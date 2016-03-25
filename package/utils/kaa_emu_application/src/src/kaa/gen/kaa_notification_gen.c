/*
 * Copyright 2014-2015 CyberVision, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

# include <inttypes.h>
# include <string.h>
# include "../platform/stdio1.h"
# include "kaa_notification_gen.h"
# include "../avro_src/avro/io.h"
# include "../avro_src/encoding.h"
# include "../utilities/kaa_mem.h"

/*
 * AUTO-GENERATED CODE
 */


static void kaa_notification_device_notification_destroy(void *data)
{
    if (data) {
        kaa_notification_device_notification_t *record = (kaa_notification_device_notification_t *)data;

        kaa_string_destroy(record->command_id);
        kaa_string_destroy(record->command);
        kaa_string_destroy(record->param1);
        kaa_string_destroy(record->value1);
        kaa_string_destroy(record->param2);
        kaa_string_destroy(record->value2);
        kaa_string_destroy(record->param3);
        kaa_string_destroy(record->value3);
        kaa_string_destroy(record->param4);
        kaa_string_destroy(record->value4);
        kaa_data_destroy(record);
    }
}

static void kaa_notification_device_notification_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        kaa_notification_device_notification_t *record = (kaa_notification_device_notification_t *)data;

        kaa_string_serialize(writer, record->command_id);
        kaa_string_serialize(writer, record->command);
        kaa_string_serialize(writer, record->param1);
        kaa_string_serialize(writer, record->value1);
        kaa_string_serialize(writer, record->param2);
        kaa_string_serialize(writer, record->value2);
        kaa_string_serialize(writer, record->param3);
        kaa_string_serialize(writer, record->value3);
        kaa_string_serialize(writer, record->param4);
        kaa_string_serialize(writer, record->value4);
    }
}

static size_t kaa_notification_device_notification_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        kaa_notification_device_notification_t *record = (kaa_notification_device_notification_t *)data;

        record_size += kaa_string_get_size(record->command_id);
        record_size += kaa_string_get_size(record->command);
        record_size += kaa_string_get_size(record->param1);
        record_size += kaa_string_get_size(record->value1);
        record_size += kaa_string_get_size(record->param2);
        record_size += kaa_string_get_size(record->value2);
        record_size += kaa_string_get_size(record->param3);
        record_size += kaa_string_get_size(record->value3);
        record_size += kaa_string_get_size(record->param4);
        record_size += kaa_string_get_size(record->value4);

        return record_size;
    }

    return 0;
}

kaa_notification_device_notification_t *kaa_notification_device_notification_create()
{
    kaa_notification_device_notification_t *record = 
            (kaa_notification_device_notification_t *)KAA_CALLOC(1, sizeof(kaa_notification_device_notification_t));

    if (record) {
        record->serialize = kaa_notification_device_notification_serialize;
        record->get_size = kaa_notification_device_notification_get_size;
        record->destroy = kaa_notification_device_notification_destroy;
    }

    return record;
}

kaa_notification_device_notification_t *kaa_notification_device_notification_deserialize(avro_reader_t reader)
{
    kaa_notification_device_notification_t *record = 
            (kaa_notification_device_notification_t *)KAA_MALLOC(sizeof(kaa_notification_device_notification_t));

    if (record) {
        record->serialize = kaa_notification_device_notification_serialize;
        record->get_size = kaa_notification_device_notification_get_size;
        record->destroy = kaa_notification_device_notification_destroy;

        record->command_id = kaa_string_deserialize(reader);
        record->command = kaa_string_deserialize(reader);
        record->param1 = kaa_string_deserialize(reader);
        record->value1 = kaa_string_deserialize(reader);
        record->param2 = kaa_string_deserialize(reader);
        record->value2 = kaa_string_deserialize(reader);
        record->param3 = kaa_string_deserialize(reader);
        record->value3 = kaa_string_deserialize(reader);
        record->param4 = kaa_string_deserialize(reader);
        record->value4 = kaa_string_deserialize(reader);
    }

    return record;
}

