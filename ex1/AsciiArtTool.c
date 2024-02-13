#include "AsciiArtTool.h"
#include <stdio.h>
#include <stdlib.h>

RLEList asciiArtRead(FILE *in_stream)
{
    if (!in_stream)
    {
        return NULL;
    }
    RLEList list = RLEListCreate();
    char character = (char)fgetc(in_stream);
    while (character != EOF)
    {
        if (RLEListAppend(list, character) != RLE_LIST_SUCCESS)
        {
            RLEListDestroy(list);
            return NULL;
        }
        character = (char)fgetc(in_stream);
    }
    return list;
}

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream)
{
    if (!out_stream || !list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    int size = RLEListSize(list);
    for (int i = 0; i < size; i++)
    {
        char character = RLEListGet(list, i, NULL);
        fputc(character, out_stream);
    }
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream)
{
    if (!out_stream || !list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEListResult result;
    char *str = RLEListExportToString(list, &result);
    if (result != RLE_LIST_SUCCESS)
    {
        return result;
    }
    fprintf(out_stream, "%s", str);
    free(str);
    return RLE_LIST_SUCCESS;
}
