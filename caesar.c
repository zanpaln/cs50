#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    //cheking for digital argument
    if (argc !=2 && !isdigit(argv[1]))
    {
        printf("Please enter a correct integer to encrypt.");
        return 1;
    }
    
    //get code_key
    int key = atoi(argv[1]);
    
    string str_encode = GetString();
    
    for (int i = 0, l = strlen(str_encode); i < l; i++)
    {
        if (isupper(str_encode[i]))
            printf("%c", 'A' + (str_encode[i] - 'A' + key) % 26);
        else if (islower(str_encode[i]))
            printf("%c", 'a' + (str_encode[i] - 'a' + key) % 26);
        //for non alpha symbols
        else
            printf("%c", str_encode[i]);
    }
    printf("\n");
    
    return 0;
}
