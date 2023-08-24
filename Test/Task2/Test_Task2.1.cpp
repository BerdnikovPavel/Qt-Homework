#include <iostream>

using namespace std;

#define rows 5
#define cols 5

void spotOpening(int a[rows][cols], int i, int j)
{
    if (i < 0 || i >= rows || j < 0 || j >= cols || a[i][j] == 1 || a[i][j] == 3)
    {
        return;
    }

    a[i][j] = 3;
    spotOpening(a, i, j + 1);
    spotOpening(a, i + 1, j);
    spotOpening(a, i, j - 1);
    spotOpening(a, i - 1, j);
}

int spotCounter(int a[rows][cols])
{
    int spotCounter = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (a[i][j] == 0)
            {
                spotCounter++;
                spotOpening(a, i, j);
            }
        }
    }
    return spotCounter;
}


int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Количество строк: " << rows << endl
        << "Количество столбцов: " << cols << endl;


    int arr[rows][cols];
    for (int i = 0; i < rows; i++)
    {
        cout << endl;
        for (int j = 0; j < cols; j++) 
        {
            arr[i][j] = rand() % 2;
            cout << arr[i][j] <<" ";
        }
    }
    cout << endl;

    int spotCount = spotCounter(arr);
    cout << endl << "Количество пятен: " << spotCount << endl;
}