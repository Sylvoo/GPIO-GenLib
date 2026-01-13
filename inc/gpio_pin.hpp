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

#include <type_traits>
#include <stdexcept>

#include "mcu_type.hpp"
#include "gpio.hpp"
#include "gpio_port.hpp"


namespace ss{
    /**
     * @brief GPIO implementation representing a single pin of a GPIO_port.
     *
     * @tparam T MCU register type constrained by @ref ss::McuType.
     */
    template <McuType T>
    class GPIO_pin : public GPIO
    {
        using reg_t = std::remove_cv_t<T>;     /**< Register type without cv-qualifiers. */
        GPIO_port<reg_t>& port;                /**< Underlying GPIO port. */
        const reg_t bit;                       /**< Bit index within the port. */


        /**
         * @brief Construct a GPIO pin bound to a port and bit index.
         *
         * @param portx GPIO_port reference.
         * @param pbit  Bit index within the port.
         *
         * @throws std::invalid_argument If the bit index is invalid.
         */
        public:
        GPIO_pin(GPIO_port<reg_t>& portx, reg_t pbit) : port(portx), bit(pbit)
        {
            if (!port.validateBit(pbit)) {
                throw std::invalid_argument("Pin number out of range");
            }
        };

        /** @brief Set pin direction. */
        void setDirection(const Direction direction) override
        {
        port.setDirection(bit, direction == Direction::output); // true jesli output false jesli input
        }

        /** @brief Set pin state. */
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

        /** @brief Configure pin mode. */
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

        /** @brief Configure pull resistor mode. */
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
                    throw std::invalid_argument("wanted invalid pull mode of a pin"); 
                    break;
            }
        }
        
        /** @brief Read current pin logic level. */
        bool read() const override
        {
            return port.readBit(bit);
        }

        /**
         * @brief Initialize pin to default safe state.
         *
         * @details Configures pin as input with pull disabled.
         */
        void init() override
        {
            setDirection(Direction::input);
            setPullMode(PullMode::noPull); 
        }
    };

} // namespace ss