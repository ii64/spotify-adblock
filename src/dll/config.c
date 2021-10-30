#ifndef _CONF
#define _CONF

#include <stdlib.h>
#include <cyaml/cyaml.h>
#include <string.h>

#include "lib/log.c/src/log.h"
#include "config.h"

#ifndef YAML_LOG_LEVEL
#define YAML_LOG_LEVEL CYAML_LOG_DEBUG
#endif

#define UNUSED(_x) ((void)(_x))

void my_cyaml_logger(
    cyaml_log_t level,
    void *ctx,
    const char *fmt,
    va_list args)
{
    static const char ffmt[] = "libcyaml: %7.7s: %s";
    static const char *const strings[] = {
        [CYAML_LOG_DEBUG] = "DEBUG",
        [CYAML_LOG_INFO] = "INFO",
        [CYAML_LOG_NOTICE] = "NOTICE",
        [CYAML_LOG_WARNING] = "WARNING",
        [CYAML_LOG_ERROR] = "ERROR",
    };

    UNUSED(ctx);

    size_t fmt_sz = snprintf(NULL, 0, ffmt, strings[level], fmt);
    char *fmter = (char *)malloc(fmt_sz);
    sprintf(fmter, ffmt, strings[level], fmt);

    int sz = sizeof(char) * 256;
    char *buf = (char *)malloc(sz);
    int n;
    for (;;)
    {
        n = vsnprintf(buf, sz, fmter, args);
        if (n >= sz)
        {
            sz = sz + (sizeof(char) * 256);
            buf = (char *)realloc(buf, sz); // resize
            continue;
        }
        break;
    }
    buf[strlen(buf) - 2] = '\0'; // newline
    log_debug(buf);
    free(fmter);
    free(buf);
}

static const cyaml_config_t config = {
    .log_fn = my_cyaml_logger,
    .mem_fn = cyaml_mem,
    .log_level = YAML_LOG_LEVEL,
};

// cef_acl
static const cyaml_schema_field_t cef_acl_fields_schema[] = {
    CYAML_FIELD_STRING_PTR(
        "method", CYAML_FLAG_POINTER, struct cef_acl, method, 0, CYAML_UNLIMITED),
    CYAML_FIELD_STRING_PTR(
        "substr", CYAML_FLAG_POINTER, struct cef_acl, substr, 0, CYAML_UNLIMITED),
    CYAML_FIELD_END};
static const cyaml_schema_value_t cef_acl_entry = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                        struct cef_acl, cef_acl_fields_schema),
};

// string array
static const cyaml_schema_value_t str_array_entry = {
    CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, CYAML_UNLIMITED),
};

// base
static const cyaml_schema_field_t base_entry[] = {
    CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER,
                           struct config_data, name,
                           0, CYAML_UNLIMITED),

    CYAML_FIELD_SEQUENCE_COUNT("block_host", CYAML_FLAG_POINTER,
                               struct config_data, block_host, block_host_count,
                               &str_array_entry, 0, CYAML_UNLIMITED),

    CYAML_FIELD_SEQUENCE_COUNT("cef_acl", CYAML_FLAG_POINTER,
                               struct config_data, cef_acl, cef_acl_count,
                               &cef_acl_entry, 0, CYAML_UNLIMITED),
    CYAML_FIELD_END};
static const cyaml_schema_value_t cfg_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
                        struct config_data, base_entry),
};

//
config_data_t *configs = NULL;
//

int config_load_file(char *filename)
{
    int ret = CONF_ERR;
    log_debug("config loading: %s", filename);
    cyaml_err_t err = cyaml_load_file(filename, &config,
                                      &cfg_schema, (cyaml_data_t **)&configs, NULL);
    if (err != CYAML_OK)
    {
        ret = CONF_ERR_LOAD;
        goto err;
    }
    log_debug("config loaded: %s", filename);
    ret = CONF_OK;
err:
    return ret;
}

int config_close()
{
    int ret;
    cyaml_err_t err = cyaml_free(&config, &cfg_schema, configs, 0);
    if (err != CYAML_OK)
    {
        ret = CONF_ERR_FREE;
        goto err;
    }
    ret = CONF_OK;
err:
    return ret;
}

#endif