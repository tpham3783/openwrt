/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fw_upgrade.h
 * Author: russell
 *
 * Created on June 4, 2016, 2:53 PM
 */

#ifndef CIO_MODEM_FW_UPGRADE_H
#define CIO_MODEM_FW_UPGRADE_H

#include <errno.h>
//#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <cio_notification_manager.h>
#include "cio_defaults.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct {
        char *os;
        char *device_at;
        char *device_boot;
        char *upgrade;
        char *directory;
        char *kernel;
        char *filesys;
        char *modem_fw;
        char *password;
    } Parameters_t;
#ifdef __cplusplus
}

#endif
// typedef enum Bool {
//         false = 0, true = 1
//     } Bool;

typedef char Bool; 
   
    #define false  0
    #define true  1
    
    typedef enum Processor {
        unknown = 0, 
        i386 = 1, 
        mipsle = 2 
    } processor;
    
Bool LT_1XXX_Upgrade(Parameters_t *param);
Bool SW_XXXX_Upgrade(Parameters_t *param);
Bool TELIT_XXXX_Upgrade(Parameters_t *param);
processor GetProcessor();
Bool Execute(char *cmd);
Bool DoFileExists(char * fileName);
char Modem_Upgrade(kaa_notification_t *notification);

#endif /* CIO_MODEM_FW_UPGRADE_H */

