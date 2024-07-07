#pragma once
#include <stdio.h>

typedef struct dimensoes {
    int altura, largura;
} Dimensoes;

typedef struct pixelRGB {
    int red, blue, green;
} PixelRGB;

typedef struct pixelGray {
    int value;
} PixelGray;

typedef struct imageGray {
    Dimensoes dim;
    PixelGray *pixels;
} ImageGray;

typedef struct imageRGB {
    Dimensoes dim;
    PixelRGB *pixels;
} ImageRGB;


// Lista Duplamente Encadeada RGB
typedef struct elementoduploRGB{
    struct elementoduploRGB *ant;
    struct elementoduploRGB *prox;
    ImageRGB *image;
} ElementoDuploRGB;

// Lista Duplamente Encadeada Gray
typedef struct elementoduploGray{
    struct elementoduploGray *ant;
    struct elementoduploGray *prox;
    ImageGray *image;
} ElementoDuploGray;

// Funções de criação e liberação
void print_pixel_gray(PixelGray pixel);
ImageGray *conversion_image_gray(const ImageRGB *imagemrgb);
ImageGray *create_image_gray(int largura, int altura, FILE *arq);
void exibir_image_gray(ImageGray *image);
void free_image_gray(ImageGray *image);

void print_pixel_color(PixelRGB pixel);
ImageRGB *create_image_rgb(int largura, int altura, FILE *arq);
void exibir_image(ImageRGB *image);
void free_image_rgb(ImageRGB *image);

// Operações para ImageGray
ImageGray *flip_vertical_gray(ImageGray *image);
ImageGray *flip_horizontal_gray(const ImageGray *image);
ImageGray *transpose_gray(const ImageGray *image);
ImageGray *randomicoGray(ImageGray *image, int res, int tW, int tH, int kernel);

// Operações para ImageRGB
ImageRGB *flip_vertical_rgb(const ImageRGB *image);
ImageRGB *flip_horizontal_rgb(const ImageRGB *image);
ImageRGB *transpose_rgb(const ImageRGB *image);
ImageRGB *randomicoRGB(ImageRGB *image, int res, int tW, int tH, int kernel);

// Manipulação por pixel para ImageGray
ImageGray *clahe_gray(const ImageGray *image, int tile_width, int tile_height);
ImageGray *median_blur_gray(const ImageGray *image, int kernel_size);

// Manipulação por pixel para ImageRGB
ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height);
ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size);

// Manipulação de Lista Gray
void addInicioDuplamente_Gray(ElementoDuploGray **l, ImageGray *image);
ElementoDuploGray *retornaInicioGray(ElementoDuploGray *l);
void freelistagray(ElementoDuploGray **l);

// Manipulação de Lista RGB
void addInicioDuplamente_RGB(ElementoDuploRGB **l, ImageRGB *image);
ElementoDuploRGB *retornaInicioRGB(ElementoDuploRGB *l);
void freelistargb(ElementoDuploRGB **l);

// Manipulação de Lista Gray e RGB randomico
void freelistargbRand(ElementoDuploRGB **l);
void freelistagrayRand(ElementoDuploGray **l);