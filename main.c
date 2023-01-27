#include "application.h"

#include <print.h>

static int _print_usage()
{
    print("usage : appinfo -t yelp.desktop");
    return 1;
}

int main(int argc, char **argv)
{
    Application *app = app_init();

    CStringAuto *deskfile = cstr_new_size(32);
    AppAction action = DESK_UNDEFINED;

    int n = 1;
    while (n < argc)
    {
        const gchar *part = argv[n];

        if (strcmp(part, "-t") == 0)
        {
            if (++n >= argc)
            {
                //print("n    : %d", n);
                //print("argc : %d", argc);
                return _print_usage();
            }

            action = DESK_TOGGLE;
            cstr_copy(deskfile, argv[n]);
        }
        else
        {
            return _print_usage();
        }

        ++n;
    }

    if (!cstr_endswith(deskfile, ".desktop", true))
        cstr_append(deskfile, ".desktop");

    app_desktop_edit(app, c_str(deskfile), action);

    app_cleanup(app);

    return 0;
}


