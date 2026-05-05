#ifndef SPIN_DATA_OBJECTS_H
#define SPIN_DATA_OBJECTS_H

#include <stdbool.h>
#include <stdint.h>

#include <thingset.h>
#include <thingset/sdk.h>

#include "mux_control.h"

#ifndef MUX_TS_NO_SUBSET
#define MUX_TS_NO_SUBSET 0
#endif

#define ID_MUX              0x70
#define ID_MUX_MUX1         0x701
#define ID_MUX_MUX1_CH1     0x7011
#define ID_MUX_MUX1_CH2     0x7012
#define ID_MUX_MUX1_CH3     0x7013
#define ID_MUX_MUX1_CH4     0x7014
#define ID_MUX_MUX2         0x702
#define ID_MUX_MUX2_CH1     0x7021
#define ID_MUX_MUX2_CH2     0x7022
#define ID_MUX_MUX2_CH3     0x7023
#define ID_MUX_MUX2_CH4     0x7024
#define ID_MUX_MUX3         0x703
#define ID_MUX_MUX3_CH1     0x7031
#define ID_MUX_MUX3_CH2     0x7032
#define ID_MUX_MUX3_CH3     0x7033
#define ID_MUX_MUX3_CH4     0x7034

static bool mux_apply_request[MUX_COUNT][MUX_CHANNEL_COUNT] = {};

static int mux_route_post_write(uint8_t mux_index,
                                uint8_t channel_index,
                                enum thingset_callback_reason reason)
{
    if (reason != THINGSET_CALLBACK_POST_WRITE) {
        return 0;
    }

    mux_routes[mux_index][channel_index].selected_channel &= 0x0F;

    if (mux_apply_request[mux_index][channel_index]) {
        mux_apply(mux_index, channel_index);
        mux_apply_request[mux_index][channel_index] = false;
    }

    return 0;
}

#define DEFINE_MUX_CALLBACK(MIDX, CIDX) \
static int mux_##MIDX##_##CIDX##_cb(enum thingset_callback_reason reason, \
                                    const thingset_data_object *obj) \
{ \
    (void)obj; \
    return mux_route_post_write(MIDX, CIDX, reason); \
}

DEFINE_MUX_CALLBACK(0, 0)
DEFINE_MUX_CALLBACK(0, 1)
DEFINE_MUX_CALLBACK(0, 2)
DEFINE_MUX_CALLBACK(0, 3)
DEFINE_MUX_CALLBACK(1, 0)
DEFINE_MUX_CALLBACK(1, 1)
DEFINE_MUX_CALLBACK(1, 2)
DEFINE_MUX_CALLBACK(1, 3)
DEFINE_MUX_CALLBACK(2, 0)
DEFINE_MUX_CALLBACK(2, 1)
DEFINE_MUX_CALLBACK(2, 2)
DEFINE_MUX_CALLBACK(2, 3)

THINGSET_ADD_GROUP(TS_ID_ROOT, ID_MUX, "Mux", THINGSET_NO_CALLBACK);
THINGSET_ADD_GROUP(ID_MUX, ID_MUX_MUX1, "Mux1", THINGSET_NO_CALLBACK);
THINGSET_ADD_GROUP(ID_MUX, ID_MUX_MUX2, "Mux2", THINGSET_NO_CALLBACK);
THINGSET_ADD_GROUP(ID_MUX, ID_MUX_MUX3, "Mux3", THINGSET_NO_CALLBACK);

#define ADD_MUX_ROUTE_OBJECTS(PARENT_ID, GROUP_ID, LABEL, CB, MIDX, CIDX, \
                              ID_WCHANNEL, ID_WENABLE, ID_XAPPLY, ID_RCHANNEL, ID_RENABLE) \
THINGSET_ADD_GROUP(PARENT_ID, GROUP_ID, LABEL, &CB); \
THINGSET_ADD_ITEM_UINT8(GROUP_ID, ID_WCHANNEL, "wChannel", \
                        &mux_routes[MIDX][CIDX].selected_channel, THINGSET_ANY_RW, MUX_TS_NO_SUBSET); \
THINGSET_ADD_ITEM_BOOL(GROUP_ID, ID_WENABLE, "wEnable", \
                       &mux_routes[MIDX][CIDX].enabled, THINGSET_ANY_RW, MUX_TS_NO_SUBSET); \
THINGSET_ADD_ITEM_BOOL(GROUP_ID, ID_XAPPLY, "xApply", \
                       &mux_apply_request[MIDX][CIDX], THINGSET_ANY_RW, MUX_TS_NO_SUBSET); \
THINGSET_ADD_ITEM_UINT8(GROUP_ID, ID_RCHANNEL, "rChannel", \
                        &mux_routes[MIDX][CIDX].selected_channel, THINGSET_ANY_R, MUX_TS_NO_SUBSET); \
THINGSET_ADD_ITEM_BOOL(GROUP_ID, ID_RENABLE, "rEnable", \
                       &mux_routes[MIDX][CIDX].enabled, THINGSET_ANY_R, MUX_TS_NO_SUBSET);

ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX1, ID_MUX_MUX1_CH1, "Ch1", mux_0_0_cb, 0, 0, 0x7111, 0x7112, 0x7113, 0x7114, 0x7115)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX1, ID_MUX_MUX1_CH2, "Ch2", mux_0_1_cb, 0, 1, 0x7116, 0x7117, 0x7118, 0x7119, 0x711A)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX1, ID_MUX_MUX1_CH3, "Ch3", mux_0_2_cb, 0, 2, 0x7141, 0x7142, 0x7143, 0x7144, 0x7145)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX1, ID_MUX_MUX1_CH4, "Ch4", mux_0_3_cb, 0, 3, 0x7146, 0x7147, 0x7148, 0x7149, 0x714A)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX2, ID_MUX_MUX2_CH1, "Ch1", mux_1_0_cb, 1, 0, 0x7121, 0x7122, 0x7123, 0x7124, 0x7125)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX2, ID_MUX_MUX2_CH2, "Ch2", mux_1_1_cb, 1, 1, 0x7126, 0x7127, 0x7128, 0x7129, 0x712A)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX2, ID_MUX_MUX2_CH3, "Ch3", mux_1_2_cb, 1, 2, 0x7151, 0x7152, 0x7153, 0x7154, 0x7155)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX2, ID_MUX_MUX2_CH4, "Ch4", mux_1_3_cb, 1, 3, 0x7156, 0x7157, 0x7158, 0x7159, 0x715A)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX3, ID_MUX_MUX3_CH1, "Ch1", mux_2_0_cb, 2, 0, 0x7131, 0x7132, 0x7133, 0x7134, 0x7135)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX3, ID_MUX_MUX3_CH2, "Ch2", mux_2_1_cb, 2, 1, 0x7136, 0x7137, 0x7138, 0x7139, 0x713A)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX3, ID_MUX_MUX3_CH3, "Ch3", mux_2_2_cb, 2, 2, 0x7161, 0x7162, 0x7163, 0x7164, 0x7165)
ADD_MUX_ROUTE_OBJECTS(ID_MUX_MUX3, ID_MUX_MUX3_CH4, "Ch4", mux_2_3_cb, 2, 3, 0x7166, 0x7167, 0x7168, 0x7169, 0x716A)

#endif /* SPIN_DATA_OBJECTS_H */
