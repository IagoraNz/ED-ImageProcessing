#include "interface.h"

// Declaração global de image_widget
GtkWidget *image_widget = NULL;
GtkWidget *button_previous = NULL;
GtkWidget *button_next = NULL;
ElementoDuploRGB *current_image = NULL;
int quantidade = 0, tilew = 0, tileh = 0, kernel = 0;

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

int get_input_value(const char *message) {
    GtkWidget *dialog, *content_area;
    GtkWidget *entry;
    int result = 0;

    // Criando o diálogo
    dialog = gtk_dialog_new_with_buttons("Input", NULL, GTK_DIALOG_MODAL, "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), message);
    gtk_box_pack_start(GTK_BOX(content_area), entry, TRUE, TRUE, 0);
    gtk_widget_show_all(dialog);

    // Verificando a resposta do usuário
    gint resultdialog = gtk_dialog_run(GTK_DIALOG(dialog));

    if (resultdialog == GTK_RESPONSE_CANCEL) {
        gtk_widget_destroy(dialog);
        return 0;
    }

    // Esperando a resposta do usuário
    if (resultdialog == GTK_RESPONSE_OK) {
        result = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
    }
    gtk_widget_destroy(dialog);
    
    return result;
}

void update_values(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    quantidade = get_input_value("Enter quantidade");
    if (quantidade == 0)
        return;
    tilew = get_input_value("Enter tile width");
    if (tilew == 0)
        return;
    tileh = get_input_value("Enter tile heigth");
    if (tileh == 0)
        return;
    kernel = get_input_value("Enter kernel size");
    if (kernel == 0)
        return;
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
    (void) widget;
    (void) data;
    addInicioDuplamente_RGB(&current_image, transpose_rgb(current_image->image));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void clahe_click(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    tileh = get_input_value("Enter tile heigth");
    if (tileh == 0){
        return;
    }
    tilew = get_input_value("Enter tile width");
    if (tilew == 0){
        return;
    }
    addInicioDuplamente_RGB(&current_image, clahe_rgb(current_image->image, tilew, tileh));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void flip_vertical_click(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    addInicioDuplamente_RGB(&current_image, flip_vertical_rgb(current_image->image));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void flip_horizontal_click(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    addInicioDuplamente_RGB(&current_image, flip_horizontal_rgb(current_image->image));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void blur_click(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    kernel = get_input_value("Enter kernel size");

    if (kernel == 0){
        return;
    }

    if (kernel % 2 == 0) {
        kernel++;
    }
    addInicioDuplamente_RGB(&current_image, median_blur_rgb(current_image->image, kernel));
    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}

void random_click(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    update_values(widget, data);
    if (quantidade == 0 || tilew == 0 || tileh == 0 || kernel == 0)
        return;
    srand(time(NULL));
    int i, res;
    ImageRGB *tempImage = current_image->image;
    for(i = 0; i < quantidade; i++){
        res = rand() % 5 + 1;
        tempImage = randomicoRGB(tempImage, res, tilew, tileh, kernel);
        addInicioDuplamente_RGB(&current_image, tempImage);
    }

    show_image_rgb(current_image->image, image_widget);
    update_navigation_buttons(current_image);
}


// Função para o evento de clique no botão anterior
void previous_click(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
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
    (void) widget;
    (void) data;
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
    (void) widget;
    (void) data;
    if (current_image == NULL || current_image->image == NULL) {
        fprintf(stderr, "Não ha imagem atual para salvar.\n");
        return;
    }

    const char *output_path = "../utils/output_image_rgb.png"; // Pode mudar para outro caminho ou pedir ao usuário para escolher
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, current_image->image->dim.largura, current_image->image->dim.altura);
    set_img_to_pixbuf_rgb(pixbuf, current_image->image);
    gdk_pixbuf_save(pixbuf, output_path, "png", NULL, NULL);
    g_object_unref(pixbuf);

    printf("Imagem salva em %s\n", output_path); // Log
}

// Função callback para o evento de fechamento da janela RGB
gboolean on_window_close_rgb(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    (void) widget;
    (void) event;
    freelistargb(user_data);
    gtk_main_quit();  // Encerra o loop principal do GTK
    return FALSE;  // Retorna FALSE para permitir o fechamento padrão da janela
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
    gtk_window_set_title(GTK_WINDOW(window), "Interface RGB");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "delete-event", G_CALLBACK(on_window_close_rgb), &current_image);

    // Criando um GtkBox para organizar os botões e a imagem
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Criando um container para os botões superiores
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 10);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, TRUE, TRUE, 0);

    // Criando os botões superiores
    GtkWidget *button1 = gtk_button_new_with_label("Blur");
    GtkWidget *button2 = gtk_button_new_with_label("Flip vertical");
    GtkWidget *button3 = gtk_button_new_with_label("Clahe");
    GtkWidget *button4 = gtk_button_new_with_label("Flip horizontal");
    GtkWidget *button5 = gtk_button_new_with_label("Transpose");
    GtkWidget *button6 = gtk_button_new_with_label("Randômico");
    GtkWidget *spacer = gtk_label_new("       ");
    GtkWidget *spacer2 = gtk_label_new("");
    button_previous = gtk_button_new_with_label("<");
    button_next = gtk_button_new_with_label(">");
    GtkWidget *button_save = gtk_button_new_with_label("Salvar");

    // Empacotando os botões superiores no container
    gtk_box_pack_start(GTK_BOX(button_box), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button5, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button6, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button_save, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), spacer, TRUE, TRUE, 0);

    GtkWidget *historic_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // gtk_container_set_border_width(GTK_CONTAINER(historic_box), 10);
    gtk_box_pack_start(GTK_BOX(button_box), historic_box, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(historic_box), button_previous, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(historic_box), spacer2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(historic_box), button_next, TRUE, TRUE, 0);

    // Conectando os sinais dos botões
    g_signal_connect(button_previous, "clicked", G_CALLBACK(previous_click), NULL);
    g_signal_connect(button_next, "clicked", G_CALLBACK(next_click), NULL);

    // Conectando os sinais dos botões
    g_signal_connect(button5, "clicked", G_CALLBACK(transpose_click), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(clahe_click), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(flip_vertical_click), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(flip_horizontal_click), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(blur_click), NULL);
    g_signal_connect(button6, "clicked", G_CALLBACK(random_click), NULL);
    g_signal_connect(button_save, "clicked", G_CALLBACK(save_current_image), NULL);

    // Criando um container para a imagem
    image_widget = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(main_box), image_widget, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);

    // Exibindo a imagem inicial
    show_image_rgb(current_image->image, image_widget);

    // Atualizando a sensibilidade dos botões de navegação
    update_navigation_buttons(current_image);

    // Exibindo a janela
    gtk_widget_show_all(window);

    // Iniciando o loop principal do GTK
    gtk_main();
    freelistargb(&current_image);
    current_image = NULL;
}

// Declaração global de widgets e imagens - Gray
GtkWidget *image_widget_gray = NULL;
GtkWidget *button_previous_gray = NULL;
GtkWidget *button_next_gray = NULL;
ElementoDuploGray *current_image_gray = NULL;

void set_img_to_pixbuf_gray(GdkPixbuf *pixbuf, ImageGray *image) {
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);

    for (int y = 0; y < image->dim.altura; y++) {
        for (int x = 0; x < image->dim.largura; x++) {
            int i = y * rowstride + x * n_channels;
            guchar gray_value = image->pixels[y * image->dim.largura + x].value;
            pixels[i] = gray_value; // R
            pixels[i + 1] = gray_value; // G
            pixels[i + 2] = gray_value; // B
        }
    }
}

void show_image_gray(ImageGray *image, GtkWidget *image_widget) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, image->dim.largura, image->dim.altura);
    set_img_to_pixbuf_gray(pixbuf, image);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
    g_object_unref(pixbuf);
}

void update_navigation_buttons_gray(ElementoDuploGray *lista) {
    gtk_widget_set_sensitive(button_previous_gray, lista->prox != NULL);
    gtk_widget_set_sensitive(button_next_gray, lista->ant != NULL);
}

void transpose_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    addInicioDuplamente_Gray(&current_image_gray, transpose_gray(current_image_gray->image));
    show_image_gray(current_image_gray->image, image_widget_gray);
    update_navigation_buttons_gray(current_image_gray);
}

void clahe_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    tileh = get_input_value("Enter tile heigth");
    if (tileh == 0)
        return;
    tilew = get_input_value("Enter tile width");
    if (tilew == 0)
        return;
    addInicioDuplamente_Gray(&current_image_gray, clahe_gray(current_image_gray->image, tilew, tileh));
    show_image_gray(current_image_gray->image, image_widget_gray);
    update_navigation_buttons_gray(current_image_gray);
}

void flip_vertical_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    addInicioDuplamente_Gray(&current_image_gray, flip_vertical_gray(current_image_gray->image));
    show_image_gray(current_image_gray->image, image_widget_gray);
    update_navigation_buttons_gray(current_image_gray);
}

void flip_horizontal_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    addInicioDuplamente_Gray(&current_image_gray, flip_horizontal_gray(current_image_gray->image));
    show_image_gray(current_image_gray->image, image_widget_gray);
    update_navigation_buttons_gray(current_image_gray);
}

void blur_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    kernel = get_input_value("Enter kernel size");
    if (kernel == 0){
        return;
    }

    if (kernel % 2 == 0) {
        kernel++;
    }
    addInicioDuplamente_Gray(&current_image_gray, median_blur_gray(current_image_gray->image, kernel));
    show_image_gray(current_image_gray->image, image_widget_gray);
    update_navigation_buttons_gray(current_image_gray);
}

void random_click_gray(GtkWidget *widget, gpointer data) {
    update_values(widget, data);
    srand(time(NULL));
    int i, res;
    ImageGray *tempImage = current_image_gray->image;
    for(i = 0; i < quantidade; i++) {
        res = rand() % 5 + 1;
        tempImage = randomicoGray(tempImage, res, tilew, tileh, kernel);
        addInicioDuplamente_Gray(&current_image_gray, tempImage);
    }

    show_image_gray(current_image_gray->image, image_widget_gray);
    update_navigation_buttons_gray(current_image_gray);
}

void previous_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    if (current_image_gray->prox != NULL) {
        current_image_gray = current_image_gray->prox;
        printf("Previous image dimensions: %d x %d\n", current_image_gray->image->dim.altura, current_image_gray->image->dim.largura); // Log
        show_image_gray(current_image_gray->image, image_widget_gray);
        update_navigation_buttons_gray(current_image_gray);
    } else {
        printf("No previous image available.\n"); // Log
    }
}

void next_click_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    if (current_image_gray->ant != NULL) {
        current_image_gray = current_image_gray->ant;
        printf("Next image dimensions: %d x %d\n", current_image_gray->image->dim.altura, current_image_gray->image->dim.largura); // Log
        show_image_gray(current_image_gray->image, image_widget_gray);
        update_navigation_buttons_gray(current_image_gray);
    } else {
        printf("No next image available.\n"); // Log
    }
}

void save_current_image_gray(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    if (current_image_gray == NULL || current_image_gray->image == NULL) {
        fprintf(stderr, "Não ha imagem atual para salvar.\n");
        return;
    }

    const char *output_path = "../utils/output_image_gray.png"; // Pode mudar para outro caminho ou pedir ao usuário para escolher
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, current_image_gray->image->dim.largura, current_image_gray->image->dim.altura);
    set_img_to_pixbuf_gray(pixbuf, current_image_gray->image);
    gdk_pixbuf_save(pixbuf, output_path, "png", NULL, NULL);
    g_object_unref(pixbuf);

    printf("Imagem salva em %s\n", output_path); // Log
}

gboolean on_window_close_gray(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    (void) widget;
    (void) event;
    freelistagray(user_data);
    gtk_main_quit();  // Encerra o loop principal do GTK
    return FALSE;  // Retorna FALSE para permitir o fechamento padrão da janela
}

void show_interface_gray() {
    FILE *arq = fopen("input_gray.txt", "rb");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return;
    }
    int altura, largura;
    fscanf(arq, "%d", &altura);
    fscanf(arq, "%d", &largura);
    ImageGray *image = create_image_gray(largura, altura, arq);
    fclose(arq);

    // Verificar se a imagem foi criada corretamente
    if (image == NULL) {
        fprintf(stderr, "Erro ao criar a imagem\n");
        return;
    }

    // Adicionar a imagem inicial à lista
    addInicioDuplamente_Gray(&current_image_gray, image);

    // Inicializando o GTK
    gtk_init(NULL, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Interface Gray");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "delete-event", G_CALLBACK(on_window_close_gray), &current_image_gray);

    // Criando um GtkBox para organizar os botões e a imagem
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Criando um container para os botões superiores
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 10);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, TRUE, TRUE, 0);

    // Criando os botões superiores
    GtkWidget *button1 = gtk_button_new_with_label("Blur");
    GtkWidget *button2 = gtk_button_new_with_label("Flip vertical");
    GtkWidget *button3 = gtk_button_new_with_label("Clahe");
    GtkWidget *button4 = gtk_button_new_with_label("Flip horizontal");
    GtkWidget *button5 = gtk_button_new_with_label("Transpose");
    GtkWidget *button6 = gtk_button_new_with_label("Randômico");
    GtkWidget *spacer = gtk_label_new("       ");
    GtkWidget *spacer2 = gtk_label_new("");
    button_previous_gray = gtk_button_new_with_label("<");
    button_next_gray = gtk_button_new_with_label(">");
    GtkWidget *button_save = gtk_button_new_with_label("Salvar");

    // Empacotando os botões superiores no container
    gtk_box_pack_start(GTK_BOX(button_box), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button5, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button6, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), button_save, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), spacer, TRUE, TRUE, 0);

    GtkWidget *historic_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // gtk_container_set_border_width(GTK_CONTAINER(historic_box), 10);
    gtk_box_pack_start(GTK_BOX(button_box), historic_box, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(historic_box), button_previous_gray, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(historic_box), spacer2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(historic_box), button_next_gray, TRUE, TRUE, 0);

    // Conectando os sinais dos botões
    g_signal_connect(button_previous_gray, "clicked", G_CALLBACK(previous_click_gray), NULL);
    g_signal_connect(button_next_gray, "clicked", G_CALLBACK(next_click_gray), NULL);

    // Conectando os sinais dos botões
    g_signal_connect(button5, "clicked", G_CALLBACK(transpose_click_gray), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(clahe_click_gray), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(flip_vertical_click_gray), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(flip_horizontal_click_gray), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(blur_click_gray), NULL);
    g_signal_connect(button6, "clicked", G_CALLBACK(random_click_gray), NULL);
    g_signal_connect(button_save, "clicked", G_CALLBACK(save_current_image_gray), NULL);

    // Criando um container para a imagem
    image_widget_gray = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(main_box), image_widget_gray, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);

    // Exibindo a imagem inicial
    show_image_gray(current_image_gray->image, image_widget_gray);

    // Atualizando a sensibilidade dos botões de navegação
    update_navigation_buttons_gray(current_image_gray);

    // Exibindo a janela
    gtk_widget_show_all(window);

    // Iniciando o loop principal do GTK
    gtk_main();
    
    current_image_gray = NULL;
}

int width, height;

// Função para redimensionar o GdkPixbuf para um tamanho desejado
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int new_width, int new_height) {
    return gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
}

// Função para converter a imagem redimensionada em texto RGB
void txt_from_resized_image_rgb(const char *image_path, const char *output_path, int target_width, int target_height) {
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(image_path, &error);

    if (!pixbuf) {
        fprintf(stderr, "Error loading image: %s\n", error->message);
        g_error_free(error);
        return;
    }

    // Redimensionar o pixbuf para o tamanho desejado
    GdkPixbuf *resized_pixbuf = resize_pixbuf(pixbuf, target_width, target_height);
    g_object_unref(pixbuf); // Liberar o pixbuf original após redimensionamento

    int width = gdk_pixbuf_get_width(resized_pixbuf);
    int height = gdk_pixbuf_get_height(resized_pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(resized_pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(resized_pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(resized_pixbuf);

    FILE *file = fopen(output_path, "w");
    if (!file) {
        fprintf(stderr, "Error opening output file\n");
        g_object_unref(resized_pixbuf);
        return;
    }

    fprintf(file, "%d\n%d\n", width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            guchar *p = pixels + y * rowstride + x * n_channels;
            fprintf(file, "%d %d %d,", p[0], p[1], p[2]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    g_object_unref(resized_pixbuf);

    printf("Imagem redimensionada e convertida para texto RGB em %s\n", output_path);
}

// Função para abrir o diálogo de seleção de arquivo e redimensionar a imagem
void on_open_image(GtkWidget *widget, gpointer data) {
    (void) widget;
    (void) data;
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open Image",
                                         NULL,
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
    chooser = GTK_FILE_CHOOSER(dialog);

    gtk_file_chooser_set_current_folder(chooser, ".");
    gtk_file_chooser_set_local_only(chooser, TRUE);
    gtk_file_chooser_set_select_multiple(chooser, FALSE);

    // Adicionando filtros para os tipos de arquivo de imagem
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Image files");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_filter_add_mime_type(filter, "image/jpeg");
    gtk_file_filter_add_mime_type(filter, "image/bmp");
    gtk_file_filter_add_pattern(filter, "*.png");
    gtk_file_filter_add_pattern(filter, "*.jpg");
    gtk_file_filter_add_pattern(filter, "*.jpeg");
    gtk_file_filter_add_pattern(filter, "*.bmp");
    gtk_file_chooser_add_filter(chooser, filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *file_path = gtk_file_chooser_get_filename(chooser);
        // Definir o tamanho desejado em pixels
        int target_width = 512; 
        int target_height = 512; 
        
        txt_from_resized_image_rgb(file_path, "input_rgb.txt", target_width, target_height);
        g_free(file_path);
    }

    gtk_widget_destroy(dialog);
}




// Função chamada ao clicar no botão "Carregar Imagem"
void load_click(GtkWidget *widget, gpointer data) {
    on_open_image(widget, data);
    FILE *arq = fopen("input_rgb.txt", "rb");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return;
    }
    int altura, largura;
    fscanf(arq, "%d", &altura);
    fscanf(arq, "%d", &largura);
    ImageRGB *image = create_image_rgb(largura, altura, arq);
    ImageGray *aux2 = conversion_image_gray(image);
    FILE *arq2 = fopen("input_gray.txt", "wb");
    fprintf(arq2, "%d\n%d\n", aux2->dim.largura, aux2->dim.altura);
    for (int i = 0; i < aux2->dim.altura; i++) {
        for (int j = 0; j < aux2->dim.largura; j++) {
            fprintf(arq2, "%d,", aux2->pixels[i * aux2->dim.largura + j].value);
        }
        fprintf(arq2, "\n");
    }
}

// Função para exibir a interface principal
void show_main_interface() {
    gtk_init(NULL, NULL);
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Interface principal");

    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *button_rgb = gtk_button_new_with_label("Interface RGB");
    GtkWidget *button_gray = gtk_button_new_with_label("Interface Gray");
    GtkWidget *button_load = gtk_button_new_with_label("Carregar imagem");
    GtkWidget *button_exit = gtk_button_new_with_label("Sair");

    gtk_box_pack_start(GTK_BOX(main_box), button_rgb, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), button_gray, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), button_load, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), button_exit, TRUE, TRUE, 0);

    g_signal_connect(button_rgb, "clicked", G_CALLBACK(show_interface_rgb), NULL);
    g_signal_connect(button_gray, "clicked", G_CALLBACK(show_interface_gray), NULL);
    g_signal_connect(button_load, "clicked", G_CALLBACK(load_click), NULL);
    g_signal_connect(button_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}