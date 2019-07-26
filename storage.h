

#ifndef storage_H
#define storage_H

#define NUMBER_COINS			10
#define AMOUNT					100000010

#define INDEXES_HIGH_BITS		16
#define INDEXES_HIGH_AMOUNT		(1 << INDEXES_HIGH_BITS)


struct Cell
{
    unsigned amount;
    unsigned coins[NUMBER_COINS];
};

struct Storage
{
    struct Cell **pindexes_high[INDEXES_HIGH_AMOUNT];
    unsigned memory_used;
};

struct Storage* StorageCreate(void);
void StorageDestroy(struct Storage *storage);

int StorageSetCell(struct Storage *storage, unsigned n, unsigned *coins);
unsigned* StorageGetCoins(struct Storage *storage, unsigned n);
unsigned* StorageGetNextValidCoins(struct Storage *storage, unsigned *n);

#endif
