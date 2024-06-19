#pragma once
#include "../img/image.h"
#include <gtk/gtk.h>

void set_img_to_pixbuf_rgb(GdkPixbuf *pixbuf, ImageRGB *image);

void show_image_rgb(ImageRGB *image, GtkWidget *image_widget);

void transpose_click(GtkWidget *widget, gpointer data);

void previous_click(GtkWidget *widget, gpointer data);
void next_click(GtkWidget *widget, gpointer data);

void show_interface_rgb();