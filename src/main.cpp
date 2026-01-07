#include <exception>
#include <iostream>
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <concepts>  

#include "gpio_port.hpp"
#include "gpio_pin.hpp"
#include "gpio.hpp"
#include "mcu_type.hpp" 

template<typename T>
void printBit(T value)
{
    int i = (sizeof(T) * 8) - 1; 
    for (int j = i; j >= 0; j--)
    {
        printf("%u", (value>>j) & 1u);
        if(j%4 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}

constexpr GPIO::Direction OUTPUT = GPIO::Direction::output;
constexpr GPIO::Direction INPUT  = GPIO::Direction::input;

constexpr GPIO::PinState HIGH = GPIO::PinState::high;
constexpr GPIO::PinState LOW  = GPIO::PinState::low;

int main()
{   
    volatile uint8_t DDRB = 0;
    volatile uint8_t PORTB = 0;
    volatile uint8_t PINB = 0;

    static const uint8_t pin3 = 3;
    GPIO_port<AVR> portB(DDRB, PORTB, PINB);
    GPIO_pin<AVR> GPIO_pin_3(portB, pin3);

    GPIO_pin_3.init();
    GPIO_pin_3.setDirection(OUTPUT);
    GPIO_pin_3.setPinState(HIGH);
    printf("AFTER OUTPUT, HIGH: ");
    printBit(DDRB);

    GPIO_pin_3.setPinState(LOW);
    printf("AFTER State->LOW on PORTB: ");
    printBit(PORTB);

}