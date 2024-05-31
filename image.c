#include<stdio.h>
#include<stdlib.h>
#include "image.h"

ImageGray *create_image_gray(const ImageRGB *imagemrgb){

    ImageGray *image = (ImageGray*)malloc(sizeof(ImageGray));
    if(image==NULL){
        printf("ERRO NA ALOCACAO DA MEMORIA DA IMAGE GRAY.");
        return NULL;
    }

    image->dim.altura=imagemrgb->dim.altura;
    image->dim.largura=imagemrgb->dim.largura;

    image->pixels = (PixelGray*)malloc(image->dim.largura*image->dim.altura*sizeof(PixelGray));
    if(image->pixels==NULL){
        free(image);
        return NULL;
    }

    for(int x=0; x<image->dim.altura; x++){
        for(int y=0; y<image->dim.largura; y++){
            int idx = x*image->dim.largura+y;
            int resultado = (imagemrgb->pixels[idx].red+imagemrgb->pixels[idx].green+imagemrgb->pixels[idx].blue) / 3;
            image->pixels[idx].value = resultado;
        }
    }

    return image;

}

void free_image_gray(ImageGray *image){
    if(image != NULL){
        free(image->pixels);
        free(image);
    }
}
