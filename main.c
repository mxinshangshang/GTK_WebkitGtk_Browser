#include <stdlib.h>
#include <gtk/gtk.h>

#include "ui.h"
#include "callback.h"
#include "browser.h"

int main (int argc, char *argv[])
{
  GtkWidget *win = NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = (GtkWidget *)ui_lookup("main", "awaybrowser");
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  browser_tab_new();

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();
  return 0;
}
