#include <stdlib.h>

#include "hashmap.h"

Key_Value_Pair *CreateKeyValuePair(void *key, void *value)
{
    Key_Value_Pair *key_value_pair = malloc(sizeof *key_value_pair);
    key_value_pair->key = key;
    key_value_pair->value = value;
    return key_value_pair;
}

void DeleteKeyValuePair(Key_Value_Pair *key_value_pair)
{
    free(key_value_pair);
}

Hash_Map *CreateHashMap(unsigned long (*hash_function)(void *key), int (*key_compare_function)(void *key1, void *key2))
{
    Hash_Map *map = malloc(sizeof *map);
    map->hash_function = hash_function;
    map->key_compare_function = key_compare_function;
    map->size = 0;
    map->slot_count = 127;
    map->slots = malloc(sizeof(Doubly_Linked_List *) * map->slot_count);
    for (unsigned long i = 0; i < map->slot_count; i++)
    {
        map->slots[i] = CreateDoublyLinkedList();
    }

    return map;
}

void DeleteHashMap(Hash_Map *map)
{
    for (unsigned long i = 0; i < map->slot_count; i++)
    {
        DeleteDoublyLinkedList(map->slots[i]);
    }
    free(map->slots);
    free(map);
}

void ResetHashMap(Hash_Map *map)
{
    for (unsigned long i = 0; i < map->slot_count; i++)
    {
        DeleteDoublyLinkedList(map->slots[i]);
    }
    free(map->slots);
    map->slot_count = 127;
    for (unsigned long i = 0; i < map->slot_count; i++)
    {
        map->slots[i] = CreateDoublyLinkedList();
    }
    map->size = 0;
}

void HashMapResize(Hash_Map *map, unsigned long new_size)
{
    // Create a temporary set to store this set's data while we rebuild.
    Hash_Map *tmp = malloc(sizeof *tmp);

    // Transfer all the data in the given set to the tmp set
    tmp->slots = map->slots;
    tmp->slot_count = map->slot_count;
    tmp->size = map->size;

    // Set the size of the given set to the new_size
    map->slot_count = new_size;

    // Reset the number of elements to 0.
    map->size = 0;

    // Allocate space for the new lists.
    // The array previously stored here will be deleted at the end when tmp is deleted.
    map->slots = malloc(map->slot_count * sizeof *map->slots);

    // Create new lists for the resized set.
    for (unsigned long i = 0; i < map->slot_count; i++)
    {
        map->slots[i] = CreateDoublyLinkedList();
    }

    // Add each of the elements back to the set.
    for (unsigned long i = 0; i < tmp->slot_count; i++)
    {
        Doubly_Linked_List_Node *node = tmp->slots[i]->head;
        while (node != NULL)
        {
            HashMapPut(map, ((Key_Value_Pair *)node->object)->key, ((Key_Value_Pair *)node->object)->value);
            DeleteKeyValuePair(node->object);
            node = node->next;
        }
    }

    // Delete the temporary hash set.
    DeleteHashMap(tmp);
}

void HashMapGrow(Hash_Map *map)
{
    HashMapResize(map, (map->slot_count << 1) | 1);
}

void HashMapShrink(Hash_Map *map)
{
    HashMapResize(map, map->slot_count >> 1);
}

Key_Value_Pair *HashMapPut(Hash_Map *map, void *key, void *value)
{
    unsigned long index = map->hash_function(key) % map->slot_count;

    Doubly_Linked_List *list = map->slots[index];

    Doubly_Linked_List_Node *node = list->head;

    while (node != NULL && map->key_compare_function(((Key_Value_Pair *)(node->object))->key, key) != 1)
    {
        node = node->next;
    }

    Key_Value_Pair *ret = NULL;

    if (node)
    {
        DoublyLinkedListNodeRemove(node);

        ret = node->object;

        DeleteDoublyLinkedListNode(node);
    }
    else
    {
        map->size++;
    }

    DoublyLinkedListAddObjectToEnd(list, CreateKeyValuePair(key, value));

    return ret;
}

void *HashMapGet(Hash_Map *map, void *key)
{
    unsigned long index = map->hash_function(key) % map->slot_count;

    Doubly_Linked_List *list = map->slots[index];

    Doubly_Linked_List_Node *node = list->head;

    while (node != NULL && map->key_compare_function(((Key_Value_Pair *)(node->object))->key, key) != 1)
    {
        node = node->next;
    }

    if (node)
    {
        return ((Key_Value_Pair *)(node->object))->value;
    }
    else
    {
        return NULL;
    }
}

Key_Value_Pair *HashMapRemove(Hash_Map *map, void *key)
{
    unsigned long index = map->hash_function(key) % map->slot_count;

    Doubly_Linked_List *list = map->slots[index];

    Doubly_Linked_List_Node *node = list->head;

    while (node != NULL && map->key_compare_function(((Key_Value_Pair *)(node->object))->key, key) != 1)
    {
        node = node->next;
    }

    if (node)
    {
        DoublyLinkedListNodeRemove(node);
        Key_Value_Pair *ret = node->object;
        DeleteDoublyLinkedListNode(node);
        map->size--;
        return ret;
    }
    else
    {
        return NULL;
    }
}

Key_Value_Pair **HashMapToArray(Hash_Map *map)
{
    Key_Value_Pair **array = malloc(map->size * (sizeof *array));
    unsigned long array_index = 0;

    for (unsigned long slot_index = 0; slot_index < map->slot_count; slot_index++)
    {
        for (Doubly_Linked_List_Node *node = map->slots[slot_index]->head; node != NULL; node = node->next)
        {
            array[array_index++] = node->object;
        }
    }

    return array;
}

// Hashes a string down to an unsigned long
unsigned long HashString(void *str)
{
    // Set the initial value to 1.
    unsigned long ret = 1;

    // A constant being applied to more evenly distribute the hashes
    //(Based on the golden ratio)
    unsigned long gold = 0x9E3779B97F4A7C15;
    for (char *c = str; *c != '\0'; c++)
    {
        // Multiply the previous hash with the golden ratio, and add the current character
        ret = ret * gold + *c;
    }

    // Return the hash
    return ret;
}

int CompareString(void *str1, void *str2)
{
    return strcmp(str1, str2) == 0;
}

unsigned long HashInt(void *value)
{
    return *(unsigned int *)value;
}

int CompareInt(void *value1, void *value2)
{
    return *(unsigned int *)value1 == *(unsigned int *)value2;
}

unsigned long HashLong(void *value)
{
    return *(unsigned long *)value;
}

int CompareLong(void *value1, void *value2)
{
    return *(unsigned long *)value1 == *(unsigned long *)value2;
}

unsigned long HashChar(void *value)
{
    return *(unsigned char *)value;
}

int CompareChar(void *value1, void *value2)
{
    return *(unsigned char *)value1 == *(unsigned char *)value2;
}

unsigned long HashPointer(void *ptr)
{
    return (unsigned long)ptr;
}

int ComparePointer(void *ptr1, void *ptr2)
{
    return ptr1 == ptr2;
}