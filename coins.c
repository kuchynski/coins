
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define AMOUNT			2500000

void PrintTable(const unsigned *coins);
void PrintList(const int *list);
unsigned SearchResultEnumeration(const int amount, const int level, const int *coins, unsigned *result);
unsigned SearchResultList(const int amount, const int level, const int *coins, unsigned *result);

int attempt = 0;

int main()
{
	const int amount = AMOUNT;
	int coins[NUMBER_COINS] = {1120, 2120, 3230, 4230, 5234, 6534, 7634, 8234, 9123, 10345};
	unsigned result[NUMBER_COINS] = {0};
	int i;
	
	//for(i = 0; i < NUMBER_COINS; i ++)
		//coins[i] = (i + 1) * 5;
	
	PrintTable(coins);
	
	attempt = 0;
	unsigned count1 = SearchResultList(amount, NUMBER_COINS - 1, coins, result);
	if(count1) {
		printf("Solution exists %ud(attempts %d)\n", count1, attempt);
		PrintTable(result);
	}
	else
		printf("There is no solution (attempts %d)\n", attempt);
	
	attempt = 0;
	//unsigned count2 = SearchResultEnumeration(amount, NUMBER_COINS - 1, coins, result);
	//printf("Have done: %d %d.\n", count2, attempt);
}

void PrintTable(const unsigned *coins)
{
	for(int i = 0; i < NUMBER_COINS; i++)
		printf("\t%d", *(coins + i));
	printf("\n");
}

void CopyTable(unsigned *sum, unsigned *a)
{
	memcpy(sum, a, sizeof(unsigned) * NUMBER_COINS);
}

void FoldTables(unsigned *sum, unsigned *a, unsigned *b)
{
	int i;
	for(i = 0; i < NUMBER_COINS; i ++)
		sum[i] = a[i] + b[i];
}

unsigned SearchResultList(const int amount, const int level, const int *coins, unsigned *result)
{
	int i, j;
	unsigned ret = 0;
	int middle = (amount & 0x01)? (amount >> 1) + 1 : (amount >> 1);
	struct Storage *storage;
	unsigned table_fold[NUMBER_COINS];

	storage = StorageCreate();
	if(!storage)
		return -1;

	for(i = 0; i < NUMBER_COINS; i ++) {
		int coin = coins[i];
		int a = coin;
		for(j = 0; j < NUMBER_COINS; j ++)
			table_fold[j] = 0;
		
		table_fold[i] = 1;
		while(a <= amount) {
			attempt ++;
			StorageSetCell(storage, a, table_fold);
//printf("- %d %d\n", i, a);
			if(a == amount) {
				ret ++;
			}
			table_fold[i] ++;
			a += coin;
		}
	}

	
	for(i = 1; i <= middle; i ++) {
		unsigned *table_i = StorageGetCoins(storage, i);
		if(table_i) {
			int a = i;// + 1;
			int new_a = a + i;
			for(; new_a <= amount; a++, new_a++) {				
				unsigned *table_a = StorageGetCoins(storage, a);
				attempt ++;
				if(table_a) {					
					FoldTables(table_fold, table_a, table_i);
					StorageSetCell(storage, new_a, table_fold);
					if(new_a == amount) {
						//PrintTable(table_fold);
						ret ++;
					}
				}
			}
		}
	}

	if(ret)
		CopyTable(result, StorageGetCoins(storage, amount));
	StorageDestroy(storage);
	
	return ret;
}

// ---------------------------------------------------- NP ------------------------------------------------

void PrintList(const int *list)
{
	while(*list)
		printf("\t%d", *list++);
	printf("\n");
}

unsigned global_count = 0;

unsigned SearchResultEnumeration(const int amount, const int level, const int *coins, unsigned *result)
{
	int n = amount / *(coins + level);
	attempt ++;
	
	while(n >= 0) {
		const int weight = *(coins + level) * n;
		*(result + level) = n;
		n = (level == 0)? -1 : n - 1;
		if(weight == amount) {
//			PrintTable(result);
			global_count ++;
		}
		else if(level != 0)
			SearchResultEnumeration(amount - weight, level - 1, coins, result);
	}
	*(result + level) = 0;

	return global_count;
}


