#include "postgres.h"

#include <ctype.h>

#include "utils/builtins.h"
#include "utils/varlena.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hammingdistance);
Datum
hammingdistance(PG_FUNCTION_ARGS)
{
	const char *str1;
	const char *str2;
	int i;
    int count;

    str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
    str2 = text_to_cstring(PG_GETARG_TEXT_PP(1));

    i = 0;
    count = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
            count++;
        i++;
    }

	PG_RETURN_INT32(count);
}
