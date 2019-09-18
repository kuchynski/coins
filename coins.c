
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "storage.h"
#include "init.h"

void PrintTable(const unsigned *coins);
void PrintResult(const unsigned *coins, const unsigned *result, const int amount);
unsigned SearchResultRecursive(const int amount, const int level, const int *coins, int *result);
unsigned SearchResultList(const int amount, const int level, const int *coins, unsigned *result, unsigned *memory_used);

unsigned attempt;

void main()
{
	const int amount = AMOUNT;
//	int coins[COINS_NUMBER] =	{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 
//								31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
//								73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
//								127, 131};
	int coins[COINS_NUMBER] =	{101, 202, 303, 404, 505, 606, 707, 808, 909, 1010,
								1111, 1212, 1313, 1414, 1515, 1616, 1717, 1818, 1919, 2020,
								2121, 2222, 2323, 2424, 2525, 2626, 2727, 2828, 2929, 3030,
								3131, 3232, 3333, 3434, 3535, 3636, 3737, 3838, 3939, 4040,
								4141, 4242, 4343, 4444, 4545, 4646, 4747, 4848, 4949, 5050,	
								5151, 5252, 5353, 5454, 5555, 5656, 5757, 5858, 5959, 6060,
								6161, 6262, 6363, 6464, 6565, 6666, 6767, 6868, 6969, 7070, 
								7171, 7272, 7373, 7474, 7575, 7676, 7777, 7878, 7979, 8080,
								8181, 8282, 8383, 8484, 8585, 8686, 8787, 8888, 8989, 9090,
								9191, 9292, 9393, 9494, 9595, 9696, 9797, 9898, 9999, 100100}; 
	unsigned result[COINS_NUMBER] = {0};
	int i;
	time_t time_begin, time_end;
	unsigned memory_used;
	
	PrintTable(coins);
	
	printf("\nList method --------------------------------------------\n");
	time_begin = time(0);
	attempt = 0;
	unsigned count1 = SearchResultList(amount, COINS_NUMBER - 1, coins, result, &memory_used);
	if(count1) {
		printf("Solution exists. The best one: ");
		PrintResult(coins, result, amount);
		printf("\nAttempts: %u, memory used: %u.\n", attempt, memory_used);
	}
	else {
		printf("There is no solution.\n");
		printf("Attempts: %u, memory used: %u.\n", attempt, memory_used);
	}
	time_end = time(0);
	printf("%d seconds have been spent\n", (int)(time_end - time_begin));
	
/*	printf("\nRecursive method --------------------------------------------\n");
	time_begin = time(0);
	attempt = 0;
	unsigned count2 = SearchResultRecursive(amount, COINS_NUMBER - 1, coins, result);
	PrintTable(result);
	if(count2)
		printf("Solution exists %d times, attempts %d\n", count2, attempt);
	else
		printf("There is no solution, attempts %d.\n", attempt);
	time_end = time(0);
	printf("%d seconds has been spent\n", (int)(time_end - time_begin));*/\	

}

void PrintTable(const unsigned *coins)
{
	for(int i = 0; i < COINS_NUMBER; i++)
		printf("%d\t", *(coins + i));
	printf("\n");
}

void PrintResult(const unsigned *coins, const unsigned *result, const int amount)
{
	printf("%d = ", amount);
	for(int i = 0; i < COINS_NUMBER; i++)
		if(*(result + i))
			printf("%d*%d + ", *(result + i), *(coins + i));
	printf("0\n");
}

void CopyTable(unsigned *sum, unsigned *a)
{
	memcpy(sum, a, sizeof(unsigned) * COINS_NUMBER);
}

void FoldTables(unsigned *sum, unsigned *a, unsigned *b)
{
	int i;
	for(i = 0; i < COINS_NUMBER; i ++)
		sum[i] = a[i] + b[i];
}

unsigned SearchResultList(const int amount, const int level, const int *coins, unsigned *result, unsigned *memory_used)
{
	int i, j;
	unsigned ret = 0;
	int middle = (amount & 0x01)? (amount >> 1) + 1 : (amount >> 1);
	struct Storage *storage;
	unsigned table_fold[COINS_NUMBER];

	// the initialization
	storage = StorageCreate();
	if(!storage)
		return -1;

	for(i = 0; i < COINS_NUMBER; i ++) {
		int coin = coins[i];
		int a = coin;
		for(j = 0; j < COINS_NUMBER; j ++)
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

int tmp_q;
	// the calculation 
	for(i = 1; i <= middle; i ++) {
		unsigned *table_i = StorageGetCoins(storage, i);
		if(table_i) {
			int a = i;
			int new_a = a + i;

			for(; new_a <= amount; a++, new_a++) {				
				unsigned *table_a = StorageGetNextValidCoins(storage, &a);
				attempt ++;				
				if(table_a) {
					new_a = a + i;
					FoldTables(table_fold, table_a, table_i);
					StorageSetCell(storage, new_a, table_fold);
tmp_q = new_a;
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
printf("--- %d ---\n", tmp_q);
	if(ret)
		CopyTable(result, StorageGetCoins(storage, amount));
	*memory_used = StorageDestroy(storage);
	
	return ret;
}

// ----------------------------------------------------  ------------------------------------------------

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


