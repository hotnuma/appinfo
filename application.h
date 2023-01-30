#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstringlist.h>
#include <glib.h>

typedef enum
{
    DESK_UNDEFINED,
    DESK_SHOW,
    DESK_HIDE,
    DESK_TOGGLE,
    DESK_LISTALL,
    DESK_LISTVISIBLE,

} AppAction;

typedef struct
{
    gchar *current_desktop;

    GList *sysdirs;
    gchar *userdir;

    CString *syspath;
    CString *userpath;

    CStringList *inlist;

} Application;

Application* app_init();
void app_cleanup(Application *app);

bool app_desktop_edit(Application *app, const gchar *id, AppAction action);
bool app_get_syspath(Application *app, const gchar *id);
bool app_get_userpath(Application *app, const gchar *id);

void app_makelist(AppAction action);

#endif // APPLICATION_H


