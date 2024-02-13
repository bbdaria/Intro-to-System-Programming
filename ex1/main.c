#include <stdio.h>
#include "RLEList.h"
#include "AsciiArtTool.h"

#define FLAG 1
#define SOURCE 2
#define TARGET 3

char invertingFunction(char character)
{
    if (character == '@')
    {
        return ' ';
    }
    else
    {
        if (character == ' ')
        {
            return '@';
        }
    }
    return character;
}
void runCommand(char flag, FILE *source, FILE *target)
{
    RLEList list = asciiArtRead(source);
    switch (flag)
    {
    case 'i':
        if (RLEListMap(list, invertingFunction) == RLE_LIST_SUCCESS)
        {
            asciiArtPrint(list, target);
        }
        break;
    default:
        // we already checked for default. only 'i', 'e' possible.
        asciiArtPrintEncoded(list, target);
    }
    RLEListDestroy(list);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        return 0;
    }
    char *flag = argv[FLAG];
    char *source = argv[SOURCE];
    char *target = argv[TARGET];

    if (flag[0] != '-' || flag[2] != '\0')
    {
        return 0;
    }
    if (flag[1] == 'i' || flag[1] == 'e')
    {
        FILE *targetFile = fopen(target, "w");
        FILE *sourceFile = fopen(source, "r");
        if (targetFile == NULL || sourceFile == NULL)
        {
            if (targetFile)
            {
                fclose(targetFile);
            }
            else if (sourceFile)
            {
                fclose(sourceFile);
            }
            return 0;
        }
        runCommand(flag[1], sourceFile, targetFile);
        fclose(targetFile);
        fclose(sourceFile);
    }
    return 0;
}
