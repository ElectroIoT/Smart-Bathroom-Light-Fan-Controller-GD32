#ifndef APP_H
#define APP_H

#include "controller.h"

/** Initialise hardware (relays OFF) and controller. */
void app_init(void);
/** One non-blocking main-loop pass: read PIR, update timers, drive relays. */
void app_step(void);
/** Read-only access for tests / debugging. */
const ctrl_t *app_state(void);

#endif
