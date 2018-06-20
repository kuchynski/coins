
#include <iostream>

using namespace std;

class Exchange 
{
public:
	Exchange(const int a, const int n, const int *c);
	~Exchange();
    void Run();
private:
	const int total_amount;
    const int number_of_coins;
	int count_result;
    int *coins;
	int *result;

	void PrintTable(const int *table, const char *str = "");
	void SearchResult(const int rest_amount, const int level);
};

int main()
{
    const int amount = 25;
    const int number_of_coins = 5;
    const int coins[10] = {6, 7, 8, 9, 10};

    Exchange exchange(amount, number_of_coins, coins);

    exchange.Run();
}

Exchange::Exchange(const int a, const int n, const int *c): total_amount(a), number_of_coins(n), count_result(0)
{
    coins = new int[number_of_coins];
    result = new int[number_of_coins];
    for(int i = number_of_coins; i >= 0; i --)
        *(coins + i) = *(c + i);
    PrintTable(coins, "Coins:");
}

Exchange::~Exchange()
{
    delete [] coins;
    delete [] result;
}

void Exchange::Run()
{
    SearchResult(total_amount, number_of_coins - 1);
    cout << "Have done: " << count_result << endl;
}

void Exchange::PrintTable(const int *table, const char *str)
{
    cout << str;
	for(int i = 0; i < number_of_coins; i++)
        cout << '\t' << *(table + i);
    cout << endl;
}

void Exchange::SearchResult(const int rest_amount, const int level)
{
	int n = rest_amount / *(coins + level);

	while(n >= 0) {
		const int weight = *(coins + level) * n;
		*(result + level) = n;
		n = (level == 0)? -1 : n - 1;
		if(weight == rest_amount) {
			PrintTable(result);
			count_result ++;
		}
		else if(level != 0)
			SearchResult(rest_amount - weight, level - 1);
	}
	*(result + level) = 0;
}
