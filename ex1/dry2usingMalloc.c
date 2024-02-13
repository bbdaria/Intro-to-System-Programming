#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node_t
{
    int val;
    struct node_t *next;
} *Node;

// functions for ex1
bool isListSorted(Node list);
Node mergeSortedLists(Node list1, Node list2);
int getLenList(Node list);
Node allocateNewList(int size);
void freeList(Node list);

// functions for testing and building
Node createNode(int value);
void printList(Node list);

bool isListSorted(Node list)
{
    if (!list)
    {
        return true;
    }
    int curr = list->val;
    while (list)
    {
        if (list->val < curr)
        {
            return false;
        }
        curr = list->val;
        list = list->next;
    }
    return true;
}

int getLenList(Node list)
{
    // returns length of list
    int count = 0;
    while (list)
    {
        count++;
        list = list->next;
    }
    return count;
}
void freeList(Node list)
{
    if (!list)
    {
        return;
    }
    while (list->next != NULL)
    {
        Node temp = list->next;
        free(list);
        list = temp;
    }
    free(list);
}

Node allocateNewList(int size)
{
    if (size == 0)
    {
        // not possible because we already returned null when one of the lists are null
        return NULL;
    }
    Node root = malloc(sizeof(Node));
    if (!root)
    {
        // first allocation failed
        return NULL;
    }
    Node curr = root;
    for (int i = 1; i < size; i++)
    {
        Node temp = malloc(sizeof(Node));
        if (!temp)
        {
            // i allocation failed, need to free the whole list
            freeList(root);
            return NULL;
        }
        curr->next = temp;
        curr = curr->next;
    }
    return root;
}
Node mergeSortedLists(Node list1, Node list2)
{

    if (!list1 || !list2)
    {
        return NULL;
    }
    if (!isListSorted(list1) || !isListSorted(list2))
    {
        return NULL;
    }
    int len1 = getLenList(list1);             // length of list1
    int len2 = getLenList(list2);             // length of list2
    Node root = allocateNewList(len1 + len2); // returns the first node of the new list
    if (!root)
    {
        return NULL;
    }
    Node curr = root; // so we dont lose the root
    for (int i = 0; i < len1 + len2; i++)
    {
        if (!list1 && list2)
        {
            curr->val = list2->val;
            curr = curr->next;
            list2 = list2->next;
        }
        else
        {
            if (!list2 && list1)
            {
                curr->val = list1->val;
                curr = curr->next;
                list1 = list1->next;
            }
            else
            {
                if (list1->val > list2->val)
                {
                    curr->val = list2->val;
                    curr = curr->next;
                    list2 = list2->next;
                }
                else
                {
                    curr->val = list1->val;
                    curr = curr->next;
                    list1 = list1->next;
                }
            }
        }
    }
    return root;
}

// THE FOLLOWING FUNCTIONS ARE NOT PART OF THE EXCERCISE.
// THEY ARE MEANT FOR TESTING ONLY.
Node createNode(int value)
{
    Node newNode = malloc(sizeof(struct node_t));
    if (!newNode)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->val = value;
    newNode->next = NULL;
    return newNode;
}

// Function to print the values in a linked list
void printList(Node list)
{
    while (list)
    {
        printf("%d -> ", list->val);
        list = list->next;
    }
    printf("NULL\n");
}

int main(void)
{
    // Creating list1: 1 -> 4 -> 9
    Node list1 = createNode(1);
    list1->next = createNode(4);
    list1->next->next = createNode(9);

    // Creating list2: 2 -> 4 -> 8
    Node list2 = createNode(2);
    list2->next = createNode(4);
    list2->next->next = createNode(8);

    // Printing the created lists
    printf("List 1: ");
    printList(list1);

    printf("List 2: ");
    printList(list2);

    Node newList = mergeSortedLists(list1, list2);
    printList(newList);

    // Freeing the allocated memory for the lists
    freeList(list1);
    freeList(list2);
    freeList(newList);

    return 0;
}
