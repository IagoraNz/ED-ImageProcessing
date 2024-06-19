#include "interface.h"

// Declaração global de image_widget
GtkWidget *image_widget = NULL;
GtkWidget *button_previous = NULL;
GtkWidget *button_next = NULL;
ElementoDuploRGB *current_image = NULL;

// Função para copiar os pixels da imagem para o GdkPixbuf
void set_img_to_pixbuf_rgb(GdkPixbuf *pixbuf, ImageRGB *image) {
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

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
void show_image_rgb(ImageRGB *image, GtkWidget *image_widget) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, image->dim.largura, image->dim.altura);
    set_img_to_pixbuf_rgb(pixbuf, image);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
    g_object_unref(pixbuf);
}

// Função para atualizar a sensibilidade dos botões de navegação
void update_navigation_buttons(ElementoDuploRGB *lista) {
    gtk_widget_set_sensitive(button_previous, lista->prox != NULL);
    gtk_widget_set_sensitive(button_next, lista->ant != NULL);
}

// Função para o evento de clique no botão de transposição
void transpose_click(GtkWidget *widget, gpointer data) {
    addInicioDuplamente_RGB(&current_image, transpose_rgb(current_image->image));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void clahe_click(GtkWidget *widget, gpointer data) {
    addInicioDuplamente_RGB(&current_image, clahe_rgb(current_image->image, 512, 512));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void flip_vertical_click(GtkWidget *widget, gpointer data) {
    addInicioDuplamente_RGB(&current_image, flip_vertical_rgb(current_image->image));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void flip_horizontal_click(GtkWidget *widget, gpointer data) {
    addInicioDuplamente_RGB(&current_image, flip_horizontal_rgb(current_image->image));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void blur_click(GtkWidget *widget, gpointer data) {
    addInicioDuplamente_RGB(&current_image, median_blur_rgb(current_image->image, 3));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

// Função para o evento de clique no botão anterior
void previous_click(GtkWidget *widget, gpointer data) {
    if (current_image->prox != NULL) {
        current_image = current_image->prox;
        printf("Previous image dimensions: %d x %d\n", current_image->image->dim.altura, current_image->image->dim.largura); // Log
        show_image_rgb(current_image->image, image_widget);
        update_navigation_buttons(current_image);
    } else {
        printf("No previous image available.\n"); // Log
    }
}

// Função para o evento de clique no botão próximo
void next_click(GtkWidget *widget, gpointer data) {
    if (current_image->ant != NULL) {
        current_image = current_image->ant;
        printf("Next image dimensions: %d x %d\n", current_image->image->dim.altura, current_image->image->dim.largura); // Log
        show_image_rgb(current_image->image, image_widget);
        update_navigation_buttons(current_image);
    } else {
        printf("No next image available.\n"); // Log
    }
}

// Função para salvar a imagem atual
void save_current_image(GtkWidget *widget, gpointer data) {
    if (current_image == NULL || current_image->image == NULL) {
        fprintf(stderr, "Não há imagem atual para salvar.\n");
        return;
    }

    const char *output_path = "output_image.png"; // Pode mudar para outro caminho ou pedir ao usuário para escolher
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, current_image->image->dim.largura, current_image->image->dim.altura);
    set_img_to_pixbuf_rgb(pixbuf, current_image->image);
    gdk_pixbuf_save(pixbuf, output_path, "png", NULL, NULL);
    g_object_unref(pixbuf);

    printf("Imagem salva em %s\n", output_path); // Log
}

// Função para exibir a interface gráfica
void show_interface_rgb() {
    FILE *arq = fopen("input_rgb.txt", "rb");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return;
    }
    int altura, largura;
    fscanf(arq, "%d", &altura);
    fscanf(arq, "%d", &largura);
    ImageRGB *image = create_image_rgb(largura, altura, arq);
    fclose(arq);

    // Verificar se a imagem foi criada corretamente
    if (image == NULL) {
        fprintf(stderr, "Erro ao criar a imagem\n");
        return;
    }

    // Adicionar a imagem inicial à lista
    addInicioDuplamente_RGB(&current_image, image);

    // Inicializando o GTK
    gtk_init(NULL, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "RGB-Interface");
    gtk_window_set_default_size(GTK_WINDOW(window), image->dim.largura + 200, image->dim.altura + 200);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Criando um GtkBox para organizar os botões e a imagem
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Criando um container para os botões superiores
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 10);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);

    // Criando os botões superiores
    GtkWidget *button1 = gtk_button_new_with_label("Blur");
    GtkWidget *button2 = gtk_button_new_with_label("FlipVertical");
    GtkWidget *button3 = gtk_button_new_with_label("Clahe");
    GtkWidget *button4 = gtk_button_new_with_label("FlipHorizontal");
    GtkWidget *button5 = gtk_button_new_with_label("Transpose");
    GtkWidget *button_save = gtk_button_new_with_label("Salvar");

    // Empacotando os botões superiores no container
    gtk_box_pack_start(GTK_BOX(button_box), button1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button5, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button_save, FALSE, FALSE, 0);

    // Conectando os sinais dos botões
    g_signal_connect(button5, "clicked", G_CALLBACK(transpose_click), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(clahe_click), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(flip_vertical_click), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(flip_horizontal_click), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(blur_click), NULL);
    g_signal_connect(button_save, "clicked", G_CALLBACK(save_current_image), NULL);

    // Criando um container para a imagem
    image_widget = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(main_box), image_widget, TRUE, TRUE, 0);

    // Exibindo a imagem inicial
    show_image_rgb(current_image->image, image_widget);

    // Criando um container para os botões inferiores
    GtkWidget *bottom_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_set_border_width(GTK_CONTAINER(bottom_box), 10);
    gtk_box_pack_end(GTK_BOX(main_box), bottom_box, FALSE, FALSE, 0);

    // Criando os botões inferiores
    button_previous = gtk_button_new_with_label("Anterior");
    button_next = gtk_button_new_with_label("Próximo");

    // Conectando os sinais dos botões
    g_signal_connect(button_previous, "clicked", G_CALLBACK(previous_click), NULL);
    g_signal_connect(button_next, "clicked", G_CALLBACK(next_click), NULL);

    // Empacotando os botões inferiores no container
    gtk_box_pack_end(GTK_BOX(bottom_box), button_previous, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(bottom_box), button_next, FALSE, FALSE, 0);

    // Atualizando a sensibilidade dos botões de navegação
    update_navigation_buttons(current_image);

    // Exibindo a janela
    gtk_widget_show_all(window);

    // Iniciando o loop principal do GTK
    gtk_main();
}
