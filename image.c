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

ImageRGB *transpose_rgb(const ImageRGB *image){
    ImageRGB *transposed = (ImageRGB*)malloc(sizeof(ImageRGB));
    transposed->dim.altura = image->dim.largura;
    transposed->dim.largura = image->dim.altura;
    transposed->pixels = (PixelRGB*)malloc((transposed->dim.altura * transposed->dim.largura) * sizeof(PixelRGB));
    int i, j;
    for (i = 0; i < image->dim.altura; i++) {
        for (j = 0; j < image->dim.largura; j++) {
            transposed->pixels[j * transposed->dim.largura + i].blue = image->pixels[i * image->dim.largura + j].blue;
            transposed->pixels[j * transposed->dim.largura + i].red = image->pixels[i * image->dim.largura + j].red;
            transposed->pixels[j * transposed->dim.largura + i].green = image->pixels[i * image->dim.largura + j].green;
        }
    }
    return transposed;
}

ImageRGB *flip_vertical_rgb(const ImageRGB *image){
    ImageRGB *flipVert = (ImageRGB*)malloc(sizeof(ImageRGB));
    flipVert->dim.altura = image->dim.altura;
    flipVert->dim.largura = image->dim.largura;
    flipVert->pixels = (PixelRGB*)malloc((flipVert->dim.altura * flipVert->dim.largura) * sizeof(PixelRGB));
    int i, j, x, y;
    for (i = image->dim.altura - 1, x = 0; i >= 0; i--, x++) {
        for (j = image->dim.largura - 1, y = 0; j >= 0; j--, y++) {
            flipVert->pixels[x * flipVert->dim.largura + y].red = image->pixels[i * image->dim.largura + j].red;
            flipVert->pixels[x * flipVert->dim.largura + y].green = image->pixels[i * image->dim.largura + j].green;
            flipVert->pixels[x * flipVert->dim.largura + y].blue = image->pixels[i * image->dim.largura + j].blue;
        }
    }
    return flipVert;
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

void histogramatile(const ImageRGB *image, int tileX, int tileY, int tile_width, int tile_height, int *histogramared, int *histogramagreen, int *histogramblue){
    int i, j, idx;
    
    for(i = 0; i < 256; i++){
        histogramared[i] = 0;
        histogramagreen[i] = 0;
        histogramblue[i] = 0;
    }
    for(i = tileY; i < tileY + tile_height && i < image->dim.altura; i++){
        for(j = tileX; j < tileX + tile_width && j < image->dim.largura; j++){
            idx = i * image->dim.largura + j;
            histogramared[image->pixels[idx].red]++;
            histogramagreen[image->pixels[idx].green]++;
            histogramblue[image->pixels[idx].blue]++;
        }
    }
}

void histocumulativo(int *histograma, int *histocumulativo, int total_pixels, float clip_limit){
    int excesso = 0, i, dist;
    for(i = 0; i < 256; i++){
        if(histograma[i] > clip_limit){
            excesso += histograma[i] - clip_limit;
            histograma[i] = clip_limit;
        }
    }

    dist = excesso / 256;
    for(i = 0; i < 256; i++){
        histograma[i] += dist;
    }

    histocumulativo[0] = histograma[0];
    for(i = 1; i < 256; i++){
        histocumulativo[i] = histocumulativo[i - 1] + histograma[i];
    }

    for(i = 0; i < 256; i++){
        histocumulativo[i] = histocumulativo[i] * 255 / total_pixels;
    }
}

void aplicarClahe(ImageRGB *image, int tileX, int tileY, int tile_width, int tile_height, int *histocumulativo_red, int *histocumulativo_green, int *histocumulativo_blue){
    int i, j, idx;
    for(i = tileY; i < tileY + tile_height && i < image->dim.altura; i++){
        for(j = tileX; j < tileX + tile_width && j < image->dim.largura; j++){
            idx = i * image->dim.largura + j;
            image->pixels[idx].red = histocumulativo_red[image->pixels[idx].red];
            image->pixels[idx].green = histocumulativo_green[image->pixels[idx].green];
            image->pixels[idx].blue = histocumulativo_blue[image->pixels[idx].blue];
        }
    }
}

ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height){
    int i, j, tileX, tileY;

    ImageRGB *imageclahe = (ImageRGB *)malloc(sizeof(ImageRGB));
    imageclahe->dim.altura = image->dim.altura;
    imageclahe->dim.largura = image->dim.largura;
    imageclahe->pixels = (PixelRGB *)malloc(sizeof(PixelRGB) * image->dim.altura * image->dim.largura);

    int histoRed[256], histoGreen[256], histoBlue[256];
    int histocumulativo_red[256], histocumulativo_green[256], histocumulativo_blue[256], totalPixels = tile_width * tile_height;
    float clipLimite = totalPixels / 256.0 * 2.0;

    for(i = 0; i < image->dim.altura; i++) {
        for(j = 0; j < image->dim.largura; j++){
            imageclahe->pixels[i * image->dim.largura + j] = image->pixels[i * image->dim.largura + j];
        }
    }

    for(tileY = 0; tileY < image->dim.altura; tileY += tile_height){
        for(tileX = 0; tileX < image->dim.largura; tileX += tile_width){
            histogramatile(image, tileX, tileY, tile_width, tile_height, histoRed, histoGreen, histoBlue);
            histocumulativo(histoRed, histocumulativo_red, totalPixels, clipLimite);
            histocumulativo(histoGreen, histocumulativo_green, totalPixels, clipLimite);
            histocumulativo(histoBlue, histocumulativo_blue, totalPixels, clipLimite);
            aplicarClahe(imageclahe, tileX, tileY, tile_width, tile_height, histocumulativo_red, histocumulativo_green, histocumulativo_blue);
        }
    }

    return imageclahe;
}

int main() {
    FILE *arq = fopen("../utils/input_image_example_RGB.txt", "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    int altura, largura, tile_width, tile_height;

    ImageRGB *image = NULL;

    int opc, kernel_size;
    do {
        printf("_____ MENU ______\n");
        printf("0 - Sair\n");
        printf("1 - Criar imagem RGB\n");
        printf("2 - CLAHE RGB\n");
        printf("3 - RGB no blur\n");
        printf("4 - TranposeRGB\n");
        printf("5 - FlipVerticalRGB\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opc);
        switch (opc) {
            case 0:
                printf("Saindo...\n");
                system("PAUSE");
                system("cls");
                break;
            case 1:
                if (image != NULL) {
                    free_image_rgb(image);
                }
                fscanf(arq, "%d", &altura);
                fscanf(arq, "%d", &largura);
                image = create_image_rgb(largura, altura, arq);
                printf("Imagem criada com sucesso!\n");
                system("PAUSE");
                system("cls");
                break;
            case 2:
                if(!image){
                    printf("Crie uma imagem RGB primeiro!\n");
                } 
                else{
                    do{
                        printf("Digite o tamanho do tile (largura): ");
                        scanf("%d", &tile_width);
                        printf("Digite o tamanho do tile (altura): ");
                        scanf("%d", &tile_height);
                    }while(tile_width <= 0 || tile_height <= 0);
                    ImageRGB *new_image = clahe_rgb(image, tile_width, tile_height);
                    exibir_image(new_image);
                    free_image_rgb(new_image);
                }
                system("PAUSE");
                system("cls");
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
                system("PAUSE");
                system("cls");
                break;
            case 4:
                if(!image){
                    printf("Crie uma imagem RGB primeiro!\n");
                } 
                else{
                    ImageRGB *transposed = transpose_rgb(image);
                    system("PAUSE");
                    exibir_image(transposed);
                    transposed = transpose_rgb(transposed);
                    system("PAUSE");
                    exibir_image(transposed);
                }
                system("PAUSE");
                system("cls");
                break;
            case 5:
                if(!image){
                    printf("Crie uma imagem RGB primeiro!\n");
                } 
                else{
                    ImageRGB *flipVert = flip_vertical_rgb(image);
                    system("PAUSE");
                    exibir_image(flipVert);
                    flipVert = flip_vertical_rgb(flipVert);
                    system("PAUSE");
                    exibir_image(flipVert);
                }
                system("PAUSE");
                system("cls");
                break;
            default:
                printf("Opcao invalida!\n");
                system("PAUSE");
                system("cls");
        }
    } while (opc != 0);

    if (image != NULL) {
        free_image_rgb(image);
    }
    fclose(arq);

    return 0;
}
