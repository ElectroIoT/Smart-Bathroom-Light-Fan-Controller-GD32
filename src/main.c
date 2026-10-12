#include "app.h"

int main(void)
{
    app_init();
    for (;;) {
        app_step();     /* non-blocking; no delays anywhere */
    }
}
