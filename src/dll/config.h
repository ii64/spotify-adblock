#ifndef _CONF_H
#define _CONF_H

#define CONF_OK 2
#define CONF_ERR 4
#define CONF_ERR_LOAD CONF_ERR | 8
#define CONF_ERR_FREE CONF_ERR | 16

typedef struct cef_acl
{
    const char *method;
    const char *substr;
} cef_acl_t;

typedef struct config_data
{
    char *name;
    struct cef_acl *cef_acl;
    unsigned cef_acl_count;

    const char **block_host;
    unsigned block_host_count;
} config_data_t;

extern config_data_t *configs;

int config_load_file(char *filename);
int config_close();

#endif