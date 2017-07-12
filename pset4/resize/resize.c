
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize infile outfile\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
    if (n < 1 || n > 100)
    {
        printf("Factor must be in range 1..100!\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_n;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_n = bf;
    
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi,bi_n;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_n = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine new dimmentions;
    bi_n.biWidth= bi.biWidth*n;
    bi_n.biHeight= bi.biHeight*n;
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_n = (4 - (bi_n.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi_n.biSizeImage = ((sizeof(RGBTRIPLE)*bi_n.biWidth) + padding_n)*abs(bi_n.biHeight);
    bf_n.bfSize = bi_n.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    //
    long offset = bi.biWidth * sizeof(RGBTRIPLE);
    
    // write new outfile's BITMAPFILEHEADER
    fwrite(&bf_n, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write new outfile's BITMAPINFOHEADER
    fwrite(&bi_n, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //vertical rezise
        for(int vert_n = 0; vert_n<n; vert_n++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                for(int c = 0; c<n; c++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // add new padding if needed
            for (int k = 0; k < padding_n; k++)
            {
                fputc(0x00, outptr);
            }
                
            // Return to the beginning of a scanline
            if (vert_n < n - 1)
                fseek(inptr, -offset, SEEK_CUR);
        }
        
        // skip over old padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
