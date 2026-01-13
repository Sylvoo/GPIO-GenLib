/**
 * @file gpio_port.hpp
 * @brief GPIO port backend operating on registers.
 */
#pragma once
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <bitset>

#include "mcu_type.hpp"

namespace ss{

    /**
     * @brief GPIO port for DDR/PORT/PIN registers.
     *
     * @tparam T MCU register type constrained by @ref ss::McuType.
     */
    template<McuType T>
    class GPIO_port
    {
        using reg_t = std::remove_cv_t<T>; /**< Register type without cv-qualifiers. */

        protected:
            volatile reg_t& DDRx;   /**< Data Direction register. */
            volatile reg_t& PORTx;  /**< Output / pull-up control register. */
            volatile reg_t& PINx;   /**< Input register. */

            static constexpr reg_t bitMask(reg_t bit)
            {
                return (reg_t)(1 << bit);
            }

        public:
            /**
             * @brief Construct a port registers.
             *
             * @param ddr  Direction register reference.
             * @param port Output register reference.
             * @param pin  Input register reference.
             */
            GPIO_port(volatile reg_t& ddr, volatile reg_t& port, volatile reg_t& pin) : DDRx(ddr), PORTx(port), PINx(pin) {};


            /**
             * @brief Validate bit index for the underlying register width.
             * @param bit Bit index.
             * @return true if valid.
             * @throws std::out_of_range If bit is out of range.
             * @throws std::runtime_error If register type is unsupported.
             */
            bool validateBit(reg_t bit) const 
            {
                if constexpr(std::is_same_v<reg_t, uint8_t>)
                {
                    if(bit > 7) 
                    {
                        throw std::out_of_range("Pin out of range!");
                    }
                    else
                    {
                        return true;
                    } 
                }
                else if constexpr (std::is_same_v<reg_t, uint32_t>)
                {
                    if(bit > 31) 
                    {
                        throw std::out_of_range("Pin out of range!");
                    }
                    else
                    {
                        return true;
                    } 
                }
                else 
                {
                    throw std::runtime_error("Wrong type of registers");
                    return false;
                }
            }

        
        /**
         * @brief Set pin direction.
         * @param bit Bit index.
         * @param is_output true for output, false for input.
         * @throws std::invalid_argument If bit is invalid.
         */
        void setDirection(reg_t bit, bool is_output)
        {
            if(validateBit(bit))
            {
                if(is_output)
                {
                    DDRx |= (reg_t)bitMask(bit);
                    PORTx &= (reg_t)~bitMask(bit);
                    PINx &= (reg_t)~bitMask(bit);
                }
                else
                {
                    DDRx &= (reg_t)(~bitMask(bit));
                    PORTx |= (reg_t)bitMask(bit);
                    PINx |= (reg_t)bitMask(bit);
                }
            }
            else 
            {
                throw std:: invalid_argument("Cannot set Direction");
            }
        }


        /**
         * @brief Set output state of a bit.
         * @param bit Bit index.
         * @param to_high true for high, false for low.
         * @throws std::invalid_argument If bit is invalid.
         */
        void setBit(reg_t bit, bool to_high)
        {
            if(validateBit(bit))
            {
                if(to_high)
                {
                    PORTx |= (reg_t)bitMask(bit);
                    PINx |= (reg_t)bitMask(bit);
                }
                else
                {
                    PORTx &= (reg_t)~(bitMask(bit));
                    PINx &= (reg_t)~(bitMask(bit));
                }
            }
            else 
            {
                throw std:: invalid_argument("Cannot do a setBit() operation ");
            }
        }

        /**
         * @brief Read input state of a bit.
         * @param bit Bit index.
         * @return true if high, false if low.
         * @throws std::invalid_argument If bit is invalid.
         */
        bool readBit(const reg_t bit) const
        {
            if(validateBit(bit))
            {
                return ((PINx & (reg_t)bitMask(bit)) != 0);
            }
            else 
            {
                throw std:: invalid_argument("Cannot do read() operation");
            }
        }


        /**
         * @brief Enable/disable pull-up for a bit.
         * @param bit Bit index.
         * @param is_pullUp true to enable pull-up, false to disable.
         */
        void pullUpBit(reg_t bit, bool is_pullUp)
        {
            if(validateBit(bit))
            {
                if(is_pullUp)
                {
                    setDirection(bit, false);
                    PORTx |= (reg_t)bitMask(bit);
                    PINx |= (reg_t)bitMask(bit);
                }   
                else
                {
                    PORTx &= (reg_t)~(bitMask(bit));
                    PINx &= (reg_t)~(bitMask(bit));
                }
            }
        }

        /**
         * @brief Grant access to GPIO_port internals for printRegister function.
         *
         * @details
         * Declares @ref printRegisters as a friend, allowing it to access
         * protected register fields for diagnostic purposes.
         */
        friend void printRegisters<reg_t>(const GPIO_port<reg_t>& port);
    };

    /**
     * @brief Print raw GPIO port register values.
     *
     * @tparam T MCU register type.
     * @param port GPIO port instance whose registers are printed.
     *
     * @details
     * Intended for debugging and diagnostics only.
     * Prints DDRx, PORTx and PINx as fixed-width binary values using @c std::bitset.
     */
    template <typename T>
    void printRegisters(const GPIO_port<T>& port)
    {
        using reg_t = std::remove_cv_t<T>;
        constexpr std::size_t width = sizeof(reg_t) * 8;

        std::cout << "DDRx="  << std::bitset<width>(port.DDRx)
                << " PORTx=" << std::bitset<width>(port.PORTx)
                << " PINx="  << std::bitset<width>(port.PINx)
                << "\n";
    }
} // namespace ss