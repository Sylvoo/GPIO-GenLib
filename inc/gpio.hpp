/**
 * @file GPIO.hpp
 * @brief Generic GPIO abstraction (interface) for platform-specific implementations.
 *
 * @details
 * This header declares the GPIO interface used to control a single digital I/O pin.
 */
#pragma once

namespace ss{

    /**
     * @class GPIO
     * @brief Abstract interface representing a single GPIO pin.
     *
     * @warning This is an interface. Methods are pure virtual and must be implemented by a derived class.
     */

    class GPIO
    {
        public:
            /**
             * @enum Direction
             * @brief Logical pin direction setting.
             */
            enum class Direction
            {
                input,  /**< Configure pin as input. */
                output  /**< Configure pin as output. */
            };

            /**
             * @enum PinState
             * @brief Digital output/input logic levels.
             */
            enum class PinState
            {
                low = 0,    /**< Logic low. */
                high = 1    /**< Logic high. */
            };

            /**
             * @enum PinMode
             * @brief High-level pin configuration modes.
             * @details
             * Mode values are intended to describe common configurations used on typical MCUs. 
             */
            enum class PinMode
            {
                input,        /**< Input without pull configuration. */
                inputPullUp,  /**< Input with internal pull-up enabled */
                output        /**< Output mode. */
            };

            /**
             * @enum PullMode
             * @brief Pull resistor configuration for input pins.
             */
            enum class PullMode
            {
                pull,   /**< Enable pull resistor. */
                noPull, /**< Disable pull resistors. */
                // pullDown
            };

            /**
             * @brief Virtual destructor.
             *
             * @details
             * Ensures proper cleanup through a base-class pointer.
             */
            virtual ~GPIO(){};
            
            /**
             * @brief Initialize default configuration of registers.
             *
             * @details
             * Default initializes as INPUT with NOPULLUP
             */
            virtual void init() = 0;

            /**
             * @brief Set the GPIO direction.
             *
             * @param direction Desired direction (INPUT or OUTPUT).
             */
            virtual void setDirection(Direction direction) = 0;

            /**
             * @brief Drive the pin to the given logic level.
             *
             * @param state Desired logic level (HIGH or LOW).
             */
            virtual void setPinState(PinState state) = 0;

            /**
             * @brief Set the pin to the given Mode.
             *
             * @param state Desired pin mode (INPUT_PULLUP_MODE, INPUT_MODE or OUTPUT_MODE).
             */
            virtual void setPinMode(PinMode state) = 0;

            /**
             * @brief Set the pin to the given Pull Mode.
             *
             * @param state Desired pull mode (PULL_UP or NO_PULL).
             */
            virtual void setPullMode(PullMode state) = 0;

            /**
             * @brief Read the current digital level on the pin.
             *
             * @return true if the pin is at logic high; false if at logic low.
             */
            virtual bool read() const = 0;
    };

}