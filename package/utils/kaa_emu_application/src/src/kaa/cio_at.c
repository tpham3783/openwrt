#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "cio_at.h"
#include "cio_defaults.h"


int run_atcommand(char *at_cmd, char *status) {

#ifdef EMULATOR
    strcpy(status, "AT OK");
#else
    FILE *fp;
    char command[150];

    char *response;


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
    sprintf(status, "%s", response);
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

    run_atcommand("AT+CGMI", response);
    if (strstr(response, "Telit")) {
        strcpy(status, "Telit");

    } else if (strstr(response, "Sierra Wireless")) {
        strcpy(status, "Sierra");
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

