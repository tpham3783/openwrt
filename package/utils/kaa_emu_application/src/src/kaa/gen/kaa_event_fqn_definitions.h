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

 
 /*
  * This header contains information about all the Event families and
  * events are supported by current SDK.
  */
  
# ifndef KAA_EVENT_FQN_DEFINITIONS_
# define KAA_EVENT_FQN_DEFINITIONS_

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    char       *ecf_name;
    size_t      supported_incoming_fqns_count;
    char      **supported_incoming_fqns;
} event_class_family_t;

static const char * OpenWRTCommandEventClassFamilyFQNS[1] = {
                                                    "com.afl.cio.event.CommandEventResponse"};
static const char * CIOEventClassFamilyFQNS[3] = {
                                                    "com.afl.cio.event.cioeventclassfamily.AlertEvent",
                                                    "com.afl.cio.event.cioeventclassfamily.RebootEvent",
                                                    "com.afl.cio.event.cioeventclassfamily.DeviceCheckInEvent"};

# define SUPPORTED_EVENT_CLASS_FAMILIES_SIZE 2

static const event_class_family_t SUPPORTED_EVENT_CLASS_FAMILIES[SUPPORTED_EVENT_CLASS_FAMILIES_SIZE] =
{
    {
        /* .ecf_name = */                       "OpenWRTCommandEventClassFamily",
        /* .supported_incoming_fqns_count = */  1,
        /* .supported_incoming_fqns = */        (char **)OpenWRTCommandEventClassFamilyFQNS
    },
    {
        /* .ecf_name = */                       "CIOEventClassFamily",
        /* .supported_incoming_fqns_count = */  3,
        /* .supported_incoming_fqns = */        (char **)CIOEventClassFamilyFQNS
    }
};

# ifdef __cplusplus
}      /* extern "C" */
# endif

# endif