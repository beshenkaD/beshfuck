#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct _map Map;

Map *map_new(void);
void map_free(Map *table);
void *map_get(Map *table, const char *key);
const char *map_set(Map *table, const char *key, void *value);
size_t map_length(Map *table);

typedef struct {
	const char *key;
	void *value;

	Map *_map;
	size_t _index;
} MapIter;

MapIter map_iterator(Map *map);
bool map_iterator_next(MapIter *it);

#endif
