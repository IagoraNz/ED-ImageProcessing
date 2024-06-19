#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"

#define PIXELS 256

void print_pixel_gray(PixelGray pixel) {
    printf("\033[48;2;%d;%d;%dm  \033[0m", pixel.value, pixel.value, pixel.value);
}

ImageGray *conversion_image_gray(const ImageRGB *imagemrgb){

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

ImageGray *create_image_gray(int largura, int altura, FILE *arquivogray){
    ImageGray *image = (ImageGray*)malloc(sizeof(ImageGray));
    if(image==NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA DA CREATE IMAGE GRAY");
        return NULL;
    }

    image->dim.altura = altura;
    image->dim.largura = largura;

    image->pixels = (PixelGray*)malloc(sizeof(PixelGray)*altura*largura);
    if(image->pixels == NULL){
        free(image);
        return NULL;
    }

    int i=0;
    while(i<altura*largura && fscanf(arquivogray, "%d,", &image->pixels[i].value)==1){
        i++;
    }

    return image;
}

void exibir_image_gray(ImageGray *image) {
    for (int x = 0; x < image->dim.altura; x++) {
        for (int y = 0; y < image->dim.largura; y++) {
            print_pixel_gray(image->pixels[x * image->dim.largura + y]);
        }
        printf("\n");
    }
}

void free_image_gray(ImageGray *image){
    if(image != NULL){
        free(image->pixels);
        free(image);
    }
}

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

ImageGray *transpose_gray(const ImageGray *image){
    ImageGray *transposed = (ImageGray*)malloc(sizeof(ImageGray));
    if(transposed == NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA PARA A TRANSPOSE GRAY.\n");
        return NULL;
    }

    transposed->dim.altura = image->dim.largura;
    transposed->dim.largura = image->dim.altura;
    transposed->pixels = (PixelGray*)malloc((transposed->dim.altura*transposed->dim.largura)*sizeof(PixelGray));
    if(transposed->pixels == NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA PARA OS PIXELS DE TRANSPOSE GRAY.\n");
        free(transposed);
        return NULL;
    }

    for(int i = 0; i < image->dim.altura; i++){
        for(int j = 0; j < image->dim.largura; j++){
            transposed->pixels[j*transposed->dim.largura+i].value = image->pixels[i*image->dim.largura+j].value;
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

ImageRGB *flip_horizontal_rgb(const ImageRGB *image){
    ImageRGB *flipHorizontal = (ImageRGB*)malloc(sizeof(ImageRGB));
    if(flipHorizontal == NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA PARA FLIP HORIZONTAL RGB.\n");
        return NULL;
    }

    flipHorizontal->dim.altura = image->dim.altura;
    flipHorizontal->dim.largura = image->dim.largura;

    flipHorizontal->pixels = (PixelRGB*)malloc((flipHorizontal->dim.altura*flipHorizontal->dim.largura)*sizeof(PixelRGB));
    if(flipHorizontal->pixels == NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA PARA OS PIXELS DE FLIP HORIZONTAL RGB.\n");
        return NULL;
    }

    for (int i=0; i<image->dim.altura; i++){
        for(int j=0, y=image->dim.largura - 1; j<image->dim.largura; j++, y--){
            flipHorizontal->pixels[i*flipHorizontal->dim.largura+j] = image->pixels[i*image->dim.largura+y];
        }
    }

    return flipHorizontal;
}

ImageGray *flip_horizontal_gray(const ImageGray *image){
    ImageGray *flipHorizontalGray = (ImageGray*)malloc(sizeof(ImageGray));
    if(flipHorizontalGray == NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA PARA FLIP HORIZONTAL GRAY.\n");
        return NULL;
    }

    flipHorizontalGray->dim.altura = image->dim.altura;
    flipHorizontalGray->dim.largura = image->dim.largura;

    flipHorizontalGray->pixels = (PixelGray*)malloc((flipHorizontalGray->dim.altura*flipHorizontalGray->dim.largura)*sizeof(PixelGray));
    if(flipHorizontalGray == NULL){
        printf("ERRO NA ALOCACAO DE MEMORIA PARA OS PIXELS DE FLIP HORIZONTAL GRAY.\n");
        free(flipHorizontalGray);
        return NULL;
    }

    for (int i=0; i<image->dim.altura; i++){
        for(int j=0, y=image->dim.largura - 1; j<image->dim.largura; j++, y--){
            flipHorizontalGray->pixels[i*flipHorizontalGray->dim.largura+j] = image->pixels[i*image->dim.largura+y];
        }
    }

    return flipHorizontalGray;
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

ImageGray *median_blur_gray(const ImageGray *image, int kernel_size){
    ImageGray *image_blur = (ImageGray*)malloc(sizeof(ImageGray));
    image_blur->dim.altura = image->dim.altura;
    image_blur->dim.largura = image->dim.largura;
    image_blur->pixels = (PixelGray*)malloc((image_blur->dim.altura * image_blur->dim.largura) * sizeof(PixelGray));

    int offset = kernel_size / 2, i, j, k, l, contador, idx, linha, coluna;

    for(i = 0; i < image->dim.altura; i++){
        for(j = 0; j < image->dim.largura; j++){
            contador = 0;
            int grays[kernel_size * kernel_size];
            for(k = -offset; k <= offset; k++){
                for(l = -offset; l <= offset; l++){
                    linha = i + k;
                    coluna = j + l;
                    if(linha >= 0 && linha < image->dim.altura && coluna >= 0 && coluna < image->dim.largura){
                        idx = linha * image->dim.largura + coluna;
                        grays[contador] = image->pixels[idx].value;
                        contador++;
                    }
                }
            }

            qsort(grays, contador, sizeof(int), cmpfunc);

            image_blur->pixels[i * image->dim.largura + j].value = grays[contador / 2];
        }
    }

    return image_blur;
}

void histogramatile(const ImageRGB *image, int tileX, int tileY, int tile_width, int tile_height, int *histogramared, int *histogramagreen, int *histogramblue){
    int i, j, idx;
    
    for(i = 0; i < PIXELS; i++){
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
    for(i = 0; i < PIXELS; i++){
        if(histograma[i] > clip_limit){
            excesso += histograma[i] - clip_limit;
            histograma[i] = clip_limit;
        }
    }

    dist = excesso / PIXELS;
    for(i = 0; i < PIXELS; i++){
        histograma[i] += dist;
    }

    histocumulativo[0] = histograma[0];
    for(i = 1; i < PIXELS; i++){
        histocumulativo[i] = histocumulativo[i - 1] + histograma[i];
    }

    for(i = 0; i < PIXELS; i++){
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

void homogeneizarClahe(ImageRGB *image, int tileX, int tileY, int tile_width, int tile_height,  int *histocumulativo_red, int *histocumulativo_green, int *histocumulativo_blue,  int *next_histocumulativo_red, int *next_histocumulativo_green, int *next_histocumulativo_blue, int *below_histocumulativo_red, int *below_histocumulativo_green, int *below_histocumulativo_blue, int *diag_histocumulativo_red, int *diag_histocumulativo_green, int *diag_histocumulativo_blue) {
    int i, j, idx, srcX, srcY, interRed, interGreen, interBlue;
    float razaoX, razaoY;
    for(i = 0; i < tile_height; i++){
        for(j = 0; j < tile_width; j++){
            srcX = tileX + j;
            srcY = tileY + i;
            if (srcX >= image->dim.largura || srcY >= image->dim.altura) continue;
            idx = srcY * image->dim.largura + srcX;
            
            razaoX = (float)j / tile_width;
            razaoY = (float)i / tile_height;

            interRed = (1 - razaoX) * (1 - razaoY) * histocumulativo_red[image->pixels[idx].red] + razaoX * (1 - razaoY) * next_histocumulativo_red[image->pixels[idx].red] + (1 - razaoX) * razaoY * below_histocumulativo_red[image->pixels[idx].red] + razaoX * razaoY * diag_histocumulativo_red[image->pixels[idx].red];
            interGreen = (1 - razaoX) * (1 - razaoY) * histocumulativo_green[image->pixels[idx].green] + razaoX * (1 - razaoY) * next_histocumulativo_green[image->pixels[idx].green] + (1 - razaoX) * razaoY * below_histocumulativo_green[image->pixels[idx].green] + razaoX * razaoY * diag_histocumulativo_green[image->pixels[idx].green];               
            interBlue = (1 - razaoX) * (1 - razaoY) * histocumulativo_blue[image->pixels[idx].blue] + razaoX * (1 - razaoY) * next_histocumulativo_blue[image->pixels[idx].blue] + (1 - razaoX) * razaoY * below_histocumulativo_blue[image->pixels[idx].blue] + razaoX * razaoY * diag_histocumulativo_blue[image->pixels[idx].blue];
                              
            image->pixels[idx].red = interRed;
            image->pixels[idx].green = interGreen;
            image->pixels[idx].blue = interBlue;
        }
    }
}

ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height) {
    int i, j, k, tileX, tileY;

    ImageRGB *imageclahe = (ImageRGB *)malloc(sizeof(ImageRGB));
    imageclahe->dim.altura = image->dim.altura;
    imageclahe->dim.largura = image->dim.largura;
    imageclahe->pixels = (PixelRGB *)malloc(sizeof(PixelRGB) * image->dim.altura * image->dim.largura);

    int histoRed[PIXELS], histoGreen[PIXELS], histoBlue[PIXELS];
    int cumulativoRed[PIXELS], cumulativoGreen[PIXELS], cumulativoBlue[PIXELS];
    int proxRed[PIXELS], proxGreen[PIXELS], proxBlue[PIXELS];
    int proxCumuRed[PIXELS], proxCumuGreen[PIXELS], proxCumuBlue[PIXELS];
    int abaixoRed[PIXELS], abaixoGreen[PIXELS], abaixoBlue[PIXELS];
    int abaixoCumuRed[PIXELS], abaixoCumuGreen[PIXELS], abaixoCumuBlue[PIXELS];
    int diagRed[PIXELS], diagGreen[PIXELS], diagBlue[PIXELS];
    int diagCumuRed[PIXELS], diagCumuGreen[PIXELS], diagCumuBlue[PIXELS], totalPixels = tile_width * tile_height;
    float clipLimite = totalPixels / 256.0 * 2.0;

    for(i = 0; i < image->dim.altura; i++){
        for(j = 0; j < image->dim.largura; j++){
            imageclahe->pixels[i * image->dim.largura + j] = image->pixels[i * image->dim.largura + j];
        }
    }

    for(tileY = 0; tileY < image->dim.altura; tileY += tile_height){
        for(tileX = 0; tileX < image->dim.largura; tileX += tile_width){
            histogramatile(image, tileX, tileY, tile_width, tile_height, histoRed, histoGreen, histoBlue);
            histocumulativo(histoRed, cumulativoRed, totalPixels, clipLimite);
            histocumulativo(histoGreen, cumulativoGreen, totalPixels, clipLimite);
            histocumulativo(histoBlue, cumulativoBlue, totalPixels, clipLimite);

            if(tileX + tile_width < image->dim.largura){
                histogramatile(image, tileX + tile_width, tileY, tile_width, tile_height, proxRed, proxGreen, proxBlue);
                histocumulativo(proxRed, proxCumuRed, totalPixels, clipLimite);
                histocumulativo(proxGreen, proxCumuGreen, totalPixels, clipLimite);
                histocumulativo(proxBlue, proxCumuBlue, totalPixels, clipLimite);
            } 
            else{
                for(k = 0; k < PIXELS; k++){
                    proxCumuRed[k] = cumulativoRed[k];
                    proxCumuGreen[k] = cumulativoGreen[k];
                    proxCumuBlue[k] = cumulativoBlue[k];
                }
            }

            if(tileY + tile_height < image->dim.altura){
                histogramatile(image, tileX, tileY + tile_height, tile_width, tile_height, abaixoRed, abaixoGreen, abaixoBlue);
                histocumulativo(abaixoRed, abaixoCumuRed, totalPixels, clipLimite);
                histocumulativo(abaixoGreen, abaixoCumuGreen, totalPixels, clipLimite);
                histocumulativo(abaixoBlue, abaixoCumuBlue, totalPixels, clipLimite);

                if(tileX + tile_width < image->dim.largura){
                    histogramatile(image, tileX + tile_width, tileY + tile_height, tile_width, tile_height, diagRed, diagGreen, diagBlue);
                    histocumulativo(diagRed, diagCumuRed, totalPixels, clipLimite);
                    histocumulativo(diagGreen, diagCumuGreen, totalPixels, clipLimite);
                    histocumulativo(diagBlue, diagCumuBlue, totalPixels, clipLimite);
                } 
                else{
                    for(k = 0; k < PIXELS; k++){
                        diagCumuRed[k] = abaixoCumuRed[k];
                        diagCumuGreen[k] = abaixoCumuGreen[k];
                        diagCumuBlue[k] = abaixoCumuBlue[k];
                    }
                }
            } 
            else{
                for(k = 0; k < PIXELS; k++){
                    abaixoCumuRed[k] = cumulativoRed[k];
                    abaixoCumuGreen[k] = cumulativoGreen[k];
                    abaixoCumuBlue[k] = cumulativoBlue[k];
                }
                for(k = 0; k < PIXELS; k++){
                    diagCumuRed[k] = proxCumuRed[k];
                    diagCumuGreen[k] = proxCumuGreen[k];
                    diagCumuBlue[k] = proxCumuBlue[k];
                }
            }

            homogeneizarClahe(imageclahe, tileX, tileY, tile_width, tile_height, cumulativoRed, cumulativoGreen, cumulativoBlue, proxCumuRed, proxCumuGreen, proxCumuBlue, abaixoCumuRed, abaixoCumuGreen, abaixoCumuBlue, diagCumuRed, diagCumuGreen, diagCumuBlue);
        }
    }

    return imageclahe;
}

void histogramatile_gray(const ImageGray *image, int tileX, int tileY, int tile_width, int tile_height, int *histogramagray){
    int i, j, idx;
    
    for(i = 0; i < PIXELS; i++){
        histogramagray[i] = 0;
    }
    for(i = tileY; i < tileY + tile_height && i < image->dim.altura; i++){
        for(j = tileX; j < tileX + tile_width && j < image->dim.largura; j++){
            idx = i * image->dim.largura + j;
            histogramagray[image->pixels[idx].value]++;
        }
    }
}

void aplicarClahe_gray(ImageGray *image, int tileX, int tileY, int tile_width, int tile_height, int *histocumulativo_gray){
    int i, j, idx;
    for(i = tileY; i < tileY + tile_height && i < image->dim.altura; i++){
        for(j = tileX; j < tileX + tile_width && j < image->dim.largura; j++){
            idx = i * image->dim.largura + j;
            image->pixels[idx].value = histocumulativo_gray[image->pixels[idx].value];
        }
    }
}

void homogeneizarClahe_gray(ImageGray *image, int tileX, int tileY, int tile_width, int tile_height,  int *histocumulativo_gray, int *next_histocumulativo_gray, int *below_histocumulativo_gray, int *diag_histocumulativo_gray) {
    int i, j, idx, srcX, srcY, interGray;
    float razaoX, razaoY;
    for(i = 0; i < tile_height; i++){
        for(j = 0; j < tile_width; j++){
            srcX = tileX + j;
            srcY = tileY + i;
            if (srcX >= image->dim.largura || srcY >= image->dim.altura) continue;
            idx = srcY * image->dim.largura + srcX;
            
            razaoX = (float)j / tile_width;
            razaoY = (float)i / tile_height;

            interGray = (1 - razaoX) * (1 - razaoY) * histocumulativo_gray[image->pixels[idx].value] + razaoX * (1 - razaoY) * next_histocumulativo_gray[image->pixels[idx].value] + (1 - razaoX) * razaoY * below_histocumulativo_gray[image->pixels[idx].value] + razaoX * razaoY * diag_histocumulativo_gray[image->pixels[idx].value];
     
            image->pixels[idx].value = interGray;
        }
    }
}

ImageGray *clahe_gray(const ImageGray *image, int tile_width, int tile_height){
    ImageGray *image_clahe_gray = (ImageGray*)malloc(sizeof(ImageGray));
    image_clahe_gray->dim.altura = image->dim.altura;
    image_clahe_gray->dim.largura = image->dim.largura;
    image_clahe_gray->pixels = (PixelGray*)malloc((image_clahe_gray->dim.altura * image_clahe_gray->dim.largura) * sizeof(PixelGray));

    int histoGray[PIXELS], cumulativoGray[PIXELS], proxGray[PIXELS], proxCumuGray[PIXELS];
    int abaixoGray[PIXELS], abaixoCumuGray[PIXELS], diagGray[PIXELS], diagCumuGray[PIXELS];
    int totalPixels = tile_width * tile_height, i, j, k, tileX, tileY;
    float clipLimite = totalPixels / 256.0 * 2.0;

    for(i = 0; i < image->dim.altura; i++){
        for(j = 0; j < image->dim.largura; j++){
            image_clahe_gray->pixels[i * image->dim.largura + j] = image->pixels[i * image->dim.largura + j];
        }
    }

    for(tileY = 0; tileY < image->dim.altura; tileY += tile_height){
        for(tileX = 0; tileX < image->dim.largura; tileX += tile_width){
            histogramatile_gray(image, tileX, tileY, tile_width, tile_height, histoGray);
            histocumulativo(histoGray, cumulativoGray, totalPixels, clipLimite);

            if(tileX + tile_width < image->dim.largura){
                histogramatile_gray(image, tileX + tile_width, tileY, tile_width, tile_height, proxGray);
                histocumulativo(proxGray, proxCumuGray, totalPixels, clipLimite);
            } 
            else{
                for(k = 0; k < PIXELS; k++)
                    proxCumuGray[k] = cumulativoGray[k];
            }

            if(tileY + tile_height < image->dim.altura){
                histogramatile_gray(image, tileX, tileY + tile_height, tile_width, tile_height, abaixoGray);
                histocumulativo(abaixoGray, abaixoCumuGray, totalPixels, clipLimite);

                if(tileX + tile_width < image->dim.largura){
                    histogramatile_gray(image, tileX + tile_width, tileY + tile_height, tile_width, tile_height, diagGray);
                    histocumulativo(diagGray, diagCumuGray, totalPixels, clipLimite);
                } 
                else{
                    for(k = 0; k < PIXELS; k++)
                        diagCumuGray[k] = abaixoCumuGray[k];
                }
            } 
            else{
                for(k = 0; k < PIXELS; k++)
                    abaixoCumuGray[k] = cumulativoGray[k];
                for(k = 0; k < PIXELS; k++)
                    diagCumuGray[k] = proxCumuGray[k];
            }

            homogeneizarClahe_gray(image_clahe_gray, tileX, tileY, tile_width, tile_height, cumulativoGray, proxCumuGray, abaixoCumuGray, diagCumuGray);
        }
    }
    
    return image_clahe_gray;
}

ImageRGB *randomicoRGB(ImageRGB *image){
    int num, i, res, tW, tH, kernel;
    srand(time(NULL));
    ImageRGB *imgRand = (ImageRGB*)malloc(sizeof(ImageRGB));
    ImageRGB *tempImage = image;

    printf("\tDigite a quantidade de operacoes: ");
    scanf("%d", &num);

    printf("\tPara a operacao de CLAHE declare o tileW e o tileH: ");
    scanf("%d %d", &tW, &tH);

    printf("\tPara a operacao de blur declare o kernel size: ");
    scanf("%d", &kernel);
    printf("\n");
    for(i = 0; i < num; i++){
        res = rand() % 5 + 1;
        if(res == 1){
            imgRand = flip_vertical_rgb(tempImage);
            tempImage = imgRand;
            printf("\t%d - Flip vertical aplicado\n", i + 1);
        }
        else if(res == 2){
            imgRand = flip_horizontal_rgb(tempImage);
            tempImage = imgRand;
            printf("\t%d - Flip horizontal aplicado\n", i + 1);
        }
        else if(res == 3){
            imgRand = transpose_rgb(tempImage);
            tempImage = imgRand;
            printf("\t%d - Transpose aplicado\n", i + 1);
        }
        else if(res == 4){
            imgRand = median_blur_rgb(tempImage, kernel);
            tempImage = imgRand;
            printf("\t%d - Efeito blur aplicado\n", i + 1);
        }
        else if(res == 5){
            imgRand = clahe_rgb(tempImage, tW, tH);
            tempImage = imgRand;
            printf("\t%d - Efeito clahe aplicado\n", i + 1);
        }
    }

    return imgRand;
}

ImageGray *flip_vertical_gray(ImageGray *image){
    ImageGray *flipVert = (ImageGray*)malloc(sizeof(ImageGray));
    flipVert->dim.altura = image->dim.altura;
    flipVert->dim.largura = image->dim.largura;
    flipVert->pixels = (PixelGray*)malloc((flipVert->dim.altura * flipVert->dim.largura) * sizeof(PixelGray));
    int i, j, x, y;
    for(i = image->dim.altura - 1, x = 0; i >= 0; i--, x++){
        for(j = image->dim.largura - 1, y = 0; j >= 0; j--, y++){
            flipVert->pixels[x * flipVert->dim.largura + y].value = image->pixels[i * image->dim.largura + j].value;
        }
    }
    return flipVert;
}

ElementoDuploGray *retornaInicioGray(ElementoDuploGray *l) {
    if (l == NULL) return NULL;
    while (l->ant != NULL) {
        l = l->ant;
    }
    return l;
}

ElementoDuploGray *retornaFimGray(ElementoDuploGray *l) {
    if (l == NULL) return NULL;
    while (l->prox != NULL) {
        l = l->prox;
    }
    return l;
}

ElementoDuploGray *ProximaImagemGray(ElementoDuploGray *l) {
    if (l == NULL) return NULL;
    return l->ant;
}

ElementoDuploGray *ImagemAnteriorGray(ElementoDuploGray *l) {
    if (l == NULL) return NULL;
    return l->prox;
}

ElementoDuploGray* addInicioDuplamenteGray(ElementoDuploGray* l, ImageGray* image) {
    ElementoDuploGray *novo = (ElementoDuploGray*)malloc(sizeof(ElementoDuploGray));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(1);
    }
    novo->image = image;

    if (l == NULL) {
        novo->prox = NULL;
        novo->ant = NULL;
        return novo;
    }
    
    ElementoDuploGray *inicio = retornaInicioGray(l);
    
    novo->prox = inicio;
    novo->ant = NULL;
    inicio->ant = novo;
    
    return novo;
}

void mostrarHistoricoGray(ElementoDuploGray *l) {
    if (l == NULL) {
        printf("Nao ha imagens no historico.\n");
        return;
    }
    
    ElementoDuploGray *aux = retornaFimGray(l);
    while (aux != NULL) {
        exibir_image_gray(aux->image);
        aux = aux->prox;
    }
}

ElementoDuploRGB *retornaInicioRGB(ElementoDuploRGB *l){
    if (!l) return NULL;
    while(l->ant != NULL)
        l = l->ant;

    return l;
}

ElementoDuploRGB *retornaFimRGB(ElementoDuploRGB *l){
    if (!l) return NULL;
    while(l->prox != NULL)
        l = l->prox;

    return l;
}

ElementoDuploRGB *ProximaImagemRGB(ElementoDuploRGB *l){
    if (!l) return NULL;
    return l->ant;
}

ElementoDuploRGB *ImagemAnteriorRGB(ElementoDuploRGB *l){
    if (!l) return NULL;
    return l->prox;
}

ElementoDuploRGB *addInicioDuplamenteRGB(ElementoDuploRGB *l, ImageRGB *image){
    ElementoDuploRGB *novo = (ElementoDuploRGB*)malloc(sizeof(ElementoDuploRGB));
    if(!novo){
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(1);
    }
    novo->image = image;

    if(!l){
        novo->prox = NULL;
        novo->ant = NULL;
        return novo;
    }

    ElementoDuploRGB *inicio = retornaInicioRGB(l);

    novo->prox = inicio;
    novo->ant = NULL;
    inicio->ant = novo;

    return novo;
}

void mostrarHistoricoRGB(ElementoDuploRGB *l){
    if(l){
        ElementoDuploRGB *aux = retornaFimRGB(l);
        while(aux){
            exibir_image(aux->image);
            aux = aux->prox;
        }
    }
    else{
        printf("Nao ha imagens no historico.\n");
        return;
    }
}

int _main(){
    FILE *arq = fopen("../utils/input_image_example_RGB.txt", "r");
    if(arq == NULL){
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    FILE *arquivogray = fopen("../utils/input_image_example_Gray.txt", "r");
    if(arquivogray == NULL){
        perror("ERRO AO ABRIR O ARQUIVO GRAY.");
        return 1;
    }

    int altura, largura, tile_width, tile_height;

    ImageRGB *image = NULL;
    ImageGray *imagegray = NULL;

    int opc, kernel_size;
    do {
        printf("_____ MENU ______\n");
        printf("0 - Sair\n");
        printf("1 - Criar imagem RGB\n");
        printf("2 - CLAHE RGB\n");
        printf("3 - RGB no blur\n");
        printf("4 - TranposeRGB\n");
        printf("5 - FlipVerticalRGB\n");
        printf("6 - Converte RGB para Gray\n");
        printf("7 - Criar imagem Gray\n");
        printf("8 - FlipHorizontalRGB\n");
        printf("9 - FlipHorizontalGRAY\n");
        printf("10 - Gray no blur\n");
        printf("11 - FlipVerticalGRAY\n");
        printf("12 - CLAHE GRAY\n");
        printf("13 - Tranposegray\n");
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
                exibir_image(image);
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
                break;
            case 6:
                if(!image){
                    printf("Crie uma imagem RGB primeiro!\n");
                } 
                else{
                    ImageGray *imagegray = conversion_image_gray(image);
                    system("PAUSE");
                    exibir_image_gray(imagegray);
                    free_image_gray(imagegray);
                }
                system("PAUSE");
                system("cls");
                break;
            case 7:
                if(imagegray != NULL){
                    free_image_gray(imagegray);
                }
                fscanf(arquivogray, "%d", &altura);
                fscanf(arquivogray, "%d", &largura);
                imagegray = create_image_gray(largura, altura, arquivogray);
                printf("IMAGEM EM ESCALA DE CINZA CRIADA COM SUCESSO!\n");
                exibir_image_gray(imagegray);
                break;
            case 8:
                if(!image){
                    printf("CRIE UMA IMAGEM RGB PRIMEIRO!\n");
                }else{
                    ImageRGB *flipHorizontal = flip_horizontal_rgb(image);
                    exibir_image(flipHorizontal);
                    free_image_rgb(flipHorizontal);
                }
                break;
            case 9:
                if(!imagegray){
                    printf("CRIE UMA IMAGEM GRAY PRIMEIRO!\n");
                }else{
                    ImageGray *flipGray = flip_horizontal_gray(imagegray);
                    exibir_image_gray(flipGray);
                    free_image_gray(flipGray);
                }
                // exibir_image_gray(imagegray);
                break;
            case 10:
                if(!imagegray){
                    printf("Crie uma imagem gray primeiro!\n");
                } 
                else{
                    do{
                        printf("Digite o tamanho do kernel: ");
                        scanf("%d", &kernel_size);
                        if(kernel_size % 2 == 0){
                            printf("O tamanho precisa ser impar\n");
                        }
                    }while(kernel_size % 2 == 0);
                    ImageGray *new_image = median_blur_gray(imagegray, kernel_size);
                    exibir_image_gray(new_image);
                    free_image_gray(new_image);
                }
                system("PAUSE");
                system("cls");
                break;
            
            case 11:
                if(!imagegray){
                    printf("Crie uma imagem gray primeiro!\n");
                } 
                else{
                    ImageGray *flipGray = flip_vertical_gray(imagegray);
                    exibir_image_gray(flipGray);
                    free_image_gray(flipGray);
                }
                break;
            case 12:
                if(!imagegray){
                    printf("Crie uma imagem gray primeiro!\n");
                } 
                else{
                    do{
                        printf("Digite o tamanho do tile (largura): ");
                        scanf("%d", &tile_width);
                        printf("Digite o tamanho do tile (altura): ");
                        scanf("%d", &tile_height);
                    }while(tile_width <= 0 || tile_height <= 0);
                    ImageGray *new_image = clahe_gray(imagegray, tile_width, tile_height);
                    exibir_image_gray(new_image);
                    free_image_gray(new_image);
                }
                system("PAUSE");
                system("cls");
                break;
            case 13:
                if(!imagegray){
                    printf("Crie uma imagem Gray primeiro!\n");
                }else{
                    ImageGray *transposedGray = transpose_gray(imagegray);
                    exibir_image_gray(transposedGray);
                }
                break;
            case 14:
                if(image != NULL){
                    exibir_image(image);
                    printf("\n");
                    exibir_image_gray(imagegray);
                }
                fclose(arq);
                break;
            default:
                printf("Opcao invalida!\n");
                system("PAUSE");
                system("cls");
                break;
        }
    }while(opc != 0);

    if(image != NULL){
        free_image_rgb(image);
    }
    fclose(arq);

    return 0;
}