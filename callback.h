#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <gtk/gtk.h>
#include <webkit/webkit.h>

void btn_back_clicked(GtkButton *button, gpointer user_data);
void btn_forward_clicked(GtkButton *button, gpointer user_data);
void btn_go_clicked(GtkButton *button, gpointer user_data);
void btn_stop_clicked(GtkButton *button, gpointer user_data);
void edt_address_activate(GtkEntry *entry, gpointer user_data);
void btn_tab_clicked(GtkButton *button, gpointer user_data);
gboolean notebook_clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void notebook_change_page (GtkNotebook *notebook, gpointer arg1, guint arg2, gpointer user_data);
gboolean label_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data);



void webkit_connects(WebKitWebView *view);
void  webkit_title_changed(WebKitWebView  *webView, WebKitWebFrame *frame, gchar *title, gpointer user_data);
gboolean webkit_ready(WebKitWebView *webView, WebKitWebFrame *arg1, gpointer user_data);
void webkit_progress_changed (WebKitWebView *webkitwebview, gint progress, gpointer user_data);
void webkit_load_started(WebKitWebView  *webView, WebKitWebFrame *frame, gpointer user_data);
void webkit_load_finished(WebKitWebView  *webView, WebKitWebFrame *frame, gpointer user_data);
gboolean webkit_close(WebKitWebView *webView, gpointer user_data);
WebKitWebView* webkit_new_window (WebKitWebView  *webView, WebKitWebFrame *frame, gpointer user_data);
gboolean webkit_new_window2 (WebKitWebView *webView, WebKitWebFrame *frame, WebKitNetworkRequest *request, WebKitWebNavigationAction *navigation_action, WebKitWebPolicyDecision *policy_decision, gpointer user_data);

#endif
