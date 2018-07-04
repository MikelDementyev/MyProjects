#ifndef PNG_OPEN_H
#define PNG_OPEN_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define PNG_DEBUG 3
#include <png.h>
int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

int read_png_file(char* file_name, int *_width, int *_height)
{
        unsigned char header[8];    // 8 is the maximum size that can be checked

        /* open file and test for it being a png */
        FILE *fp = fopen(file_name, "rb");
        if (!fp) {
                printf("[read_png_file] File %s could not be opened for reading\n", file_name);
                printf("%d\n", errno);
                return 0;
        }
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8)) {
                printf("[read_png_file] File %s is not recognized as a PNG file\n", file_name);
                return 0;
        }


        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
                printf("[read_png_file] png_create_read_struct failed\n");

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
                printf("[read_png_file] png_create_info_struct failed\n");

        if (setjmp(png_jmpbuf(png_ptr)))
                printf("[read_png_file] Error during init_io\n");

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        *_width = width;
        *_height = height;
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
                printf("[read_png_file] Error during read_image\n");

        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (y=0; y<height; y++)
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

        png_read_image(png_ptr, row_pointers);

        fclose(fp);
        return 1;
}


void write_png_file(char* file_name)
{
        /* create file */
        FILE *fp = fopen(file_name, "wb");
        if (!fp)
                printf("[write_png_file] File %s could not be opened for writing\n", file_name);


        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
                printf("[write_png_file] png_create_write_struct failed\n");

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
                printf("[write_png_file] png_create_info_struct failed\n");

        if (setjmp(png_jmpbuf(png_ptr)))
                printf("[write_png_file] Error during init_io\n");

        png_init_io(png_ptr, fp);


        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
                printf("[write_png_file] Error during writing header\n");

        png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);


        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
                printf("[write_png_file] Error during writing bytes\n");

        png_write_image(png_ptr, row_pointers);


        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
                printf("[write_png_file] Error during end of write\n");

        png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
        for (y=0; y<height; y++)
                free(row_pointers[y]);
        free(row_pointers);

        fclose(fp);
}

int isSquare(int x0, int y0, int x1, int y1)
{
    if ( x0 < 0) {
        printf("Fail with x0\n");
        return 0;
    }

    if ( y0 < 0) {
        printf("Fail with y0\n");
        return 0;
    }

    if ( x1 <= x0 ) {
        printf("Fail with x1\n");
        return 0;
    }

    if ( y1 >= y0 ) {
        printf("Fail with y1\n");
        return 0;
    }

    if ( ( x1 - x0 ) == ( y0 - y1 ) ) {
        printf("Is square\n");
        return 1;
    }

    printf("Is not square\n");
    return 0;
}

int isOtsideOfImage(int width, int heigth, int x0, int y0, int x1, int y1) {
    if(x0 > width){
        printf("Fail with x0\n");
        return 0;
    }

    if(x1 > width){
        printf("Fail with x1\n");
        return 0;
    }

    if(y0 > heigth){
        printf("Fail with y0\n");
        return 0;
    }

    if(y1 > heigth){
        printf("Fail with y1\n");
        return 0;
    }

    return 1;
}


void drawX(int x0, int y0, int x1, int y1) {

    int step = 4;
    if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
        printf("[process_file] input file is PNG_COLOR_TYPE_RGB\n");
        step = 3;
    }

    int xs = x0;
    int xf = x1;
    for ( int y = y0; y < y1; y++, xs++, xf-- ) {
        png_byte* row = row_pointers[y];
        png_byte* ptr = &(row[xs*step]);
        for(int pos = 0; pos < step; pos++) {
            ptr[pos] = 0;
            ptr[pos+1] = 0;
            ptr[pos+2] = 0;
            ptr[pos+3] = 0;
        }
        ptr = &(row[xf*step]);
        for(int pos = 0; pos < step; pos++) {
            ptr[pos] = 0;
            ptr[pos+1] = 0;
            ptr[pos+2] = 0;
            ptr[pos+3] = 0;
        }

    }
}

#endif // PNG_OPEN_H
