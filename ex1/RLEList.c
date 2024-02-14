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
static int getListLength(RLEList list); // return the length of a list
static RLEList mergeList(RLEList list); // merges duplicate nodes

RLEList RLEListCreate()
{
    RLEList head = (RLEList)malloc(sizeof(struct RLEList_t));
    if (!head)
    {
        return NULL;
    }
    head->next = NULL;
    head->times = 0;
    head->character = '\0';
    return head;
}

void RLEListDestroy(RLEList list)
{
    if (!list)
    {
        return;
    }
    RLEList temp = list;
    while (list)
    {
        temp = list;
        list = list->next;
        free(temp);
    }
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
        RLEList newNode = (RLEList)malloc(sizeof(struct RLEList_t)); // allocating new node to add to list
        // could've used RLEListCreate, rly didnt want to
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
        if (index < node->times) // we've reached the character we want to remove
        {
            if (node->times == 0)
            {
                if (node == list) // its the first of the list
                {
                    if (afterNode == NULL)
                    // node has already been deleted.
                    {
                        return RLE_LIST_SUCCESS;
                    }
                    // we will move all values from list->next to list, and free list->next
                    list->character = afterNode->character;
                    list->times = afterNode->times;
                    list = afterNode->next;
                    free(afterNode);
                    return RLE_LIST_SUCCESS;
                }

                free(node); // the node is somewhere inside the list.
                prevToNode->next = afterNode;
                list = mergeList(list);
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
    RLEList node = list; // iterator for the list
    for (int i = 0; i < numberOfCharacters;)
    {
        i += node->times; // iterating through the characters
        if (index < i)    // reached the character we want
        {
            if (!result)
            {
                return node->character;
            }
            else
            {
                *result = RLE_LIST_SUCCESS;
                return node->character;
            }
        }
        node = node->next;
    }
    if (!result)
    {
        return node->character;
    }
    *result = RLE_LIST_SUCCESS; // we are not supposed to reach this.
    return node->character;
}

static int getNumberOfDigits(int val)
// recieves a number, returns its number of digits.
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
// recieves an exponant, returns the value 10^exponant
{
    int power = 1;
    for (int i = 0; i < exponant; i++)
    {
        power *= 10;
    }
    return power;
}
static int getListLength(RLEList list)
// recieves list, returns the length of it
{
    int count = 0;
    RLEList iterator = list;
    while (iterator)
    {
        count++;
        iterator = iterator->next;
    }
    return count;
}

char *RLEListExportToString(RLEList list, RLEListResult *result)
{

    if (!list)
    {
        if (result)
        {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }

    int len = getListLength(list);
    if (len == 0)
    {
        char *str = (char *)malloc(sizeof(char) * 1);
        if (!str)
        {
            *result = RLE_LIST_OUT_OF_MEMORY;
            return NULL;
        }
        *result = RLE_LIST_SUCCESS;
        str[0] = '\0';
        return str;
    }
    int *numberOfDigits = (int *)malloc(sizeof(int) * len); // an array of the number of digits that the i-th node has

    if (!numberOfDigits)
    {
        *result = RLE_LIST_OUT_OF_MEMORY;
        return 0;
    }
    RLEList iterator = list;
    int sum = 2 * len; // for the character and the \n
    for (int i = 0; i < len; i++)
    {
        numberOfDigits[i] = getNumberOfDigits(iterator->times); // initializing the array
        sum += numberOfDigits[i];                               // the sum of the total amout of charactors. will be the length of the string
        iterator = iterator->next;
    }
    char *str = (char *)malloc(sizeof(char) * sum + 1); // leaving room for \0
    if (!str)
    {
        *result = RLE_LIST_OUT_OF_MEMORY;
        free(numberOfDigits);
        return 0;
    }
    iterator = list;
    int k = 0; // the iterator for the array
    for (int i = 0; i < len; i++)
    {
        str[k] = iterator->character; // the character
        k++;
        int val = iterator->times;
        for (int j = numberOfDigits[i]; j > 0; j--) // adding the number of appearances in the string
        {
            int digit = (int)(val / (getPowerOfTen(j - 1))); // most significant digit
            val = val % getPowerOfTen(j - 1);                // updating the number
            str[k] = digit + '0';                            // putting the digit as a char in the string
            k++;
        }
        iterator = iterator->next;
        str[k] = '\n';
        k++;
    }
    str[k] = '\0';
    if (result)
    {
        *result = RLE_LIST_SUCCESS;
    }
    free(numberOfDigits);
    return str;
}
static RLEList mergeList(RLEList list)
{
    // merges duplicated nodes. a5->a3->null goes to
    // a8->null
    RLEList iterator = list;
    RLEList iteratorNext;
    while (iterator)
    {
        if (!iterator->next)
        {
            break;
        }
        iteratorNext = iterator->next;
        while (iterator->character == iteratorNext->character)
        {
            iterator->times += iteratorNext->times;
            iterator->next = iteratorNext->next;
            free(iteratorNext);
            iteratorNext = iterator->next;
            if (!iteratorNext)
            {
                break;
            }
        }
        iterator = iterator->next;
    }
    return list;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
    if (!list || !map_function)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList iterator = list;
    while (iterator)
    {
        iterator->character = map_function(iterator->character);
        iterator = iterator->next;
    }

    // now we will check if we can minimize the list
    list = mergeList(list);
    return RLE_LIST_SUCCESS;
}
