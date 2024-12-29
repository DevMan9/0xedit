#include <stdlib.h>

#include "twowayhashmap.h"

Two_Way_Hash_Map *CreateTwoWayHashMap(
    unsigned long (*forward_hash_function)(void *key),
    int (*forward_key_compare_function)(void *key1, void *key2),
    unsigned long (*backward_hash_function)(void *key),
    int (*backward_key_compare_function)(void *key1, void *key2))
{
    Two_Way_Hash_Map *ret = malloc(sizeof *ret);
    ret->forward = CreateHashMap(forward_hash_function, forward_key_compare_function);
    ret->backward = CreateHashMap(backward_hash_function, backward_key_compare_function);
    return ret;
}

void DeleteTwoWayHashMap(Two_Way_Hash_Map *map)
{
    DeleteHashMap(map->forward);
    DeleteHashMap(map->backward);
    free(map);
}

void ResetTwoWayHashMap(Two_Way_Hash_Map *map)
{
    ResetHashMap(map->forward);
    ResetHashMap(map->backward);
}

void TwoWayHashMapResize(Two_Way_Hash_Map *map, unsigned long new_size)
{
    HashMapResize(map->forward, new_size);
    HashMapResize(map->backward, new_size);
}

void TwoWayHashMapGrow(Two_Way_Hash_Map *map)
{
    HashMapGrow(map->forward);
    HashMapGrow(map->backward);
}

void TwoWayHashMapShrink(Two_Way_Hash_Map *map)
{
    HashMapShrink(map->forward);
    HashMapShrink(map->backward);
}

Key_Value_Pair *TwoWayHashMapPut(Two_Way_Hash_Map *map, void *key, void *value)
{
    free(HashMapPut(map->forward, key, value));
    return HashMapPut(map->backward, key, value);
}

void *TwoWayHashMapForwardGet(Two_Way_Hash_Map *map, void *key)
{
    return HashMapGet(map->forward, key);
}

Key_Value_Pair *TwoWayHashMapForwardRemove(Two_Way_Hash_Map *map, void *key)
{
    free(HashMapRemove(map->backward, key));
    return HashMapRemove(map->forward, key);
}

void *TwoWayHashMapBackwardGet(Two_Way_Hash_Map *map, void *key)
{
    return HashMapGet(map->backward, key);
}

Key_Value_Pair *TwoWayHashMapBackwardRemove(Two_Way_Hash_Map *map, void *key)
{
    free(HashMapRemove(map->forward, key));
    return HashMapRemove(map->backward, key);
}

Key_Value_Pair **TwoWayHashMapToArray(Two_Way_Hash_Map *map)
{
    return HashMapToArray(map->forward);
}