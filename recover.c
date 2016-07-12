/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    // set input file
    char* recoverFile = "card.raw";
    
    // open it
    FILE* inptr = fopen(recoverFile, "r");
    
    // check file is open
    if(inptr == NULL)
    {
        printf("Could not open %s.\n", recoverFile);
        return 1;
    }
    
    // block size for FAT, count recovered photo, logical vaiable to set start point of jpeg file
    unsigned char block[512];
    int jpegCount = 0;
    bool jpegStart = false;
    
    // open outfile
    FILE* outptr;
    
    //reed all block's from  infile
    while(fread(block, 512, 1, inptr) != 0)
    {
        // check start bytes for jpeg file
        if((int)block[0] == 255 && (int)block[1] == 216 && (int)block[2] == 255 && ((int)block[3] == 224 || (int)block[3] == 225))
        {
            // chek for already start a jpeg
            if(jpegStart)
                fclose(outptr);
            else
                jpegStart = true;
            
            // set file name to ###.jpeg    
            char photo[4];
            if(jpegCount < 10)
                sprintf(photo, "00%i.jpg", jpegCount);
            else if (jpegCount < 100)
                sprintf(photo, "0%i.jpg", jpegCount);
            
            // open output file for write 
            outptr = fopen(photo, "w");
            if(outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create a file %s.\n", photo);
                return 2;
            }
            
            // upgrade counter
            jpegCount++;
        }
        if(jpegStart)
            fwrite(&block, 512, 1, outptr);
    }
    
    //free pointers
    fclose(outptr);
    fclose(inptr);
}
