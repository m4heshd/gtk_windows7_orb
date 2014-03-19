#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>



static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer user_data);
static gboolean draw(GtkWidget *widget, cairo_t *new_cr, gpointer user_data);
static void clicked(GtkWindow *win, GdkEventButton *event, gpointer user_data);



gboolean on_button_press (GtkWidget* widget,
  GdkEventButton * event, GdkWindowEdge edge)
{
  if (event->type == GDK_BUTTON_PRESS)
  {
    if (event->button == 1) {
      gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
          event->button,
	  event->x_root,
	  event->y_root,
	  event->time);
    }
  }

  return FALSE;
}




int main(int argc, char **argv)
{
   char imgpath[50];

    ///////////////////////////////////Get app Path///////////////////////////////////

      char path_save[PATH_MAX];
  char abs_exe_path[PATH_MAX];
  char *p;

  if(!(p = strrchr(argv[0], '/')))
    getcwd(abs_exe_path, sizeof(abs_exe_path));
  else
  {
    *p = '\0';
    getcwd(path_save, sizeof(path_save));
    chdir(argv[0]);
    getcwd(abs_exe_path, sizeof(abs_exe_path));
    chdir(path_save);
  }

  ////////////////////////////////////////////////////////////////////////////////

    printf("\n\n-*-*-*-*-* A piece of Gtk+ code by MAhezh -*-*-*-*-*-*\n\n");

    gtk_init(&argc, &argv);


    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 10, 10);
    gtk_window_set_title(GTK_WINDOW(window), "Win orb");
    g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);

    gtk_widget_set_app_paintable(window, TRUE);

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw), NULL);
    g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(screen_changed), NULL);

    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
      g_signal_connect(G_OBJECT(window), "button-press-event",
      G_CALLBACK(on_button_press), NULL);
    //g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(clicked), NULL);

    GtkWidget* fixed_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed_container);

    sprintf(imgpath,"%s/img/metronew.png",abs_exe_path);

    GtkWidget *image = gtk_image_new_from_file(imgpath);
    gtk_container_add(GTK_CONTAINER(fixed_container), image);

    screen_changed(window, NULL, NULL);

////////////////////////////////////// Window 2 //////////////////////////////////////////////


        GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window2), 10, 10);
    gtk_window_set_title(GTK_WINDOW(window2), "Win orb");
    gtk_window_set_opacity(GTK_WINDOW(window2),0.0);
    g_signal_connect(G_OBJECT(window2), "delete-event", gtk_main_quit, NULL);

    gtk_widget_set_app_paintable(window2, TRUE);

    g_signal_connect(G_OBJECT(window2), "draw", G_CALLBACK(draw), NULL);
    g_signal_connect(G_OBJECT(window2), "screen-changed", G_CALLBACK(screen_changed), NULL);

    gtk_window_set_decorated(GTK_WINDOW(window2), FALSE);
    gtk_widget_add_events(window2, GDK_BUTTON_PRESS_MASK);
//      g_signal_connect(G_OBJECT(window2), "button-press-event",
//      G_CALLBACK(on_button_press), NULL);
        g_signal_connect_swapped(G_OBJECT(window2), "button-press-event",
      G_CALLBACK(on_button_press), window2);


    //g_signal_connect(G_OBJECT(window2), "button-press-event", G_CALLBACK(clicked), NULL);

    GtkWidget* fixed_container2 = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window2), fixed_container2);

    sprintf(imgpath,"%s/img/metroglow.png",abs_exe_path);
    GtkWidget *image2 = gtk_image_new_from_file(imgpath);
    gtk_container_add(GTK_CONTAINER(fixed_container2), image2);

    screen_changed(window2, NULL, NULL);



////////////////////////////////////////////////////////////////////////////////////////////////


    gtk_widget_show_all(window);
    gtk_widget_show_all(window2);
    //window_2();

/////////////////////////////////////OPACITY///////////////////////////////////////////////////////


int timer;
float vl = 0.0;
int tnt;



gboolean mouse_enter()
{
    if(tnt==1)
    {
        timer = g_timeout_add ( 3, mouse_enter, NULL );
      vl = vl + 0.008;
      gtk_window_set_opacity(GTK_WINDOW(window2), vl);



        if (vl >= 1.0)
        {
        g_source_remove (timer);
        return FALSE;
        printf("Still running\n");
        //vl = 0;
        }
    }
}

gboolean mouse_leave() //function for timeouts must be a boolean
{
    if(tnt==0)
    {

      timer = g_timeout_add ( 3, mouse_leave, NULL );
      vl = vl - 0.008;
      gtk_window_set_opacity(GTK_WINDOW(window2), vl);


        if (vl <= 0.0)
        {
        g_source_remove (timer);
        return FALSE;
        //vl = 1.0;
        }

    }
}

int me()
{
    //g_source_remove (timer);
    tnt=1;
    printf("Entered %d\n", tnt);

    mouse_enter();

}

int ml()
{
    //g_source_remove (timer);
    tnt=0;
    printf("Left %d\n", tnt);

    mouse_leave();

}

  g_signal_connect(window2, "enter-notify-event",
      G_CALLBACK(me), NULL);
  g_signal_connect(window2, "leave-notify-event",
      G_CALLBACK(ml), NULL);

////////////////////////////////////////////////////////////////////////////////////////////////

    gtk_main();


    return 0;
}

gboolean supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
    /* To check if the display supports alpha channels, get the visual */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if (!visual)
    {
        printf("Your screen does not support alpha channels!\n");
        visual = gdk_screen_get_system_visual(screen);
        supports_alpha = FALSE;
    }
    else
    {
        //printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    gtk_widget_set_visual(widget, visual);
}

static gboolean draw(GtkWidget *widget, cairo_t *cr, gpointer userdata)
{
   cairo_t *new_cr = gdk_cairo_create(gtk_widget_get_window(widget));

    if (supports_alpha)
    {
        cairo_set_source_rgba (new_cr, 1.0, 1.0, 1.0, 0); /* transparent */
    }
    else
    {
        cairo_set_source_rgb (new_cr, 1.0, 1.0, 1.0); /* opaque white */
    }

    /* draw the background */
    cairo_set_operator (new_cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (new_cr);

    cairo_destroy(new_cr);

    return FALSE;
}

static void clicked(GtkWindow *win, GdkEventButton *event, gpointer user_data)
{
    /* toggle window manager frames */
    gtk_window_set_decorated(win, !gtk_window_get_decorated(win));
}


