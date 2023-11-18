#ifndef INPUT_H
#define INPUT_H

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware.h"

// buttons store their state next update, pots always store
namespace PicoSynth {
    class GPIOHandle {
        public:
            GPIOHandle();
            void Update();
            bool GetAnyFretPressed();
            bool GetFretPressed(uint32_t fret_mask);
            int GetFretState();
            bool GetJoystickButtonPressed();
            uint16_t GetJoystickXRaw();
            uint16_t GetJoystickYRaw();
            void SetDebugLED(bool status);
        private:
            uint16_t m_JoystickX = 0;
            uint16_t m_JoystickY = 0;
            uint32_t m_FretState = 0;
            bool m_JoystickButton = false;
    };
};

#endif