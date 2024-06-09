#include <stdio.h>
#include <gtk/gtk.h>
#include "image.h" // Supondo que você tenha seu próprio arquivo image.h


static void set_img_to_pixbuf_rgb(GdkPixbuf *pixbuf, ImageRGB *image) {
    // Obtendo os pixels do GdkPixbuf
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

    // Copiando os pixels da imagem para o GdkPixbuf
    for (int y = 0; y < image->dim.altura; y++) {
        for (int x = 0; x < image->dim.largura; x++) {
            int i = y * rowstride + x * 3;
            pixels[i + 0] = image->pixels[y * image->dim.largura + x].red;
            pixels[i + 1] = image->pixels[y * image->dim.largura + x].green;
            pixels[i + 2] = image->pixels[y * image->dim.largura + x].blue;
        }
    }
}

// Função para exibir uma imagem RGB em uma janela GTK
void show_image_rgb(ImageRGB *image) {

    // Inicializando o GTK
    gtk_init(NULL, NULL);

    // Criando uma janela
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Imagem RGB");
    gtk_window_set_default_size(GTK_WINDOW(window), image->dim.largura, image->dim.altura);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Criando um container para a imagem
    GtkWidget *image_widget = gtk_image_new();
    gtk_container_add(GTK_CONTAINER(window), image_widget);

    // Criando um GdkPixbuf para a imagem
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, image->dim.largura, image->dim.altura);
    set_img_to_pixbuf_rgb(pixbuf, image);

    // Exibindo a imagem no widget
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);

    // Exibindo a janela
    gtk_widget_show_all(window);

    // Iniciando o loop principal do GTK
    gtk_main();
}

int main(int argc, char *argv[]) {
    // Carregando a imagem RGB
    FILE *arq = fopen("input_rgb.txt", "rb");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 1;
    }
    int altura, largura;
    fscanf(arq, "%d", &altura);
    fscanf(arq, "%d", &largura);
    ImageRGB *historicoRGBAtual = create_image_rgb(largura, altura, arq);
    fclose(arq);   

    // Exibindo a imagem RGB
    show_image_rgb(historicoRGBAtual);

    // Liberando a imagem RGB
    free_image_rgb(historicoRGBAtual);
    return 0;
}
