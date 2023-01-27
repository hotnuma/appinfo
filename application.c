#include "application.h"

#include <cfile.h>
#include <libstr.h>
#include <libpath.h>
#include <print.h>

#include <stdbool.h>

static Application _app_global;

Application* app_init()
{
    Application *app = &_app_global;

    const gchar *val = getenv("XDG_CURRENT_DESKTOP");
    if (val)
        app->current_desktop = g_strdup(val);

    app->sysdirs = NULL;
    const gchar* const* dirs = g_get_system_data_dirs();
    for (gint i = 0; dirs[i]; ++i)
    {
        app->sysdirs = g_list_prepend(app->sysdirs,
                                        g_build_filename(dirs[i],
                                                         "applications",
                                                         NULL));
    }
    app->sysdirs = g_list_reverse(app->sysdirs);

    app->userdir = g_build_filename(g_get_user_data_dir(),
                                       "applications",
                                       NULL);
    app->syspath = cstr_new_size(64);
    app->userpath = cstr_new_size(64);

    app->inlist = cstrlist_new_size(10);

    return app;
}

void app_cleanup(Application *app)
{
    if (app->current_desktop)
    {
        g_free(app->current_desktop);
        app->current_desktop = NULL;
    }

    if (app->sysdirs)
    {
        g_list_free_full(app->sysdirs, g_free);
        app->sysdirs = NULL;
    }

    if (app->userdir)
    {
        g_free(app->userdir);
        app->userdir = NULL;
    }

    cstr_free(app->syspath);
    app->syspath = NULL;

    cstr_free(app->userpath);
    app->userpath = NULL;

    cstrlist_free(app->inlist);
    app->inlist = NULL;
}

bool app_get_syspath(Application *app, const gchar *id)
{
    CString *srcpath = app->syspath;

    for (GList *lp = app->sysdirs; lp; lp = lp->next)
    {
        const gchar *dir = (const gchar*) lp->data;

        path_join(srcpath, dir, id);

        if (file_exists(c_str(srcpath)))
            return true;
    }

    cstr_clear(srcpath);

    return false;
}

bool app_get_userpath(Application *app, const gchar *id)
{
    CString *userpath = app->userpath;

    path_join(userpath, app->userdir, id);

    if (file_exists(c_str(userpath)))
        return true;

    return false;
}

bool app_desktop_edit(Application *app, const gchar *id, AppAction show)
{
    if (!app->current_desktop)
        return false;

    const gchar *currdesktop = app->current_desktop;
    const gchar *srcpath = NULL;
    const gchar *destpath = NULL;

    if (!app_get_syspath(app, id))
        return false;

    if (app_get_userpath(app, id))
    {
        srcpath = c_str(app->userpath);
        destpath = srcpath;
    }
    else
    {
        srcpath = c_str(app->syspath);
        destpath = c_str(app->userpath);
    }

    print(srcpath);
    print(destpath);

    CFileAuto *file = cfile_new();
    if (!cfile_read(file, srcpath))
        return 1;

    if (!cfile_open(file, destpath, "wb"))
        return 1;

    char *ptr = cfile_data(file);
    char *result;
    int length;

    while (str_getlineptr(&ptr, &result, &length))
    {
        result[length] = '\0';

        if (strncmp(result, "OnlyShowIn=", 11) == 0)
        {
            result += 11;

            CStringList *parts = app->inlist;

            cstrlist_split(parts, result, ";", false, true);

            cfile_write(file, "OnlyShowIn=");

            bool is_visible = false;

            gint size = cstrlist_size(parts);
            for (gint i = 0; i < size; ++i)
            {
                const gchar *part = c_str(cstrlist_at(parts, i));

                if (strcmp(part, currdesktop) == 0)
                {
                    is_visible = true;
                    continue;
                }

                print(part);

                cfile_write(file, part);
                cfile_write(file, ";");
            }

            if (show == DESK_SHOW || (!is_visible && show == DESK_TOGGLE))
            {
                cfile_write(file, currdesktop);
                cfile_write(file, ";");
            }

            cfile_write(file, "\n");

            continue;
        }

        cfile_write(file, result);
        cfile_write(file, "\n");
    }

    return true;
}



