#ifndef hashmap_h
#define hashmap_h

#include <string.h>

#include "doublylinkedlist.h"

typedef struct Hash_Map_S
{
    unsigned long size;
    unsigned long slot_count;
    Doubly_Linked_List **slots;
    unsigned long (*hash_function)(void *key);
    int (*key_compare_function)(void *key1, void *key2);
} Hash_Map;

typedef struct Key_Value_Pair_S
{
    void *key;
    void *value;
} Key_Value_Pair;

// Allocates space for a Key_Value_Pair.
Key_Value_Pair *CreateKeyValuePair(void *key, void *value);

// Frees a Key_Value_Pair.
// DOES NOT FREE STORED DATA.
void DeleteKeyValuePair(Key_Value_Pair *kvp);

// Allocates space for a Hash_Map.
// The hash_function is a user defined function that specifies how a key should be hashed.
// The key_compare_function is a function that specifies how 2 keys should be compared.
//      Returns 1 only when 2 keys are considered equal.
Hash_Map *CreateHashMap(unsigned long (*hash_function)(void *key), int (*key_compare_function)(void *key1, void *key2));

// Frees a given Hash_Map.
// DOES NOT FREE DATA.
void DeleteHashMap(Hash_Map *map);

// Sets the default values of a Hash_Map and frees any used Doubly_Linked_Lists
// DOES NOT FREE DATA.
void ResetHashMap(Hash_Map *map);

// Changes the number of slots in the given Hash_Map.
// Remaps the currently contained Key_Value_Pairs.
void HashMapResize(Hash_Map *map, unsigned long new_size);

// Increases map->slot_count to double + 1;
void HashMapGrow(Hash_Map *map);

// Decreases map->slot_count to half;
void HashMapShrink(Hash_Map *map);

// Puts a given Key and Value in the map.
// If the key already exists, replaces it with the new key and value and returns the old Key_Value_Pair.
Key_Value_Pair *HashMapPut(Hash_Map *map, void *key, void *value);

// Returns the value that the given key maps to.
void *HashMapGet(Hash_Map *map, void *key);

// Removes the given key and its assosiated value from the given map.
// Returns the Key_Value_Pair to be disposed of by the user.
Key_Value_Pair *HashMapRemove(Hash_Map *map, void *key);

// Allocates space to return all the Key_Value_Pairs found in the given map.
Key_Value_Pair **HashMapToArray(Hash_Map *map);

// ---------------------
// Simple Hash Functions and compare functions
// ---------------------
unsigned long HashString(void *str);

int CompareString(void *str1, void *str2);

unsigned long HashInt(void *value);

int CompareInt(void *value1, void *value2);

unsigned long HashLong(void *value);

int CompareLong(void *value1, void *value2);

unsigned long HashChar(void *value);

int CompareChar(void *char1, void *char2);

unsigned long HashPointer(void *ptr);

int ComparePointer(void *ptr1, void *ptr2);

#endif /* hashmap_h */