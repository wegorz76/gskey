#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>

#include <gskey.h>
#include <libskey/skey.h>

static void fn_generate(GtkButton *button, gpointer user_data)
{
  GSkeyWidgets *ptr;
  gchar *seed, *pass;
  gint number, n;
  char filename[256], key[8], buf[33];
  FILE *config;

  ptr = (GSkeyWidgets *) user_data;
  seed = (gchar *) gtk_entry_get_text(GTK_ENTRY(ptr->entrySeed));
  pass = (gchar *) gtk_entry_get_text(GTK_ENTRY(ptr->entryPassword));
  number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ptr->spinNumber));

  /* Utwórz ci±g md5 */
  n = number;
  if(keycrunch(key,seed,pass) != 0) fprintf(stderr, "key crunch failed\n");
  while(n-- != 0) f(key);
  gtk_entry_set_text(GTK_ENTRY(ptr->entryMD5String), (gchar *) btoe(buf,key));
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(ptr->spinNumber), --number);

  sprintf(filename, "%s/%s", g_getenv("HOME"), ".gskey");
  umask(0077);
  if ((config = fopen(filename, "wb")) == NULL)
      fprintf(stderr, "Can't write to configuration file %s\n", filename);
  else
    {
      fprintf(config, "%s\n", seed);
      fprintf(config, "%s\n", pass);
      fprintf(config, "%d\n", number);
    }
  fclose(config);
}


static void fn_end(GtkButton *button, gpointer user_data)
{
  gtk_main_quit();
}


static void fn_map(GtkWidget *widget, gpointer user_data)
{
  GSkeyWidgets *ptr;
  gchar seed[33], pass[33], number[9], filename[256];
  FILE *config;
  int ret;

  ptr = (GSkeyWidgets *) user_data;
  printf("GSkey v. 2.0 GNU GPL. Zbigniew Kempczynski - wegorz76@gmail.com\n");
  sprintf(filename, "%s/%s", getenv("HOME"), ".gskey");
  if ((config = fopen(filename, "rb")) == NULL)
    {
      fprintf(stderr, "No configuration file %s\n", filename);
      fprintf(stderr, "Using empty values.\n");
      strcpy(seed, "EMPTY");
      strcpy(pass, "EMPTY");
      strcpy(number, "256");
    }
  else
    {
      ret = fscanf(config, "%s\n", seed);
      if (ret != 1) {
	      fprintf(stderr, "No seed in config file\n");
	      exit(0);
      }
			
      ret = fscanf(config, "%s\n", pass);
      if (ret != 1) {
	      fprintf(stderr, "No pass in config file\n");
	      exit(0);
      }

      ret = fscanf(config, "%s\n", number);
      if (ret != 1) {
	      fprintf(stderr, "No number in config file\n");
	      exit(0);
      }


    }
  gtk_entry_set_text(GTK_ENTRY(ptr->entrySeed), seed);
  gtk_entry_set_text(GTK_ENTRY(ptr->entryPassword), pass);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(ptr->spinNumber), atoi(number));
}

static gboolean fn_destroy(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  gtk_main_quit();
  return TRUE;
}

/* ============================================================================ */
GSkeyWidgets *GSkey_create (void)
{
  GSkeyWidgets *ptr;
  GtkWidget *vbox1, *table1, *table2, *label1, *label2, *label3, *label4;
  GtkObject *spin_number_adj;
  GtkWidget *hsep1, *hsep2, *hbuttonbox1, *button_generate, *button_quit;  
  
  ptr = g_new(GSkeyWidgets, 1);
  ptr->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (ptr->window), "ptr->window", ptr->window);
  gtk_window_set_title (GTK_WINDOW (ptr->window), "GSkey");
  gtk_window_set_modal (GTK_WINDOW (ptr->window), TRUE);
  gtk_window_set_policy (GTK_WINDOW (ptr->window), FALSE, FALSE, FALSE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (ptr->window), vbox1);

  table1 = gtk_table_new (3, 2, FALSE);
  gtk_box_pack_start (GTK_BOX (vbox1), table1, FALSE, TRUE, 0);

  label1 = gtk_label_new ("Seed: ");
  gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_RIGHT);

  label2 = gtk_label_new ("Password: ");
  gtk_table_attach (GTK_TABLE (table1), label2, 0, 1, 1, 2, (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_RIGHT);

  label3 = gtk_label_new ("Generation number: ");
  gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_padding (GTK_MISC (label3), 3, 0);

  ptr->entrySeed = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (table1), ptr->entrySeed, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 2, 1);

  ptr->entryPassword = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (table1), ptr->entryPassword, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 2, 1);
  gtk_entry_set_visibility (GTK_ENTRY (ptr->entryPassword), FALSE);

  spin_number_adj = gtk_adjustment_new (1, 0, 10000, 1, 10, 10);
  ptr->spinNumber = gtk_spin_button_new (GTK_ADJUSTMENT (spin_number_adj), 1, 0);
  gtk_table_attach (GTK_TABLE (table1), ptr->spinNumber, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 2, 1);

  hsep1 = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox1), hsep1, FALSE, TRUE, 1);

  table2 = gtk_table_new (1, 2, FALSE);
  gtk_box_pack_start (GTK_BOX (vbox1), table2, FALSE, TRUE, 2);

  label4 = gtk_label_new ("MD5 sum: ");
  gtk_table_attach (GTK_TABLE (table2), label4, 0, 1, 0, 1, 
		    (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);

  ptr->entryMD5String = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (table2), ptr->entryMD5String, 1, 2, 0, 1,
		    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 2, 0);
  gtk_entry_set_editable(GTK_ENTRY(ptr->entryMD5String), FALSE);

  hsep2 = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox1), hsep2, FALSE, FALSE, 1);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, FALSE, 2);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_SPREAD);

  button_generate = gtk_button_new_with_label ("Generate");
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button_generate);
  GTK_WIDGET_SET_FLAGS (button_generate, GTK_CAN_DEFAULT);

  button_quit = gtk_button_new_with_label ("Quit");
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button_quit);
  GTK_WIDGET_SET_FLAGS (button_quit, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (ptr->window), "map", GTK_SIGNAL_FUNC (fn_map), ptr);
  gtk_signal_connect (GTK_OBJECT (ptr->window), "destroy", GTK_SIGNAL_FUNC (fn_destroy), ptr);
  gtk_signal_connect (GTK_OBJECT (button_generate), "clicked", GTK_SIGNAL_FUNC (fn_generate), ptr);
  gtk_signal_connect (GTK_OBJECT (button_quit), "clicked", GTK_SIGNAL_FUNC (fn_end), ptr);

  return ptr;
}

