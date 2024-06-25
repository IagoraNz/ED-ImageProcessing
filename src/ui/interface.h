#pragma once
#include "../img/image.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>


// Interface RGB
void show_interface_rgb(); // Função principal da interface RGB

void set_img_to_pixbuf_rgb(GdkPixbuf *pixbuf, ImageRGB *image); // Função para copiar os pixels RGB da imagem para o GdkPixbuf

void show_image_rgb(ImageRGB *image, GtkWidget *image_widget); // Função para exibir uma imagem RGB em uma janela GTK

void previous_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão anterior

void next_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão próximo

void update_navigation_buttons(ElementoDuploRGB *lista); // Função para atualizar a sensibilidade dos botões de navegação

void clahe_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de CLAHE

void flip_vertical_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de flip vertical

void flip_horizontal_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de flip horizontal

void transpose_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de transposição

void blur_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de blur

void random_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de efeito randomico

// Interface Gray
void show_interface_gray(); // Função principal da interface Gray

void set_img_to_pixbuf_gray(GdkPixbuf *pixbuf, ImageGray *image); // Função para copiar os pixels Gray da imagem para o GdkPixbuf

void show_image_gray(ImageGray *image, GtkWidget *image_widget); // Função para exibir uma imagem Gray em uma janela GTK

void previous_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão anterior

void next_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão próximo

void update_navigation_buttons_gray(ElementoDuploGray *lista); // Função para atualizar a sensibilidade dos botões de navegação

void clahe_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de CLAHE

void flip_vertical_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de flip vertical

void flip_horizontal_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de flip horizontal

void transpose_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de transposição

void blur_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de blur

void random_click_gray(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de efeito randomico

// Funções para carregar imagem
void txt_from_image_rgb(const char *image_path, const char *output_path); // Função para converter uma imagem RGB para um arquivo txt

void on_open_image(GtkWidget *widget, gpointer data); // Função para o evento de abrir imagem

// Interface Principal:

void load_click(GtkWidget *widget, gpointer data); // Função para o evento de clique no botão de carregar imagem

void show_main_interface(); // Função principal da interface principal