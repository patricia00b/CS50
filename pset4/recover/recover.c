/*
Specification
Implement a program called recover that recovers JPEGs from a forensic image.

When executed, your program should recover every one of the JPEGs from card.raw, storing each as a separate 
file in your current working directory. Your program should number the files it outputs by naming each ###.jpg, 
where ### is three-digit decimal number from 000 on up. 
read memory card -> write into JPGED
*/
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    bool found_jpeg = 0;
    unsigned char buffer[BUFFER_SIZE];
    int k = 0;
    FILE* img = NULL; 

    // ensure proper usage
    if (argc!= 2)
    {
        fprintf(stderr,"Correct usage: ./recover to_recover_nfile");
        return 1;
    }
    
    // open memory card file 
    FILE *mc = fopen(argv[1], "r");
    if (mc==NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    //while we dont reach end of memory card iterate

    while (fread(buffer, BUFFER_SIZE, 1,mc) == 1) //else end of file was reached
    {
        //start of a new JPEG
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0) //check for hearder of jpeg
        {
            if (found_jpeg == 0) //new jpeg
            {
                found_jpeg = 1; //flag on to start writing a new jpeg
            }
            else //already found a jpeg, close it
                fclose(img); //close old
                
            // in either case its necessary to start a new JPEG
            char filename[10];
            sprintf(filename,"%03i.jpg",k);
            k++;
            img = fopen(filename, "w");

        }
        if (found_jpeg == 1)
            fwrite(&buffer,BUFFER_SIZE,1, img);
        
    }
    fclose(mc);
    fclose(img); //close last image
    return 0;
}