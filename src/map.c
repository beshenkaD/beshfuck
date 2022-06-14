#include "map.h"
#include "vm_procedure.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	const char *key;
	void *value;
} MapEntry;

struct _map {
	MapEntry *entries;
	size_t capacity;
	size_t length;
};

#define INITIAL_CAPACITY 16

Map *map_new(void)
{
	Map *map = malloc(sizeof(Map));

	if (map == NULL) {
		return NULL;
	}

	map->length = 0;
	map->capacity = INITIAL_CAPACITY;

	map->entries = calloc(map->capacity, sizeof(MapEntry));
	if (map->entries == NULL) {
		free(map);
		return NULL;
	}

	return map;
}

void map_free(Map *table)
{
	for (size_t i = 0; i < table->capacity; i++) {
		free((void *)table->entries[i].key);
	}

	free(table->entries);
	free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char *key)
{
	uint64_t hash = FNV_OFFSET;
	for (const char *p = key; *p; p++) {
		hash ^= (uint64_t)(unsigned char)(*p);
		hash *= FNV_PRIME;
	}
	return hash;
}

void *map_get(Map *map, const char *key)
{
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(map->capacity - 1));

	while (map->entries[index].key != NULL) {
		if (strcmp(key, map->entries[index].key) == 0) {
			return map->entries[index].value;
		}

		index++;
		if (index >= map->capacity) {
			index = 0;
		}
	}
	return NULL;
}

static const char *map_set_entry(MapEntry *entries, size_t capacity,
				 const char *key, void *value, size_t *plength)
{
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

	while (entries[index].key != NULL) {
		if (strcmp(key, entries[index].key) == 0) {
			entries[index].value = value;
			return entries[index].key;
		}

		index++;

		if (index >= capacity) {
			index = 0;
		}
	}

	if (plength != NULL) {
		key = strdup(key);

		if (key == NULL) {
			return NULL;
		}

		(*plength)++;
	}

	entries[index].key = (char *)key;
	entries[index].value = value;
	return key;
}

static bool map_expand(Map *map)
{
	size_t new_capacity = map->capacity * 2;
	if (new_capacity < map->capacity) {
		return false;
	}

	MapEntry *new_entries = calloc(new_capacity, sizeof(MapEntry));
	if (new_entries == NULL) {
		return false;
	}

	for (size_t i = 0; i < map->capacity; i++) {
		MapEntry entry = map->entries[i];
		if (entry.key != NULL) {
			map_set_entry(new_entries, new_capacity, entry.key,
				      entry.value, NULL);
		}
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;
	return true;
}

const char *map_set(Map *map, const char *key, void *value)
{
	assert(value != NULL);

	if (value == NULL) {
		return NULL;
	}

	if (map->length >= map->capacity / 2) {
		if (!map_expand(map)) {
			return NULL;
		}
	}

	return map_set_entry(map->entries, map->capacity, key, value, &map->length);
}

size_t map_length(Map *map)
{
	return map->length;
}

MapIter map_iterator(Map *map)
{
	MapIter it;
	it._map = map;
	it._index = 0;
	return it;
}

bool map_iterator_next(MapIter *it)
{
	Map *map = it->_map;
	while (it->_index < map->capacity) {
		size_t i = it->_index;
		it->_index++;
		if (map->entries[i].key != NULL) {
			MapEntry entry = map->entries[i];
			it->key = entry.key;
			it->value = entry.value;
			return true;
		}
	}
	return false;
}
