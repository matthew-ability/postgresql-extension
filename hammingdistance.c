#include "postgres.h"

#include <ctype.h>
#include <limits.h>

#include "utils/builtins.h"
#include "utils/varlena.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hammingdistance);
Datum
hammingdistance(PG_FUNCTION_ARGS)
{
    const char *str1, *str2;
    int i,count;
    size_t len1,len2;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    str2 = text_to_cstring(PG_GETARG_TEXT_PP(1));
    len1 = strlen(str1);
    len2 = strlen(str2);
    if (len1 != len2){    
        PG_RETURN_INT32(INT_MAX);
    }
    i = 0;
    count = 0;
    while (i < len1) {
        if (str1[i] != str2[i])
            count++;
        i++;
    }

    PG_RETURN_INT32(count);
}
