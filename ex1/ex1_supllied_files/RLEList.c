#include "RLEList.h"
#include <stdlib.h>

struct RLEList_t
{
    char character;
    int times;
    RLEList next;
};

static int getPowerOfTen(int exponant); // returns pow(10, exponant)
static int getNumberOfDigits(int val);  // returns number of digits in val

// implement the functions here
RLEList RLEListCreate(void)
{
    RLEList head = (RLEList)malloc(sizeof(RLEList));
    if (!head)
    {
        return NULL;
    }
    head->next = NULL;
    head->times = 0;
    return head;
}

void RLEListDestroy(RLEList list)
{
    if (!list)
    {
        return;
    }
    RLEListDestroy(list->next);
    free(list);
}

RLEListResult RLEListAppend(RLEList list, char value)
{
    if (!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEList iterator = list;
    while (iterator->next)
    {
        iterator = iterator->next;
    }
    if (iterator->times == 0) // we have an initialized node with no data
    {
        iterator->character = value;
        iterator->times = 1;
        return RLE_LIST_SUCCESS;
    }
    if (iterator->character == value)
    {
        // if the last char was the value, we dont need a new node. we'll just update count.
        iterator->times += 1;
    }
    else
    {
        RLEList newNode = (RLEList)malloc(sizeof(RLEList));
        if (!newNode)
        {
            return RLE_LIST_OUT_OF_MEMORY;
        }
        newNode->next = NULL;
        newNode->character = value;
        newNode->times = 1;
        iterator->next = newNode;
    }
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list)
{
    if (!list)
    {
        // list given is NULL
        return -1;
    }
    int sum = 0;
    RLEList iterator = list;
    while (iterator)
    {
        sum += iterator->times;
        iterator = iterator->next;
    }
    return sum;
}

RLEListResult RLEListRemove(RLEList list, int index)
// did you understand the function bubi?
{
    if (!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    int numberOfCharacters = RLEListSize(list);
    if (index < 0 || index >= numberOfCharacters)
    {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    RLEList node = list;
    RLEList afterNode = NULL;
    RLEList prevToNode = node;

    for (node = list; node != NULL; node = node->next)
    {
        afterNode = node->next;
        if (index == 0)
        {
            if (node->times == 0)
            {
                if (node == list)
                {
                    list = list->next;
                    free(prevToNode);
                    return RLE_LIST_SUCCESS;
                }

                free(node);
                prevToNode->next = afterNode;
                return RLE_LIST_SUCCESS;
            }
            node->times -= 1;
            return RLE_LIST_SUCCESS;
        }
        else
        {
            index -= node->times;
        }
        prevToNode = node;
    }
    return RLE_LIST_SUCCESS; // we are not supposed to reach this.
}

char RLEListGet(RLEList list, int index, RLEListResult *result)
{
    if (!list)
    {
        if (!result)
        {
            return 0;
        }
        *result = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    int numberOfCharacters = RLEListSize(list);
    if (index < 0 || index >= numberOfCharacters)
    {
        if (!result)
        {
            return 0;
        }
        *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    RLEList node; // iterator for the list
    for (node = list; node != NULL; node = node->next)
    {
        if (numberOfCharacters == 0)
        {
            if (!result)
            {
                return node->character;
            }

            *result = RLE_LIST_SUCCESS;
            return node->character;
        }
        numberOfCharacters -= node->times;
        if (numberOfCharacters == 0)
        {
            break; // we should'nt exit the for without finding a char, so this is just reprocussion
        }
    }
    if (!result)
    {
        return node->character;
    }
    *result = RLE_LIST_SUCCESS;
    return node->character;
}
static int getNumberOfDigits(int val)
{
    int count = 0;
    while (val > 0)
    {
        count++;
        val = val / 10;
    }
    return count;
}

static int getPowerOfTen(int exponant)
{
    int power = 1;
    for (int i = 0; i < exponant; i++)
    {
        power *= 10;
    }
    return power;
}

char *RLEListExportToString(RLEList list, RLEListResult *result)
{

    if (!list)
    {
        if (!result)
        {
            return NULL;
        }
        *result = RLE_LIST_NULL_ARGUMENT;
        return NULL;
    }
    if (!result)
    {
        return NULL;
    }
    RLEList temp1 = list;
    int len = 0;
    while (temp1)
    {
        // finding the length of the list
        len++;
        temp1 = temp1->next;
    }
    int *numberOfDigits = (int *)malloc(sizeof(int) * len); // an array of the number of digits that the i-th node has
    if (!numberOfDigits)
    {
        *result = RLE_LIST_OUT_OF_MEMORY;
        return 0;
    }
    temp1 = list;
    int sum = 2 * len - 1; // for the character and the \n
    for (int i = 0; i < len; i++)
    {
        numberOfDigits[i] = getNumberOfDigits(temp1->times); // initializing the array
        sum += numberOfDigits[i];                            // the sum of the total amout of charactors. will be the length of the string
        temp1 = temp1->next;
    }
    char *str = (char *)malloc(sizeof(char) * sum + 1); // leaving room for \0
    if (!str)
    {
        *result = RLE_LIST_OUT_OF_MEMORY;
        free(numberOfDigits);
        return 0;
    }
    temp1 = list;
    int k = 0; // the iterator for the array
    for (int i = 0; i < len; i++)
    {
        str[k] = temp1->character; // the character
        k++;
        int val = temp1->times;
        for (int j = numberOfDigits[i]; j > 0; j--) // adding the number of appearances in the string
        {
            int digit = (int)(val / (getPowerOfTen(j - 1))); // most significant digit
            val = val % getPowerOfTen(j - 1);                // updating the number
            str[k] = digit + '0';                            // putting the digit in the string
            k++;
        }
        temp1 = temp1->next;
        str[k] = '\n';
        k++;
    }
    str[k] = '\0';
    *result = RLE_LIST_SUCCESS;
    free(numberOfDigits);
    return str;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
    if (!list)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList iterator = list;
    while (iterator)
    {
        iterator->character = map_function(iterator->character);
        iterator = iterator->next;
    }
    return RLE_LIST_SUCCESS;
}
