

#ifndef storage_H
#define storage_H

struct Storage;

struct Storage* StorageCreate(void);
unsigned StorageDestroy(struct Storage *storage);

int StorageSetCell(struct Storage *storage, unsigned n, unsigned *coins);
unsigned* StorageGetCoins(struct Storage *storage, unsigned n);
unsigned* StorageGetNextValidCoins(struct Storage *storage, unsigned *n);

#endif
