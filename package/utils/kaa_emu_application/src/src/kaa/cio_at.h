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



/**********************************************************Monitring APIs*********************************************/
unsigned int get_firmware_version(char* openwrt_version);

unsigned int get_system_uptime(char* system_uptime);
unsigned int get_lan_ip(char* lan_ip_address);
unsigned int get_wan_ip(char* wan_ip_address);
unsigned int get_wwan_ip(char* wwan_ip_address);
unsigned int get_interface_status(int interface_name);
unsigned int get_wan_status(void);
unsigned long get_product_id(void);
unsigned int get_imei_number(char* imei);
unsigned int get_mode(char* mode);
unsigned int get_iccid(char* iccid);
unsigned int get_snr(char* snr);
unsigned int get_signal_strength(char* signal_strength);
unsigned int get_apn(char* apn);
unsigned int get_wwan_data_usage(char* data_usage);
char *get_device_details(char* device_details);
unsigned int get_phoneNumber(char* phoneNumber);
unsigned int get_time(char* cTime);


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* CIO_AT_H_ */

