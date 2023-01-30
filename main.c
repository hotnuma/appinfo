#include "application.h"
#include <locale.h>
#include <print.h>

static int _print_usage()
{
    print("*** usage : appinfo -t yelp.desktop");
    return 1;
}

int main(int argc, char **argv)
{
    const char *lang = getenv("LANG");
    if (lang)
        setlocale(LC_ALL, lang);

    Application *app = app_init();

    AppAction action = DESK_UNDEFINED;
    const char *deskfile = NULL;

    int n = 1;
    while (n < argc)
    {
        const gchar *part = argv[n];

        if (strcmp(part, "-h") == 0)
        {
            if (++n >= argc)
                return _print_usage();

            action = DESK_HIDE;
            deskfile = argv[n];
        }
        else if (strcmp(part, "-s") == 0)
        {
            if (++n >= argc)
                return _print_usage();

            action = DESK_SHOW;
            deskfile = argv[n];
        }
        else if (strcmp(part, "-t") == 0)
        {
            if (++n >= argc)
                return _print_usage();

            action = DESK_TOGGLE;
            deskfile = argv[n];
        }
        else if (strcmp(part, "-a") == 0)
        {
            action = DESK_LISTALL;
        }
        else
        {
            return _print_usage();
        }

        ++n;
    }

    if (action == DESK_HIDE || action == DESK_SHOW || action == DESK_TOGGLE)
        app_desktop_edit(app, deskfile, action);

    else if (action == DESK_LISTALL || action == DESK_LISTVISIBLE)
        appinfo_list(action);

    app_cleanup(app);

    return 0;
}


