
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define NUMBER_OF_COINS	NUMBER_COINS
#define AMOUNT			25001

void PrintTable(const int *coins);
void PrintList(const int *list);
int SearchResultEnumeration(const int amount, const int level, const int *coins, int *result);
int SearchResultList(const int amount, const int level, const int *coins, int *result);

int attempt = 0;

int main()
{
	const int amount = AMOUNT;
	int coins[NUMBER_OF_COINS];// = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
	int result[NUMBER_OF_COINS] = {0};
	int i;
	
	for(i = 0; i < NUMBER_OF_COINS; i ++)
		coins[i] = (i + 1) * 7;
	
	PrintTable(coins);
	
	attempt = 0;
	int count1 = SearchResultList(amount, NUMBER_OF_COINS - 1, coins, result);
	if(count1)
		printf("Solution exists %d(attempts %d)\n", count1, attempt);
	else
		printf("There is no solution (attempts %d)\n", attempt);
	
	attempt = 0;
	int count2 = SearchResultEnumeration(amount, NUMBER_OF_COINS - 1, coins, result);
	printf("Have done: %d %d.\n", count2, attempt);
}

int SearchResultList(const int amount, const int level, const int *coins, int *result)
{
	int i;
	int ret = 0;
	int middle = (AMOUNT & 0x01)? (AMOUNT >> 1) + 1 : (AMOUNT >> 1);
	char *table = malloc(sizeof(char) * (AMOUNT+1));

	memset(table, 0, sizeof(char) * (AMOUNT+1));

	for(i = 0; i < NUMBER_OF_COINS; i ++) {
		int coin = coins[i];
		int a = coin;
		while(a <= amount) {
			attempt ++;
			table[a] = 1;
//printf("- %d %d\n", i, a);
			if(a == amount) {
				//printf("w1\n");
				ret ++;
			}
			a += coin;
		}
	}

	for(i = 1; i <= middle; i ++) {
		if(table[i]) {
			int a = i + 1;
			int new_a = a + i;
			for(; new_a <= AMOUNT; a++, new_a++) {				
				attempt ++;
				if(table[a]) {
					table[new_a] = 1;
//printf("-- %d %d\n", i, a);
					if(new_a == amount) {
//printf("w2 %d %d\n", a, i);
						ret ++;
					}
				}
			}
		}
	}

	free(table);
	return ret;
}

void PrintTable(const int *coins)
{
	for(int i = 0; i < NUMBER_OF_COINS; i++)
		printf("\t%d", *(coins + i));
	printf("\n");
}

void PrintList(const int *list)
{
	while(*list)
		printf("\t%d", *list++);
	printf("\n");
}

int global_count = 0;

int SearchResultEnumeration(const int amount, const int level, const int *coins, int *result)
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


