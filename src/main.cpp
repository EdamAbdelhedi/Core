#include "SpinAPI.h"
#include "TaskAPI.h"
#include "cut_pwm_control.h"
#include "spin_data_objects.h"
#include "hrtim.h"

namespace
{
    void loop_background_task()
    {
        task.suspendBackgroundMs(1000);
    }
}

void stop_all_pwm_outputs()
{
    for (uint8_t unit = 0; unit < HRTIM_CHANNELS; ++unit) {
        const auto hrtim_unit = static_cast<hrtim_tu_number_t>(unit);
        spin.pwm.stopDualOutput(hrtim_unit);
    }
}

bool cut_pwm_initialized = false;

void setup_routine()
{
    uint32_t background_task_number = task.createBackground(loop_background_task);
    task.startBackground(background_task_number);
}

uint32_t cut_multi_freq_hz = 0;
uint32_t cut_multi_min_freq_hz = 0;
uint8_t cut_multi_modulation = 0;
uint8_t cut_pwm_a_modulation = 0;
uint8_t cut_pwm_b_modulation = 0;
uint8_t cut_pwm_c_modulation = 0;
uint8_t cut_pwm_d_modulation = 0;
uint8_t cut_pwm_e_modulation = 0;
uint8_t cut_pwm_f_modulation = 0;
uint8_t cut_pwm_a_switch_conv = 0;
uint8_t cut_pwm_b_switch_conv = 0;
uint8_t cut_pwm_c_switch_conv = 0;
uint8_t cut_pwm_d_switch_conv = 0;
uint8_t cut_pwm_e_switch_conv = 0;
uint8_t cut_pwm_f_switch_conv = 0;
bool cut_multi_enabled = false;
uint8_t cut_multi_apply_status = 0;
uint16_t cut_pwm_a_period = 0;
uint16_t cut_pwm_b_period = 0;
uint16_t cut_pwm_c_period = 0;
uint16_t cut_pwm_d_period = 0;
uint16_t cut_pwm_e_period = 0;
uint16_t cut_pwm_f_period = 0;

bool cut_pwm_a1_enabled = false;
bool cut_pwm_a2_enabled = false;
bool cut_pwm_b1_enabled = false;
bool cut_pwm_c1_enabled = false;
bool cut_pwm_c2_enabled = false;
bool cut_pwm_d1_enabled = false;
bool cut_pwm_d2_enabled = false;
bool cut_pwm_e1_enabled = false;
bool cut_pwm_e2_enabled = false;
bool cut_pwm_f1_enabled = false;
bool cut_pwm_f2_enabled = false;

float cut_pwm_a_duty = 0.0f;
float cut_pwm_b_duty = 0.0f;
float cut_pwm_c_duty = 0.0f;
float cut_pwm_d_duty = 0.0f;
float cut_pwm_e_duty = 0.0f;
float cut_pwm_f_duty = 0.0f;

int16_t cut_pwm_a_phase_deg = 0;
int16_t cut_pwm_b_phase_deg = 0;
int16_t cut_pwm_c_phase_deg = 0;
int16_t cut_pwm_d_phase_deg = 0;
int16_t cut_pwm_e_phase_deg = 0;
int16_t cut_pwm_f_phase_deg = 0;

uint16_t cut_pwm_a_dead_rise_ns = 0;
uint16_t cut_pwm_b_dead_rise_ns = 0;
uint16_t cut_pwm_c_dead_rise_ns = 0;
uint16_t cut_pwm_d_dead_rise_ns = 0;
uint16_t cut_pwm_e_dead_rise_ns = 0;
uint16_t cut_pwm_f_dead_rise_ns = 0;

uint16_t cut_pwm_a_dead_fall_ns = 0;
uint16_t cut_pwm_b_dead_fall_ns = 0;
uint16_t cut_pwm_c_dead_fall_ns = 0;
uint16_t cut_pwm_d_dead_fall_ns = 0;
uint16_t cut_pwm_e_dead_fall_ns = 0;
uint16_t cut_pwm_f_dead_fall_ns = 0;

namespace
{
    hrtim_cnt_t resolve_modulation(uint8_t requested)
    {
        if (requested == static_cast<uint8_t>(Lft_aligned)) {
            return Lft_aligned;
        }
        if (requested == static_cast<uint8_t>(UpDwn)) {
            return UpDwn;
        }
        cut_multi_apply_status = 1;
        return UpDwn;
    }

    hrtim_switch_convention_t resolve_switch_convention(uint8_t requested)
    {
        if (requested == static_cast<uint8_t>(PWMx1)) {
            return PWMx1;
        }
        if (requested == static_cast<uint8_t>(PWMx2)) {
            return PWMx2;
        }
        cut_multi_apply_status = 1;
        return PWMx1;
    }

    void restore_left_aligned_voltage_outputs(hrtim_tu_number_t unit, hrtim_cnt_t modulation)
    {
        if (modulation != Lft_aligned) {
            return;
        }

        tu_channel[unit]->switch_conv.set_H   = SET_PER;
        tu_channel[unit]->switch_conv.reset_H = RST_CMP1;
        tu_channel[unit]->switch_conv.set_L   = SET_CMP1;
        tu_channel[unit]->switch_conv.reset_L = RST_PER;
    }
}

void cut_pwm_apply(void)
{
    cut_multi_apply_status = 0;

    if (cut_pwm_initialized) {
        stop_all_pwm_outputs();
    }

    if (cut_multi_min_freq_hz == cut_multi_freq_hz) {
        spin.pwm.initFixedFrequency(cut_multi_freq_hz);
    }
    else {
        spin.pwm.initVariableFrequency(cut_multi_freq_hz, cut_multi_min_freq_hz);
    }
    cut_pwm_initialized = true;

    if (cut_pwm_a1_enabled || cut_pwm_a2_enabled) {
        const auto modulation = resolve_modulation(cut_pwm_a_modulation);
        const auto switch_convention = resolve_switch_convention(cut_pwm_a_switch_conv);
        spin.pwm.setModulation(PWMA, modulation);
        spin.pwm.setSwitchConvention(PWMA, switch_convention);
        spin.pwm.setMode(PWMA, VOLTAGE_MODE);
        restore_left_aligned_voltage_outputs(PWMA, modulation);
        spin.pwm.setDeadTime(PWMA, cut_pwm_a_dead_rise_ns, cut_pwm_a_dead_fall_ns);
        spin.pwm.initUnit(PWMA);
        cut_pwm_a_period = spin.pwm.getPeriod(PWMA);
        spin.pwm.setDeadTime(PWMA, cut_pwm_a_dead_rise_ns, cut_pwm_a_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMA, cut_pwm_a_duty);
        spin.pwm.setPhaseShift(PWMA, cut_pwm_a_phase_deg);
    }

    if (cut_pwm_b1_enabled) {
        const auto modulation = resolve_modulation(cut_pwm_b_modulation);
        const auto switch_convention = resolve_switch_convention(cut_pwm_b_switch_conv);
        spin.pwm.setModulation(PWMB, modulation);
        spin.pwm.setSwitchConvention(PWMB, switch_convention);
        spin.pwm.setMode(PWMB, VOLTAGE_MODE);
        restore_left_aligned_voltage_outputs(PWMB, modulation);
        spin.pwm.setDeadTime(PWMB, cut_pwm_b_dead_rise_ns, cut_pwm_b_dead_fall_ns);
        spin.pwm.initUnit(PWMB);
        cut_pwm_b_period = spin.pwm.getPeriod(PWMB);
        spin.pwm.setDeadTime(PWMB, cut_pwm_b_dead_rise_ns, cut_pwm_b_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMB, cut_pwm_b_duty);
        spin.pwm.setPhaseShift(PWMB, cut_pwm_b_phase_deg);
    }

    if (cut_pwm_c1_enabled || cut_pwm_c2_enabled) {
        const auto modulation = resolve_modulation(cut_pwm_c_modulation);
        const auto switch_convention = resolve_switch_convention(cut_pwm_c_switch_conv);
        spin.pwm.setModulation(PWMC, modulation);
        spin.pwm.setSwitchConvention(PWMC, switch_convention);
        spin.pwm.setMode(PWMC, VOLTAGE_MODE);
        restore_left_aligned_voltage_outputs(PWMC, modulation);
        spin.pwm.setDeadTime(PWMC, cut_pwm_c_dead_rise_ns, cut_pwm_c_dead_fall_ns);
        spin.pwm.initUnit(PWMC);
        cut_pwm_c_period = spin.pwm.getPeriod(PWMC);
        spin.pwm.setDeadTime(PWMC, cut_pwm_c_dead_rise_ns, cut_pwm_c_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMC, cut_pwm_c_duty);
        spin.pwm.setPhaseShift(PWMC, cut_pwm_c_phase_deg);
    }

    if (cut_pwm_d1_enabled || cut_pwm_d2_enabled) {
        const auto modulation = resolve_modulation(cut_pwm_d_modulation);
        const auto switch_convention = resolve_switch_convention(cut_pwm_d_switch_conv);
        spin.pwm.setModulation(PWMD, modulation);
        spin.pwm.setSwitchConvention(PWMD, switch_convention);
        spin.pwm.setMode(PWMD, VOLTAGE_MODE);
        restore_left_aligned_voltage_outputs(PWMD, modulation);
        spin.pwm.setDeadTime(PWMD, cut_pwm_d_dead_rise_ns, cut_pwm_d_dead_fall_ns);
        spin.pwm.initUnit(PWMD);
        cut_pwm_d_period = spin.pwm.getPeriod(PWMD);
        spin.pwm.setDeadTime(PWMD, cut_pwm_d_dead_rise_ns, cut_pwm_d_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMD, cut_pwm_d_duty);
        spin.pwm.setPhaseShift(PWMD, cut_pwm_d_phase_deg);
    }

    if (cut_pwm_e1_enabled || cut_pwm_e2_enabled) {
        const auto modulation = resolve_modulation(cut_pwm_e_modulation);
        const auto switch_convention = resolve_switch_convention(cut_pwm_e_switch_conv);
        spin.pwm.setModulation(PWME, modulation);
        spin.pwm.setSwitchConvention(PWME, switch_convention);
        spin.pwm.setMode(PWME, VOLTAGE_MODE);
        restore_left_aligned_voltage_outputs(PWME, modulation);
        spin.pwm.setDeadTime(PWME, cut_pwm_e_dead_rise_ns, cut_pwm_e_dead_fall_ns);
        spin.pwm.initUnit(PWME);
        cut_pwm_e_period = spin.pwm.getPeriod(PWME);
        spin.pwm.setDeadTime(PWME, cut_pwm_e_dead_rise_ns, cut_pwm_e_dead_fall_ns);
        spin.pwm.setDutyCycle(PWME, cut_pwm_e_duty);
        spin.pwm.setPhaseShift(PWME, cut_pwm_e_phase_deg);
    }

    if (cut_pwm_f1_enabled || cut_pwm_f2_enabled) {
        const auto modulation = resolve_modulation(cut_pwm_f_modulation);
        const auto switch_convention = resolve_switch_convention(cut_pwm_f_switch_conv);
        spin.pwm.setModulation(PWMF, modulation);
        spin.pwm.setSwitchConvention(PWMF, switch_convention);
        spin.pwm.setMode(PWMF, VOLTAGE_MODE);
        restore_left_aligned_voltage_outputs(PWMF, modulation);
        spin.pwm.setDeadTime(PWMF, cut_pwm_f_dead_rise_ns, cut_pwm_f_dead_fall_ns);
        spin.pwm.initUnit(PWMF);
        cut_pwm_f_period = spin.pwm.getPeriod(PWMF);
        spin.pwm.setDeadTime(PWMF, cut_pwm_f_dead_rise_ns, cut_pwm_f_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMF, cut_pwm_f_duty);
        spin.pwm.setPhaseShift(PWMF, cut_pwm_f_phase_deg);
    }

    if (!cut_multi_enabled) {
        return;
    }

    if (cut_pwm_a1_enabled && cut_pwm_a2_enabled) {
        spin.pwm.startDualOutput(PWMA);
    }
    else {
        if (cut_pwm_a1_enabled) {
            spin.pwm.startSingleOutput(PWMA, TIMING_OUTPUT1);
        }
        if (cut_pwm_a2_enabled) {
            spin.pwm.startSingleOutput(PWMA, TIMING_OUTPUT2);
        }
    }

    if (cut_pwm_b1_enabled) {
        spin.pwm.startSingleOutput(PWMB, TIMING_OUTPUT1);
    }

    if (cut_pwm_c1_enabled && cut_pwm_c2_enabled) {
        spin.pwm.startDualOutput(PWMC);
    }
    else {
        if (cut_pwm_c1_enabled) {
            spin.pwm.startSingleOutput(PWMC, TIMING_OUTPUT1);
        }
        if (cut_pwm_c2_enabled) {
            spin.pwm.startSingleOutput(PWMC, TIMING_OUTPUT2);
        }
    }

    if (cut_pwm_d1_enabled && cut_pwm_d2_enabled) {
        spin.pwm.startDualOutput(PWMD);
    }
    else {
        if (cut_pwm_d1_enabled) {
            spin.pwm.startSingleOutput(PWMD, TIMING_OUTPUT1);
        }
        if (cut_pwm_d2_enabled) {
            spin.pwm.startSingleOutput(PWMD, TIMING_OUTPUT2);
        }
    }

    if (cut_pwm_e1_enabled && cut_pwm_e2_enabled) {
        spin.pwm.startDualOutput(PWME);
    }
    else {
        if (cut_pwm_e1_enabled) {
            spin.pwm.startSingleOutput(PWME, TIMING_OUTPUT1);
        }
        if (cut_pwm_e2_enabled) {
            spin.pwm.startSingleOutput(PWME, TIMING_OUTPUT2);
        }
    }

    if (cut_pwm_f1_enabled && cut_pwm_f2_enabled) {
        spin.pwm.startDualOutput(PWMF);
    }
    else {
        if (cut_pwm_f1_enabled) {
            spin.pwm.startSingleOutput(PWMF, TIMING_OUTPUT1);
        }
        if (cut_pwm_f2_enabled) {
            spin.pwm.startSingleOutput(PWMF, TIMING_OUTPUT2);
        }
    }
}

int main(void)
{
    setup_routine();
    return 0;
}



