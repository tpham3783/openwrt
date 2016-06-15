/*
 * CIO Firmware Update helper file
 * 
*/

#include "cio_fw_update.h"
#include "cio_defaults.h"
#include "cio_notification_manager.h"


unsigned char keep_cofig_flag=0;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int frm_update_check( char *url_rx,char *checksum_rx,int conf_rx,char *status) 
{
    char cmd[256] ; 

    //char bin_filename[BIN_DESTINATION_PATH_SIZE];
    char *temp = NULL;
    unsigned int i;

    static kaa_schema_data_t kaa_schema_data_Input;

    kaa_schema_data_Input.url = malloc(URL_SIZE_IN_BYTES);
    kaa_schema_data_Input.checksum = malloc(URL_SIZE_IN_BYTES);
    
    strcpy(kaa_schema_data_Input.url,url_rx);
    strcpy(kaa_schema_data_Input.checksum,checksum_rx);
    dprint("Url: %s; Checksum: %s\n", kaa_schema_data_Input.url,kaa_schema_data_Input.checksum);

    temp = strstr(kaa_schema_data_Input.url,".bin");
        
    if (temp){
    /* code */
        memset(cmd,'\0',sizeof(cmd));
        sprintf(cmd,"mkdir -p %s",LOCAL_DIRECTORY);
        system(cmd);

        memset(cmd,'\0',sizeof(cmd));
        sprintf(cmd,"rm -rf %s/*",LOCAL_DIRECTORY);
        system(cmd);
        
        if(download_file( kaa_schema_data_Input.url,FW_BIN_PATH,status))    //Curl Dowloads file to FW_BIN_PATH
            return 0;

        if(File_Checker(FW_BIN_PATH,CHECK_FOR_FILE,NULL)){
        
            memset(cmd,'\0',sizeof(cmd));
            sprintf(cmd,"md5sum %s > %s",FW_BIN_PATH,MD5SUM_FILE_PATH);
                
            system(cmd);

            if(File_Checker(MD5SUM_FILE_PATH,READ_DATA,kaa_schema_data_Input.checksum)){
                    
                dprint("Checksum validation Success\n");
                if(kaa_schema_data_Input.url){
                    free(kaa_schema_data_Input.url);
                }

                if(kaa_schema_data_Input.checksum){
                    free(kaa_schema_data_Input.checksum);
                }

                if(conf_rx){
                    // system("mkdir -p /tmp/kaa");
                //  system("rm -rf /tmp/kaa/*");
                    // system("sysupgrade -b /tmp/kaa/backup-AP102B.tar.gz");
                    keep_cofig_flag=1;
                }
                strcpy(status,"Success:Firmware_Update");
            }
            else{
                dprint("Checksum validation Failed\n");

                if(kaa_schema_data_Input.url){
                    free(kaa_schema_data_Input.url);
                }
                if(kaa_schema_data_Input.checksum){
                    free(kaa_schema_data_Input.checksum);
                }
                strcpy(status,"Failed:Checksum Error");
            }
        }
        else{
                
            dprint("Failed:Download_Error\n");

            if(kaa_schema_data_Input.url){
                free(kaa_schema_data_Input.url);
            }

            if(kaa_schema_data_Input.checksum){
                free(kaa_schema_data_Input.checksum);
            }
            strcpy(status,"Failed:Download Error");
        }
    }
    else{
            
        dprint("Failed:File_Format_Error\n");

        if(kaa_schema_data_Input.url){
            free(kaa_schema_data_Input.url);
        }
        if(kaa_schema_data_Input.checksum){
            free(kaa_schema_data_Input.checksum);
        }
        strcpy(status,"Failed:File Format Error");
    }
    return 0;
}

int config_restore(char *url, char *checksum,char *status)
{
    char cmd[256] ; 
    
    char config_filename[CONFIG_DESTINATION_PATH_SIZE];
    char *temp = NULL;
    unsigned int i;

    static kaa_schema_data_t kaa_schema_data_Input;

    kaa_schema_data_Input.url = malloc(URL_SIZE_IN_BYTES);
    kaa_schema_data_Input.checksum = malloc(URL_SIZE_IN_BYTES);
    
    strcpy(kaa_schema_data_Input.url,url);
    strcpy(kaa_schema_data_Input.checksum,checksum);
    dprint("Url: %s; Checksum: %s\n", kaa_schema_data_Input.url,kaa_schema_data_Input.checksum);

    temp = strstr(kaa_schema_data_Input.url,".tar.gz");
        
    if (temp) {
        memset(cmd,'\0',sizeof(cmd));
        sprintf(cmd,"mkdir -p %s",LOCAL_DIRECTORY);
        system(cmd);

        memset(cmd,'\0',sizeof(cmd));
        sprintf(cmd,"rm -rf %s/*",LOCAL_DIRECTORY);
        system(cmd);
        
        if(download_file( kaa_schema_data_Input.url,CONFIG_FILE_PATH,status)) {
            return 0;
        }                                                       //Curl Dowloads file to CONFIG_FILE_PATH
        if(File_Checker(CONFIG_FILE_PATH,CHECK_FOR_FILE,NULL)) {
        
            memset(cmd,'\0',sizeof(cmd));
            sprintf(cmd,"md5sum %s > %s",CONFIG_FILE_PATH,MD5SUM_FILE_PATH);
            system(cmd);

            if(File_Checker(MD5SUM_FILE_PATH,READ_DATA,kaa_schema_data_Input.checksum)) {
                    
                dprint("Checksum validation Success\n");
                if(kaa_schema_data_Input.url) {
                    free(kaa_schema_data_Input.url);
                }

                if(kaa_schema_data_Input.checksum) {
                    free(kaa_schema_data_Input.checksum);
                }        
                strcpy(status,"Success:Config_Update");
            } else {
                dprint("Checksum validation Failed\n");
                if(kaa_schema_data_Input.url) {
                    free(kaa_schema_data_Input.url);
                }
                if(kaa_schema_data_Input.checksum) {
                    free(kaa_schema_data_Input.checksum);
                }
                strcpy(status,"Failed:Checksum Error");
            }

        }else{
            dprint("Failed:Download Error\n");
            if(kaa_schema_data_Input.url) {
                free(kaa_schema_data_Input.url);
            }
            if(kaa_schema_data_Input.checksum) {
                free(kaa_schema_data_Input.checksum);
            }
            strcpy(status,"Failed:Download Error");
        }

    }else {
            
        dprint("Failed:File Format Error\n");
        if(kaa_schema_data_Input.url) {
            free(kaa_schema_data_Input.url);
        }
        if(kaa_schema_data_Input.checksum) {
            free(kaa_schema_data_Input.checksum);
        }
        strcpy(status,"Failed:File Format Error");
    }
    return 0;
}

int File_Checker( char *filename,unsigned int process,char *checksum)
{
    char buffer[64];
    memset(buffer,'\0',sizeof(buffer));

    FILE *handler = fopen(filename,"r");

   if (handler){
        if(process == READ_DATA){    
            fread(buffer,sizeof(char),CHEKSUM_SIZE_IN_BYTES,handler); 
            buffer[CHEKSUM_SIZE_IN_BYTES] = '\0';
            if(!strncmp(buffer,checksum,CHEKSUM_SIZE_IN_BYTES)){
                 fclose(handler);
                 return 1;
            } else {
                 fclose(handler);
                 return 0;
            }

        } else if(process == CHECK_FOR_FILE) {
              fclose(handler);
              return 1;
        } 
    } else  {
          return 0;
    }
    return 0;
}

int sys_upgrade(unsigned int operation)
{
    sleep(SAFE_SLEEP_TIME_SEC);
    char cmd[100] = {};

    switch(operation)
    {
        case FIRMWARE_UPDATE:
        if(keep_cofig_flag){
            keep_cofig_flag=0;
            sprintf(cmd,"sysupgrade -v -c %s",FW_BIN_PATH);
            dprint("Going to update firmware without conifg change .. %s", cmd);

        }else{
            sprintf(cmd,"sysupgrade -v -n %s",FW_BIN_PATH);
            dprint("Going to update firmware .. %s", cmd);
        }
        break;
        case CONFIG_UPDATE:
        sprintf(cmd,"sysupgrade -r %s",CONFIG_FILE_PATH);
        dprint("Going to Config Update with conifg change ... %s", cmd);
        break;

        default: return 1;
    }
    if(ENABLE_CRITICAL_COMMANDS){
        system(cmd);
    }
    return 0;

}

int download_file( char *url,char *outfilename,char *status)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;

  long fail=1;
  char curlErrorMessage[CURL_ERROR_MESSAGE_SIZE];
     
    //char *url = "172.16.2.16:8080/rootdir/DeviceFWs/telit.bin";
    //char outfilename[FILENAME_MAX] = "/home/ushusadmin/Downloads/te1.bin";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_FAILONERROR,fail); 

        memset(curlErrorMessage,'\0',sizeof(curlErrorMessage));

        
        if(DEBUG){
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        }

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorMessage);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 90L); //in seconds
         /* complete within 'n' seconds */
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {

            snprintf(status,127,"Failed:%s:%s",curl_easy_strerror(res),curlErrorMessage);
            curl_easy_cleanup(curl);
            fclose(fp);
            return 1;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);


    }
    return 0;

}
int factory_reset(void)
{
    char cmd[50];
    
    if(ENABLE_CRITICAL_COMMANDS){
       

        memset(cmd,'\0',sizeof(cmd));
        sprintf(cmd,"firstboot -y");
    
        system(cmd);
    
        sleep(SAFE_SLEEP_TIME_SEC);
        memset(cmd,'\0',sizeof(cmd));
        sprintf(cmd,"reboot -f");
     
        system(cmd);

    }
return 0;
}
