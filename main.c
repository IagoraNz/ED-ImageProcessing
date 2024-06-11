#include <stdio.h>
#include <stdlib.h>
#include "image.c"


int main(){
    int opc, altura = 0, largura = 0, ops = 0;
    FILE *arq = fopen("../utils/input_image_example_Gray.txt", "r");
    if(arq == NULL){
        perror("ERRO AO ABRIR O ARQUIVO GRAY.");
        return 1;
    }
    fscanf(arq, "%d", &altura);
    fscanf(arq, "%d", &largura);
    ImageGray *image = create_image_gray(altura, largura, arq);
    fclose(arq);
    ElementoDuploGray *elemento = NULL;
    elemento = addInicioDuplamenteGray(elemento, image);
    do
    {
        printf("-------------------\n");
        printf("1 - FlipHorizontalGRAY\n");
        printf("2 - Gray no blur\n");
        printf("3 - FlipVerticalGRAY\n");
        printf("4 - CLAHE GRAY\n");
        printf("5 - Tranposegray\n");
        printf("6 - Exibir Historico\n");
        printf("7 - Exibir Imagem Atual\n");
        printf("0 - Sair\n");
        printf("Informe a opção desejada: ");
        scanf("%d", &opc);
        switch (opc)
        {
        case 1:
            elemento = addInicioDuplamenteGray(elemento, flip_horizontal_gray(elemento->image));
            printf("FlipHorizontalGRAY\n");
            break;
        case 2:
            elemento = addInicioDuplamenteGray(elemento, median_blur_gray(elemento->image, 11));
            printf("Gray no blur\n");
            break;
        case 3:
            elemento = addInicioDuplamenteGray(elemento, flip_vertical_gray(elemento->image));
            printf("FlipVerticalGRAY\n");
            break;
        case 4:
            elemento = addInicioDuplamenteGray(elemento, clahe_gray(elemento->image, 512, 512));
            printf("CLAHE GRAY\n");
            break;
        case 5:
            elemento = addInicioDuplamenteGray(elemento, transpose_gray(elemento->image));
            printf("Tranposegray\n");
            break;
        case 6:
            mostrarHistoricoGray(elemento);
            break;
        case 7:
            exibir_image_gray(elemento->image);
            if (elemento->prox != NULL)
            {
                do
                {
                    printf("1 - Proxima Imagem\n");
                    printf("2 - Imagem Anterior\n");
                    printf("0 - Sair\n");
                    printf("Informe a opção desejada: ");
                    scanf("%d", &ops);
                    switch (ops)
                    {
                        case 1:
                            elemento = ProximaImagemGray(elemento);
                            break;
                        case 2:
                            elemento = ImagemAnteriorGray(elemento);
                            break;
                        default:
                            break;

                    }
                    if (ops != 0)
                    {
                        exibir_image_gray(elemento->image);
                    }
                } while (ops != 0);  
            }
            break;
        default:    
            break;
        }
    } while (opc != 0);
    
    
    return 0;
}