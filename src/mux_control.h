#ifndef MUX_CONTROL_H
#define MUX_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

constexpr uint8_t MUX_COUNT = 3;
constexpr uint8_t MUX_CHANNEL_COUNT = 4;

struct MuxRouteState
{
    uint8_t selected_channel;
    bool enabled;
};

extern MuxRouteState mux_routes[MUX_COUNT][MUX_CHANNEL_COUNT];

void mux_apply(uint8_t mux_index, uint8_t channel_index);

#endif /* MUX_CONTROL_H */
