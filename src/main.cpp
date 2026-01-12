#include <iostream>
#include <cstdint>
#include "gpio.hpp"
#include "gpio_port.hpp"
#include "gpio_pin.hpp"

void demonstrateAVR()
{
    std::cout << " ------- AVR PORTS SIMULATION -------";
    volatile ss::AVR DDRB  = 0;
    volatile ss::AVR PORTB = 0;
    volatile ss::AVR PINB  = 0;

    ss::GPIO_port<ss::AVR> portB(DDRB, PORTB, PINB);

    ss::GPIO_pin<ss::AVR> led_pin(portB, 3);
    ss::GPIO_pin<ss::AVR> button_pin(portB, 2);

    led_pin.init();
    button_pin.init();
    printRegisters<ss::AVR>(portB);

    led_pin.setDirection(ss::OUTPUT);
    printRegisters<ss::AVR>(portB);

    button_pin.setPinMode(ss::INPUT_PULLUP_MODE);
    printRegisters<ss::AVR>(portB);
}

int main()
{
    demonstrateAVR();
}