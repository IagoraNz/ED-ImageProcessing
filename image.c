#include <stdio.h>
#include <stdlib.h>
#include "image.h"

void print_pixel_color(PixelRGB pixel) {
    printf("\033[48;2;%d;%d;%dm  \033[0m", pixel.red, pixel.green, pixel.blue);
}

ImageRGB *create_image_rgb(int largura, int altura, FILE *arq) {
    ImageRGB *image = (ImageRGB *)malloc(sizeof(ImageRGB));

    image->dim.altura = altura;
    image->dim.largura = largura;

    image->pixels = (PixelRGB *)malloc(sizeof(PixelRGB) * altura * largura);

    int i = 0;
    while (i < altura * largura && fscanf(arq, "%d %d %d,", &image->pixels[i].red, &image->pixels[i].green, &image->pixels[i].blue) == 3) {
        i++;
    }

    return image;
}

void exibir_image(ImageRGB *image) {
    int i, j;
    for (i = 0; i < image->dim.altura; i++) {
        for (j = 0; j < image->dim.largura; j++) {
            print_pixel_color(image->pixels[i * image->dim.largura + j]);
        }
        printf("\n");
    }
}

void free_image_rgb(ImageRGB *image) {
    free(image->pixels);
    free(image);
}

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size) {
    ImageRGB *imageblur = (ImageRGB *)malloc(sizeof(ImageRGB));
    imageblur->dim.altura = image->dim.altura;
    imageblur->dim.largura = image->dim.largura;
    imageblur->pixels = (PixelRGB *)malloc(sizeof(PixelRGB) * image->dim.altura * image->dim.largura);

    int offset = kernel_size / 2, i, j, k, l, contador, idx, linha, coluna;
    for(i = 0; i < image->dim.altura; i++){
        for(j = 0; j < image->dim.largura; j++){
            contador = 0;
            int reds[kernel_size * kernel_size], greens[kernel_size * kernel_size], blues[kernel_size * kernel_size];
            for(k = -offset; k <= offset; k++){
                for(l = -offset; l <= offset; l++){
                    linha = i + k;
                    coluna = j + l;
                    if(linha >= 0 && linha < image->dim.altura && coluna >= 0 && coluna < image->dim.largura){
                        idx = linha * image->dim.largura + coluna;
                        reds[contador] = image->pixels[idx].red;
                        greens[contador] = image->pixels[idx].green;
                        blues[contador] = image->pixels[idx].blue;
                        contador++;
                    }
                }
            }

            qsort(reds, contador, sizeof(int), cmpfunc);
            qsort(greens, contador, sizeof(int), cmpfunc);
            qsort(blues, contador, sizeof(int), cmpfunc);

            imageblur->pixels[i * image->dim.largura + j].red = reds[contador / 2];
            imageblur->pixels[i * image->dim.largura + j].green = greens[contador / 2];
            imageblur->pixels[i * image->dim.largura + j].blue = blues[contador / 2];
        }
    }

    return imageblur;
}

int main() {
    FILE *arq = fopen("../utils/input_image_example_RGB.txt", "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    int altura, largura;

    ImageRGB *image = NULL;

    int opc, kernel_size;
    do {
        printf("_____ MENU ______\n");
        printf("0 - Sair\n");
        printf("1 - Criar imagem RGB\n");
        printf("2 - CLAHE RGB\n");
        printf("3 - RGB no blur\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opc);
        switch (opc) {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                if (image != NULL) {
                    free_image_rgb(image);
                }
                fscanf(arq, "%d", &altura);
                fscanf(arq, "%d", &largura);
                image = create_image_rgb(largura, altura, arq);
                printf("Imagem criada com sucesso!\n");
                break;
            case 2:
                break;
            case 3:
                if(!image){
                    printf("Crie uma imagem RGB primeiro!\n");
                } 
                else{
                    do{
                        printf("Digite o tamanho do kernel: ");
                        scanf("%d", &kernel_size);
                        if(kernel_size % 2 == 0){
                            printf("O tamanho precisa ser impar\n");
                        }
                    }while(kernel_size % 2 == 0);
                    ImageRGB *new_image = median_blur_rgb(image, kernel_size);
                    exibir_image(new_image);
                    free_image_rgb(new_image);
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opc != 0);

    if (image != NULL) {
        free_image_rgb(image);
    }
    fclose(arq);

    return 0;
}
