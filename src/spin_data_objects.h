#ifndef SPIN_DATA_OBJECTS_H
#define SPIN_DATA_OBJECTS_H

#include <stdint.h>
#include <stdbool.h>
#include <thingset.h>
#include <thingset/sdk.h>

#include "SpinAPI.h"
#include "cut_pwm_control.h"

#ifndef SPIN_TS_NO_SUBSET
#define SPIN_TS_NO_SUBSET 0
#endif

#define ID_SPIN              0x70
#define ID_CUT               0x7A

#define ID_SPIN_PWM          0x704
#define ID_CUT_PWM_OUT       0x7A1

THINGSET_ADD_GROUP(TS_ID_ROOT, ID_SPIN, "Spin", THINGSET_NO_CALLBACK);
THINGSET_ADD_GROUP(TS_ID_ROOT, ID_CUT, "Cut", THINGSET_NO_CALLBACK);

static bool cut_pwm_apply_request = false;
static int cut_pwm_out_cb(enum thingset_callback_reason reason,
                          const thingset_data_object *obj)
{
    (void)obj;
    if (reason != THINGSET_CALLBACK_POST_WRITE) {
        return 0;
    }

    if (cut_pwm_apply_request) {
        cut_pwm_apply();
        cut_pwm_apply_request = false;
    }

    return 0;
}

THINGSET_ADD_GROUP(ID_CUT, ID_CUT_PWM_OUT, "PwmOut", &cut_pwm_out_cb);
THINGSET_ADD_ITEM_UINT32(ID_CUT_PWM_OUT, 0x7A11, "wFreq_Hz", &cut_multi_freq_hz, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT32(ID_CUT_PWM_OUT, 0x7A12, "wMinFreq_Hz", &cut_multi_min_freq_hz, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A13, "wMod", &cut_multi_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A14, "wEnable", &cut_multi_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A15, "xApply", &cut_pwm_apply_request, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A16, "wA1", &cut_pwm_a1_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A17, "wA2", &cut_pwm_a2_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A18, "wB1", &cut_pwm_b1_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A19, "wC1", &cut_pwm_c1_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A1A, "wC2", &cut_pwm_c2_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A1B, "wD1", &cut_pwm_d1_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A1C, "wD2", &cut_pwm_d2_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A1D, "wE1", &cut_pwm_e1_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A1E, "wE2", &cut_pwm_e2_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A1F, "wF1", &cut_pwm_f1_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_CUT_PWM_OUT, 0x7A20, "wF2", &cut_pwm_f2_enabled, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_FLOAT (ID_CUT_PWM_OUT, 0x7A21, "wADuty", &cut_pwm_a_duty, 4, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_INT16 (ID_CUT_PWM_OUT, 0x7A22, "wAPhase_deg", &cut_pwm_a_phase_deg, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A23, "wADeadRise", &cut_pwm_a_dead_rise_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A24, "wADeadFall", &cut_pwm_a_dead_fall_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A3A, "wAMod", &cut_pwm_a_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_FLOAT (ID_CUT_PWM_OUT, 0x7A25, "wBDuty", &cut_pwm_b_duty, 4, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_INT16 (ID_CUT_PWM_OUT, 0x7A26, "wBPhase_deg", &cut_pwm_b_phase_deg, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A27, "wBDeadRise", &cut_pwm_b_dead_rise_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A28, "wBDeadFall", &cut_pwm_b_dead_fall_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A3B, "wBMod", &cut_pwm_b_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_FLOAT (ID_CUT_PWM_OUT, 0x7A29, "wCDuty", &cut_pwm_c_duty, 4, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_INT16 (ID_CUT_PWM_OUT, 0x7A2A, "wCPhase_deg", &cut_pwm_c_phase_deg, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A2B, "wCDeadRise", &cut_pwm_c_dead_rise_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A2C, "wCDeadFall", &cut_pwm_c_dead_fall_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A3C, "wCMod", &cut_pwm_c_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_FLOAT (ID_CUT_PWM_OUT, 0x7A2D, "wDDuty", &cut_pwm_d_duty, 4, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_INT16 (ID_CUT_PWM_OUT, 0x7A2E, "wDPhase_deg", &cut_pwm_d_phase_deg, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A2F, "wDDeadRise", &cut_pwm_d_dead_rise_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A30, "wDDeadFall", &cut_pwm_d_dead_fall_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A3D, "wDMod", &cut_pwm_d_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_FLOAT (ID_CUT_PWM_OUT, 0x7A31, "wEDuty", &cut_pwm_e_duty, 4, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_INT16 (ID_CUT_PWM_OUT, 0x7A32, "wEPhase_deg", &cut_pwm_e_phase_deg, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A33, "wEDeadRise", &cut_pwm_e_dead_rise_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A34, "wEDeadFall", &cut_pwm_e_dead_fall_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A3E, "wEMod", &cut_pwm_e_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_FLOAT (ID_CUT_PWM_OUT, 0x7A35, "wFDuty", &cut_pwm_f_duty, 4, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_INT16 (ID_CUT_PWM_OUT, 0x7A36, "wFPhase_deg", &cut_pwm_f_phase_deg, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A37, "wFDeadRise", &cut_pwm_f_dead_rise_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A38, "wFDeadFall", &cut_pwm_f_dead_fall_ns, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A3F, "wFMod", &cut_pwm_f_modulation, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

THINGSET_ADD_ITEM_UINT8 (ID_CUT_PWM_OUT, 0x7A39, "rStatus", &cut_multi_apply_status, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A40, "rAPeriod", &cut_pwm_a_period, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A41, "rBPeriod", &cut_pwm_b_period, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A42, "rCPeriod", &cut_pwm_c_period, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A43, "rDPeriod", &cut_pwm_d_period, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A44, "rEPeriod", &cut_pwm_e_period, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT16(ID_CUT_PWM_OUT, 0x7A45, "rFPeriod", &cut_pwm_f_period, THINGSET_ANY_R, SPIN_TS_NO_SUBSET);

typedef enum : uint8_t {
    PWM_ACT_INIT_BURST = 23,
    PWM_ACT_SET_BURST,
    PWM_ACT_START_BURST,
    PWM_ACT_STOP_BURST,
    PWM_ACT_DEINIT_BURST
} spin_pwm_action_t;

typedef struct {
    uint8_t tu;
    uint8_t action;
    uint32_t burst_cmp;
    uint32_t burst_per;
    bool exec;
} spin_pwm_cmd_t;

static spin_pwm_cmd_t spin_pwm_cmd = {
    .tu = 0,
    .action = PWM_ACT_INIT_BURST,
    .burst_cmp = 0,
    .burst_per = 0,
    .exec = false
};

static int spin_pwm_cb(enum thingset_callback_reason reason,
                       const thingset_data_object *obj)
{
    (void)obj;
    if (reason != THINGSET_CALLBACK_POST_WRITE || !spin_pwm_cmd.exec) {
        return 0;
    }

    spin_pwm_cmd.exec = false;

    if (spin_pwm_cmd.tu >= HRTIM_CHANNELS) {
        return -EINVAL;
    }

    switch (spin_pwm_cmd.action) {
    case PWM_ACT_INIT_BURST:
        spin.pwm.initBurstMode();
        break;
    case PWM_ACT_SET_BURST:
        spin.pwm.setBurstMode((int)spin_pwm_cmd.burst_cmp, (int)spin_pwm_cmd.burst_per);
        break;
    case PWM_ACT_START_BURST:
        spin.pwm.startBurstMode();
        break;
    case PWM_ACT_STOP_BURST:
        spin.pwm.stopBurstMode();
        break;
    case PWM_ACT_DEINIT_BURST:
        spin.pwm.deInitBurstMode();
        break;
    default:
        break;
    }

    return 0;
}

THINGSET_ADD_GROUP(ID_SPIN, ID_SPIN_PWM, "Pwm", &spin_pwm_cb);
THINGSET_ADD_ITEM_UINT8 (ID_SPIN_PWM, 0x7201, "wTU", &spin_pwm_cmd.tu, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT8 (ID_SPIN_PWM, 0x7203, "wAction", &spin_pwm_cmd.action, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_BOOL  (ID_SPIN_PWM, 0x7204, "xExec", &spin_pwm_cmd.exec, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT32(ID_SPIN_PWM, 0x7217, "wBurstCmp", &spin_pwm_cmd.burst_cmp, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);
THINGSET_ADD_ITEM_UINT32(ID_SPIN_PWM, 0x7218, "wBurstPer", &spin_pwm_cmd.burst_per, THINGSET_ANY_RW, SPIN_TS_NO_SUBSET);

#endif


