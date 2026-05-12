#ifndef CUT_PWM_CONTROL_H
#define CUT_PWM_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

extern uint32_t cut_multi_freq_hz;
extern uint32_t cut_multi_min_freq_hz;
extern uint8_t cut_multi_modulation;
extern uint8_t cut_pwm_a_modulation;
extern uint8_t cut_pwm_b_modulation;
extern uint8_t cut_pwm_c_modulation;
extern uint8_t cut_pwm_d_modulation;
extern uint8_t cut_pwm_e_modulation;
extern uint8_t cut_pwm_f_modulation;
extern bool cut_multi_enabled;
extern uint8_t cut_multi_apply_status;
extern uint16_t cut_pwm_a_period;
extern uint16_t cut_pwm_b_period;
extern uint16_t cut_pwm_c_period;
extern uint16_t cut_pwm_d_period;
extern uint16_t cut_pwm_e_period;
extern uint16_t cut_pwm_f_period;

extern bool cut_pwm_a1_enabled;
extern bool cut_pwm_a2_enabled;
extern bool cut_pwm_b1_enabled;
extern bool cut_pwm_c1_enabled;
extern bool cut_pwm_c2_enabled;
extern bool cut_pwm_d1_enabled;
extern bool cut_pwm_d2_enabled;
extern bool cut_pwm_e1_enabled;
extern bool cut_pwm_e2_enabled;
extern bool cut_pwm_f1_enabled;
extern bool cut_pwm_f2_enabled;

extern float cut_pwm_a_duty;
extern float cut_pwm_b_duty;
extern float cut_pwm_c_duty;
extern float cut_pwm_d_duty;
extern float cut_pwm_e_duty;
extern float cut_pwm_f_duty;

extern int16_t cut_pwm_a_phase_deg;
extern int16_t cut_pwm_b_phase_deg;
extern int16_t cut_pwm_c_phase_deg;
extern int16_t cut_pwm_d_phase_deg;
extern int16_t cut_pwm_e_phase_deg;
extern int16_t cut_pwm_f_phase_deg;

extern uint16_t cut_pwm_a_dead_rise_ns;
extern uint16_t cut_pwm_b_dead_rise_ns;
extern uint16_t cut_pwm_c_dead_rise_ns;
extern uint16_t cut_pwm_d_dead_rise_ns;
extern uint16_t cut_pwm_e_dead_rise_ns;
extern uint16_t cut_pwm_f_dead_rise_ns;

extern uint16_t cut_pwm_a_dead_fall_ns;
extern uint16_t cut_pwm_b_dead_fall_ns;
extern uint16_t cut_pwm_c_dead_fall_ns;
extern uint16_t cut_pwm_d_dead_fall_ns;
extern uint16_t cut_pwm_e_dead_fall_ns;
extern uint16_t cut_pwm_f_dead_fall_ns;

void cut_pwm_apply(void);

#endif /* CUT_PWM_CONTROL_H */


