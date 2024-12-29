#include <stdlib.h>

#include "doublylinkedlisthashset.h"

Doubly_Linked_List_Hash_Set *CreateDoublyLinkedListHashSet(
    unsigned long (*hash_function)(void *key),
    int (*key_compare_function)(void *key1, void *key2))
{
    // Allocate space for a Doubly_Linked_List_Hash_Set.
    Doubly_Linked_List_Hash_Set *dllset = malloc(sizeof *dllset);

    // Initialize the Doubly_Linked_List.
    dllset->list = CreateDoublyLinkedList();

    // Initialize the Hash_Map.
    dllset->map = CreateHashMap(hash_function, key_compare_function);

    // Return the newly created set.
    return dllset;
}

void DeleteDoublyLinkedListHashSet(Doubly_Linked_List_Hash_Set *dllset)
{
    // Delete the list.
    DeleteDoublyLinkedList(dllset->list);

    // Delete the map
    DeleteHashMap(dllset->map);

    // Free the set.
    free(dllset);
}

int DoublyLinkedListHashSetPut(Doubly_Linked_List_Hash_Set *dllset, void *key)
{
    // Get the value stored in the map against the given key.
    Doubly_Linked_List_Node *node = HashMapGet(dllset->map, key);

    // If no value exists in the map
    if (!node)
    {
        // Then create a new Doubly_Linked_List_Node with the key.
        node = CreateDoublyLinkedListNode(key);

        // Add that node to the end of the list.
        DoublyLinkedListAddNodeToEnd(dllset->list, node);

        // Put that node in the map.
        HashMapPut(dllset->map, key, node);

        // Return 1 indicating that the given key is now stored in this set.
        return 1;
    }
    else
    {
        // Else the key already exists in the set.
        // Return 0 to indicate that this set DID NOT store the given key.
        return 0;
    }
}

void *DoublyLinkedListHashSetRemove(Doubly_Linked_List_Hash_Set *dllset, void *key)
{
    // Remove the Key_Value_Pair with the given key from the map.
    Key_Value_Pair *removed_kvp = HashMapRemove(dllset->map, key);

    // Initialize the return value to NULL.
    void *ret = NULL;

    // If a Key_Value_Pair was removed from the map,
    if (removed_kvp)
    {
        // Then set the return value to the removed key.
        // We don't know how to destroy this object, but the user does.
        ret = removed_kvp->key;

        // Remove the corresponding node from the list.
        DoublyLinkedListNodeRemove(removed_kvp->value);

        // Delete the corresponding node.
        // We DO know how to destroy a Doubly_Linked_List_Node, and the user does not own this data.
        DeleteDoublyLinkedListNode(removed_kvp->value);
    }

    return ret;
}

int DoublyLinkedListHashSetHas(Doubly_Linked_List_Hash_Set *dllset, void *key)
{
    // Check if the map contains the given key.
    return (HashMapGet(dllset->map, key) != NULL);
}