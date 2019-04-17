#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <gtk/gtk.h>
#include <glib.h>
#include <pthread.h>
#include "file_ops.h"
#include "process.h"
#include "sharememory.h"


typedef struct window
{
    GtkWidget *main_wind;
  
    GtkWidget *table;
    GtkWidget *image;
    GtkWidget *label_song_name_list[7];
    GtkWidget *scrolled_window;
    GtkWidget *label_lrc[7];

    GtkWidget *label_time;
    GtkWidget *label_percent;
    GtkWidget *label_current_song_name;

    GtkWidget *button_start;
    GtkWidget *button_prev;
    GtkWidget *button_next;
    GtkWidget *change_skin;
    GtkWidget *button_volup;
    GtkWidget *button_create_chlid_wind;
    GtkWidget *progress_vol;
    GtkWidget *eventbox_vol;
    GtkWidget *button_exit;
    GtkWidget *button_song_ctrl;
    GtkWidget *progress;
    GtkWidget *color_selection;
    
    GtkWidget *clist;
    GtkWidget *eventbox;
    GdkColor color;
    
    
   
}WINDOW;

WINDOW wind;

typedef struct child_window
{
    GtkWidget *wind;
    GtkWidget *table;
    GtkWidget *button_close_child_wind;
}CHILD_WINDOW;

CHILD_WINDOW child_wind;

int mouse_x;
double coor_percent;
double progress_pos;
int volume;
double coor_percent_vol;
double progress_pos_vol;

WINDOW *Create_Main_Wind(WINDOW * wind,int *argc, char ***argv,char *title,char *background_file_path);
void chang_background(GtkWidget *widget, int w, int h, const gchar *path);
GdkPixbufAnimation *create_pixbuf_animation_from_date(const char *path, int w, int h);
void set_label_font_size(GtkWidget *label, int size);
void sungtk_clist_set_label_color(GtkWidget *label, const char *color_buf);
GtkWidget *create_button_from_file(const char *file_path, const int w, const int h);
void load_image(GtkWidget *image,const char *file_path,const int w,const int h);
void set_button_image(char* picture_path,GtkWidget *button);
void deal_button(GtkButton *button,gpointer   user_data);
void selection_made( GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data );
void song_list_event(GtkWidget *widget, GdkEventMotion *event, gpointer gui_data);
void deal_event_motion(GtkWidget *eventbox, GdkEventMotion *event, gpointer guidata);
gboolean deal_event_release(GtkWidget *eventbox, GdkEventButton *event, gpointer guidata);
gboolean set_label_song_time(gpointer data);
gboolean set_label_time_length(gpointer data);
gboolean set_label_file_name(gpointer data);
gboolean set_label_lrc(gpointer data);
void quit_app(GtkWidget * window,gpointer user_data);



#endif