/*
 * Defaults
 */
#ifndef CIO_FW_UPDATE_H_
#define CIO_FW_UPDATE_H_


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Input data
 */

typedef struct {
    char *url;
    char *checksum;
    int conf;

}kaa_schema_data_t;


/*
 * Function initiating firmware update
 */


int download_file(char *url, char *outfilename, char *status);
int md5_Text_Read(char *filename, unsigned int process,char *checksum);
int sys_upgrade(unsigned int operation);
int factory_reset(void);


#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif /* CIO_FW_UPDATE_H_ */

