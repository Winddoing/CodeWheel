/*#############################################################
 *     File Name	: img.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月04日 星期五 18时43分16秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>


#include <gtk/gtk.h>
#include <stdlib.h>

#define ROWS 500
#define COLS 800
#define BYTES_PER_PIXEL 3

typedef struct {
    GtkImage *image;
    int rows, cols, stride;
} ImageData;

void free_pixels(guchar *pixels, gpointer data) {
    free(pixels);
}

void setrgb(guchar *a, int row, int col, int stride,
        guchar r, guchar g, guchar b) {
    int p = row * stride + col * BYTES_PER_PIXEL;
    a[p] = r; a[p+1] = g; a[p+2] = b;
}

int update_pic(gpointer data) {
    static int N = 0;
    if (N > 100) return FALSE; // stop timer

    ImageData *id = (ImageData*)data;
    GdkPixbuf *pb = gtk_image_get_pixbuf(id->image);
    gdk_pixbuf_fill(pb, 0); // clear to black

    guchar *g = gdk_pixbuf_get_pixels(pb);
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (r / 20 % 2 && (c + N)/ 20 % 2)
                setrgb(g, r, c, id->stride, 255, 255, 255);
    N++;

    gtk_image_set_from_pixbuf(GTK_IMAGE(id->image), pb);

    return TRUE; // continue timer
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    ImageData id;
    id.rows = ROWS;
    id.cols = COLS;
    id.stride = COLS * BYTES_PER_PIXEL;
    id.stride += (4 - id.stride % 4) % 4; // ensure multiple of 4

    guchar *pixels = calloc(ROWS * id.stride, 1);

    GdkPixbuf *pb = gdk_pixbuf_new_from_data(
            pixels,
            GDK_COLORSPACE_RGB,     // colorspace
            0,                      // has_alpha
            8,                      // bits-per-sample
            COLS, ROWS,             // cols, rows
            id.stride,              // rowstride
            free_pixels,            // destroy_fn
            NULL                    // destroy_fn_data
            );

    id.image = GTK_IMAGE(gtk_image_new_from_pixbuf(pb));

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image");
    gtk_window_set_default_size(GTK_WINDOW(window), COLS, ROWS);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(id.image));

    g_timeout_add(250,         // milliseconds
            update_pic,  // handler function
            &id);        // data

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
