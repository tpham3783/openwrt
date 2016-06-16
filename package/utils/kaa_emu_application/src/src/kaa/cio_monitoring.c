/*
 * CIO Health Monitoring parser file
 * 
*/

#include "cio_notification_manager.h"
#include "cio_fw_update.h"
#include "cio_defaults.h"
#include "cio_at.h"
#include "time.h"
#include "cio_event.h"
#include <time.h>
#include "cio_monitoring.h"


#include <stdint.h>



#include <kaa/kaa_error.h>
#include <kaa/kaa_context.h>
 #include <kaa/kaa_common.h>
#include <kaa/platform/kaa_client.h>

#include "gen/kaa_profile_definitions.h"
#include <kaa/utilities/kaa_log.h>
#include <kaa/kaa_logging.h>
#include <kaa/platform-impl/common/ext_log_upload_strategies.h>

 #include "cio_defaults.h"

extern char emulatedImei[16];
extern unsigned int  previous_interface_states;

unsigned int system_powerON(void)
{

	system("uci set cio_monitoring.lan.data_RXBytes=0");
	return 0;
	
}

unsigned int system_health_update(void)
{
	//char status[LOG_BUF_SZ] = {};
	char t_status[LOG_BUF_SZ] = {};
	char cmd[MONITORING_CMD_LENGTH] = {};


	/********************************* System LAN Interface Properties Read *********************************************/
	
	
	//memset(status,'\0',sizeof(status));
	memset(t_status,'\0',sizeof(t_status));
	memset(cmd,'\0',sizeof(cmd));

	if (!execute_system_command("ifconfig br-lan | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'",".",t_status)){
	    sprintf(cmd,"uci set cio_monitoring.lan.ipaddr=%s",t_status);
	    system(cmd);
	    
					

		

	} else {

		system("uci set cio_monitoring.lan.ipaddr=0");
		system("uci set cio_monitoring.lan.connection_state=0");
	}



	



	/******************************************************** System Uptime Update ****************************************************/
	memset(t_status,'\0',sizeof(t_status));
	memset(cmd,'\0',sizeof(cmd));

	if(!execute_system_command("cat /proc/uptime | awk '{print $1;}'",NULL,t_status))
	{
		sprintf(cmd,"uci set cio_monitoring.system.uptime=%s",t_status);
	    system(cmd);
	}




	system("uci commit");

	return 0;
}

unsigned int device_reboot_event(void)
{
	char cTime[LOG_BUF_SZ]={};

	kaa_cio_event_class_family_reboot_event_t * tx_event_data =
    kaa_cio_event_class_family_reboot_event_create();

    
   #ifdef EMULATOR
    tx_event_data->device_id = kaa_string_copy_create(emulatedImei);
    dprint("IMEI: %s\n",emulatedImei);
    #else
    char IMEI[LOG_BUF_SZ] = {};
    sprintf(IMEI,"%s",get_device_imei());
    tx_event_data->device_id = kaa_string_copy_create(IMEI);
    #endif

	get_time(cTime);
    tx_event_data->timestamp = kaa_string_copy_create(cTime);

    tx_event_data->param1 = kaa_string_move_create("Reboot", NULL);
    tx_event_data->param2 = kaa_string_move_create("Reboot", NULL);
    

    kaa_event_block_id trx_id = 0;
    kaa_error_t error_code = kaa_event_create_transaction(kaa_client_get_context(get_client())->event_manager, &trx_id);
    KAA_RETURN_IF_ERR(error_code);

    error_code = kaa_event_manager_send_kaa_cio_event_class_family_reboot_event(
    kaa_client_get_context(get_client())->event_manager,tx_event_data,NULL );
    KAA_RETURN_IF_ERR(error_code);
  

   
    if(tx_event_data) 
    {
       
          kaa_data_destroy(tx_event_data);
    }
   
    error_code = kaa_event_finish_transaction(kaa_client_get_context(get_client())->event_manager, trx_id);
    KAA_RETURN_IF_ERR(error_code); 

    return 0;
}


unsigned int device_alert_event(char* alert_name,char* trigger_value,char* description)
{
	char cTime[LOG_BUF_SZ]={};
	char phone_number[LOG_BUF_SZ]={};

	kaa_cio_event_class_family_alert_event_t * tx_event_data =
    kaa_cio_event_class_family_alert_event_create();


    
    #ifdef EMULATOR
    tx_event_data->device_id = kaa_string_copy_create(emulatedImei);
    dprint("IMEI: %s\n",emulatedImei);
    #else
    char IMEI[LOG_BUF_SZ] = {};
    sprintf(IMEI,"%s",get_device_imei());
    tx_event_data->device_id = kaa_string_copy_create(IMEI);
    #endif
    

    get_time(cTime);
    tx_event_data->timestamp = kaa_string_copy_create(cTime);

    tx_event_data->alert_name = kaa_string_copy_create(alert_name);
    tx_event_data->trigger_value = kaa_string_copy_create(trigger_value);
    tx_event_data->description = kaa_string_copy_create(description);

    get_phoneNumber(phone_number);
    tx_event_data->phone_number = kaa_string_copy_create(phone_number);

    tx_event_data->param1 = kaa_string_move_create("NULL", NULL);
    tx_event_data->param2 = kaa_string_move_create("NULL", NULL);


    kaa_event_block_id trx_id = 0;
    kaa_error_t error_code = kaa_event_create_transaction(kaa_client_get_context(get_client())->event_manager, &trx_id);
    KAA_RETURN_IF_ERR(error_code);

    error_code = kaa_event_manager_send_kaa_cio_event_class_family_alert_event(
    kaa_client_get_context(get_client())->event_manager,tx_event_data,NULL );
    KAA_RETURN_IF_ERR(error_code);
  

   
    if(tx_event_data) 
    {
        
        kaa_data_destroy(tx_event_data);
    }
   
    error_code = kaa_event_finish_transaction(kaa_client_get_context(get_client())->event_manager, trx_id);
    KAA_RETURN_IF_ERR(error_code); 
    return 0;
}




unsigned int device_check_in_event(void)
{

	char wwan_ip[LOG_BUF_SZ]={};
	char wan_ip[LOG_BUF_SZ]={};
	char lan_ip[LOG_BUF_SZ]={};
	char firmware_version[LOG_BUF_SZ]={};
	char uptime[LOG_BUF_SZ]={};
	char wwan_data[LOG_BUF_SZ]={};
	char mode[LOG_BUF_SZ]={};
	char product_id[LOG_BUF_SZ]={};
	char iccid[LOG_BUF_SZ]={};
	char snr[LOG_BUF_SZ]={};
	char signal_strength[LOG_BUF_SZ]={};
	char apn[LOG_BUF_SZ]={};
	char phone_number[LOG_BUF_SZ]={};
    char cTime[LOG_BUF_SZ]={};

    
	kaa_cio_event_class_family_device_check_in_event_t * tx_event_data =
    kaa_cio_event_class_family_device_check_in_event_create();

    #ifdef EMULATOR
    tx_event_data->device_id = kaa_string_copy_create(emulatedImei);
    dprint("IMEI: %s\n",emulatedImei);
    #else
    char IMEI[LOG_BUF_SZ] = {};
    sprintf(IMEI,"%s",get_device_imei());
    tx_event_data->device_id = kaa_string_copy_create(IMEI);
    #endif

    get_wwan_ip(wwan_ip);
    tx_event_data->wwan_ip_address = kaa_string_copy_create(wwan_ip);

    if(strlen(wwan_ip) > 2){
    	tx_event_data->wwan_connect_status = kaa_string_copy_create("1");
        dprint("WWAN Status: 1\n");
    } else {
    	tx_event_data->wwan_connect_status = kaa_string_copy_create("0");
        dprint("WWAN Status: 0\n");
    }

    
    get_wan_ip(wan_ip);
    tx_event_data->wan_ip_address = kaa_string_copy_create(wan_ip);

    if(strlen(wan_ip) > 2){
    	tx_event_data->wan_connect_status = kaa_string_copy_create("1");
    } else {
    	tx_event_data->wan_connect_status = kaa_string_copy_create("0");
    }

    get_lan_ip(lan_ip);
    tx_event_data->lan_ip_address = kaa_string_copy_create(lan_ip);

    if(strlen(lan_ip) > 2){
    	tx_event_data->lan_connect_status = kaa_string_copy_create("1");
    } else {
    	tx_event_data->lan_connect_status = kaa_string_copy_create("0");
    }

    get_firmware_version(firmware_version);
    tx_event_data->firmware_version = kaa_string_copy_create(firmware_version);

    get_system_uptime(uptime);
    tx_event_data->system_up_time = kaa_string_copy_create(uptime);

    get_wwan_data_usage(wwan_data);
    tx_event_data->wwan_data_usage = kaa_string_copy_create(wwan_data);

    get_mode(mode);
    tx_event_data->mode = kaa_string_copy_create(mode);

    
    sprintf(product_id,"%lu",get_product_id());
    tx_event_data->product_model_number = kaa_string_copy_create(product_id);

    get_iccid(iccid);
    tx_event_data->icc_id_number = kaa_string_copy_create(iccid);

    get_snr(snr);
    tx_event_data->snr = kaa_string_copy_create(snr);

    get_signal_strength(signal_strength);
    tx_event_data->signal_strength = kaa_string_copy_create(signal_strength);

    get_apn(apn);
    tx_event_data->apn = kaa_string_copy_create(apn);

    get_time(cTime);
    tx_event_data->last_updated_time = kaa_string_copy_create(cTime);

    get_phoneNumber(phone_number);
    tx_event_data->phone_number = kaa_string_copy_create(phone_number);

    tx_event_data->param1 = kaa_string_move_create("NULL", NULL);
    tx_event_data->param2 = kaa_string_move_create("NULL", NULL);
    kaa_error_t error_code ;
    // kaa_event_block_id trx_id = 0;
    // error_code = kaa_event_create_transaction(kaa_client_get_context(get_client())->event_manager, &trx_id);
    // KAA_RETURN_IF_ERR(error_code);

    error_code = kaa_event_manager_send_kaa_cio_event_class_family_device_check_in_event(
    kaa_client_get_context(get_client())->event_manager,tx_event_data,NULL );
    KAA_RETURN_IF_ERR(error_code);

  	dprint("\nWWan IP : %s\nWan IP : %s\nLan IP : %s\n", wwan_ip, wan_ip, lan_ip);
            
    dprint("Firmware Version : %s\nSystemUpTime : %s\nData Usage : %sMode : %s\nProduct Id : %s\nICC Id : %s\n",
            firmware_version, uptime,wwan_data, mode, product_id,iccid );

    dprint("SNR : %s\nSignal Strength : %sAPN = %s\nLast Updated Time : %s\nPhone No : %s\n",
            snr,signal_strength, apn, cTime, phone_number);
   
    if(tx_event_data) 
    {
       
        kaa_data_destroy(tx_event_data);
    }
   
    // error_code = kaa_event_finish_transaction(kaa_client_get_context(get_client())->event_manager, trx_id);
    // KAA_RETURN_IF_ERR(error_code); 
    return 0;
}


unsigned int updtae_sys_time(void){

	system("ntpd -q -p ptbtime1.ptb.de");

}

unsigned int check_device_alerts(void){

    unsigned int current_interface_states = 0;
    unsigned int iwan,ilan,iwwan;

    iwwan   =  get_interface_status(WWAN);
    iwan    =  get_interface_status(WAN)<<1;
    ilan    =  get_interface_status(LAN)<<2;
    

    dprint("%d...%d...%d",iwwan,iwan,ilan);

    current_interface_states = (iwwan) + (iwan) + (ilan);


    if((current_interface_states & WWAN) != (previous_interface_states & WWAN)){
        dprint("WWAN Alert ................\n");
        if((current_interface_states & WWAN)){
            device_alert_event(INT_STATE_CHANGE,"Up","WWAN");  
      } else {
            device_alert_event(INT_STATE_CHANGE,"Down","WWAN"); 
      }
        
        
    }

    if((current_interface_states & WAN) != (previous_interface_states & WAN)){
          dprint("WAN Alert ................\n");
        if((current_interface_states & WAN)){
            device_alert_event(INT_STATE_CHANGE,"Up","WAN");  
        } else {
            device_alert_event(INT_STATE_CHANGE,"Down","WAN"); 
        }
     
    }
    if((current_interface_states & LAN) != (previous_interface_states & LAN)){
          dprint("LAN Alert ................\n");
        if((current_interface_states & LAN)){
            device_alert_event(INT_STATE_CHANGE,"Up","LAN");  
        } else {
            device_alert_event(INT_STATE_CHANGE,"Down","LAN"); 
        }
       
    }

    dprint("current_interface_states = %d previous_interface_states = %d..................\n",
        current_interface_states,previous_interface_states);

    if(previous_interface_states != current_interface_states){

        previous_interface_states=current_interface_states;
    }

    return 0;
}
