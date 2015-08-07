#ifndef __BROWSER_H__
#define __BROWSER_H__

#include <gtk/gtk.h>
#include <webkit/webkit.h>

int browser_tab_new();
GtkWindow *browser_window_new(WebKitWebView *webView, WebKitWebFrame *frame, WebKitNetworkRequest *request, WebKitWebNavigationAction *navigation_action, WebKitWebPolicyDecision *policy_decision, gpointer user_data);
void browser_goback(WebKitWebView *view);
void browser_goforward(WebKitWebView *view);
void browser_refresh(WebKitWebView *view);
void browser_stop(WebKitWebView *view);
void browser_go(WebKitWebView *view, const char *uri);


#endif
