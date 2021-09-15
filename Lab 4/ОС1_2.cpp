#include <iostream>
#include <stdio.h>
#include <conio.h>

using namespace std;

int main()
{
    int num;
    printf("Enter the number ( 0 < number < 256 ): ");
    scanf("%d", &num);
    if((num > 0)||(num < 256))
    {
        printf("Matching symbol: %c\n", num);
        printf("Matching next symbol: %c\n", num+1);
    }
    else printf("Wrong input!\n");
    return 0;
}
