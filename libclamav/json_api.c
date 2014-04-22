/*
 * JSON Object API
 * 
 * Copyright (C) 2014 Cisco Systems, Inc.
 * 
 * Authors: Kevin Lin
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#if HAVE_CONFIG_H
#include "clamav-config.h"
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "libclamav/crypto.h"

#include "cltypes.h"
#include "others.h"
#include "json_api.h"

#ifdef HAVE_JSON
int cli_jsonnull(json_object *obj, const char* key)
{
    json_object *fpobj = json_object_new_object();
    if (NULL == fpobj) {
        cli_errmsg("json: no memory for json string object.\n");
        return CL_EMEM;
    }
    json_object_object_add(obj, key, fpobj);
    return CL_SUCCESS;
}

int cli_jsonstr(json_object *obj, const char* key, const char* s)
{
    json_object *fpobj = json_object_new_string(s);
    if (NULL == fpobj) {
        cli_errmsg("json: no memory for json string object.\n");
        return CL_EMEM;
    }
    json_object_object_add(obj, key, fpobj);
    return CL_SUCCESS;
}

int cli_jsonint(json_object *obj, const char* key, int32_t val)
{
    json_object *fpobj = json_object_new_int(val);
    if (NULL == fpobj) {
        cli_errmsg("json: no memory for json int object.\n");
        return CL_EMEM;
    }
    json_object_object_add(obj, key, fpobj);
    return CL_SUCCESS;
}

#ifdef JSON10
int cli_jsonint64(json_object *obj, const char* key, int64_t i)
{
    json_object *fpobj = json_object_new_int64(i);
    if (NULL == fpobj) {
        cli_errmsg("json: no memory for json int object.\n");
        return CL_EMEM;
    }
    json_object_object_add(obj, key, fpobj);
    return CL_SUCCESS;
}
#else
int cli_jsonint64(json_object *obj, const char* key, int64_t i)
{
    int32_t li, hi;
    json_object *fpobj0, *fpobj1;
    json_object *fparr = json_object_new_array();
    if (NULL == fparr) {
        cli_errmsg("json: no memory for json array object.\n");
        return CL_EMEM;
    }

    hi = (uint32_t)((i & 0xFFFFFFFF00000000) >> 32);
    li = (uint32_t)(i & 0xFFFFFFFF);

    fpobj0 = json_object_new_int(li);
    if (NULL == fpobj0) {
        cli_errmsg("json: no memory for json int object.\n");
        return CL_EMEM;
    }
    fpobj1 = json_object_new_int(hi);
    if (NULL == fpobj1) {
        cli_errmsg("json: no memory for json int object.\n");
        return CL_EMEM;
    }

    /* little-endian array */
    json_object_array_add(fparr, fpobj0);
    json_object_array_add(fparr, fpobj1);
    json_object_object_add(obj, key, fparr);
}
//#define cli_jsonint64(o,n,i) cli_dbgmsg("%s: %lld [%llx]\n", n, i, i)
#endif

int cli_jsonbool(json_object *obj, const char* key, int i)
{
    json_object *fpobj = json_object_new_boolean(i);
    if (NULL == fpobj) {
        cli_errmsg("json: no memory for json boolean object.\n");
        return CL_EMEM;
    }
    json_object_object_add(obj, key, fpobj);
    return CL_SUCCESS;
}

int cli_jsondouble(json_object *obj, const char* key, double d)
{
    json_object *fpobj = json_object_new_double(d);
    if (NULL == fpobj) {
        cli_errmsg("json: no memory for json int object.\n");
        return CL_EMEM;
    }
    json_object_object_add(obj, key, fpobj);
    return CL_SUCCESS;
}
#else
#endif