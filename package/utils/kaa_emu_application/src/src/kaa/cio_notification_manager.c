/*
 * CIO Notification parser file
 * 
*/
#include "cio_fw_update.h"
#include "cio_notification_manager.h"

#include "cio_defaults.h"
#include "cio_at.h"
#include "time.h"

#include "cio_event.h"
 
void clear_line_character(char* str,char* chr,unsigned int str_len);
int parse_notification(kaa_notification_t *notification, char* status)
{
    int8_t ret = SUCCESS;
    int command = atoi(notification->command->data);
    kaa_string_t *param1value = NULL;
    kaa_string_t *param2value = NULL;
    memset(status,'\0',LOG_BUF_SZ);
    switch(command) {
        case UPDATE_FW:
            dprint("Firmware update command received\n");           
            //TODO: Check the param for each values
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            param2value = (kaa_string_t *)((kaa_notification_t *)notification)->value2->data;
            kaa_string_t *keep_config = (kaa_string_t *)((kaa_notification_t *)notification)->value3->data;
            int keep_conf = atoi((char*)keep_config);
            dprint("%s %s %d\n",(char*)param1value, (char*)param2value, keep_conf);
            frm_update_check( (char*)param1value, (char*)param2value, keep_conf,status);
            break;
            
        case PERFORM_RESET:
            dprint("Factory Reset Command received\n");
            
            strcpy(status,"Factory Defaulted and Rebooting");
            break;
        
        case RESTORE_CFG:
            dprint("Configuration restore command received\n");
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            param2value = (kaa_string_t *)((kaa_notification_t *)notification)->value2->data;
            config_restore( (char*)param1value, (char*)param2value, status);
            break;
        
        case BACKUP_CFG:
            dprint("Configuration backup command received\n");
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            data_backup((char*)param1value,CONFIG_UPLOAD,status);
            break;
        
        case GET_SYS_LOG:
            dprint("System Log Upload Command received\n");
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            data_backup((char*)param1value,SYSTEM_LOG_UPLOAD,status);
            break;
        
        case GET_KERNEL_LOG:
            dprint("Kernel Log Upload Command received\n");
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            data_backup((char*)param1value,KERNAL_LOG_UPLOAD,status);
            //kaa_client_stop(get_client());
            break;
        
        case EXECUTE_AT_CMD:
            dprint("AT Command Received\n");
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            run_atcommand((char*)param1value, (char*)status);
            break;
        
        case GET_FIREWALL:
            dprint("Get Firewall Command Received\n");
            read_firewall_settings((char *)status);
            break;
        
        case SET_FIREWALL:
            dprint("Set Firewall Command Received\n");
            param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
            param2value = (kaa_string_t *)((kaa_notification_t *)notification)->value2->data;
            set_firewall_settings((char*)param1value,(char*)param2value,NULL);
            strcpy(status,"SUCCESS");
            break;
                
        case CUSTOM_COMMAND:
            execute_custom_command(notification,status);
            break;    
            
        default:
            ret = FAILED;
            break;    
    }
    return ret;
}

int execute_custom_command(kaa_notification_t *notification, char* status)
{
    kaa_string_t *param1value = NULL;
    param1value = (kaa_string_t *)((kaa_notification_t *)notification)->value1->data;
    int sub_command = atoi(param1value);
    switch(sub_command) {
        case SOFT_REBOOT:
            dprint("Soft Reboot Command Received.\n");
            strcpy(status,"Initiating_Reboot");           
            break;
            
        case GET_DEVICE_STATUS:
            dprint("Get Current Command Received.\n");
            get_current_status(status);          
            break;
            
        default:
            dprint("Unknown Sub Command received.\n");
            break;       
    }
    return 0;
}

unsigned int get_current_status(char* status)
{
    //TODO: Get all status info
    //wwanip|wwanstatus|wanip|wanStatus|lanip|lanStatus|fwVersion|/*ProductModel*/|SystemUpTime|
    //DataUsage|Mode|IMEI|ProductId|iccid|snr|signalStrength|apn|lastupdatedTime
    
    char wwanip[LOG_BUF_SZ] = {};
    get_wwan_ip(wwanip);
   // clear_line_character(wwanip,"\n",LOG_BUF_SZ);
    
    int wwan_status = get_interface_status(WWAN);
    
    char wanip[LOG_BUF_SZ] = {};
    get_wan_ip(wanip);
    // clear_line_character(wanip,"\n",LOG_BUF_SZ);

    int wan_status = get_interface_status(WAN);
 
    char lanip[LOG_BUF_SZ] = {};
    get_lan_ip(lanip);
    // clear_line_character(lanip,"\n",LOG_BUF_SZ);
        
    int lan_status = get_interface_status(LAN);
     
    char firmware_version[LOG_BUF_SZ] = {};
    get_firmware_version(firmware_version);
    // clear_line_character(firmware_version,"\n",LOG_BUF_SZ);
            
    char system_uptime[SYSTEM_UPTIME_SIZE] = {};
    get_system_uptime(system_uptime);
    // clear_line_character(system_uptime,"\n",LOG_BUF_SZ);
        
    char wwan_data_usage[LOG_BUF_SZ] = {};
    get_wwan_data_usage(wwan_data_usage);
    // clear_line_character(wwan_data_usage,"\n",LOG_BUF_SZ);
        
    
    char mode[LOG_BUF_SZ] = {};
    get_mode(mode);
    // clear_line_character(mode,"\n",LOG_BUF_SZ);
        
    char IMEI[LOG_BUF_SZ] = {};
    sprintf(IMEI,"%s",get_device_imei());

    long product_id = get_product_id();
    
    char iccid[LOG_BUF_SZ] = {};
    get_iccid(iccid);
    clear_line_character(iccid,"\n",LOG_BUF_SZ);
        
    char snr[LOG_BUF_SZ] = {};
    get_snr(snr);
    // clear_line_character(snr,"\n",LOG_BUF_SZ);

    char signal_strength[LOG_BUF_SZ] = {};
    get_signal_strength(signal_strength);
    // clear_line_character(signal_strength,"\n",LOG_BUF_SZ);

    char apn[LOG_BUF_SZ] = {};
    get_apn(apn);
    // clear_line_character(apn,"\n",LOG_BUF_SZ);

    kaa_time_t rawtime = KAA_TIME();
    
    sprintf(status,"%s|%d|%s|%d|%s|%d|%s|%s|%s|%s|%s|%ld|%s|%s|%s|%s|%lu",
        wwanip,wwan_status,wanip,wan_status,lanip,lan_status,firmware_version,
        system_uptime, wwan_data_usage,mode,IMEI,product_id,iccid,snr,signal_strength,apn,(unsigned long)rawtime);
    // sprintf(status,"%s|%s|%s|%s|%s|%s|%ld|%s|%s|%s|%s|%lu"
    //     ,status,firmware_version,system_uptime, wwan_data_usage,mode,IMEI,product_id,iccid,snr,signal_strength,apn,(unsigned long)rawtime);
    
     // sprintf(status,
     //    "||0||1|1|v0.0.8|328307.81\n|12563456.23,12563456.23\n|3g\n|111222333444555|123456789|5151515151515151|56.25|-45.69|vodafone|1460716566");  
    // sprintf(status,
    //     "|||0\n|192.168.1.150|1|v0.0.8|828307.81\n|12563456.23,12563456.23\n|3g\n|111222333444555|123456789|5151515151515151|56.25|-45.69|vodafone|1460716566");  
    
    dprint("\n\nSTATUS: %s\n\n\n",status); 
    return 0;
}

int data_backup(char *url, unsigned int Upload_data,char *status)
{

    dprint("Recieved url:%s\n",url);
    char *backup_file_path = NULL;
    char *backup_file_name = NULL;
    CURLcode res;
    time_t rawtime;
    struct tm * timeinfo;
    char cmd[100];
    long fail=1;
    char curlErrorMessage[CURL_ERROR_MESSAGE_SIZE];

    time ( &rawtime );
    timeinfo = localtime( &rawtime );

    backup_file_path = malloc(sizeof(char)*64);
    backup_file_name = malloc(sizeof(char)*64);
    system("mkdir -p /tmp/kaa");
    system("rm -rf /tmp/kaa/*");
    
    switch(Upload_data)
    {
        case CONFIG_UPLOAD:
            sprintf(backup_file_name,"backup-AP102B-%d-%d-%d.tar.gz",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,timeinfo->tm_mday);
            sprintf(backup_file_path,"/tmp/kaa/%s",backup_file_name);
            sprintf(cmd,"sysupgrade -b %s",backup_file_path);
            break;
            
        case SYSTEM_LOG_UPLOAD:            
            sprintf(backup_file_name,"SYS-LOG-AP102B-%d-%d-%d.txt",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,timeinfo->tm_mday);
            sprintf(backup_file_path,"/tmp/kaa/%s",backup_file_name);
            sprintf(cmd,"logread > %s",backup_file_path);
            break;
            
        case KERNAL_LOG_UPLOAD:
            sprintf(backup_file_name,"KERNAL-LOG-AP102B-%d-%d-%d.txt",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,timeinfo->tm_mday);
            sprintf(backup_file_path,"/tmp/kaa/%s",backup_file_name);
            sprintf(cmd,"dmesg > %s",backup_file_path);
            break;
            
        default:
            sprintf(status,"Failed:Unrecognized Function Call");
            if (backup_file_path) {
                free(backup_file_path);
                backup_file_path = NULL;
            }
            if (backup_file_name) {
                free(backup_file_name);
                backup_file_name = NULL;
            }
        return 0;
    }
    #ifdef EMULATOR
    sprintf(cmd,"sudo dmesg > %s",backup_file_path);
    dprint("Execute -> %s\n",cmd);
    system(cmd);
    //strcpy(status,"SUCCESS");
    //return 0;
    #else
    system(cmd);
    #endif    
    

    FILE *handler = fopen(backup_file_path,"r");
    if(handler){
        fclose(handler);
        dprint("Closing Handler\n");
    }else {
        sprintf(status,"Failed:To create file %s",backup_file_name);
        dprint("%s\n",status);
        
        if (backup_file_path) {
            free(backup_file_path);
            backup_file_path = NULL;
        }
        if (backup_file_name) {
            free(backup_file_name);
            backup_file_name = NULL;
        }
        return 0;
    }

    CURL *curl;
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";

    curl_global_init(CURL_GLOBAL_ALL);
 
    /* Fill in the file upload field */ 
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "backup",
               CURLFORM_FILE, backup_file_path,
               CURLFORM_END);
 
    curl = curl_easy_init();
    /* initialize custom header list (stating that Expect: 100-continue is not
     wanted */ 
    headerlist = curl_slist_append(headerlist, buf);
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_FAILONERROR,fail); 

        memset(curlErrorMessage,'\0',sizeof(curlErrorMessage));

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorMessage);
 
        /* what URL that receives this POST */ 
        curl_easy_setopt(curl, CURLOPT_URL,url);
        
        //  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        if(DEBUG){
          curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
          curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        }
     
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK) {
          // fprintf(stderr, "curl_easy_perform() failed: %s\n",
          //         curl_easy_strerror(res));
          snprintf(status,LOG_BUF_SZ, "Failed:%s:%s",curl_easy_strerror(res),curlErrorMessage);
        } else {
            sprintf(status,"Success:File Uploaded:%s",backup_file_name);
        }      
     
        /* always cleanup */ 
        curl_easy_cleanup(curl);
     
        /* then cleanup the formpost chain */ 
        curl_formfree(formpost);
        /* free slist */ 
        curl_slist_free_all (headerlist);
    }

    dprint("%s\n",status);

    if (backup_file_path) {
        free(backup_file_path);
        backup_file_path = NULL;
    }
    if (backup_file_name) {
        free(backup_file_name);
        backup_file_name = NULL;
    }

    return 0;
}


unsigned int set_firewall_settings(char *firewall_setting,char *hostname,char *status)
{
    unsigned int index = 0;
    unsigned int i = 0;
    unsigned char cHostname[64] = {};
    char t_data[128] = {};
    unsigned char cfirewall_settings[32] = {};
    unsigned int ifirewall_settings = 0;
    #ifdef EMULATOR
    printf("Firewall Settings:%s Hostname:%s\n",firewall_setting,hostname);
    #else
    dprint("Firewall Settings:%s Hostname:%s\n",firewall_setting,hostname);  

    //sprintf(t_data,"%s",data);

    // index  = (unsigned int)(strstr(t_data,"Firewall_Settings:"));
    // index -= (unsigned int)t_data ;
    // index += (unsigned int)sizeof("Firewall_Settings:");

    // for(i=0;t_data[index]!=0x3A;i++){
    //     cfirewall_settings[i]=t_data[index];
    //     index++;
    //     if(t_data[index]==0x3A) {
    //         break;    
    //     }

    // }
    ifirewall_settings = strtol(firewall_setting,NULL,16);
    
    if(ifirewall_settings & DHCP_RENEW) {
        system("uci set firewall.@rule[0].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[0].target=REJECT");
    }
    if(ifirewall_settings & ALLOW_PING) {
        system("uci set firewall.@rule[1].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[1].target=REJECT");
    }
    if(ifirewall_settings & ALLOW_IGMP) {
        system("uci set firewall.@rule[2].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[2].target=REJECT");
    }
    if(ifirewall_settings & ALLOW_DHCPv6) {
        system("uci set firewall.@rule[3].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[3].target=REJECT");
    }
    if(ifirewall_settings & ALLOW_MLD) {
        system("uci set firewall.@rule[4].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[4].target=REJECT");
    }
    if(ifirewall_settings & ALLOW_ICMPv6_INPUT) {
        system("uci set firewall.@rule[5].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[5].target=REJECT");
    }
    if(ifirewall_settings & ALLOW_ICMPv6_FWD) {
        system("uci set firewall.@rule[6].target=ACCEPT");
    } else {
        system("uci set firewall.@rule[6].target=REJECT");
    }

    system("uci commit");
    system("/etc/init.d/firewall restart");


    // index = ((unsigned int)(strstr(t_data,"Hostname:"))-(unsigned int)t_data) + (unsigned int)sizeof("Hostname:");

    // for(i=0;t_data[index]!=0x00;i++){
    //     cHostname[i]=t_data[index];
    //     index++;
    //     if(t_data[index]==0x00) {
    //         break;    
    //     }

    // }
    memset(t_data,'\0',sizeof(t_data));
    sprintf(t_data,"uci set system.@system[0].hostname=%s",hostname);
    system(t_data);
    system("uci commit");
    system("/etc/init.d/boot restart");
    system("echo $(uci get system.@system[0].hostname) > /proc/sys/kernel/hostname");
    
    #endif 
    return 0;

}

unsigned int read_firewall_settings(char *status)
{
    unsigned int errcode = 0;
    unsigned int firewall_settings = 0;
    static char t_status[LOG_BUF_SZ] ={};
    unsigned char hostname_err = 0;
    #ifdef EMULATOR
    snprintf(status,LOG_BUF_SZ,"Success:Firewall_Settings:FFFF:Err_Code:0000:Hostname:CIO_EMULATOR");
    dprint("%s\n", status);
    #else
    if (!execute_system_command("uci get firewall.@rule[0].target","ACCEPT",t_status)){
        firewall_settings|=DHCP_RENEW;
    } else if (!execute_system_command("uci get firewall.@rule[0].target","REJECT",t_status)) {
        firewall_settings&=(~DHCP_RENEW);

    } else {
        errcode|=DHCP_RENEW;
    }

    memset(t_status,'\0',sizeof(t_status));

    if (!execute_system_command("uci get firewall.@rule[1].target","ACCEPT",t_status)){
        firewall_settings|=ALLOW_PING;
    } else if (!execute_system_command("uci get firewall.@rule[1].target","REJECT",t_status)) {
        firewall_settings&=(~ALLOW_PING);


    } else {
        errcode|=ALLOW_PING;
    }

    memset(t_status,'\0',sizeof(t_status));

    if (!execute_system_command("uci get firewall.@rule[2].target","ACCEPT",t_status)){
        firewall_settings|=ALLOW_IGMP;
    } else if (!execute_system_command("uci get firewall.@rule[2].target","REJECT",t_status)) {
        firewall_settings&=(~ALLOW_IGMP);
    } else {
        errcode|=ALLOW_IGMP;
    }

    memset(t_status,'\0',sizeof(t_status));

    if (!execute_system_command("uci get firewall.@rule[3].target","ACCEPT",t_status)){
        firewall_settings|=ALLOW_DHCPv6;
    } else if (!execute_system_command("uci get firewall.@rule[3].target","REJECT",t_status)) {
        firewall_settings&=(~ALLOW_DHCPv6);
    } else {
        errcode|=ALLOW_DHCPv6;
    }

    memset(t_status,'\0',sizeof(t_status));

    if (!execute_system_command("uci get firewall.@rule[4].target","ACCEPT",t_status)){
        firewall_settings|=ALLOW_MLD;
    } else if (!execute_system_command("uci get firewall.@rule[4].target","REJECT",t_status)) {
        firewall_settings&=(~ALLOW_MLD);
    } else {
        errcode|=ALLOW_MLD;
    }

    memset(t_status,'\0',sizeof(t_status));

    if (!execute_system_command("uci get firewall.@rule[5].target","ACCEPT",t_status)){
        firewall_settings|=ALLOW_ICMPv6_INPUT;
    } else if (!execute_system_command("uci get firewall.@rule[5].target","REJECT",t_status)) {
        firewall_settings&=(~ALLOW_ICMPv6_INPUT);
    } else {
        errcode|=ALLOW_ICMPv6_INPUT;
    }

    memset(t_status,'\0',sizeof(t_status));

    if (!execute_system_command("uci get firewall.@rule[6].target","ACCEPT",t_status)){
        firewall_settings|=ALLOW_ICMPv6_FWD;
    } else if (!execute_system_command("uci get firewall.@rule[6].target","REJECT",t_status)) {
        firewall_settings&=(~ALLOW_ICMPv6_FWD);
    } else {
        errcode|=ALLOW_ICMPv6_FWD;
    }

    firewall_settings|=errcode;

    memset(t_status,'\0',sizeof(t_status));

    execute_system_command("uci get system.@system[0].hostname",NULL,t_status);
   

    snprintf(status,LOG_BUF_SZ,"Success:Firewall_Settings:%04X:Err_Code:%d:Hostname:%s",firewall_settings,errcode,t_status);
    dprint("%s\n", status);
    #endif 
    return 0;
    
}

unsigned int execute_system_command(char *cmd,char *response,char *status)
{
    char exec_cmd[256]={};
    
    char *t_response = NULL;
    memset(status,'\0',sizeof(status));



    t_response = (char *) malloc(sizeof(char) * MAX_LENGTH_OF_SYSTEM_CMD_RESPONSE);
    memset(t_response,'\0',MAX_LENGTH_OF_SYSTEM_CMD_RESPONSE);
    sprintf(exec_cmd,"%s",cmd);
    

    FILE *handler = popen(exec_cmd, "r");
    if (handler){
        fgets(t_response, MAX_LENGTH_OF_SYSTEM_CMD_RESPONSE, handler);
        if(response){
            if(strstr(t_response,response)){
                snprintf(status,LOG_BUF_SZ, "%s", t_response);
                dprint("%s\n", status);
                pclose(handler);
                free(t_response);
                return 0;
            }
        } else {
            if(t_response){
                snprintf(status,LOG_BUF_SZ, "%s", t_response);
            } else {
                strcpy(status,"ERROR");
            }
            
            dprint("%s\n", status);
            pclose(handler);
            free(t_response);
            return 0;
        }
        snprintf(status,LOG_BUF_SZ, "Failed:Unexpected_Response:%s", t_response);
        dprint("%s\n", status);
        pclose(handler);
        free(t_response);
        return 1;
    
    } else {
        dprint("Failed to run SYSTEM command\n");
        snprintf(status,LOG_BUF_SZ, "Failed:Command Execute Error");
        free(t_response);
        return 1;
    }
}

int process_request(char* log_record_message)
{
    if(strcmp(log_record_message,"Success:Firmware_Update")==0){

        dprint("Initiating Firmware Update\n");
        sys_upgrade(FIRMWARE_UPDATE);

    }else if(strcmp(log_record_message,"Success:Config_Update")==0){

        dprint("Initiating Config Update\n");
        sys_upgrade(CONFIG_UPDATE);

    }else if(strcmp(log_record_message,"Initiating_Reboot") == 0){

        dprint("Initiating Reboot\n");
         if(ENABLE_CRITICAL_COMMANDS){
            system("reboot");
        }

    }else if(strcmp(log_record_message,"Factory Defaulted and Rebooting")==0){

       dprint("Factory Defaulted and Rebooting\n");
       
       factory_reset();

    }

    return 0;
}
void clear_line_character(char* str,char* chr,unsigned int str_len)
{
    unsigned int i=0;
    for(i=0;i<str_len;i++)
    {
        if((*str) == (*chr))
        {
            *str = 0x00;
        }

        str++;
    }
}