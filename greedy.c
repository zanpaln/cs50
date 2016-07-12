#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float input_delivery; 
    int total_delivery = 0;
    int count_coins = 0;
    
    do 
    {
        printf("O hai! How much change is owed?\n");
        input_delivery=GetFloat();
    }
    while ((total_delivery = round(input_delivery*100))<0);
    
    total_delivery = round(input_delivery*100);
    
    while (total_delivery / 25 >= 1)
    {
        count_coins++;
        total_delivery -= 25;
    }
    while (total_delivery < 25 && total_delivery / 10 >= 1)
    {
        count_coins++;
        total_delivery -= 10;
    }
    while (total_delivery < 10 && total_delivery / 5 >= 1)
    {
        count_coins++;
        total_delivery -= 5;
    }
    while (total_delivery < 5 && total_delivery / 1 >= 1)
    {
        count_coins++;
        total_delivery -= 1;
    }
    
    printf("%d", count_coins);
    printf("\n");
}
