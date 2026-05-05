#include "SpinAPI.h"
#include "mux_control.h"
#include "spin_data_objects.h"

namespace
{
    constexpr uint8_t DEFAULT_MUX_CHANNEL = 0;

    struct MuxChannelPins
    {
        uint8_t s0;
        uint8_t s1;
        uint8_t s2;
        uint8_t s3;
        uint8_t mux1_enable;
        uint8_t mux2_enable;
        uint8_t mux3_enable;
    };

    constexpr MuxChannelPins MUX_CH1_PINS = {PA0, PA1, PA2, PA3, PC0, PC1, PC2};
    constexpr MuxChannelPins MUX_CH2_PINS = {PA4, PA5, PA6, PA7, PC3, PC4, PC5};
    constexpr MuxChannelPins MUX_CH3_PINS = {PB0, PB1, PB2, PB3, PC6, PC7, PC8};
    constexpr MuxChannelPins MUX_CH4_PINS = {PB4, PB5, PB6, PB7, PC9, PC10, PC11};

    constexpr MuxChannelPins get_channel_pins(uint8_t channel_index)
    {
        switch (channel_index) {
        case 0:
            return MUX_CH1_PINS;
        case 1:
            return MUX_CH2_PINS;
        case 2:
            return MUX_CH3_PINS;
        default:
            return MUX_CH4_PINS;
        }
    }

    constexpr uint8_t get_enable_pin(const MuxChannelPins &pins, uint8_t mux_index)
    {
        switch (mux_index) {
        case 0:
            return pins.mux1_enable;
        case 1:
            return pins.mux2_enable;
        default:
            return pins.mux3_enable;
        }
    };

    void write_mux_bit(uint8_t pin, bool state)
    {
        if (state) {
            spin.gpio.setPin(pin);
        }
        else {
            spin.gpio.resetPin(pin);
        }
    }

    void configure_channel_pins(uint8_t channel_index)
    {
        const MuxChannelPins pins = get_channel_pins(channel_index);

        spin.gpio.configurePin(pins.s0, OUTPUT);
        spin.gpio.configurePin(pins.s1, OUTPUT);
        spin.gpio.configurePin(pins.s2, OUTPUT);
        spin.gpio.configurePin(pins.s3, OUTPUT);
        spin.gpio.configurePin(pins.mux1_enable, OUTPUT);
        spin.gpio.configurePin(pins.mux2_enable, OUTPUT);
        spin.gpio.configurePin(pins.mux3_enable, OUTPUT);
    }

    void disable_all_mux_outputs(uint8_t channel_index)
    {
        const MuxChannelPins pins = get_channel_pins(channel_index);

        for (uint8_t mux_index = 0; mux_index < MUX_COUNT; ++mux_index) {
            write_mux_bit(get_enable_pin(pins, mux_index), true);
        }
    }

    void route_shared_signal(uint8_t channel_index, uint8_t selected_channel)
    {
        const uint8_t channel = selected_channel & 0x0F;
        const MuxChannelPins pins = get_channel_pins(channel_index);

        write_mux_bit(pins.s0, (channel & 0x01u) != 0u);
        write_mux_bit(pins.s1, (channel & 0x02u) != 0u);
        write_mux_bit(pins.s2, (channel & 0x04u) != 0u);
        write_mux_bit(pins.s3, (channel & 0x08u) != 0u);
    }

    void set_mux_enable(uint8_t channel_index, uint8_t mux_index, bool enabled)
    {
        const MuxChannelPins pins = get_channel_pins(channel_index);
        write_mux_bit(get_enable_pin(pins, mux_index), !enabled);
    }

    void setup_routine()
    {
        for (uint8_t channel_index = 0; channel_index < MUX_CHANNEL_COUNT; ++channel_index) {
            configure_channel_pins(channel_index);
            disable_all_mux_outputs(channel_index);
        }

        for (uint8_t mux_index = 0; mux_index < MUX_COUNT; ++mux_index) {
            for (uint8_t channel_index = 0; channel_index < MUX_CHANNEL_COUNT; ++channel_index) {
                mux_apply(mux_index, channel_index);
            }
        }
    }
}

MuxRouteState mux_routes[MUX_COUNT][MUX_CHANNEL_COUNT] = {
    {
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
    },
    {
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
    },
    {
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
        {DEFAULT_MUX_CHANNEL, false},
    },
};

void mux_apply(uint8_t mux_index, uint8_t channel_index)
{
    if (mux_index >= MUX_COUNT || channel_index >= MUX_CHANNEL_COUNT) {
        return;
    }

    MuxRouteState &route = mux_routes[mux_index][channel_index];
    route.selected_channel &= 0x0F;

    if (route.enabled) {
        disable_all_mux_outputs(channel_index);
        route_shared_signal(channel_index, route.selected_channel);
        set_mux_enable(channel_index, mux_index, true);
    }
    else {
        set_mux_enable(channel_index, mux_index, false);
    }
}

int main(void)
{
    setup_routine();
    return 0;
}
