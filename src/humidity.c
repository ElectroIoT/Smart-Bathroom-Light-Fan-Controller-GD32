#include "humidity.h"

bool dht_decode(const uint8_t raw[5], int sensor_type, uint8_t *rh_pct)
{
    uint8_t sum = (uint8_t)(raw[0] + raw[1] + raw[2] + raw[3]);
    uint32_t rh;

    if (sum != raw[4])
        return false;
    if (sensor_type == 22) {
        rh = (((uint32_t)raw[0] << 8) | raw[1]) / 10u;   /* tenths of a percent */
    } else {
        rh = raw[0];                                      /* DHT11: integer % */
    }
    if (rh > 100u)
        return false;
    *rh_pct = (uint8_t)rh;
    return true;
}

void humidity_init(humidity_t *h)
{
    h->humid = false;
    h->valid = true;
    h->fails = 0;
    h->last_rh = 0;
}

void humidity_update(humidity_t *h, bool read_ok, uint8_t rh_pct,
                     uint8_t on_pct, uint8_t off_pct, uint8_t fail_limit)
{
    if (!read_ok) {
        if (h->fails < 255u) h->fails++;
        if (h->fails >= fail_limit) {
            h->valid = false;
            h->humid = false;     /* a broken sensor must not hold the fan on */
        }
        return;
    }
    h->fails = 0;
    h->valid = true;
    h->last_rh = rh_pct;
    if (rh_pct >= on_pct)
        h->humid = true;
    else if (rh_pct <= off_pct)
        h->humid = false;         /* between the thresholds: keep the state */
}
