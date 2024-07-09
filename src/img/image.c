#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define PIXELS 256

void print_pixel_gray(PixelGray pixel){
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

void exibir_image_gray(ImageGray *image){
    for(int x=0; x<image->dim.altura; x++){
        for(int y=0; y<image->dim.largura; y++){
            print_pixel_gray(image->pixels[x*image->dim.largura+y]);
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
    if(image != NULL){
        free(image->pixels);
        free(image);
    }
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

    int proxTileW, proxTileH;

    for(tileY = 0; tileY < image->dim.altura; tileY += tile_height){
        for(tileX = 0; tileX < image->dim.largura; tileX += tile_width){
            histogramatile(image, tileX, tileY, tile_width, tile_height, histoRed, histoGreen, histoBlue);
            histocumulativo(histoRed, cumulativoRed, totalPixels, clipLimite);
            histocumulativo(histoGreen, cumulativoGreen, totalPixels, clipLimite);
            histocumulativo(histoBlue, cumulativoBlue, totalPixels, clipLimite);
            
            proxTileW = tile_width;
            proxTileH = tile_height;

            if(tileX + tile_width < image->dim.largura){
                histogramatile(image, tileX + tile_width, tileY, proxTileW, proxTileH, proxRed, proxGreen, proxBlue);
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
                histogramatile(image, tileX, tileY + tile_height, tile_width, proxTileH, abaixoRed, abaixoGreen, abaixoBlue);
                histocumulativo(abaixoRed, abaixoCumuRed, totalPixels, clipLimite);
                histocumulativo(abaixoGreen, abaixoCumuGreen, totalPixels, clipLimite);
                histocumulativo(abaixoBlue, abaixoCumuBlue, totalPixels, clipLimite);

                if(tileX + tile_width < image->dim.largura){
                    histogramatile(image, tileX + tile_width, tileY + tile_height, proxTileW, proxTileH, diagRed, diagGreen, diagBlue);
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

    int proxTileW, proxTileH;

    for(tileY = 0; tileY < image->dim.altura; tileY += tile_height){
        for(tileX = 0; tileX < image->dim.largura; tileX += tile_width){
            histogramatile_gray(image, tileX, tileY, tile_width, tile_height, histoGray);
            histocumulativo(histoGray, cumulativoGray, totalPixels, clipLimite);
            
            proxTileW = tile_width;
            proxTileH = tile_height;

            if(tileX + tile_width < image->dim.largura){
                histogramatile_gray(image, tileX + tile_width, tileY, proxTileW, proxTileH, proxGray);
                histocumulativo(proxGray, proxCumuGray, totalPixels, clipLimite);
            } 
            else{
                for(k = 0; k < PIXELS; k++)
                    proxCumuGray[k] = cumulativoGray[k];
            }

            if(tileY + tile_height < image->dim.altura){
                histogramatile_gray(image, tileX, tileY + tile_height, tile_width, proxTileH, abaixoGray);
                histocumulativo(abaixoGray, abaixoCumuGray, totalPixels, clipLimite);

                if(tileX + tile_width < image->dim.largura){
                    histogramatile_gray(image, tileX + tile_width, tileY + tile_height, proxTileW, proxTileH, diagGray);
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

ImageRGB *randomicoRGB(ImageRGB *image, int res, int tW, int tH, int kernel) {
    ImageRGB *imgRand = (ImageRGB*)malloc(sizeof(ImageRGB));

    if(res == 1){
        imgRand = flip_vertical_rgb(image);
        printf("\tFlip vertical aplicado\n");
    } 
    else if(res == 2){
        imgRand = flip_horizontal_rgb(image);
        printf("\tFlip horizontal aplicado\n");
    } 
    else if(res == 3){
        imgRand = transpose_rgb(image);
        printf("\tTranspose aplicado\n");
    } 
    else if(res == 4){
        imgRand = median_blur_rgb(image, kernel);
        printf("\tEfeito blur aplicado\n");
    } 
    else if(res == 5){
        imgRand = clahe_rgb(image, tW, tH);
        printf("\tEfeito clahe aplicado\n");
    }

    return imgRand;
}


ImageGray *randomicoGray(ImageGray *image, int res, int tW, int tH, int kernel){
    ImageGray *imgRand = (ImageGray*)malloc(sizeof(ImageGray));

    if(res == 1){
        imgRand = flip_vertical_gray(image);
        printf("\tFlip vertical aplicado\n");
    }
    else if(res == 2){
        imgRand = flip_horizontal_gray(image);
        printf("\tFlip horizontal aplicado\n");
    }
    else if(res == 3){
        imgRand = transpose_gray(image);
        printf("\tTranspose aplicado\n");
    }
    else if(res == 4){
        imgRand = median_blur_gray(image, kernel);
        printf("\tEfeito blur aplicado\n");
    }
    else if(res == 5){
        imgRand = clahe_gray(image, tW, tH);
        printf("\tEfeito clahe aplicado\n");
    }

    return imgRand;
}

ElementoDuploGray *retornaInicioGray(ElementoDuploGray *l) {
    if (l == NULL) return NULL;
    while (l->ant != NULL) {
        l = l->ant;
    }
    return l;
}

void freeGray(ElementoDuploGray *l) {
    ElementoDuploGray *aux;
    while (l != NULL) {
        aux = l;
        l = l->ant;
        free_image_gray(aux->image);
        free(aux);
        printf("Lista de imagen!\n");
    }
}

void addInicioDuplamente_Gray(ElementoDuploGray **l, ImageGray *image) {
    ElementoDuploGray *novo = (ElementoDuploGray*)malloc(sizeof(ElementoDuploGray));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(1);
    }

    if (*l != NULL){
        if ((*l)->ant != NULL){
            freeGray((*l)->ant);
            (*l)->ant = NULL;
        }
    }
    
    novo->image = image;
    novo->ant = NULL;

    if(!*l){
        novo->prox = NULL;
    } 
    else{
        novo->prox = *l;
        (*l)->ant = novo;
    }

    // Atualiza o ponteiro para o início da lista
    *l = novo;
    printf("Imagem adicionada ao historico.\n");
}

ElementoDuploRGB *retornaInicioRGB(ElementoDuploRGB *l){
    if (!l) return NULL;
    while(l->ant != NULL)
        l = l->ant;
    return l;
}

void freeRGB(ElementoDuploRGB *l){
    ElementoDuploRGB *aux;
    while(l != NULL){
        aux = l;
        l = l->ant;
        free_image_rgb(aux->image);
        free(aux);
    }
    printf("Lista de imagens!\n");
}

void addInicioDuplamente_RGB(ElementoDuploRGB **l, ImageRGB *image) {
    ElementoDuploRGB *novo = (ElementoDuploRGB*)malloc(sizeof(ElementoDuploRGB));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(1);
    }

    if (*l != NULL){
        if ((*l)->ant != NULL){
            freeRGB((*l)->ant);
            (*l)->ant = NULL;
        }
    }

    novo->image = image;
    novo->ant = NULL;

    if(!*l){
        novo->prox = NULL;
    } 
    else{
        novo->prox = *l;
        (*l)->ant = novo;
    }

    *l = novo;
    printf("Imagem adicionada ao historico.\n");
}

void freelistagray(ElementoDuploGray **l){
    ElementoDuploGray *aux;
    *l = retornaInicioGray(*l);
    while(*l != NULL){
        aux= *l;
        *l= (*l)->prox;
        free_image_gray(aux->image);
        free(aux);
    }
    printf("Lista de imagens em tons de cinza liberada.\n");
}

void freelistargb(ElementoDuploRGB **l){
    ElementoDuploRGB *aux;
    *l = retornaInicioRGB(*l);
    while(*l != NULL){
        aux = *l;
        *l = (*l)->prox;
        free_image_rgb(aux->image);
        free(aux);
    }
    printf("Lista de imagens RGB liberada.\n");
}