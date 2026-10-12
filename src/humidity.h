/**
 * humidity.h - optional humidity logic (pure, no hardware).
 *
 * Decodes the 5-byte DHT11 / DHT22 frame and turns RH readings into a
 * "humid" flag with hysteresis. Sensor failures are counted; after
 * `fail_limit` consecutive failures the sensor is treated as invalid and the
 * humid flag is cleared (so a broken sensor can never hold the fan on).
 */
#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool humid;       /* RH above the ON threshold (with hysteresis) */
    bool valid;       /* false after repeated read failures          */
    uint8_t fails;    /* consecutive failures                        */
    uint8_t last_rh;  /* last good reading, percent                  */
} humidity_t;

/** Validate checksum and extract RH percent. sensor_type: 11 = DHT11, 22 = DHT22. */
bool dht_decode(const uint8_t raw[5], int sensor_type, uint8_t *rh_pct);

void humidity_init(humidity_t *h);

/** Feed one read attempt. */
void humidity_update(humidity_t *h, bool read_ok, uint8_t rh_pct,
                     uint8_t on_pct, uint8_t off_pct, uint8_t fail_limit);

#endif
