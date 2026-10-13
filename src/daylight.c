#include "daylight.h"

void daylight_init(daylight_t *d, uint32_t now_ms)
{
    d->bright = false;      /* unknown = dark = light allowed (safe default) */
    d->cand = false;
    d->since_ms = now_ms;
}

void daylight_update(daylight_t *d, uint32_t now, bool raw_bright,
                     bool sampling, uint32_t filter_ms)
{
    if (!sampling) {                 /* lamp ON: ignore, and re-qualify later */
        d->cand = d->bright;
        d->since_ms = now;
        return;
    }
    if (raw_bright == d->bright) {
        d->cand = raw_bright;
        return;
    }
    if (raw_bright != d->cand) {
        d->cand = raw_bright;
        d->since_ms = now;
    }
    if ((uint32_t)(now - d->since_ms) >= filter_ms)
        d->bright = raw_bright;
}
