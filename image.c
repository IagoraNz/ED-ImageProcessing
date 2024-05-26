#include <stdio.h>
#include <stdlib.h>
#include "image.h"

ImageRGB *create_image_rgb(int largura, int altura){
    FILE *arq = fopen("input_image_example_RGB", "r");
    ImageRGB *image;

    fscanf(arq, "%d", &image->dim.altura);
    fscanf(arq, "%d", &image->dim.largura);
    altura = image->dim.altura;
    largura = image->dim.largura;
    
    PixelRGB **pixel = (PixelRGB*)calloc(sizeof(PixelRGB), altura * largura);
    int i = 0;
    while(!(feof(arq))){
        fscanf(arq, "%d %d %d", &(image->pixels[i].red), &(image->pixels[i].green), &(image->pixels[i].blue));
        i++;
    }    
}

int main(){
    FILE *arq = fopen("input_image_example_RGB", "r");
    ImageRGB *image;
    return 0;
}
