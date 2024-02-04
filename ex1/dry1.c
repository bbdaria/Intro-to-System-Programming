#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

char *duplicateString(char *s, int times)
{
    assert(s);
    assert(times > 0);
    int len = strlen(s);
    char *out = malloc(len * times + 1);
    char *temp = out;
    if (!out)
    {
        return NULL;
    }
    for (int i = 0; i < times; i++)
    {
        strcpy(temp, s);
        temp += len;
    }
    return out;
}

int main(void)
{

    char *string = "fuck";
    int times = 3;
    char *val = duplicateString(string, times);
    printf("%s\n", val);
    return 0;
}
