#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

  if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover inptr\n");
        return 1;
    }
	
  FILE *inptr = fopen(argv[1], "r");
   if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

  unsigned char *buffer = malloc(512);
  int jpegNum = 0;
  
  if (buffer == NULL) 
  {
	  fprintf(stderr, "Buffer cannot be null\n");
	  return 3;
  }
  

  // initialize output file pointer
  FILE *outptr  = NULL;

  char nameFile[8];
  
  while (fread(buffer, 1, 512, inptr) != 0) {
    
    if (buffer[0] == 0xff &&
        buffer[1] == 0xd8 &&
        buffer[2] == 0xff &&
        buffer[3] >= 0xe0 && buffer[3] <= 0xef) {
            
            if (outptr != NULL)
				fclose(outptr);
            
            sprintf(nameFile, "%03d.jpg", jpegNum);       
            outptr = fopen(nameFile, "wb");
            
            if (outptr == NULL) 
				{
					fprintf(stderr, "Could not open %s.\n", nameFile);
					return 4;
				}
            
            fwrite(buffer, 1, 512, outptr);
            jpegNum = jpegNum + 1;
      }
      else {
        if (outptr != NULL) {
          
          fwrite(buffer, 1, 512, outptr);
        }
      }
  }
  fclose(outptr);
  free(buffer);
  fclose(inptr);
  return 0;
}