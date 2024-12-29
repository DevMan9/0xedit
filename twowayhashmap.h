#ifndef twowayhashmap_h
#define twowayhashmap_h

#include "hashmap.h"

typedef struct Two_Way_Hash_Map_S
{
    Hash_Map *forward;
    Hash_Map *backward;
} Two_Way_Hash_Map;

Two_Way_Hash_Map *CreateTwoWayHashMap(
    unsigned long (*forward_hash_function)(void *key),
    int (*forward_key_compare_function)(void *key1, void *key2),
    unsigned long (*backward_hash_function)(void *key),
    int (*backward_key_compare_function)(void *key1, void *key2));

void DeleteTwoWayHashMap(Two_Way_Hash_Map *map);

void ResetTwoWayHashMap(Two_Way_Hash_Map *map);

void TwoWayHashMapResize(Two_Way_Hash_Map *map, unsigned long new_size);

void TwoWayHashMapGrow(Two_Way_Hash_Map *map);

void TwoWayHashMapShrink(Two_Way_Hash_Map *map);

Key_Value_Pair *TwoWayHashMapPut(Two_Way_Hash_Map *map, void *key, void *value);

void *TwoWayHashMapForwardGet(Two_Way_Hash_Map *map, void *key);

Key_Value_Pair *TwoWayHashMapForwardRemove(Two_Way_Hash_Map *map, void *key);

void *TwoWayHashMapBackwardGet(Two_Way_Hash_Map *map, void *key);

Key_Value_Pair *TwoWayHashMapBackwardRemove(Two_Way_Hash_Map *map, void *key);

Key_Value_Pair **TwoWayHashMapToArray(Two_Way_Hash_Map *map);

#endif /* twowayhashmap_h */