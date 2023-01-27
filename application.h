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

#endif // APPLICATION_H


