/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "cio_modem_fw_update.h"

#define decryption "openssl smime -decrypt -in %s -binary -inform DEM -inkey private.pem -out %s"
#define redirect " 2>&1"

Bool LT_1XXX_Upgrade(Parameters_t *param) {

    char args[BUFSIZ] = {};
    char chmod[BUFSIZ] = {};
    char cmd[BUFSIZ] = {};
    char decrypt[BUFSIZ] = {};
    char absEFile[BUFSIZ] = {}, absDFile[BUFSIZ] = {};
    char *buf, *ptr;
    FILE *pipe = NULL;
    long size;
    int ret;

    if (!param) {
        return false;
    }

    size = pathconf(".", _PC_PATH_MAX);
    if ((buf = (char *) malloc((size_t) size)) != NULL) {
        ptr = getcwd(buf, (size_t) size);
    }

    snprintf(absEFile, sizeof (absEFile), "%s/lt_1xxx_upgrade.enc", ptr);
    snprintf(absDFile, sizeof (absDFile), "%s/lt_1xxx_upgrade.dec", ptr);

    //Decrypt upgrade script
    snprintf(decrypt, sizeof (decrypt), decryption, (char *) absEFile, (char *) absDFile);
    if (Execute((char *) decrypt) == false) {
        if (DoFileExists((char *) absDFile) == true) {
            if ((ret = remove(absDFile)) != 0) {
                fprintf(stderr, "Failed to remove %s : %s\n", (char *) absDFile, strerror(errno));
            }
        }
        return false;
    }

    snprintf(chmod, sizeof (chmod), "chmod a+x %s", (char *) absDFile);
    if (Execute((char *) chmod) == false) {
        fprintf(stderr, "Can not set the permission of %s : %s\n", (char *) absDFile, strerror(errno));
    }

    snprintf(cmd, sizeof (cmd),
            "%s %s %s %s %s %s %s %s %s %s %s %s %s",
            absDFile,
            "true",
            param->os,
            (char *) getenv("MODEM_BUSNO"),
            (char *) getenv("MODEM_DEVNO"),
            param->device_at,
            param->device_boot,
            param->upgrade,
            param->directory,
            param->kernel,
            param->filesys,
            param->modem_fw,
            param->password);

    if ((param->os == NULL) ||
            ((char *) getenv("MODEM_BUSNO") == NULL) ||
            ((char *) getenv("MODEM_DEVNO") == NULL) ||
            (param->device_at == NULL) ||
            (param->device_boot == NULL) ||
            (param->upgrade == NULL) ||
            (param->directory == NULL) ||
            (param->kernel == NULL) ||
            (param->filesys == NULL) ||
            (param->modem_fw == NULL) ||
            (param->password == NULL)) {
        fprintf(stderr, "%s : passed a NULL parameter!!!\n", (char *) __FUNCTION__);
        if (DoFileExists((char *) absDFile) == true) {
            if ((ret = remove(absDFile)) != 0) {
                fprintf(stderr, "Failed to remove %s : %s\n", (char *) absDFile, strerror(errno));
            }
        }
        return false;
    }

    //Decrypt upgrade script
    if (Execute((char *) cmd) == false) {
        if (DoFileExists((char *) absDFile) == false) {
            if ((ret = remove(absDFile)) != 0) {
                fprintf(stderr, "Failed to remove %s : %s\n", (char *) absDFile, strerror(errno));
            }
        }
        return false;
    }

    //If we got here then we should have upgraded the modem
    if (DoFileExists((char *) absDFile) == true) {
        memset(cmd, 0, sizeof (cmd));
        snprintf(cmd, sizeof (cmd), "rm -rf %s", (char *) absDFile);
        Execute((char *) cmd);
    }

    return true;
}


