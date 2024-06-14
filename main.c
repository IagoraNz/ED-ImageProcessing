#include <stdio.h>
#include <stdlib.h>
#include "image.c"


int main(){
    int opc, altura = 0, largura = 0, ops = 0, tipo, kernel_size, tile_width, tile_height;

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

    int altRGB = 0, largRGB = 0;

    FILE *arqRGB = fopen("../utils/input_image_example_RGB.txt", "r");
    if(!arqRGB){
        printf("Erro ao abrir o arquivo RGB");
        exit(1);
    }
    fscanf(arqRGB, "%d", &altRGB);
    fscanf(arqRGB, "%d", &largRGB);
    ImageRGB *imageRGB = create_image_rgb(altRGB, largRGB, arqRGB);
    fclose(arqRGB);
    ElementoDuploRGB *elementoRGB = NULL;
    elementoRGB = addInicioDuplamenteRGB(elementoRGB, imageRGB);

    printf("\tSTART\n\n");
    printf("\t1 - Imagem Gray\n");
    printf("\t2 - Imagem RGB\n\n");
    printf("\tDigite o tipo de imagem: ");
    scanf("%d", &tipo);
    printf("\n\t");
    system("PAUSE");
    system("cls");

    switch (tipo)
    {
    case 1:
        do
        {
            printf("\tProcessamento de imagem gray\n\n");
            printf("\t1 - Flip horizontal\n");
            printf("\t2 - Efeito blur\n");
            printf("\t3 - Flip vertical\n");
            printf("\t4 - Efeito clahe\n");
            printf("\t5 - Tranpose imagem\n");
            printf("\t6 - Exibir historico\n");
            printf("\t7 - Exibir imagem atual\n");
            printf("\t0 - Sair\n");
            printf("\tInforme a opcao desejada: ");
            scanf("%d", &opc);
            switch (opc)
            {
            case 1:
                elemento = addInicioDuplamenteGray(elemento, flip_horizontal_gray(elemento->image));
                printf("\tFlip horizontal aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 2:
                elemento = addInicioDuplamenteGray(elemento, median_blur_gray(elemento->image, 11));
                printf("\tBlur aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 3:
                elemento = addInicioDuplamenteGray(elemento, flip_vertical_gray(elemento->image));
                printf("\tFlip vertical aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 4:
                elemento = addInicioDuplamenteGray(elemento, clahe_gray(elemento->image, 512, 512));
                printf("\tEfeito clahe aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 5:
                elemento = addInicioDuplamenteGray(elemento, transpose_gray(elemento->image));
                printf("\tImagem transposta com sucesso\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 6:
                mostrarHistoricoGray(elemento);
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 7:
                exibir_image_gray(elemento->image);
                if(elemento->prox != NULL){
                    do{
                        printf("\t1 - Proxima imagem\n");
                        printf("\t2 - Imagem anterior\n");
                        printf("\t0 - Sair\n");
                        printf("\tInforme a opcao desejada: ");
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
                                printf("\tOpcao invalida\n");
                                break;
                        }
                        if(ops != 0){
                            exibir_image_gray(elemento->image);
                        }
                    }while(ops != 0);  
                }
                break;
            default:
                printf("\tOpcao invalida\n");  
                break;
            }
        }while(opc != 0);
        break;

    case 2:
        do{
            printf("\tProcessamento de imagem RGB\n\n");
            printf("\t1 - Flip horizontal\n");
            printf("\t2 - Efeito blur\n");
            printf("\t3 - Flip vertical\n");
            printf("\t4 - Efeito clahe\n");
            printf("\t5 - Tranpose imagem\n");
            printf("\t6 - Exibir historico\n");
            printf("\t7 - Exibir imagem atual\n");
            printf("\t0 - Sair\n");
            printf("\tDigite a opcao desejada: ");
            scanf("%d", &opc);

            switch (opc)
            {
            case 1:
                elementoRGB = addInicioDuplamenteRGB(elementoRGB, flip_horizontal_rgb(elementoRGB->image));
                printf("\tFlip horizontal aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 2:
                printf("\tDigite o tamanho do kernel: ");
                scanf("%d", &kernel_size);
                elementoRGB = addInicioDuplamenteRGB(elementoRGB, median_blur_rgb(elementoRGB->image, kernel_size));
                printf("\tBlur aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 3:
                elementoRGB = addInicioDuplamenteRGB(elementoRGB, flip_vertical_rgb(elementoRGB->image));
                printf("\tFlip vertical aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 4:
                printf("\tDigite o tamanho do tile width: ");
                scanf("%d", &tile_width);
                printf("\tDigite o tamanho do tile height: ");
                scanf("%d", &tile_height);
                elementoRGB = addInicioDuplamenteRGB(elementoRGB, clahe_rgb(elementoRGB->image, tile_width, tile_height));
                printf("\tEfeito clahe aplicado\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 5:
                elementoRGB = addInicioDuplamenteRGB(elementoRGB, transpose_rgb(elementoRGB->image));
                printf("\tImagem transposta com sucesso\n");
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 6:
                mostrarHistoricoRGB(elementoRGB);
                printf("\n\t");
                system("PAUSE");
                system("cls");
                break;
            case 7:
                exibir_image(elementoRGB->image);
                if(elementoRGB->prox != NULL){
                    do{
                        printf("\t1 - Proxima imagem\n");
                        printf("\t2 - Imagem anterior\n");
                        printf("\t0 - Sair\n");
                        printf("\tInforme a opcao desejada: ");
                        scanf("%d", &ops);
                        switch (ops)
                        {
                            case 1:
                                elementoRGB = ProximaImagemRGB(elementoRGB);
                                break;
                            case 2:
                                elementoRGB = ImagemAnteriorRGB(elementoRGB);
                                break;
                            default:
                                printf("\tOpcao invalida\n");
                                break;
                        }
                        if(ops != 0){
                            exibir_image(elementoRGB->image);
                        }
                    }while(ops != 0);  
                }
                break;
            default:
                break;
            }
        }while(opc != 0);
        break;

    default:
        printf("\tOpcao invalida\n");
        break;
    }
    
    return 0;
}