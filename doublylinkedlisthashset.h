#ifndef doublylinkedlisthashset_h
#define doublylinkedlisthashset_h

#include "doublylinkedlist.h"
#include "hashmap.h"

// A Doubly_Linked_List_Hash_Set is a combination of a hash set and a doubly linked list
// in such a way as to leverage the speed of removing arbitrary elements
// as well as the speed of accessing "random" elements.
typedef struct Doubly_Linked_List_Hash_Set_S
{
    // This allows the user to iterate the elements of the set.
    Doubly_Linked_List *list;
    // This allows the user to quickly remove elements from the list.
    Hash_Map *map;
} Doubly_Linked_List_Hash_Set;

// Allocates space for and initializes a Doubly_Linked_List_Hash_Set.
Doubly_Linked_List_Hash_Set *CreateDoublyLinkedListHashSet(unsigned long (*hash_function)(void *key), int (*key_compare_function)(void *key1, void *key2));

// Frees a Doubly_Linked_List_Hash_Set.
// DOES NOT FREE STORED DATA.
void DeleteDoublyLinkedListHashSet(Doubly_Linked_List_Hash_Set *dllset);

// Puts the given key in the given Doubly_Linked_List_Hash_Set
// If the key already exists in the set, returns 0.
// When this function returns 0, the caller is responible for freeing the key.
// When this function returns 1, the caller should not free the key until it is removed from the set.
int DoublyLinkedListHashSetPut(Doubly_Linked_List_Hash_Set *dllset, void *key);

// Removes the given key from the given dllset and returns it, to be freed by the caller.
void *DoublyLinkedListHashSetRemove(Doubly_Linked_List_Hash_Set *dllset, void *key);

// Returns 1 if the key exists in the dllset.
int DoublyLinkedListHashSetHas(Doubly_Linked_List_Hash_Set *dllset, void *key);

#endif /* doublylinkedlisthashset_h */