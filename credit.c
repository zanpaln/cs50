#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX_CARD_NUMB 16

long long ask_card_numb()
{
    long long rslt;
    
    do
    {
        printf("Enter number of your credit card: ");
        rslt = GetLongLong();
    } while (rslt < 0);
    
    return rslt;
}

int sum(int mnt)
{
    return (mnt / 10) + (mnt % 10);
}

bool valid(char* credit)
{
    int digit = 0;
    bool uneven = true;
    
    int result = 0;
    
    for(int i = strlen(credit) - 1; i >= 0; i--)
    {
        digit = credit[i] - '0';
        
        result += uneven ? digit : sum(digit * 2);
        uneven = !uneven;
    }
    
    if(result%10 == 0)
        return true;
    else
        return false;
}

int main(int argc, char* argv[])
{
    long long credit;
    char card_str[MAX_CARD_NUMB];
    
    credit = ask_card_numb();
    sprintf(card_str, "%lld", credit);
    
    if(card_str[0] == '4' && valid(card_str))
        printf("VISA\n");
    else if(((card_str[0] == '3' && card_str[1] == '4') || (card_str[0] == '3' && card_str[1] == '7')) && valid(card_str))
        printf("AMEX\n");
    else if(((card_str[0] == '5' && card_str[1] == '1') || (card_str[0] == '5' && card_str[1] == '2') || (card_str[0] == '5' && card_str[1] == '3') || (card_str[0] == '5' && card_str[1] == '4') || (card_str[0] == '5' && card_str[1] == '5')) && valid(card_str))
        printf("MASTERCARD\n");
    else
        printf("INVALID\n");
    return 0;
}
