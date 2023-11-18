#include "waves.hpp"

namespace PicoSynth {
    Wave::Wave(GPIOHandle *p_InputHandler) {
        this->m_InputHandler = p_InputHandler;
    }

    void Wave::Initialize() {
        this->m_WaveStart = time_us_64();
        this->m_Multiplier = 8;
        this->m_WaveType = SawWave;
        this->m_MinimumRead = 0;
        // Setup GPIO.
        this->m_SliceIndex = pwm_gpio_to_slice_num(AUDIO_OUT);
        gpio_set_function(AUDIO_OUT, GPIO_FUNC_PWM);
        pwm_set_wrap(this->m_SliceIndex, ADC_MAX - 1);
        pwm_set_enabled(this->m_SliceIndex, true);
    }

/*
double get_level(Wave *wv, double newhz) {
    newhz *= wv->mult * wv->mod;
    // effective_time stores
    double effective_time = (newhz * (wv->time - wv->start) / 1000000);
    effective_time -= (int) effective_time;
    switch (wv->type) {
        case SineWave: {
            double c = cos(M_PI * effective_time);
            return c * c;
        }
        case SquareWave:
            return effective_time < 0.5f;
        case SawWave:
            return effective_time;
        case TriangleWave:
            return 2 * fabs(0.5f - effective_time);
        default:
            // error
            SET_LED_ON();
            break;
    };
    return 0;
}
*/

    // Returns a value between 0 and 1.0 that denotes the amp of the wave
    double Wave::CalculateLevel(double new_frequency, double modulation) {
        // effective_time stores
        double c_NewFrequency = new_frequency * modulation * this->m_Multiplier;
        double c_EffectiveTime = c_NewFrequency * (this->m_WaveTime - this->m_WaveStart) / 1000000;
        c_EffectiveTime -= (int) c_EffectiveTime;
        switch (this->m_WaveType) {
            case SineWave: {
                double c = cos(M_PI * c_EffectiveTime);
                return c * c;
            }
            case SquareWave:
                return c_EffectiveTime < 0.5f;
            case SawWave:
                return c_EffectiveTime;
            case TriangleWave:
                return 2 * fabs(0.5f - c_EffectiveTime);
            default:
                break;
        };
        return 0;
    }

    void Wave::Update() {
        if (this->m_InputHandler->GetJoystickButtonPressed()) {
            this->m_WaveType = (this->m_WaveType + 1) % 4;
        }
        double level = 0;
        float modulation_factor = 1.0f;
        this->m_WaveTime = time_us_64();
        if (this->m_WaveTime - this->m_MinimumRead > 100000) {
            uint16_t val = this->m_InputHandler->GetJoystickYRaw();
            printf("%llu %hu %f\n", this->m_MinimumRead - this->m_WaveTime, val, exp2f((val - 2048.0f) / 2048.0f));
            modulation_factor = exp2f((val - 2048.0f) / 2048.0f);
            this->m_MinimumRead = this->m_WaveTime;
        }   
        if (this->m_InputHandler->GetAnyFretPressed()) {
            printf("%f\n", modulation_factor);
            this->m_InputHandler->SetDebugLED(true);
            if (this->m_InputHandler->GetFretPressed(1 << 5))
                level += CalculateLevel(FREQ_C, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 3))
                level += CalculateLevel(FREQ_CS, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 12))
                level += CalculateLevel(FREQ_D, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 2))
                level += CalculateLevel(FREQ_DS, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 10))
                level += CalculateLevel(FREQ_E, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 8))
                level += CalculateLevel(FREQ_F, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 13))
                level += CalculateLevel(FREQ_FS, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 9))
                level += CalculateLevel(FREQ_G, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 7))
                level += CalculateLevel(FREQ_GS, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 11))
                level += CalculateLevel(FREQ_A, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 6))
                level += CalculateLevel(FREQ_AS, modulation_factor);
            if (this->m_InputHandler->GetFretPressed(1 << 4))
                level += CalculateLevel(FREQ_B, modulation_factor);
            level /= CHANNELS;
            pwm_set_chan_level(this->m_SliceIndex, PWM_CHAN_A, (int)(level * ADC_MAX));
        } else {
            this->m_InputHandler->SetDebugLED(false);
        }
        return;
    }
};