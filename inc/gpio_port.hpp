#pragma once
#include <exception>
#include <iostream>
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <concepts>  
#include "mcu_type.hpp"

namespace ss{

    template<McuType T>
    class GPIO_port
    {
        using reg_t = std::remove_cv_t<T>;
        protected:
            volatile reg_t& DDRx;
            volatile reg_t& PORTx;
            volatile reg_t& PINx;

            static constexpr reg_t bitMask(reg_t bit)
            {
                return (reg_t)(1 << bit);
            }

        public:
            GPIO_port(volatile reg_t& ddr, volatile reg_t& port, volatile reg_t& pin) : DDRx(ddr), PORTx(port), PINx(pin) {};

            bool validateBit(reg_t bit) const 
            {
                if constexpr(std::is_same_v<reg_t, uint8_t>)
                {
                    if(bit > 7) 
                    {
                        throw std::out_of_range("Pin out of range!");
                        return false;
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
                        return false;
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
    };

}