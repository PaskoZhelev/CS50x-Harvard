#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // Ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    float f = atof(argv[1]);

    if (f < 0.0 || f > 100.0)
    {
        printf("The factor must be in the range between 1 and 100!\n");
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    BITMAPFILEHEADER bf_new;
    bf_new = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    BITMAPINFOHEADER bi_new;
    bi_new = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Determine the new dimensions
    bi_new.biWidth  = bi.biWidth * f;
    bi_new.biHeight = bi.biHeight * f;
    
    // Determine the old and new padding
    int padding_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    int padding_new = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    
    //New Size of the bitmap should be determined
    bi_new.biSizeImage = abs(bi_new.biHeight) * (bi_new.biWidth * sizeof(RGBTRIPLE) + padding_new);
    bf_new.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi_new.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);
    
	RGBTRIPLE tempTriple[bi.biWidth][abs(bi.biHeight)];
	
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
 
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            tempTriple[j][i] = triple;
        }
        
        // skip over padding, if any
        fseek(inptr, padding_old, SEEK_CUR);
    }

	for (int i = 0, biHeight = abs(bi_new.biHeight); i < biHeight; i++)
    {
        for (int j = 0; j < bi_new.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            int tempWidth = (int) (j / f);
			int tempHeight = (int) (i / f);
            triple = tempTriple[tempWidth][tempHeight];                    
            
            // write the pixel to the file
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); 
        }
        
        // write the buffer
        for (int k = 0; k < padding_new; k++)
        {
            fputc(0x00, outptr);
        }
    }
	
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    return 0;
}