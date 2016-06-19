/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "cio_modem_fw_update.h"

Bool TELIT_XXXX_Upgrade(Parameters_t *param) {
    char cmd[BUFSIZ] = {};
    int ret = 0;
    char line[BUFSIZ];
    FILE *fp = NULL;
    processor p = GetProcessor();

    if ((p == i386) || ((p != unknown) && (p != mips))) {
        sprintf(cmd, "fwdldhosti686 -s slqssdk -d 9x15 -p %s 2>&1", (char *) param->directory);
    } else if (p == mips) {
        sprintf(cmd, "fwdldmipsel -s slqssdk -d 9x15 -p %s 2>&1", (char *) param->directory);
    } else {
        return false;
    }

    fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("pipe error ");
        return unknown;
    }

    while (fgets(line, BUFSIZ, fp)) {

        if ((ret = strncmp(line, "INFO: Firmware Upgrade successful!!!", strlen("INFO: Firmware Upgrade successful!!!"))) == 0) {
            return true;
        }
    }

    return false;
}
