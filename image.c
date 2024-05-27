#include <stdio.h>
#include <stdlib.h>
#include "image.h"

void print_pixel_color(PixelRGB pixel){
    printf("\033[48;2;%d;%d;%dm  \033[0m", pixel.red, pixel.green, pixel.blue);
}

ImageRGB *create_image_rgb(int largura, int altura, FILE *arq){
    ImageRGB *image = (ImageRGB *)malloc(sizeof(ImageRGB));

    image->dim.altura = altura;
    image->dim.largura = largura;

    image->pixels = (PixelRGB *)malloc(sizeof(PixelRGB) * altura * largura);

    int i = 0;

    while(!(feof(arq))){
        fscanf(arq,"%d %d %d,", &(image->pixels[i].red), &(image->pixels[i].green),&(image->pixels[i].blue));
        i++;
    }

    return image;
}

void exibir_image(ImageRGB *image){
    int i, j;
    for(i = 0; i < image->dim.altura; i++){
        for(j = 0; j < image->dim.largura; j++){
            print_pixel_color(image->pixels[i * image->dim.largura + j]);
        }
        printf("\n");
    }
}

void free_image_rgb(ImageRGB *image){
    free(image->pixels);
    free(image);

}

int main(){
    FILE *arq = fopen("../utils/input_image_example_RGB.txt", "r");
    if(arq == NULL){
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    int altura, largura;

    ImageRGB *image = NULL;

    int opc;
    do{
        printf("_____ MENU ______\n");
        printf("0 - Sair\n");
        printf("1 - Criar imagem RGB\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opc);
        switch (opc) {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                if(image != NULL){
                    free(image->pixels);
                    free(image);
                }
                fscanf(arq, "%d", &altura);
                fscanf(arq, "%d", &largura);
                image = create_image_rgb(largura, altura, arq);
                printf("Imagem criada com sucesso!\n");
                exibir_image(image);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }while(opc != 0);

    if(image != NULL){
        free_image_rgb(image);
    }
    fclose(arq);

    return 0;
}
