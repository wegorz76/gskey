#include <gtk/gtk.h>
#include <gskey.h>

int main (int argc, char *argv[])
{
  GSkeyWidgets *GSkey;
  
  gtk_set_locale();
  gtk_init(&argc, &argv);
  GSkey = GSkey_create();
  gtk_widget_show_all (GSkey->window);

  gtk_main ();
  return 0;
}

