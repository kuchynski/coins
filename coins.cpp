
#include <stdio.h> 

#define NUMBER_OF_COINS	5

void PrintTable(const int *coins, const char *str = "");
int SearchResult(const int amount, const int level, const int *coins, int *result);

int main()
{
	const int amount = 25;
	const int coins[NUMBER_OF_COINS] = {6, 7, 8, 9, 10};
	int result[NUMBER_OF_COINS] = {0};

	PrintTable(coins, "Coins:");
	
	int count = SearchResult(amount, NUMBER_OF_COINS - 1, coins, result);

	printf("Have done: %d.\n", count);
}

void PrintTable(const int *coins, const char *str)
{
	printf("%s", str);
	for(int i = 0; i < NUMBER_OF_COINS; i++)
		printf("\t%d", *(coins + i));
	printf("\n");
}

int SearchResult(const int amount, const int level, const int *coins, int *result)
{
	static int count = 0;
	int i = amount / coins[level];

	while(i >= 0) {
		const int weight = coins[level] * i;
		*(result + level) = i--;
		if(weight == amount) {
			PrintTable(result);
			count ++;
			continue;
		}
		if(level == 0)
			break;
		SearchResult(amount - weight, level - 1, coins, result);
	}
	*(result + level) = 0;

	return count;
}

