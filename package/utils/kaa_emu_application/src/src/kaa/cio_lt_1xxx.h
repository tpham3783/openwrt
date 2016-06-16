/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lt_1xxx.h
 * Author: russell
 *
 * Created on May 7, 2016, 12:26 PM
 */

#ifndef LT_1XXX_H
#define LT_1XXX_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

    // typedef enum Bool {
    //     false,
    //     true
    // } Bool;

    typedef char Bool;    
    #define false  0
    #define true  1

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

    Bool LT_1XXX_Upgrade(Parameters_t *param);

#ifdef __cplusplus
}
#endif

#endif /* LT_1XXX_H */

