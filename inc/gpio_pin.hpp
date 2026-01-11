/**
 * @file gpio_pin.hpp
 * @brief Template-based GPIO pin implementation.
 *
 * @details
 * This header defines @ref ss::GPIO_pin, a concrete implementation of the abstract @ref GPIO
 * interface. The class binds a single pin (bit index) to a @ref ss::GPIO_port instance and
 * translates high-level GPIO operations (direction/state/mode/pull/read) into port-level
 * register operations.
 *
 * The template parameter is constrained by @ref ss::McuType to ensure a supported MCU
 * register-width type (e.g. 8-bit AVR or 32-bit ARM style register type).
 *
 * @note The constructor validates the provided bit index via @c GPIO_port::validateBit().
 *       If invalid, it throws @c std::invalid_argument.
 */
#pragma once

#include <exception>
#include <iostream>
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <concepts>  

#include "mcu_type.hpp"
#include "gpio.hpp"
#include "gpio_port.hpp"


namespace ss{

    template <McuType T>
    class GPIO_pin : public GPIO
    {
        using reg_t = std::remove_cv_t<T>;
        GPIO_port<reg_t>& port;
        const reg_t bit;

        public:
        GPIO_pin(GPIO_port<reg_t>& portx, reg_t pbit) : port(portx), bit(pbit)
        {
            if (!port.validateBit(pbit)) {
                throw std::invalid_argument("Pin number out of range");
            }
        };

        void setDirection(const Direction direction) override
        {
        port.setDirection(bit, direction == Direction::output); // true jesli output false jesli input
        }

        void setPinState(const PinState state) override
        {
            switch(state)
            {
                case PinState::high:
                    port.setBit(bit, true);
                    break;
                case PinState::low:
                    port.setBit(bit, false);
                    break;
                default:
                    throw std::invalid_argument("wanted invalid state of a pin"); // usseles raczej ale dla picu jest (na razie)
                    break;
            }
        }

        void setPinMode(const PinMode state) override
        {
            switch(state)
            {
                case PinMode::input:
                    port.setDirection(bit, false);
                    break;
                case PinMode::output:
                    port.setDirection(bit, true);
                    break;
                case PinMode::inputPullUp: 
                    port.pullUpBit(bit, true);
                    break;
                default:
                    throw std::invalid_argument("wanted invalid mode of a pin"); // usseles raczej ale dla picu jest (na razie)
                    break;
            }
        }

        void setPullMode(const PullMode state) override
        {
            switch(state)
            {
                case PullMode::noPull:
                    port.pullUpBit(bit, false);
                    break;
                case PullMode::pull:
                    port.pullUpBit(bit, true);
                    break;
                default:
                    throw std::invalid_argument("wanted invalid pull mode of a pin"); // usseles raczej ale dla picu jest (na razie)
                    break;
            }
        }
        
        bool read() const override
        {
            return port.readBit(bit);
        }

        void init() override
        {
            setDirection(Direction::input); // domyslnie 0
            setPullMode(PullMode::noPull); // domyslnie pullUp wylaczony 
        }
    };

}