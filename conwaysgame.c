/*
* PP Project
* 27th of October (2016)
* Author: BOGDAN MACOVEI
* Title: Conway's Game of Life
*/

/// Libraries

#include <stdio.h>
#include <stdlib.h>

/// Define and macro

#define NMAX 1000 // represent the maximum size for lines and columns

/*
* This macro function is displayed in the beginning
* to collect the necessary data
*/

#define readInitialData(type, n, m, k, matrix)\
{\
    scanf("%c", &type); \
    scanf("%d", &n); \
    scanf("%d", &m); \
    scanf("%d", &k); \
    getGameMatrix(matrix, n, m); \
}

/// Global variable

int maxPopulation; // represent the maximum population in k-evolution

/// getData and writeData functions

void getGameMatrix(int matrix[][NMAX], int n, int m)
{
    int i, j;
    for(i = 1; i <= n; i++)
        for(j = 1; j <= m; j++)
            scanf("%d", &matrix[i][j]);
}

void writeGameMatrix(int matrix[][NMAX], int n, int m)
{
    printf("\n");
    int i, j;
    for(i = 1; i <= n; i++)
    {
        for(j = 1; j <= m; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

/// Filling area functions

/*
* we'll consider that the matrix is between lines 1 and m
* and between columns 1 and m
* so we'll make 0 all of the elements from
* line 0, line n+1, column 0, column m+1
* to correctly calculate the number of the not null values in the area
*/

void fillExternArea(int matrix[][NMAX], int n, int m)
{
    int i, j;
    for(i = 0; i <= n+1; i++)
        for(j = 0; j <= m+1; j++)
            if(i == 0 || j == 0 || i == n+1 || j == m+1)
                matrix[i][j] = 0;
}

/*
* filling the extern area with matrix's elements
* is the solving of Torr's case
* so, the rule is:
* line 0: contains line n
* line n+1: contains line 0
* column 0: contains column m
* column m+1: contains column 0
* so, for one 5x5 matrix: (o is the current element,
        x are the element in o's area)

   a55 a51 a52 a53 a54 a55 a51
   a15  o   x  a13 a14  x  a11
   a25  x   x  a23 a24  x  a21
   a35 a31 a32 a33 a34 a35 a31
   a45 a41 a42 a43 a44 a45 a41
   a55  x   x  a53 a54  x  a51
   a15 a11 a12 a13 a14 a15 a11
*/

void fillExternAreaTorr(int matrix[][NMAX], int n, int m)
{
    int i;

    matrix[0][0] = matrix[n][m];
    matrix[0][m+1] = matrix[n][1];
    matrix[n+1][m+1] = matrix[1][1];
    matrix[n+1][0] = matrix[1][m];

    for(i = 1; i <= m; i++)
    {
        matrix[0][i] = matrix[n][i];
        matrix[n+1][i] = matrix[1][i];
    }
    for(i = 1; i <= n; i++)
    {
        matrix[i][m+1] = matrix[i][1];
        matrix[i][0] = matrix[i][m];
    }
}

/// Counting the elements in the area

/*
* this function return the number of values that are 1 or 2
* (the signification of this numbers are presented in the next function)
* for a specified line and for a specified column
* so, for the a[line][column], we will calculate
* the sum of a[line-1][column-1] + ... + a[line+1][column+1]
* assuming that we'll verify all of this 8 positions
*/

unsigned int getAreaCounter(int matrix[][NMAX], int line, int column)
{
    unsigned int number = 0, i;
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(i = 0; i < 8; i++)
        if(matrix[line + dx[i]][column + dy[i]] == 1
            || matrix[line + dx[i]][column + dy[i]] == 2)

                number++;

    return number;
}

/// Modifying matrix to solve the problem

/*
* we'll modify the matrix following the rules
* 0 represent an empty space
* 1 represent a cell
* 2 represent a cell that died
* 3 represent a new cell
* 2 will be interpreted like an empty space = 0
* and 3 will be interpreted like a cell = 1
*/

void modifyGameMatrix(int matrix[][NMAX], int n, int m)
{
    int i, j, localPopulation;
    for(i = 1; i <= n; i++)
        for(j = 1; j <= m; j++)
            if(matrix[i][j] == 1)
                if(getAreaCounter(matrix, i, j) < 2
                   || getAreaCounter(matrix, i, j) > 3)

                        matrix[i][j] = 2;

    for(i = 1; i <= n; i++)
        for(j = 1; j <= m; j++)
            if(matrix[i][j] != 1 && getAreaCounter(matrix, i, j) == 3)
                matrix[i][j] = 3;

    for(i = 1; i <= n; i++)
        for(j = 1; j <= m; j++)

            if(matrix[i][j] == 3)
                matrix[i][j] = 1;

            else if(matrix[i][j] == 2)
                matrix[i][j] = 0;

    int localMaxPopulation = getMaxGradePopulation(matrix, n, m);
    maxPopulation = (localMaxPopulation > maxPopulation)?
                        localMaxPopulation : maxPopulation;
}

/// Solving the maximum grade of population

/*
* this function return the number of the cells in one generation
* the maximum percentage is the maximum of cells / n*m
*/

int getMaxGradePopulation(int matrix[][NMAX], int n, int m)
{
    int maxim = 0, i, j;

    for(i = 1; i <= n; i++)
        for(j = 1; j <= m; j++)
            if(matrix[i][j])
                maxim++;

    return maxim;
}

/// Solving the problem

/*
* this function solve the Plane case of this problem
*/

void casePlane(int matrix[][NMAX], int n, int m, int k)
{
    /*
    * I initialize the maxPopulation
    * (the maximum grade of population in this k evolutions)
    * with the number of population at the beginning
    * I'll change the maxPopulation after the each construction of the matrix
    */
    int maxPopulation = getMaxGradePopulation(matrix, n, m);
    fillExternArea(matrix, n, m);

    int i;
    for(i = 1; i <= k; i++)
        modifyGameMatrix(matrix, n, m);

    writeGameMatrix(matrix, n, m);
    printf("%.3f%%\n",
           (float)maxPopulation/(n*m)*100);
}

/*
* this function solve the Torr case of this problem
*/

void caseTorr(int matrix[][NMAX], int n, int m, int k)
{
    /*
    * I initialize the maxPopulation
    * (the maximum grade of population in this k evolutions)
    * with the number of population at the beginning
    * I'll change the maxPopulation after the each construction of the matrix
    */
    int maxPopulation = getMaxGradePopulation(matrix, n, m);

    int i;
    for(i = 1; i <= k; i++)
    {
        /*
        * I must refill the matrix at every step
        * because i fill the extern area with matrix's elements
        * that are changing at every step
        */

        fillExternAreaTorr(matrix, n, m);
        modifyGameMatrix(matrix, n, m);
    }

    writeGameMatrix(matrix, n, m);
    printf("%.3f%%\n",
           (float)maxPopulation/(n*m)*100);
}

int main()
{
    int **matrix = calloc(NMAX, sizeof(int *));
    int i;
    for(i = 0; i < NMAX; i++)
        matrix[i] = calloc(NMAX, sizeof(int));

    int n, m, k;
    char type;

    readInitialData(type, n, m, k, matrix); // using the Macro Function

    switch(type)
    {
        case 'P':
            printf("\nThe result is:\n");
            casePlane(matrix, n, m, k);
            break;
        case 'T':
            printf("\nThe result is:\n");
            caseTorr(matrix, n, m, k);
            break;
        default:
            printf("Invalid data");
    }

    return 0;

}
