#include <stdlib.h>

#include "doublylinkedlist.h"

// Doubly_Linked_List_Node Functions
Doubly_Linked_List_Node *CreateDoublyLinkedListNode(void *object)
{
    Doubly_Linked_List_Node *node = malloc(sizeof *node);
    node->object = object;
    node->next = NULL;
    node->prev = NULL;
    node->list = NULL;

    return node;
}

void DeleteDoublyLinkedListNode(Doubly_Linked_List_Node *node)
{
    free(node);
}

void DoublyLinkedListNodeInsertAfter(Doubly_Linked_List_Node *node_in_list, Doubly_Linked_List_Node *new_node)
{
    new_node->next = node_in_list->next;
    new_node->prev = node_in_list;
    node_in_list->next = new_node;
    new_node->list = node_in_list->list;
}

void DoublyLinkedListNodeInsertBefore(Doubly_Linked_List_Node *node_in_list, Doubly_Linked_List_Node *new_node)
{
    new_node->prev = node_in_list->prev;
    new_node->next = node_in_list;
    node_in_list->prev = new_node;
    new_node->list = node_in_list->list;
}

Doubly_Linked_List_Node *DoublyLinkedListNodeAdvance(Doubly_Linked_List_Node *node, unsigned long count)
{
    while (count > 0 && node != NULL)
    {
        node = node->next;
        count--;
    }
    return node;
}

Doubly_Linked_List_Node *DoublyLinkedListNodeRegress(Doubly_Linked_List_Node *node, unsigned long count)
{
    while (count > 0 && node != NULL)
    {
        node = node->prev;
        count--;
    }
    return node;
}

// Returns the node that is count nodes forward.
// If the end of the list is reached, contiues forward from the beginning.
Doubly_Linked_List_Node *DoublyLinkedListNodeCyclicAdvance(Doubly_Linked_List_Node *node, unsigned long count)
{
    while (count > 0)
    {
        node = (node->next) ? node->next : node->list->head;
        count--;
    }
    return node;
}

// Returns the node that is count nodes backward.
// If the beginning of the list is reached, continues backward from the end.
Doubly_Linked_List_Node *DoublyLinkedListNodeCyclicRegress(Doubly_Linked_List_Node *node, unsigned long count)
{
    while (count > 0)
    {
        node = (node->prev) ? node->prev : node->list->tail;
        count--;
    }
    return node;
}

void DoublyLinkedListNodeRemove(Doubly_Linked_List_Node *node)
{
    Doubly_Linked_List *list = node->list;
    if (list == NULL)
    {
        return;
    }
    if (node->next == node->prev)
    {
        ResetDoublyLinkedList(list);
    }
    else if (node->prev == NULL)
    {
        node->next->prev = NULL;
        list->head = node->next;
    }
    else if (node->next == NULL)
    {
        node->prev->next = NULL;
        list->tail = node->prev;
    }
    else
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    node->list = NULL;
    list->size--;
}

// Doubly_Linked_List Functions
void ResetDoublyLinkedList(Doubly_Linked_List *list)
{
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

Doubly_Linked_List *CreateDoublyLinkedList()
{
    Doubly_Linked_List *list = malloc(sizeof *list);
    ResetDoublyLinkedList(list);

    return list;
}

void DeleteDoublyLinkedList(Doubly_Linked_List *list)
{
    Doubly_Linked_List_Node *node = list->head;
    while (node != NULL)
    {
        Doubly_Linked_List_Node *next = node->next;
        DeleteDoublyLinkedListNode(node);
        node = next;
    }
    free(list);
}

void DoublyLinkedListClear(Doubly_Linked_List *list)
{
    Doubly_Linked_List_Node *node = list->head;
    while (node != NULL)
    {
        Doubly_Linked_List_Node *next = node->next;
        DeleteDoublyLinkedListNode(node);
        node = next;
    }
    ResetDoublyLinkedList(list);
}

void DoublyLinkedListAddObjectToEnd(Doubly_Linked_List *list, void *object)
{
    DoublyLinkedListAddNodeToEnd(list, CreateDoublyLinkedListNode(object));
}

void DoublyLinkedListAddObjectToBeginning(Doubly_Linked_List *list, void *object)
{
    DoublyLinkedListAddNodeToBeginning(list, CreateDoublyLinkedListNode(object));
}

void DoublyLinkedListAddNodeToEnd(Doubly_Linked_List *list, Doubly_Linked_List_Node *node)
{
    if (list->tail == NULL)
    {
        DoublyLinkedListAddFirstNode(list, node);
    }
    else
    {
        DoublyLinkedListNodeInsertAfter(list->tail, node);
        list->tail = node;
        list->size++;
    }
}

void DoublyLinkedListAddNodeToBeginning(Doubly_Linked_List *list, Doubly_Linked_List_Node *node)
{
    if (list->head == NULL)
    {
        DoublyLinkedListAddFirstNode(list, node);
    }
    else
    {
        DoublyLinkedListNodeInsertBefore(list->head, node);
        list->head = node;
        list->size++;
    }
}

void DoublyLinkedListAddFirstNode(Doubly_Linked_List *list, Doubly_Linked_List_Node *node)
{
    list->size = 1;
    list->head = node;
    list->tail = node;
    node->list = list;
}

void DoublyLinkedListMakeCyclical(Doubly_Linked_List *list)
{
    if (list->head != NULL)
    {
        list->tail->next = list->head;
        list->head->prev = list->tail;
    }
}

void **DoublyLinkedListToArray(Doubly_Linked_List *list)
{
    void **ret = malloc(sizeof(void *) * (list->size + 1));
    Doubly_Linked_List_Node *node = list->head;
    void **tmp = ret;
    while (node)
    {
        *tmp = node->object;
        node = node->next;
        tmp++;
    }
    return ret;
}