
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "storage.h"
#include "init.h"

void PrintTable(const unsigned *coins);
void PrintList(const int *list);
unsigned SearchResultRecursive(const int amount, const int level, const int *coins, int *result);
unsigned SearchResultList(const int amount, const int level, const int *coins, unsigned *result, unsigned *memory_used);

unsigned attempt;

void main()
{
	const int amount = AMOUNT;
//	int coins[NUMBER_COINS] = {1123, 2120, 3230, 4230, 5234, 6534, 7634, 8234, 9123, 10345};
	int coins[NUMBER_COINS] = {12, 13, 21, 45, 54, 105, 206, 508, 1004, 1228, 2003, 5044, 10023};
	unsigned result[NUMBER_COINS] = {0};
	int i;
	time_t time_begin, time_end;
	unsigned memory_used;
	
//	for(i = 0; i < NUMBER_COINS; i ++)
//		coins[i] = (i + 1) * 5;
	
	PrintTable(coins);
	
	printf("\nList method --------------------------------------------\n");
	time_begin = time(0);
	attempt = 0;
	unsigned count1 = SearchResultList(amount, NUMBER_COINS - 1, coins, result, &memory_used);
	if(count1) {
		printf("Solution exists. The best one:\n");
		PrintTable(result);
		printf("Attempts: %u, memory used: %u.\n", attempt, memory_used);
	}
	else
		printf("There is no solution, attempts %u.\n", attempt);
	time_end = time(0);
	printf("%d seconds has been spent\n", (int)(time_end - time_begin));
	
/*	printf("\nRecursive method --------------------------------------------\n");
	time_begin = time(0);
	attempt = 0;
	unsigned count2 = SearchResultRecursive(amount, NUMBER_COINS - 1, coins, result);
	PrintTable(result);
	if(count2)
		printf("Solution exists %d times, attempts %d\n", count2, attempt);
	else
		printf("There is no solution, attempts %d.\n", attempt);
	time_end = time(0);
	printf("%d seconds has been spent\n", (int)(time_end - time_begin));
*/
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

unsigned SearchResultList(const int amount, const int level, const int *coins, unsigned *result, unsigned *memory_used)
{
	int i, j;
	unsigned ret = 0;
	int middle = (amount & 0x01)? (amount >> 1) + 1 : (amount >> 1);
	struct Storage *storage;
	unsigned table_fold[NUMBER_COINS];

	// the initialization
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
			if(a == amount) {
				ret ++;
			}
			table_fold[i] ++;
			a += coin;
		}
	}

	// the calculation 
	for(i = 1; i <= middle; i ++) {
		unsigned *table_i = StorageGetCoins(storage, i);
		if(table_i) {
			int a = i;// + 1;
			int new_a = a + i;
			for(; new_a <= amount; a++, new_a++) {				
				unsigned *table_a = StorageGetNextValidCoins(storage, &a);
				attempt ++;				
				if(table_a) {
					new_a = a + i;
					FoldTables(table_fold, table_a, table_i);
					StorageSetCell(storage, new_a, table_fold);
					if(new_a == amount) {
						//PrintTable(table_fold);
						ret ++;
					}
				}
				else
					break;
			}
		}
	}

	if(ret)
		CopyTable(result, StorageGetCoins(storage, amount));
	*memory_used = StorageDestroy(storage);
	
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

unsigned SearchResultRecursive(const int amount, const int level, const int *coins, int *result)
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
			SearchResultRecursive(amount - weight, level - 1, coins, result);
	}
	*(result + level) = 0;

	return global_count;
}


