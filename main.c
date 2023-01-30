#include "application.h"
#include <locale.h>
#include <print.h>

static int _app_exit(Application *app, bool usage, int ret)
{
    if (usage)
        print("*** usage : appinfo -t yelp.desktop");

    app_cleanup(app);
    return ret;
}

static void _set_deskfile(CString *result, const gchar *id)
{
    cstr_copy(result, id);

    if (!cstr_endswith(result, ".desktop", true))
        cstr_append(result, ".desktop");
}

int main(int argc, char **argv)
{
    const char *lang = getenv("LANG");
    if (lang)
        setlocale(LC_ALL, lang);

    Application *app = app_init();

    AppAction action = DESK_UNDEFINED;

    CStringAuto *deskfile = cstr_new_size(32);

    int n = 1;
    while (n < argc)
    {
        const gchar *part = argv[n];

        if (strcmp(part, "-h") == 0)
        {
            if (++n >= argc)
                return _app_exit(app, true, EXIT_FAILURE);

            action = DESK_HIDE;
            _set_deskfile(deskfile, argv[n]);
        }
        else if (strcmp(part, "-s") == 0)
        {
            if (++n >= argc)
                return _app_exit(app, true, EXIT_FAILURE);

            action = DESK_SHOW;
            _set_deskfile(deskfile, argv[n]);
        }
        else if (strcmp(part, "-t") == 0)
        {
            if (++n >= argc)
                return _app_exit(app, true, EXIT_FAILURE);

            action = DESK_TOGGLE;
            _set_deskfile(deskfile, argv[n]);
        }
        else if (strcmp(part, "-a") == 0)
        {
            action = DESK_LISTALL;
        }
        else if (strcmp(part, "-f") == 0)
        {
            if (++n >= argc)
                return _app_exit(app, true, EXIT_FAILURE);

            _set_deskfile(deskfile, argv[n]);

            if (!app_get_syspath(app, c_str(deskfile)))
                return _app_exit(app, false, EXIT_FAILURE);

            print(c_str(app->syspath));

            return _app_exit(app, false, EXIT_SUCCESS);
        }
        else if (strcmp(part, "-u") == 0)
        {
            if (++n >= argc)
                return _app_exit(app, true, EXIT_FAILURE);

            _set_deskfile(deskfile, argv[n]);

            if (!app_get_userpath(app, c_str(deskfile)))
                return _app_exit(app, false, EXIT_FAILURE);

            print(c_str(app->userpath));

            return _app_exit(app, false, EXIT_SUCCESS);
        }
        else
        {
            return _app_exit(app, true, EXIT_FAILURE);
        }

        ++n;
    }

    if (action == DESK_HIDE || action == DESK_SHOW || action == DESK_TOGGLE)
        app_desktop_edit(app, c_str(deskfile), action);

    else if (action == DESK_LISTALL || action == DESK_LISTVISIBLE)
        app_makelist(action);

    return _app_exit(app, false, EXIT_SUCCESS);
}


