/**
 * @file cio at command header
 * @brief Default defines and macros
 *
 * AT command related header file
 */

#ifndef CIO_AT_H_
#define CIO_AT_H_

#ifdef __cplusplus
extern "C" {
#endif


int run_atcommand(char *at_cmd, char *status);
int manufacture_identification(char *status);
char *get_device_imei();
char *get_device_type(char* device_type);
char *get_device_details(char* device_details);

#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* CIO_AT_H_ */

