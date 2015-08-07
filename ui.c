#include "ui.h"
#include <string.h>
#include <gmodule.h>

GHashTable *ui_table = NULL;

static void ui_table_init()
{
    if (!ui_table){
        ui_table = g_hash_table_new_full(g_str_hash,
                                         g_str_equal,
                                         g_free,
                                         g_object_unref);
    }
}

static GtkBuilder *ui_builder_create(const char *filename)
{
    GtkBuilder * result = gtk_builder_new();
    gtk_builder_add_from_file(result, filename, NULL);
    gtk_builder_connect_signals(result, NULL);

    return result;
}

static GtkBuilder *ui_builder_lookup(const char *name)
{
    GtkBuilder * result = NULL;
    if (!ui_table){
        ui_table_init();
    }
    result = (GtkBuilder *)g_hash_table_lookup(ui_table, name);

    if (!result){
        int len = strlen(name);
        char * filename = strcpy(g_new(char, len + 4), name);
        strcat(filename, ".ui");

        result = ui_builder_create(filename);

        g_hash_table_insert(ui_table, (gpointer)name, result);
    }

    return result;
}

gpointer ui_lookup(const char *filename, const char *name)
{
    GtkBuilder *builder = ui_builder_lookup(filename);

    return gtk_builder_get_object (builder, name);
}
