#include "callback.h"
#include "browser.h"
#include "ui.h"
#include <string.h>
#include <gdk/gdk.h>

static WebKitWebView *callback_get_current_view()
{
    GtkNotebook * page = ui_lookup("main", "tabs");
    gint pageindex = gtk_notebook_get_current_page(page);
    GtkScrolledWindow *frame = (GtkScrolledWindow *)gtk_notebook_get_nth_page(page, pageindex);
    WebKitWebView * result = (WebKitWebView *)g_object_get_data((GObject *)frame, "view");
    return result;
}

void btn_back_clicked(GtkButton *button, gpointer user_data)
{
    WebKitWebView *view = callback_get_current_view();

    if (webkit_web_view_can_go_back(view)){
        browser_goback(view);
    }
}

void btn_forward_clicked(GtkButton *button, gpointer user_data)
{
    WebKitWebView *view = callback_get_current_view();

    if (webkit_web_view_can_go_forward (view)){
        browser_goforward (view);
    }
}

void btn_go_clicked(GtkButton *button, gpointer user_data)
{
    WebKitWebView *view = callback_get_current_view();
    GtkWidget *address = ui_lookup("main", "edt_address");
    char *uri = (char *)gtk_entry_get_text(GTK_ENTRY(address));
    browser_go(view, uri);
}

void btn_stop_clicked(GtkButton *button, gpointer user_data)
{
    WebKitWebView *view = callback_get_current_view();
    WebKitLoadStatus status = webkit_web_view_get_load_status(view);

    if (status == WEBKIT_LOAD_FINISHED || status == WEBKIT_LOAD_FAILED){
        browser_refresh(view);
    }else{
        browser_stop(view);
    }
}

void edt_address_activate(GtkEntry *entry, gpointer user_data)
{
    btn_go_clicked(NULL, NULL);
}

void btn_tab_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *frame = g_object_get_data((GObject *)button, "view");

    webkit_web_view_execute_script((WebKitWebView *)frame, "window.close()");
}

gboolean notebook_clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (event->type == GDK_2BUTTON_PRESS && event->button == 0x1) {
        browser_tab_new();
    }

    return FALSE;
}

gboolean label_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (event->type == GDK_2BUTTON_PRESS && event->button == 0x1) {
        GtkWidget *button = g_object_get_data((GObject *)user_data, "button");
        btn_tab_clicked(GTK_BUTTON(button), NULL);
        return TRUE;
    }
    return FALSE;
}

void notebook_change_page (GtkNotebook *notebook, gpointer arg1, guint arg2, gpointer user_data)
{
    GtkWidget *frame = gtk_notebook_get_nth_page(notebook, arg2);
    WebKitWebView *view = g_object_get_data((GObject *)frame, "view");
    webkit_title_changed(view, NULL, (gchar *)webkit_web_view_get_title(view), view);
    webkit_ready(view, NULL, view);
}




void webkit_connects(WebKitWebView *view)
{
    g_signal_connect(view, "title-changed", G_CALLBACK(webkit_title_changed), NULL);
    g_signal_connect(view, "document-load-finished", G_CALLBACK(webkit_ready), NULL);
    g_signal_connect(view, "load-progress-changed", G_CALLBACK(webkit_progress_changed), NULL);
    g_signal_connect(view, "load-started", G_CALLBACK(webkit_load_started), NULL);
    g_signal_connect(view, "load-finished", G_CALLBACK(webkit_load_finished), NULL);
    g_signal_connect(view, "close-web-view", G_CALLBACK(webkit_close), NULL);
    g_signal_connect(view, "create-web-view", G_CALLBACK(webkit_new_window), NULL);
    //g_signal_connect(view, "new-window-policy-decision-requested", G_CALLBACK(webkit_new_window2), NULL);

}


void  webkit_title_changed(WebKitWebView  *webView, WebKitWebFrame *frame, gchar *title, gpointer user_data)
{
    if (title == NULL) title = "空白页";
    WebKitWebView *currentView = callback_get_current_view();

    GtkWindow *window = ui_lookup("main", "awaybrowser");

    gtk_label_set_text(GTK_LABEL(g_object_get_data((GObject *)webView, "label")), title);
    if (currentView == webView || user_data == webView){
        char *atitle = g_new(char, strlen(title) + strlen(" - AwayBrowser") + 1);
        strcpy(atitle, title);
        strcat(atitle, " - AwayBrowser");
        gtk_window_set_title(window, atitle);
        g_free(atitle);
    }
}

gboolean webkit_ready(WebKitWebView *webView, WebKitWebFrame *arg1, gpointer user_data)
{
    WebKitWebView *currentView = callback_get_current_view();
    gchar *uri = (gchar *)webkit_web_view_get_uri(webView);
    if (uri == NULL) uri = "about:blank";

    GtkEntry *entry = ui_lookup("main", "edt_address");

    if (currentView == webView || user_data == webView){
        gtk_entry_set_text(entry, uri);
    }
    return FALSE;
}

void webkit_progress_changed (WebKitWebView *webkitwebview, gint progress, gpointer user_data)
{
    WebKitWebView *currentView = callback_get_current_view();

    if (currentView == webkitwebview){
        GtkWidget *statusbar = ui_lookup("main", "statusbar");
        gchar *text = g_new(char, 100);

        if (progress != 100) sprintf(text, "Loading: %d%%", progress);
        else text[0] = '\0';

        gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, text);

        g_free(text);
    }
}

void webkit_load_started(WebKitWebView  *webView, WebKitWebFrame *frame, gpointer user_data)
{
    WebKitWebView *currentView = callback_get_current_view();

    if (currentView == webView){
        GtkWidget *button = ui_lookup("main", "btn_stop");
        gtk_button_set_label(GTK_BUTTON(button), "停止(_S)");
    }
}

void webkit_load_finished(WebKitWebView  *webView, WebKitWebFrame *frame, gpointer user_data)
{
    WebKitWebView *currentView = callback_get_current_view();

    if (currentView == webView){
        GtkWidget *button = ui_lookup("main", "btn_stop");
        gtk_button_set_label(GTK_BUTTON(button), "刷新(_R)");
    }
}

gboolean webkit_close(WebKitWebView *webView, gpointer user_data)
{
    GtkWidget *frame = g_object_get_data((GObject *)webView, "frame");
    GtkNotebook * page = ui_lookup("main", "tabs");
    gint index = gtk_notebook_page_num(page, frame);

    if (index != -1){
        gtk_notebook_remove_page(GTK_NOTEBOOK(page), index);

        if (gtk_notebook_get_n_pages(page) == 0){
            browser_tab_new();
        }
    }
    return TRUE;
}

WebKitWebView* webkit_new_window (WebKitWebView  *webView, WebKitWebFrame *frame, gpointer user_data)
{
    gint index = browser_tab_new();
    GtkNotebook * page = ui_lookup("main", "tabs");
    GtkWidget *frame2 = gtk_notebook_get_nth_page(page, index);
    WebKitWebView *view = g_object_get_data((GObject *)frame2, "view");

    return view;
}

gboolean webkit_new_window2 (WebKitWebView *webView, WebKitWebFrame *frame, WebKitNetworkRequest *request, WebKitWebNavigationAction *navigation_action, WebKitWebPolicyDecision *policy_decision, gpointer user_data)
{
    browser_window_new(webView, frame, request, navigation_action, policy_decision, user_data);

    return FALSE;
}
