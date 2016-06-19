/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "cio_modem_fw_update.h"

char Modem_Upgrade(kaa_notification_t *notification){
    
    Parameters_t params = {}, *pParams = &params;

    typedef enum Device_t {
        START, LT1002, SIERRAWIRELESS_7350, TELIT_LE910_NAG, UNKNOWN
    } Device;

    int device = UNKNOWN, i = 0, iterations = 0;
    int device1 = UNKNOWN;
    char os[32] = {}; 
    char deviceid_OS[64] = {}; 


    // printf("Select device to upgrade:\n");
    // printf("    <1> Upgrade LT 1002\n");
    // printf("    <2> Upgrade Sierra Wireless 7350 4G/LTE\n");
    // printf("    <3> Upgrade Telit LE910-NAG 4G/LTE\n");
    // printf("Please select device : ");
    // scanf("%i", &device);
    // if (errno != 0) {
    //     perror("Checking device entry ");
    //     return (0);
    // }
    // printf("Please enter number of iterations : ");
    // scanf("%i", &iterations);
    // if (errno != 0) {
    //     perror("Checking iteration entry ");
    //     return (0);
    // }
    iterations = NUM_MODEM_FW_ITERATION;

   // sscanf((char *)((kaa_notification_t *)notification)->param1->data, "%d|%s", device1,os);
    snprintf(deviceid_OS,60,"%s",(char *)((kaa_notification_t *)notification)->param1->data);

   if (strstr(deviceid_OS, "LT1002")){

        device = LT1002;

   } else if (strstr(deviceid_OS, "SIERRAWIRELESS_7350")) {

        device = SIERRAWIRELESS_7350 ;

   } else if (strstr(deviceid_OS, "TELIT_LE910_NAG")) {

        device = TELIT_LE910_NAG;

   } else{

     return 0;
   }
    

    if (strstr(deviceid_OS, "qnx")){

        strcpy(os,"qnx");

   } else if (strstr(deviceid_OS, "linux")) {

        strcpy(os,"linux") ;

   } else {

    return 0;

   }


    printf("Modem Updation....\n"); 
    switch (device) {
        case LT1002:
        {

            printf("Upgrading LT 1002\n");    
            pParams->os = (char *)os;
            pParams->device_at = (char *)((kaa_notification_t *)notification)->value1->data;
            pParams->device_boot = (char *)((kaa_notification_t *)notification)->param2->data;
            pParams->upgrade = (char *)((kaa_notification_t *)notification)->value2->data;
            pParams->directory = "/root";
            pParams->kernel = (char *)((kaa_notification_t *)notification)->param3->data;
            pParams->filesys = (char *)((kaa_notification_t *)notification)->value3->data;
            pParams->modem_fw = (char *)((kaa_notification_t *)notification)->param4->data;
            pParams->password = (char *)((kaa_notification_t *)notification)->value4->data;
            
            printf("Going to upgrade Modem.......\nOS:%s\ndevice_at:%s\ndevice_boot:%s\nupgrade:%s\ndirectory:%s\nkernel:%s\nfilesys:%s\nmodem_fw:%s\npassword:%s\n ",
            pParams->os,pParams->device_at,pParams->device_boot,pParams->upgrade,pParams->directory,pParams->kernel,
            pParams->filesys,pParams->modem_fw,pParams->password);


            for (i = 0; i < iterations; i++) {
                printf("Running iteration : %d\n", i);
                LT_1XXX_Upgrade(pParams);
            }

            break;
        }
        case SIERRAWIRELESS_7350:
        {
            printf("Upgrading Sierra Wireless 7350\n");
            pParams->os = (char *)os;
            //memcpy(pParams->os, Input("target os <linux or qnx>."), BUFSIZ);

#if 0
            pParams->device_at = malloc(BUFSIZ);
            memcpy(pParams->device_at, " ", BUFSIZ);

            pParams->device_boot = malloc(BUFSIZ);
            memcpy(pParams->device_boot, " ", BUFSIZ);

            pParams->upgrade = malloc(BUFSIZ);
            memcpy(pParams->upgrade, " ", BUFSIZ);
#endif
            pParams->directory = "/root";
            //pParams->directory = malloc(BUFSIZ);
            //memcpy(pParams->directory, Input("target directory <$HOME/downloads>."), BUFSIZ);

#if 0            
            pParams->kernel = malloc(BUFSIZ);
            memcpy(pParams->kernel, " ", BUFSIZ);

            pParams->filesys = malloc(BUFSIZ);
            memcpy(pParams->filesys, " ", BUFSIZ);

            pParams->modem_fw = malloc(BUFSIZ);
            memcpy(pParams->modem_fw, " ", BUFSIZ);

            pParams->password = malloc(BUFSIZ);
            memcpy(pParams->password, " ", BUFSIZ);
#endif
            
            for (i = 0; i < iterations; i++) {
                printf("Running iteration : %d\n", i);
                SW_XXXX_Upgrade(pParams);
            }
            break;
        }
        case TELIT_LE910_NAG:
        {
            printf("Upgrading Telit LE910 NAG\n");
            break;
        }
        default:
        {
            printf("Unsupported device\n");
            break;
        }
    }

    return (0);
}



processor GetProcessor() {
    int ret = 0;
    char line[BUFSIZ];
    FILE *fp = popen("uname -p 2>&1", "r");
    if (fp == NULL) {
        perror("pipe error ");
        return unknown;
    }
    while (fgets(line, BUFSIZ, fp)) {
        if ((ret = strncmp(line, "i386", strlen("i386"))) == 0) {
            return i386;
        } else if ((ret = strncmp(line, "mips", strlen("mips"))) == 0) {
            return mips;
        }
    }
    return unknown;
}

Bool Execute(char *cmd) {
    if (system((char *) cmd) == -1) {
        return false;
    }
    return true;
}

Bool DoFileExists(char * fileName) {
    struct stat buf;
    int i = stat(fileName, &buf);
    // File found
    if (i == 0) {
        return true;
    }
    return false;
}
