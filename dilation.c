#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ll long long
#define MAF printf("Sorry, Memory Allocation Failed. Cannot Process Further.\n");
#define FAF printf("Cannot process The File.\n");
#define FUP printf("Files Are Under Processing...\n");

int background = 0;
int foreground = 1;


// Dilation Code
int **dilation(int **binimg, int height, int width)
{
    printf("Image Will Be Dilated, Please Enter The Required Info...\n");
    printf("Structuring Element:\n");
    int seh, sew;
    printf("Height: ");
    scanf("%d", &seh);
    printf("Width: ");
    scanf("%d", &sew);
    int flag;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            flag = 0;
            int kernel[seh][sew];
            for (int k = 0; k < seh; k++)
            {
                for (int l = 0; l < sew; l++)
                {
                    kernel[k][l] = 1;
                }
            }
            if (((i + seh) < height) && ((j + sew) < width))
            {
                for (int k = 0; k < seh; k++)
                {
                    for (int l = 0; l < sew; l++)
                    {
                        if (binimg[i+k][j+l] == kernel[k][l])
                        {
                            flag = 1;
                            binimg[i + (seh / 2)][j + (sew / 2)] = foreground;
                            break;
                        }
                        else
                            binimg[i + (seh / 2)][j + (sew / 2)] = background;
                    }
                }
            }
        }
    }
    printf("Image Dilation Successful. Please Open The Output File To See Result...\n");
    return binimg;
}



// Export The Updated Image In The Output File
void export_image(int **img, int height, int width, FILE *out)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fprintf(out, "%d ", img[i][j]);
        }
        fprintf(out, "\n");
    }
}



// DRIVER CODE
int main(int argc, char *argv[])
{
    /* NOTE ---------- [in = input file/import from   ||   out = output file/export to] ----------*/
    FILE *in = NULL, *out = NULL;

    /* NOTE ---------- Height, Width and Maximum Pixel Value of the Image ---------- */
    int height, width, maxpix;

    /* NOTE ---------- Magic No of the Image and Str is for storing the strings read ---------- */
    char c, magicno[2], str[500];

    /* NOTE ---------- Opening Input File in Read Mode ---------- */
    in = fopen(argv[1], "rb");

    /* NOTE ---------- Opening file in write-binary('wb') mode for binarization ---------- */
    out = fopen(argv[2], "wb");

    /* NOTE ---------- Check whether the file pointers have been opened required files or not ---------- */
    if ((in != NULL) && (out != NULL))
    {
        FUP; // FUP has been defined
    }
    else
    {
        FAF; // FAF has been defined
    }

    /* NOTE ---------- Reads and Writes the Magic No of the Image ---------- */
    fgets(magicno, 3, in); // reads from input file
    fscanf(in, "\n");      // reads the newline character
    fputs("P1", out);      // 'P1' is the Magic No for a outary Image
    fputs("\n", out);      // writes the newline character

    /* NOTE ---------- Reads and Writes the Width and Height of the Image ---------- */
    fscanf(in, "%d %d", &width, &height); // reads
    fscanf(in, "\n");                     // reads the newline character
    printf("Image Resolution: %d x %d\n", width, height);
    fprintf(out, "%d %d", width, height); // writes
    fprintf(out, "\n");                   // writes the newline character


    /* NOTE ---------- 2-D Image Array to store Pixel Values ---------- */
    /* NOTE --> 'image' is the array of pointers, each of its part stores the base address of the nested arrays that's why we should use double pointer(int **image) */
    int **image = NULL;
    int **binimg;

    image = (int **)malloc(height * sizeof(int *));
    binimg = (int **)malloc(height * sizeof(int *));
    if (image == NULL)
        MAF;
    if (binimg == NULL)
        MAF;

    /* NOTE ---------- Reads and Writes Every Pixel's Value ---------- */
    for (int i = 0; i < height; i++)
    {
        image[i] = (int *)malloc(width * sizeof(int));
        binimg[i] = (int *)malloc(width * sizeof(int));

        for (int j = 0; j < width; j++)
        {
            int x;
            fscanf(in, "%d", &x);
            fscanf(in, "\n");
            image[i][j] = x;
            if (feof(in))
                break;
        }
    }

    image = dilation(image, height, width);
	export_image(image, height, width, out);

    fclose(in);
    fclose(out);
    free(binimg);
    free(image);
}
