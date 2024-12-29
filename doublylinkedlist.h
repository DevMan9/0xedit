#ifndef doublylinkedlist_h
#define doublylinkedlist_h

typedef struct Doubly_Linked_List_Node_S
{
    void *object;
    struct Doubly_Linked_List_Node_S *next;
    struct Doubly_Linked_List_Node_S *prev;
    struct Doubly_Linked_List_S *list;
} Doubly_Linked_List_Node;

typedef struct Doubly_Linked_List_S
{
    unsigned long size;
    Doubly_Linked_List_Node *head;
    Doubly_Linked_List_Node *tail;

} Doubly_Linked_List;
// --------------------------------------
// Doubly_Linked_List_Node Functions
// --------------------------------------

// Allocates space for a Doubly_Linked_List_Node and stores the given object pointer in it.
Doubly_Linked_List_Node *CreateDoublyLinkedListNode(void *object);

// Frees the given Doubly_Linked_List_Node.
// DOES NOT FREE THE STORED OBJECT POINTER.
void DeleteDoublyLinkedListNode(Doubly_Linked_List_Node *node);

// Inserts a given new_node AFTER the given node_in_list.
// Handles head and tail edge cases.
void DoublyLinkedListNodeInsertAfter(Doubly_Linked_List_Node *node_in_list, Doubly_Linked_List_Node *new_node);

// Inserts a given new_node BEFORE the given node_in_list.
// Handles head and tail edge cases.
void DoublyLinkedListNodeInsertBefore(Doubly_Linked_List_Node *node_in_list, Doubly_Linked_List_Node *new_node);

// Returns the node that is count nodes forward.
// If the end of the list is reached, returns NULL.
Doubly_Linked_List_Node *DoublyLinkedListNodeAdvance(Doubly_Linked_List_Node *node, unsigned long count);

// Returns the node that is count nodes backward.
// If the beginning of the list is reached, returns NULL.
Doubly_Linked_List_Node *DoublyLinkedListNodeRegress(Doubly_Linked_List_Node *node, unsigned long count);

// Returns the node that is count nodes forward.
// If the end of the list is reached, contiues forward from the beginning.
Doubly_Linked_List_Node *DoublyLinkedListNodeCyclicAdvance(Doubly_Linked_List_Node *node, unsigned long count);

// Returns the node that is count nodes backward.
// If the beginning of the list is reached, continues backward from the end.
Doubly_Linked_List_Node *DoublyLinkedListNodeCyclicRegress(Doubly_Linked_List_Node *node, unsigned long count);

// Removes the given node from its list.
// Handles head and tail edge cases.
void DoublyLinkedListNodeRemove(Doubly_Linked_List_Node *node);

// --------------------------------------
// Doubly_Linked_List Functions
// --------------------------------------

// Sets:
// size = 0;
// head = NULL;
// tail = NULL;
// DOES NOT FREE ANY NODES OR DATA
void ResetDoublyLinkedList(Doubly_Linked_List *list);

// Allocates space for a Doubly_Linked_List.
Doubly_Linked_List *CreateDoublyLinkedList();

// Frees all nodes and given list.
// DOES NOT FREE ANY STORED DATA.
void DeleteDoublyLinkedList(Doubly_Linked_List *list);

// Frees all nodes and resets list.
// DOES NOT FREE ANY STORED DATA.
void DoublyLinkedListClear(Doubly_Linked_List *list);

// Creates a node for the given object and appends it to the end of the given list.
void DoublyLinkedListAddObjectToEnd(Doubly_Linked_List *list, void *object);

// Creates a node for the given object and appends it to the beginning of the given list.
void DoublyLinkedListAddObjectToBeginning(Doubly_Linked_List *list, void *object);

// Appends the given node to the end of the given list.
void DoublyLinkedListAddNodeToEnd(Doubly_Linked_List *list, Doubly_Linked_List_Node *node);

// Appends the given node to the Beginning of the given list.
void DoublyLinkedListAddNodeToBeginning(Doubly_Linked_List *list, Doubly_Linked_List_Node *node);

// Handles the special case when there are no elements in the list.
void DoublyLinkedListAddFirstNode(Doubly_Linked_List *list, Doubly_Linked_List_Node *node);

void **DoublyLinkedListToArray(Doubly_Linked_List *list);

// BREAKS VARIOUS OTHER FUNCTIONS RELYANT ON NULL TERMINATION.
// void DoublyLinkedListMakeCyclical(Doubly_Linked_List *list);

#endif /* doublylinkedlist_h */