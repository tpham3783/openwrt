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

# include <stdint.h>
# include "kaa_cio_event_class_family.h"

# include "../avro_src/avro/io.h"

# include "../kaa_common.h"
# include "../kaa_event.h"
# include "../kaa_error.h"
# include "../utilities/kaa_mem.h"

extern kaa_error_t kaa_event_manager_add_event_to_transaction(kaa_event_manager_t *self, kaa_event_block_id trx_id, const char *fqn, const char *event_data, size_t event_data_size, kaa_endpoint_id_p target);
extern kaa_error_t kaa_event_manager_add_on_event_callback(kaa_event_manager_t *self, const char *fqn, kaa_event_callback_t callback);
extern kaa_error_t kaa_event_manager_send_event(kaa_event_manager_t *self, const char *fqn, const char *event_data, size_t event_data_size, kaa_endpoint_id_p target);
# ifdef kaa_broadcast_event
# undef kaa_broadcast_event
# endif
# define kaa_broadcast_event(context, fqn, fqn_length, event_data, event_data_size) \
    kaa_event_manager_send_event((context), (fqn), (fqn_length), (event_data), (event_data_size), NULL, 0)


typedef struct kaa_cio_event_class_family_ {

    on_kaa_cio_event_class_family_alert_event alert_event_listener;
    void * alert_event_context;
    on_kaa_cio_event_class_family_reboot_event reboot_event_listener;
    void * reboot_event_context;
    on_kaa_cio_event_class_family_device_check_in_event device_check_in_event_listener;
    void * device_check_in_event_context;

    unsigned char is_alert_event_callback_added;
    unsigned char is_reboot_event_callback_added;
    unsigned char is_device_check_in_event_callback_added;

} kaa_cio_event_class_family;

static kaa_cio_event_class_family listeners = {

    NULL, NULL,
    NULL, NULL,
    NULL, NULL,

    0
,
    0
,
    0

};

static void kaa_event_manager_alert_event_listener(const char * event_fqn, const char *data, size_t size, kaa_endpoint_id_p event_source)
{
    (void)event_fqn;
    if (listeners.alert_event_listener) {
        avro_reader_t reader = avro_reader_memory(data, size);
        kaa_cio_event_class_family_alert_event_t * event = kaa_cio_event_class_family_alert_event_deserialize(reader);
        avro_reader_free(reader);
        listeners.alert_event_listener(listeners.alert_event_context, event, event_source);
    }
}

kaa_error_t kaa_event_manager_set_kaa_cio_event_class_family_alert_event_listener(kaa_event_manager_t *self, on_kaa_cio_event_class_family_alert_event listener, void *context)
{
    KAA_RETURN_IF_NIL(self, KAA_ERR_BADPARAM);
    listeners.alert_event_listener = listener;
    listeners.alert_event_context = context;
    if (!listeners.is_alert_event_callback_added) {
        listeners.is_alert_event_callback_added = 1;
        return kaa_event_manager_add_on_event_callback(self, "com.afl.cio.event.cioeventclassfamily.AlertEvent", kaa_event_manager_alert_event_listener);
    }
    return KAA_ERR_NONE;
}

static void kaa_event_manager_reboot_event_listener(const char * event_fqn, const char *data, size_t size, kaa_endpoint_id_p event_source)
{
    (void)event_fqn;
    if (listeners.reboot_event_listener) {
        avro_reader_t reader = avro_reader_memory(data, size);
        kaa_cio_event_class_family_reboot_event_t * event = kaa_cio_event_class_family_reboot_event_deserialize(reader);
        avro_reader_free(reader);
        listeners.reboot_event_listener(listeners.reboot_event_context, event, event_source);
    }
}

kaa_error_t kaa_event_manager_set_kaa_cio_event_class_family_reboot_event_listener(kaa_event_manager_t *self, on_kaa_cio_event_class_family_reboot_event listener, void *context)
{
    KAA_RETURN_IF_NIL(self, KAA_ERR_BADPARAM);
    listeners.reboot_event_listener = listener;
    listeners.reboot_event_context = context;
    if (!listeners.is_reboot_event_callback_added) {
        listeners.is_reboot_event_callback_added = 1;
        return kaa_event_manager_add_on_event_callback(self, "com.afl.cio.event.cioeventclassfamily.RebootEvent", kaa_event_manager_reboot_event_listener);
    }
    return KAA_ERR_NONE;
}

static void kaa_event_manager_device_check_in_event_listener(const char * event_fqn, const char *data, size_t size, kaa_endpoint_id_p event_source)
{
    (void)event_fqn;
    if (listeners.device_check_in_event_listener) {
        avro_reader_t reader = avro_reader_memory(data, size);
        kaa_cio_event_class_family_device_check_in_event_t * event = kaa_cio_event_class_family_device_check_in_event_deserialize(reader);
        avro_reader_free(reader);
        listeners.device_check_in_event_listener(listeners.device_check_in_event_context, event, event_source);
    }
}

kaa_error_t kaa_event_manager_set_kaa_cio_event_class_family_device_check_in_event_listener(kaa_event_manager_t *self, on_kaa_cio_event_class_family_device_check_in_event listener, void *context)
{
    KAA_RETURN_IF_NIL(self, KAA_ERR_BADPARAM);
    listeners.device_check_in_event_listener = listener;
    listeners.device_check_in_event_context = context;
    if (!listeners.is_device_check_in_event_callback_added) {
        listeners.is_device_check_in_event_callback_added = 1;
        return kaa_event_manager_add_on_event_callback(self, "com.afl.cio.event.cioeventclassfamily.DeviceCheckInEvent", kaa_event_manager_device_check_in_event_listener);
    }
    return KAA_ERR_NONE;
}


kaa_error_t kaa_event_manager_send_kaa_cio_event_class_family_alert_event(kaa_event_manager_t *self, kaa_cio_event_class_family_alert_event_t *event, kaa_endpoint_id_p target)
{
    KAA_RETURN_IF_NIL2(self, event, KAA_ERR_BADPARAM);
    size_t event_size = event->get_size(event);
    char *buffer = (char *)KAA_MALLOC((event_size) * sizeof(char));
    KAA_RETURN_IF_NIL(buffer, KAA_ERR_NOMEM);
    avro_writer_t writer = avro_writer_memory(buffer, event_size);
    if (!writer) {
        KAA_FREE(buffer);
        return KAA_ERR_NOMEM;
    }
    event->serialize(writer, event);
    kaa_error_t result = kaa_event_manager_send_event(self, "com.afl.cio.event.cioeventclassfamily.AlertEvent", writer->buf, writer->written, target);
    avro_writer_free(writer);
    return result;
}

kaa_error_t kaa_event_manager_send_kaa_cio_event_class_family_reboot_event(kaa_event_manager_t *self, kaa_cio_event_class_family_reboot_event_t *event, kaa_endpoint_id_p target)
{
    KAA_RETURN_IF_NIL2(self, event, KAA_ERR_BADPARAM);
    size_t event_size = event->get_size(event);
    char *buffer = (char *)KAA_MALLOC((event_size) * sizeof(char));
    KAA_RETURN_IF_NIL(buffer, KAA_ERR_NOMEM);
    avro_writer_t writer = avro_writer_memory(buffer, event_size);
    if (!writer) {
        KAA_FREE(buffer);
        return KAA_ERR_NOMEM;
    }
    event->serialize(writer, event);
    kaa_error_t result = kaa_event_manager_send_event(self, "com.afl.cio.event.cioeventclassfamily.RebootEvent", writer->buf, writer->written, target);
    avro_writer_free(writer);
    return result;
}

kaa_error_t kaa_event_manager_send_kaa_cio_event_class_family_device_check_in_event(kaa_event_manager_t *self, kaa_cio_event_class_family_device_check_in_event_t *event, kaa_endpoint_id_p target)
{
    KAA_RETURN_IF_NIL2(self, event, KAA_ERR_BADPARAM);
    size_t event_size = event->get_size(event);
    char *buffer = (char *)KAA_MALLOC((event_size) * sizeof(char));
    KAA_RETURN_IF_NIL(buffer, KAA_ERR_NOMEM);
    avro_writer_t writer = avro_writer_memory(buffer, event_size);
    if (!writer) {
        KAA_FREE(buffer);
        return KAA_ERR_NOMEM;
    }
    event->serialize(writer, event);
    kaa_error_t result = kaa_event_manager_send_event(self, "com.afl.cio.event.cioeventclassfamily.DeviceCheckInEvent", writer->buf, writer->written, target);
    avro_writer_free(writer);
    return result;
}


kaa_error_t kaa_event_manager_add_kaa_cio_event_class_family_alert_event_event_to_block(kaa_event_manager_t *self, kaa_cio_event_class_family_alert_event_t *event, kaa_endpoint_id_p target, kaa_event_block_id trx_id)
{
    KAA_RETURN_IF_NIL2(self, event, KAA_ERR_BADPARAM);
    size_t event_size = event->get_size(event);
    char *buffer = (char *)KAA_MALLOC((event_size) * sizeof(char));
    KAA_RETURN_IF_NIL(buffer, KAA_ERR_NOMEM);
    avro_writer_t writer = avro_writer_memory(buffer, event_size);
    if (!writer) {
        KAA_FREE(buffer);
        return KAA_ERR_NOMEM;
    }
    event->serialize(writer, event);
    kaa_error_t result = kaa_event_manager_add_event_to_transaction(self, trx_id, "com.afl.cio.event.cioeventclassfamily.AlertEvent", writer->buf, writer->written, target);
    avro_writer_free(writer);
    return result;
}

kaa_error_t kaa_event_manager_add_kaa_cio_event_class_family_reboot_event_event_to_block(kaa_event_manager_t *self, kaa_cio_event_class_family_reboot_event_t *event, kaa_endpoint_id_p target, kaa_event_block_id trx_id)
{
    KAA_RETURN_IF_NIL2(self, event, KAA_ERR_BADPARAM);
    size_t event_size = event->get_size(event);
    char *buffer = (char *)KAA_MALLOC((event_size) * sizeof(char));
    KAA_RETURN_IF_NIL(buffer, KAA_ERR_NOMEM);
    avro_writer_t writer = avro_writer_memory(buffer, event_size);
    if (!writer) {
        KAA_FREE(buffer);
        return KAA_ERR_NOMEM;
    }
    event->serialize(writer, event);
    kaa_error_t result = kaa_event_manager_add_event_to_transaction(self, trx_id, "com.afl.cio.event.cioeventclassfamily.RebootEvent", writer->buf, writer->written, target);
    avro_writer_free(writer);
    return result;
}

kaa_error_t kaa_event_manager_add_kaa_cio_event_class_family_device_check_in_event_event_to_block(kaa_event_manager_t *self, kaa_cio_event_class_family_device_check_in_event_t *event, kaa_endpoint_id_p target, kaa_event_block_id trx_id)
{
    KAA_RETURN_IF_NIL2(self, event, KAA_ERR_BADPARAM);
    size_t event_size = event->get_size(event);
    char *buffer = (char *)KAA_MALLOC((event_size) * sizeof(char));
    KAA_RETURN_IF_NIL(buffer, KAA_ERR_NOMEM);
    avro_writer_t writer = avro_writer_memory(buffer, event_size);
    if (!writer) {
        KAA_FREE(buffer);
        return KAA_ERR_NOMEM;
    }
    event->serialize(writer, event);
    kaa_error_t result = kaa_event_manager_add_event_to_transaction(self, trx_id, "com.afl.cio.event.cioeventclassfamily.DeviceCheckInEvent", writer->buf, writer->written, target);
    avro_writer_free(writer);
    return result;
}

