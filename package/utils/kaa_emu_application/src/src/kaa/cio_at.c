#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "cio_at.h"
#include "cio_defaults.h"
#include <kaa/platform-impl/posix/posix_time.h>


int run_atcommand(char *at_cmd, char *status) {

#ifdef EMULATOR
    strcpy(status, "AT OK");
#else
    FILE *fp;
    char command[MONITORING_CMD_LENGTH];

    char *response;

    if(strlen(at_cmd) > MAX_AT_CMD_LENGTH){

        strcpy(status, "ERROR");
        return 0;
    }
    

    response = (char *) malloc(sizeof(char) * AT_RESPONSE_LENGTH);

    /* Open the command for reading. */

    if (system("at.sh at")) {
        free(response);
        strcpy(status, "Failed:Modem_not_supported");
        return 1;
    }

    sprintf(command, "at.sh %s", at_cmd);
    //fp = popen("/usr/bin/at.sh AT+CSQ","r");
    fp = popen(command, "r");
    if (fp == NULL) {
        dprint("Failed to run command\n");
        strcpy(status, "Failed:Failed_to_run_command");
        free(response);
        return 1;

     }

    fgets(response, AT_RESPONSE_LENGTH, fp);
    snprintf(status,LOG_BUF_SZ ,"%s", response);
    dprint("%s\n", status);

    /* close */
    pclose(fp);
    free(response);
#endif
    return 0;

}

int manufacture_identification(char *status) {
    static char response[100];

    memset(response, '\0', sizeof(response));

    run_atcommand("AT+CGMM", response);
    if (strstr(response, "LE910-EUG")) {
        strcpy(status, "ER1000T-EU");

    } else if (strstr(response, "LE910-NAG")) {
        strcpy(status, "ER1000T-NA");
    } else if (strstr(response, "SIMCOM")) {
        strcpy(status, "SIMCom");
    } else if (strstr(response, "Qualcom")) {
        strcpy(status, "Qualcom");
    } else if (strstr(response, "Failed")) {
        strcpy(status, response);

    } else {
        strcpy(status, "NA");
        return 1;
    }

    return 0;

}

char *get_device_imei()
{
    char *imei_d = NULL;
    char *temp_ptr = NULL;
    char *imei_d_address = &imei_d;
    char cmd[64];
    int i=0;

    memset(cmd,'\0',sizeof(cmd));
    sprintf(cmd,"mkdir -p %s",LOCAL_DIRECTORY);
    system(cmd);

    memset(cmd,'\0',sizeof(cmd));
    sprintf(cmd,"rm -rf %s/*",LOCAL_DIRECTORY);
    system(cmd);

    memset(cmd,'\0',sizeof(cmd));
    sprintf(cmd,"at.sh at+cgsn > %s",IMEI_FILE_PATH);
    if(system(cmd)){
        dprint("Modem not available\n");
    }

    //sleep(2);
    imei_d = (char*) malloc (sizeof(char) * (64) );
    FILE *handler = fopen(IMEI_FILE_PATH,"r");
    memset (imei_d,'\0',sizeof(imei_d));
    if (handler)
   {     // 1- check if file present
                                                // 0-Read content of file
         //getline(&imei_d, &len, handler)
        fread(imei_d,sizeof(char),64,handler);      
            
        temp_ptr=strstr(imei_d,"OK");

        if(temp_ptr) {
            temp_ptr--;
            *temp_ptr=0x0;
            dprint("IMEI %s\n", imei_d);
            fclose(handler);

            return (imei_d);
        }else if(imei_d != '\0') {

            for(i = 0; i < 15; i++)
            {
                if((*imei_d_address > 0x39)||(*imei_d_address < 0x30))
                {               
                    free(imei_d);
                    //free(temp_ptr);
                    fclose(handler);
                    dprint("IMEI Error %s\n", imei_d);
                    return "Failed:IMEI Undefined";
                }
                imei_d_address++;
            }
            *imei_d_address=0x0;
            fclose(handler);
            dprint("IMEI %s\n", imei_d);
            return (imei_d);
            
           // printf("AT ERROR\n");
            //return "Failed:AT ERROR";
        }else{
            //free(imei_d_address);
            free(imei_d);
            //free(temp_ptr);
            fclose(handler);
            dprint("AT ERROR\n");
            return "Failed:AT ERROR";
        }
    }else{
        //free(imei_d_address);
        free(imei_d);
        //free(temp_ptr);
        dprint("No reply from modem\n");
        return "Failed:Modem Unresponsive";
    }

    //return "IMEILINUXPC";
}


char *get_device_type(char* device_type)
{
    manufacture_identification(device_type);
    return device_type;
}
unsigned int get_firmware_version(char* openwrt_version)
{
    char version[8] = {};
    FILE *handler = fopen("/etc/openwrt_version", "r");
    if (handler){
        fgets(version, sizeof(version), handler);
        strcpy(openwrt_version, version);
        pclose(handler);
    }
    return 0;
}
unsigned int get_time(char* cTime){

    kaa_time_t rawtime = KAA_TIME();

    sprintf(cTime,"%lld",(long long)rawtime);
    return 0;
}

unsigned int get_phoneNumber(char* phoneNumber){

    return (execute_system_command("at.sh AT+CNUM=?| grep '+CNUM:' | cut -d, -f2 | awk '{ print $1}'",NULL,phoneNumber));
}

unsigned int get_system_uptime(char* system_uptime)
{
    return (execute_system_command("cat /proc/uptime | awk '{print $1}'",NULL,system_uptime));
}

unsigned int get_lan_ip(char* lan_ip_address)
{
    #ifdef EMULATOR
        return (execute_system_command("ifconfig eth0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'",NULL,lan_ip_address));
    #else
        return (execute_system_command("ifconfig br-lan | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'",NULL,lan_ip_address));
    #endif
}

unsigned int get_wan_ip(char* wan_ip_address)
{
    return (execute_system_command("ifconfig eth0.2| grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'",NULL,wan_ip_address));
}

unsigned int get_wwan_ip(char* wwan_ip_address)
{
    return (execute_system_command("ifconfig wwan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'",NULL,wwan_ip_address));
}

unsigned int get_interface_status(int interface_name)
{
    char ip_address[16] = {};
    switch (interface_name)
    {
        case WWAN:
            get_wwan_ip(ip_address);
            break;
        case WAN:
            get_wan_ip(ip_address);
            break;
        case LAN:
            get_lan_ip(ip_address);
            break;
        default:
            break;
    }
    
    if (strlen(ip_address) > 7)
    {
        return 1;
    }else {
        return 0;
    }
}

unsigned int get_wan_status(void)
{
    char ip_address[16] = {};
    get_wwan_ip(ip_address);
    
    if (strlen(ip_address) > 7)
    {
        return 1;
    }else {
        return 0;
    }
}

unsigned long get_product_id(void)
{
    //TODO: Get product Id once the CIO provided the same in fw.
    return (987654321);
} 

unsigned int get_imei_number(char* imei)
{
    return (run_atcommand("at+cgsn", imei));
}

unsigned int get_mode(char* mode)
{
    int iMode=0;
    char cMode[LOG_BUF_SZ]={};

    if(!execute_system_command("at.sh AT+WS46? | grep '+WS46:' | cut -d, -f1 | awk '{ print $2}'",NULL,cMode)){

        if(strstr(cMode,"ERROR")){
            strcpy(mode,cMode);
            return 0;
        }

    } else {

        strcpy(mode,cMode);
        return 0;
    }


    iMode = atoi(cMode);
    switch(iMode)
    {
        case GERAN_ONLY:
        strcpy(mode,"GERAN_ONLY");
        break;

        case UTRAN_ONLY:
        strcpy(mode,"UTRAN_ONLY");
        break;

        case NW_3GPP:
        strcpy(mode,"3GPP");
        break;

        case EUTRAN_ONLY:
        strcpy(mode,"EUTRAN_ONLY");
        break;

        case GERAN_UTRAN:
        strcpy(mode,"GERAN AND UTRAN");
        break;

        case GERAN_EUTRAN:
        strcpy(mode,"GERAN AND EUTRAN");
        break;

        case UTRAN_EUTRAN:
        strcpy(mode,"UTRAN AND EUTRAN");
        break;

        default:
        strcpy(mode,"ERROR");
        break;


    }



    return 0;
}

unsigned int get_iccid(char* iccid)
{
    return (execute_system_command("at.sh AT+ICCID | grep '+ICCID:' | cut -d: -f2 | awk '{ print $1}'",NULL,iccid));
}

unsigned int get_snr(char* snr)
{
    int snr_value=0;
    char cSNR[LOG_BUF_SZ]={};

    if(!execute_system_command("at.sh AT+CSQ | grep '+CSQ:' | cut -d, -f1 | awk '{ print $2}'",NULL,cSNR)){

        if(strstr(cSNR,"ERROR")){
            strcpy(snr,cSNR);
            return 0;
        }

    } else {

        strcpy(snr,cSNR);
        return 0;
    }


    snr_value = (atoi(cSNR) * 2) - 113;                
   
    sprintf(snr,"%d",snr_value);


    return 0;
}

unsigned int get_signal_strength(char* signal_strength)
{
    
    return (execute_system_command("at.sh AT+CSQ | grep '+CSQ:' | cut -d, -f1 | awk '{print $2}'",NULL,signal_strength));
}

unsigned int get_apn(char* apn)
{
    
     return (execute_system_command("at.sh AT+CGDCONT? | grep '+CGDCONT:' | cut -d: -f3 | awk '{print $1}'",NULL,apn));
}



unsigned int get_wwan_data_usage(char* data_usage)
{
    char RXBytes[LOG_BUF_SZ]={};
    char TXBytes[LOG_BUF_SZ]={};

    execute_system_command("ifconfig eth0 | grep 'RX bytes:' | cut -d: -f2 | awk '{print $1}'",NULL,RXBytes);
    execute_system_command("ifconfig eth0 | grep 'TX bytes:' | cut -d: -f3 | awk '{print $1}'",NULL,TXBytes);

    sprintf(data_usage,"RXBytes :%s,TXBytes :%s",RXBytes,TXBytes);
}


char *get_device_details(char* device_details)
{
    char keyname[32] = {};
    char keyvalue[32] = {};
    char dist_id[32] = {};
    char dist_release[32] = {};
    char dist_revision[32] = {};
    char dist_target[32] = {};
    
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/etc/openwrt_release", "r");
    if (fp == NULL){
        //exit(-1);
        return ("IMEI_ERROR");
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        dprint("%s", line);
        sscanf(line, "%32[^=]=%32[^=]", keyname, keyvalue);
        if (strcmp (keyname,"DISTRIB_ID") == 0) {
            strcpy(dist_id,keyvalue);
            dist_id[strlen(dist_id) - 1] = '\0';
        } else if (strcmp (keyname,"DISTRIB_RELEASE") == 0) {
            strcpy(dist_release,keyvalue); 
            dist_release[strlen(dist_release) - 1] = '\0';
        } else if (strcmp (keyname,"DISTRIB_REVISION") == 0) {
            strcpy(dist_revision,keyvalue); 
            dist_revision[strlen(dist_revision) - 1] = '\0';
        } else if (strcmp (keyname,"DISTRIB_TARGET") == 0) {
            strcpy(dist_target,keyvalue); 
            dist_target[strlen(dist_target) - 1] = '\0';
        }
    }

    fclose(fp);
    if (line){
        free(line);
    }

    sprintf(device_details,"%s_%s.2_%s_%s", dist_id, dist_release, dist_revision, dist_target);
    dprint("%s\n", device_details);
    return device_details;
}

