
#include <stdio.h> 

#define NUMBER_OF_COINS	5

void PrintTable(const int *coins);
int SearchResult(const int amount, const int level, const int *coins, int *result);

int main()
{
	const int amount = 25;
	const int coins[NUMBER_OF_COINS] = {6, 7, 8, 9, 10};
	int result[NUMBER_OF_COINS] = {0};

	printf("Coins:");
	PrintTable(coins);
	
	int count = SearchResult(amount, NUMBER_OF_COINS - 1, coins, result);

	printf("Have done: %d.\n", count);
}

void PrintTable(const int *coins)
{
	for(int i = 0; i < NUMBER_OF_COINS; i++)
		printf("\t%d", *(coins + i));
	printf("\n");
}

int SearchResult(const int amount, const int level, const int *coins, int *result)
{
	static int count = 0;
	int n = amount / *(coins + level);

	while(n >= 0) {
		const int weight = *(coins + level) * n;
		*(result + level) = n;
		n = (level == 0)? -1 : n - 1;
		if(weight == amount) {
			PrintTable(result);
			count ++;
		}
		else if(level != 0)
			SearchResult(amount - weight, level - 1, coins, result);
	}
	*(result + level) = 0;

	return count;
}

