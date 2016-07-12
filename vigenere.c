#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    //cheking enter argument's
    if (argc !=2)
    {
        printf("Please enter a correct integer to encrypt.\n");
        return 1;
    }
    else 
    {
        //cheking for alpha symbol in argument
        for (int i = 0, l = strlen(argv[1]); i < l; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                printf("Please Enter only char symbol.\n");
                return 1;
            }
        }
    }
    string key = argv[1];
    int key_lenght = strlen(key);
    
    string str_encode = GetString();
    
    for (int i = 0, j = 0, l = strlen(str_encode); i < l; i++)
    {
        //get code_key from string arguments
        int char_key = tolower(key[j % key_lenght]) - 'a';
        
        if (isupper(str_encode[i]))
        {
            printf("%c", 'A' + (str_encode[i] - 'A' + char_key) % 26);
            j++;
        }
        else if (islower(str_encode[i]))
        {
            printf("%c", 'a' + (str_encode[i] - 'a' + char_key) % 26);
            j++;
        }
        //for non alpha symbol
        else
            printf("%c", str_encode[i]);
    }
    printf("\n");
    
    return 0;
}
