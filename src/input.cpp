#include "input.hpp"
#include "hardware.h"

namespace PicoSynth {
    GPIOHandle::GPIOHandle() {
        // Joystick init.
        adc_init();
        adc_gpio_init(JSX_GPIO);
        adc_gpio_init(JSY_GPIO);
        gpio_init(JSB_GPIO);

        // Fret init.
        for (int i = 2; i <= 13; ++i) {
            gpio_init(i);
        }

        // LED init.
        gpio_init(LED_PIN);
    }

    void GPIOHandle::Update() {
        // Joystick Button
        this->m_JoystickButton = gpio_get(JSB_GPIO);

        // Joystick X
        adc_select_input(JSX_ADC);
        this->m_JoystickX = adc_read();
    
        // Frets
        this->m_FretState = 0;
        for (int i = 2; i <= 13; ++i) {
            this->m_FretState |= gpio_get(i) << i;
        }
        this->m_FretState ^= 1 << 12;
    }

    bool GPIOHandle::GetAnyFretPressed() {
        return this->m_FretState;
    }

    int GPIOHandle::GetFretState() {
        return this->m_FretState;
    }
    
    bool GPIOHandle::GetJoystickButtonPressed() {
        return this->m_JoystickButton;
    }

    bool GPIOHandle::GetFretPressed(uint32_t fret_mask) {
        return this->m_FretState & fret_mask;
    }

    uint16_t GPIOHandle::GetJoystickXRaw() {
        adc_select_input(JSX_ADC);
        return adc_read();
    }

    uint16_t GPIOHandle::GetJoystickYRaw() {
        // Joystick Y
        adc_select_input(JSY_ADC);
        return adc_read();
    }

    void GPIOHandle::SetDebugLED(bool status) {
        gpio_put(LED_PIN, status);
    }
};