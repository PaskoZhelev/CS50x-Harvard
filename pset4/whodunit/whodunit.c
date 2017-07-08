#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[]) {
    if(argc != 3)
    {
        fprintf(stderr,"Usage: ./whodunit infile outfile\n");
        return 1;
    }
    
    //filenames
    char *infileName = argv[1];
    char *outfileName = argv[2];
    
    FILE *inptFile = fopen(infileName, "r");
    if(inptFile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infileName);
        return 2;
    }
    
    FILE *outptFile = fopen(outfileName, "w");
    if (outptFile == NULL)
    {
        fclose(inptFile);
        fprintf(stderr, "Could not create %s.\n", outfileName);
        return 3;
    }
    
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptFile);

    
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptFile);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptFile);
        fclose(inptFile);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptFile);


    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptFile);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE triple;
            
            fread(&triple, sizeof(RGBTRIPLE), 1, inptFile);
            
            if (triple.rgbtRed == 255)
                triple.rgbtRed = 0;
            if (triple.rgbtBlue == 255)
                triple.rgbtBlue = 0;
            if (triple.rgbtGreen == 255)
                triple.rgbtGreen = 0;
                
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptFile);
        }
        
        fseek(inptFile, padding, SEEK_CUR);

        // put padding to the outptFile
        for (int k = 0; k < padding; k++)
            fputc(0x00, outptFile);
    }
    
    // close infile
    fclose(inptFile);

    // close outfile
    fclose(outptFile);


    return 0;
}