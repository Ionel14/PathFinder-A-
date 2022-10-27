#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <tuple>
#include <unordered_map>


void citireMatriceFisier(std::ifstream& citire, int**& matrice, int& n, int& m)
{
	citire >> n >> m;
	matrice = new int* [n];
	for (int i = 0; i < n; i++)
	{
		matrice[i] = new int[m];
		for (int j = 0; j < m; j++)
		{
			citire >> matrice[i][j];
		}
	}
}



int Manhattan(std::pair<int, int> a, std::pair<int, int> b)
{
	return abs(a.first - b.first) + abs(a.second - b.second);
}

class comparer
{
public:
	bool operator ()(std::tuple<int, int, int> A, std::tuple<int, int, int> B)
	{
		return std::get<2>(A) > std::get<2>(B);
	}
};

std::stack<std::pair<int, int>> findPath(std::pair<int, int> start, std::pair<int, int> finish, int**& Map, int n, int m)
{
	std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, comparer> frontier;
	frontier.push(std::tuple<int, int, int>(start.first, start.second, 0));

	std::unordered_map<int*, std::pair<int, int>> cameFrom;
	std::unordered_map<int*, int> costSoFar;
	cameFrom[&Map[start.first][start.second]] = start;
	costSoFar[&Map[start.first][start.second]] = 0;

	std::pair<int, int> current, next;
	int newCost, aux, priority;
	while (!frontier.empty())
	{
		current = std::pair<int, int>(std::get<0>(frontier.top()), std::get<1>(frontier.top()));
		frontier.pop();

		if (current == finish)
		{
			break;
		}

		aux = 0;

		do
		{
			next = current;
			switch (aux)
			{
			case 0:
			{
				if (next.first > 0)
				{
					next.first--;
					break;
				}
				else
				{
					aux++;
				}
			}
			case 1:
			{
				if (next.second > 0)
				{
					next.second--;
					break;
				}
				else
				{
					aux++;
				}
			}
			case 2:
			{
				if (next.first < n - 1)
				{
					next.first++;
					break;
				}
				else
				{
					aux++;
				}
			}
			case 3:
			{
				if (next.second < m - 1)
				{
					next.second++;
					break;
				}
				else
				{
					aux++;
				}
			}
			}

			if (next == current)
			{
				break;
			}

			if (Map[next.first][next.second] >= -2 && abs(Map[next.first][next.second] - Map[current.first][current.second]) <= 2)
			{
				newCost = costSoFar[&Map[current.first][current.second]] + abs(Map[next.first][next.second] - Map[current.first][current.second]) + 1;
				if (costSoFar.find(&Map[next.first][next.second]) == costSoFar.end() || newCost < costSoFar[&Map[next.first][next.second]])
				{
					costSoFar[&Map[next.first][next.second]] = newCost;
					priority = newCost + Manhattan(finish, next);
					frontier.push(std::tuple<int, int, int>(next.first, next.second, priority));
					cameFrom[&Map[next.first][next.second]] = current;
				}
			}

			aux++;

		} while (aux < 4);
	}

	std::stack<std::pair<int, int>> Path;
	current = finish;
	if (cameFrom.find(&Map[current.first][current.second]) == cameFrom.end())
	{
		std::cout << "Nu exista drum sa poata fi parcurs pentru a se ajunge la destinatie";
	}
	else
	{
		while (current != start)
		{
			Path.push(current);
			current = cameFrom[&Map[current.first][current.second]];
		}
		Path.push(start);
	}
	return Path;
}

void scriereMatriceFisier(int** matrice, int n, int m, std::unordered_map<int*, int> puncteParcurse)
{
	std::vector<int> jLocation;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			std::cout << matrice[i][j];
			if (puncteParcurse.find(&matrice[i][j]) != puncteParcurse.end() && j < m - 1 && puncteParcurse.find(&matrice[i][j + 1]) != puncteParcurse.end())
			{
				std::cout << "+";
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

void loopStartFinish(int**& arrayX, int n, int m)
{
	std::pair<int, int> start, finish;
	std::unordered_map<int*, int> puncteParcurse;
	char validation;
	do
	{
		std::cout << "Introduceti coordonatele punctului de plecare:\ni=";
		std::cin >> start.first;
		std::cout << "j=";
		std::cin >> start.second;

		std::cout << "Introduceti coordonatele punctului de sosire:\ni=";
		std::cin >> finish.first;
		std::cout << "j=";
		std::cin >> finish.second;

		if (arrayX[start.first][start.second] < -2)
		{
			std::cout << "Sunteti intr-un loc cu o altitudine < -2, deci nu se poate efectua plecarea";
		}
		else
		{
			std::stack<std::pair<int, int>> Path = findPath(start, finish, arrayX, n, m);

			if (!Path.empty())
			{
				while (!Path.empty())
				{
					std::cout << Path.top().first << " " << Path.top().second << std::endl;
					puncteParcurse[&arrayX[Path.top().first][Path.top().second]] = 0;
					Path.pop();
				}
				scriereMatriceFisier(arrayX, n, m, puncteParcurse);
				puncteParcurse.clear();
			}
		}
		std::cout << "\n\nDoriti sa introduceti o noua drumetie? y/n\n";
		std::cin >> validation;
	} while (validation == 'y');
}

int main()
{
	int** arrayX;
	int n, m;
	std::ifstream citire;
	citire.open("Harta.txt");
	citireMatriceFisier(citire, arrayX, n, m);
	citire.close();

	loopStartFinish(arrayX, n, m);

	return 0;
}
