#include "browser.h"
#include "callback.h"
#include "ui.h"
#include <string.h>

int browser_tab_new()
{
    GtkNotebook *tabs   = NULL;
    int tabindex = -1;
    GtkWidget   *titlehbox = gtk_hbox_new(FALSE, 0);
    GtkWidget   *label  = gtk_label_new("");
    GtkWidget   *eventbox = gtk_event_box_new();
    GtkWidget   *closebutton = gtk_button_new_with_label("X");
    GtkWidget   *frame  = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget   *view   = webkit_web_view_new ();

    gtk_event_box_set_visible_window(GTK_EVENT_BOX(eventbox), false);

    gtk_container_add(GTK_CONTAINER(eventbox), label);
    gtk_box_pack_start(GTK_BOX(titlehbox), eventbox, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(titlehbox), closebutton, TRUE, TRUE, 2);
    gtk_widget_show_all(titlehbox);

    g_signal_connect(closebutton, "clicked", G_CALLBACK(btn_tab_clicked), NULL);
    g_signal_connect(eventbox, "button-press-event", G_CALLBACK(label_tab_clicked), label);


    gtk_container_add(GTK_CONTAINER(frame), view);
    webkit_connects((WebKitWebView *)view);

    g_object_set_data((GObject *)frame, "label", label);
    g_object_set_data((GObject *)frame, "view", view);
    g_object_set_data((GObject *)frame, "button", closebutton);
    g_object_set_data((GObject *)frame, "hbox", titlehbox);

    g_object_set_data((GObject *)view, "label", label);
    g_object_set_data((GObject *)view, "frame", frame);

    g_object_set_data((GObject *)label, "button", closebutton);


    g_object_set_data((GObject *)closebutton, "view", view);



    tabs = ui_lookup("main", "tabs");

    tabindex = gtk_notebook_append_page(tabs, frame, titlehbox);
    gtk_widget_show_all(GTK_WIDGET(tabs));
    gtk_notebook_set_current_page(tabs, tabindex);

    if (tabindex == -1){
        g_object_unref(view);
        g_object_unref(label);
        g_object_unref(frame);
    }

    return tabindex;
}

GtkWindow *browser_window_new(WebKitWebView *webView, WebKitWebFrame *frame, WebKitNetworkRequest *request, WebKitWebNavigationAction *navigation_action, WebKitWebPolicyDecision *policy_decision, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *scrollbox;
    GtkWidget *view;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    scrollbox = gtk_scrolled_window_new(NULL, NULL);
    view = webkit_web_view_new();

    gtk_window_set_title(GTK_WINDOW(window), "Center");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_container_add(GTK_CONTAINER(scrollbox), view);
    gtk_container_add(GTK_CONTAINER(window), scrollbox);

    g_signal_connect(G_OBJECT(window), "destory", G_CALLBACK(g_object_unref), NULL);

    gtk_widget_show_all(window);

    webkit_web_view_load_request (webView, request);

    return (GTK_WINDOW(window));
}

void browser_goback(WebKitWebView *view)
{
    webkit_web_view_go_back(view);
}

void browser_goforward(WebKitWebView *view)
{
    webkit_web_view_go_forward (view);
}

void browser_refresh(WebKitWebView *view)
{
    webkit_web_view_reload(view);
}

void browser_stop(WebKitWebView *view)
{
    webkit_web_view_stop_loading(view);
}

void browser_go(WebKitWebView *view, const char *uri)
{
    char *temp = NULL;
    gint len = strlen(uri);

    if (len > 0 && strstr(uri, ":") == NULL){
        temp = strcpy(g_new(char, len + 8), "http://");
        strcat(temp, uri);
        uri = temp;
    }

    webkit_web_view_load_uri(view, uri);
    if (temp) g_free(temp);
}
