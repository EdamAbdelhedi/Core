#include "SpinAPI.h"
#include "TaskAPI.h"
#include "cut_pwm_control.h"
#include "spin_data_objects.h"

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
bool cut_multi_enabled = false;
uint8_t cut_multi_apply_status = 0;

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
        spin.pwm.setModulation(PWMA, static_cast<hrtim_cnt_t>(cut_multi_modulation));
        spin.pwm.setSwitchConvention(PWMA, PWMx1);
        spin.pwm.setMode(PWMA, VOLTAGE_MODE);
        spin.pwm.setDeadTime(PWMA, cut_pwm_a_dead_rise_ns, cut_pwm_a_dead_fall_ns);
        spin.pwm.initUnit(PWMA);
        spin.pwm.setDeadTime(PWMA, cut_pwm_a_dead_rise_ns, cut_pwm_a_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMA, cut_pwm_a_duty);
        spin.pwm.setPhaseShift(PWMA, cut_pwm_a_phase_deg);
    }

    if (cut_pwm_b1_enabled) {
        spin.pwm.setModulation(PWMB, static_cast<hrtim_cnt_t>(cut_multi_modulation));
        spin.pwm.setSwitchConvention(PWMB, PWMx1);
        spin.pwm.setMode(PWMB, VOLTAGE_MODE);
        spin.pwm.setDeadTime(PWMB, cut_pwm_b_dead_rise_ns, cut_pwm_b_dead_fall_ns);
        spin.pwm.initUnit(PWMB);
        spin.pwm.setDeadTime(PWMB, cut_pwm_b_dead_rise_ns, cut_pwm_b_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMB, cut_pwm_b_duty);
        spin.pwm.setPhaseShift(PWMB, cut_pwm_b_phase_deg);
    }

    if (cut_pwm_c1_enabled || cut_pwm_c2_enabled) {
        spin.pwm.setModulation(PWMC, static_cast<hrtim_cnt_t>(cut_multi_modulation));
        spin.pwm.setSwitchConvention(PWMC, PWMx1);
        spin.pwm.setMode(PWMC, VOLTAGE_MODE);
        spin.pwm.setDeadTime(PWMC, cut_pwm_c_dead_rise_ns, cut_pwm_c_dead_fall_ns);
        spin.pwm.initUnit(PWMC);
        spin.pwm.setDeadTime(PWMC, cut_pwm_c_dead_rise_ns, cut_pwm_c_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMC, cut_pwm_c_duty);
        spin.pwm.setPhaseShift(PWMC, cut_pwm_c_phase_deg);
    }

    if (cut_pwm_d1_enabled || cut_pwm_d2_enabled) {
        spin.pwm.setModulation(PWMD, static_cast<hrtim_cnt_t>(cut_multi_modulation));
        spin.pwm.setSwitchConvention(PWMD, PWMx1);
        spin.pwm.setMode(PWMD, VOLTAGE_MODE);
        spin.pwm.setDeadTime(PWMD, cut_pwm_d_dead_rise_ns, cut_pwm_d_dead_fall_ns);
        spin.pwm.initUnit(PWMD);
        spin.pwm.setDeadTime(PWMD, cut_pwm_d_dead_rise_ns, cut_pwm_d_dead_fall_ns);
        spin.pwm.setDutyCycle(PWMD, cut_pwm_d_duty);
        spin.pwm.setPhaseShift(PWMD, cut_pwm_d_phase_deg);
    }

    if (cut_pwm_e1_enabled || cut_pwm_e2_enabled) {
        spin.pwm.setModulation(PWME, static_cast<hrtim_cnt_t>(cut_multi_modulation));
        spin.pwm.setSwitchConvention(PWME, PWMx1);
        spin.pwm.setMode(PWME, VOLTAGE_MODE);
        spin.pwm.setDeadTime(PWME, cut_pwm_e_dead_rise_ns, cut_pwm_e_dead_fall_ns);
        spin.pwm.initUnit(PWME);
        spin.pwm.setDeadTime(PWME, cut_pwm_e_dead_rise_ns, cut_pwm_e_dead_fall_ns);
        spin.pwm.setDutyCycle(PWME, cut_pwm_e_duty);
        spin.pwm.setPhaseShift(PWME, cut_pwm_e_phase_deg);
    }

    if (cut_pwm_f1_enabled || cut_pwm_f2_enabled) {
        spin.pwm.setModulation(PWMF, static_cast<hrtim_cnt_t>(cut_multi_modulation));
        spin.pwm.setSwitchConvention(PWMF, PWMx1);
        spin.pwm.setMode(PWMF, VOLTAGE_MODE);
        spin.pwm.setDeadTime(PWMF, cut_pwm_f_dead_rise_ns, cut_pwm_f_dead_fall_ns);
        spin.pwm.initUnit(PWMF);
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
