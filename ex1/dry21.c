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

// NOTE THESE FUNCTION ARE FOR TESTING ONLY.
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
    Node root = NULL; // so we wont lose the list
    if (list1->val < list2->val)
    {
        root = list1;
        list1 = list1->next;
        root->next = NULL;
    }
    else
    {
        root = list2;
        list2 = list2->next;
        root->next = NULL;
    }

    Node curr = root; // iterator node

    while (list1 && list2)
    {
        if (list1->val < list2->val)
        {
            curr->next = list1;
            list1 = list1->next;
        }
        else
        {
            curr->next = list2;
            list2 = list2->next;
        }
        curr = curr->next;
    }
    // lists might not be same length, so the longest will be added in the end
    if (list1)
    {
        curr->next = list1;
    }
    if (list2)
    {
        curr->next = list2;
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

    return 0;
}
