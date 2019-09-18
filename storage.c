
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "storage.h"
#include "init.h"

#define INDEXES_HIGH_BITS		16
#define INDEXES_HIGH_AMOUNT		(1 << INDEXES_HIGH_BITS)
#define INDEXES_LOW_BITS      (32 - INDEXES_HIGH_BITS)
#define INDEXES_LOW_AMOUNT    (1 << INDEXES_LOW_BITS)

struct Cell
{
    unsigned amount;
    unsigned coins[COINS_NUMBER];
};

struct Storage
{
    struct Cell **pindexes_high[INDEXES_HIGH_AMOUNT];
    unsigned memory_used;
};

struct Storage* StorageCreate(void)
{
    struct Storage *storage = NULL;

    storage = malloc(sizeof(struct Storage));
    if(storage != NULL) {
        memset(storage, 0, sizeof(struct Storage));
	    storage->memory_used = sizeof(struct Storage);
	}

    return storage;
}

unsigned StorageDestroy(struct Storage *storage)
{
	unsigned memory_used = storage->memory_used;
	
    if(storage) {
        unsigned n_high_list;
        for(n_high_list = 0; n_high_list < INDEXES_HIGH_AMOUNT; n_high_list++) {
            struct Cell **pindexes = storage->pindexes_high[n_high_list];
            if(pindexes != NULL) {
                unsigned n_low_list;
                for(n_low_list = 0; n_low_list < INDEXES_LOW_AMOUNT; n_low_list++) {
                    struct Cell *index = pindexes[n_low_list];
                    if(index)
                        free(index);
                }
            }
			free(pindexes);
        }
        free(storage);
    }
	return memory_used;
}

unsigned GetAmount(unsigned *coins)
{
	unsigned ret = 0;
	unsigned i = 0;
	
	for(i = 0; i < COINS_NUMBER; i ++)
		ret += coins[i];
	
	return ret;
}

int StorageSetCell(struct Storage *storage, unsigned n, unsigned *coins)
{
    const unsigned n_high_list = n >> INDEXES_LOW_BITS;
    const unsigned n_low_list = n & (0xFFFFFFFF >> INDEXES_HIGH_BITS);
    struct Cell **pindexes = storage->pindexes_high[n_high_list];
    struct Cell *index;

    if(pindexes == NULL) {
        if((pindexes = malloc(sizeof(struct Cell*) * INDEXES_LOW_AMOUNT)) == NULL)
            return -1;

        storage->memory_used += (sizeof(struct Cell*) * INDEXES_LOW_AMOUNT);
        memset(pindexes, 0, sizeof(struct Cell*) * INDEXES_LOW_AMOUNT);
    }

    index = pindexes[n_low_list];

    if(index != NULL) {// just set new value if it's needed
		unsigned amount = GetAmount(coins);
		if(index->amount > amount) {
			index->amount = amount;
			memcpy(index->coins, coins, sizeof(index->coins));
		}
        return -1;
	}
		
    index = malloc(sizeof(struct Cell));
    if(index == NULL)
        return -1;
	storage->memory_used += (sizeof(struct Cell));
	index->amount = GetAmount(coins);
	memcpy(index->coins, coins, sizeof(index->coins));
	
    pindexes[n_low_list] = index;
    storage->pindexes_high[n_high_list] = pindexes;

    return 0;
}

struct Cell* ODGetIndex(struct Storage *storage, unsigned n)
{
    unsigned n_high_list = n >> INDEXES_LOW_BITS;
    const unsigned n_low_list = n & (0xFFFFFFFF >> INDEXES_HIGH_BITS);
    struct Cell **pindexes = storage->pindexes_high[n_high_list];

	return pindexes? pindexes[n_low_list] : NULL;
}

unsigned* StorageGetCoins(struct Storage *storage, unsigned n)
{
	struct Cell *index = ODGetIndex(storage, n);
	
	return index? index->coins : NULL;
}

unsigned* StorageGetNextValidCoins(struct Storage *storage, unsigned *n)
{
	struct Cell **pindexes;
	struct Cell *index = NULL;
	unsigned n_tmp = *n;
	unsigned n_high_list = n_tmp >> INDEXES_LOW_BITS;
	unsigned n_low_list = n_tmp & (0xFFFFFFFF >> INDEXES_HIGH_BITS);
	
	for(; n_high_list <= (AMOUNT >> INDEXES_LOW_BITS); n_high_list ++) {	
		pindexes = storage->pindexes_high[n_high_list];
		if(pindexes) {
			for(; n_low_list < INDEXES_LOW_AMOUNT; n_low_list ++) {	
				index = pindexes[n_low_list];
				if(index) {
					*n = (n_high_list << INDEXES_LOW_BITS) + n_low_list;
//printf("ne windex %d %d %d\n", *n, n_high_list, n_low_list);
					return index->coins;
				}
			}
		}
		n_low_list = 0;
	}
	
	return NULL;
}

