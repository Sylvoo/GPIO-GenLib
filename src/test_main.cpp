#include <stdexcept>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "gpio_port.hpp"
#include "gpio_pin.hpp"
#include "mcu_type.hpp"

TEST_CASE("ConceptTest: McuType")
{
    static_assert(McuType<uint8_t>);
    static_assert(McuType<uint32_t>);
    static_assert(McuType<AVR>);
    static_assert(McuType<ARM>);
    static_assert(!McuType<uint16_t>);
    static_assert(McuType<const volatile AVR>);
}

TEST_CASE("GPIO_port<AVR>: validate bit test")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);

    CHECK(portB.validateBit(0));
    CHECK(portB.validateBit(7));
    CHECK_THROWS_AS(portB.validateBit(-1), std::out_of_range);
    CHECK_THROWS_AS(portB.validateBit(10), std::out_of_range);
}

TEST_CASE("GPIO_port<ARM>: validate bit test")
{
    volatile ARM ddr=0, port=0, pin=0;
    GPIO_port<ARM> portB(ddr, port, pin);

    CHECK(portB.validateBit(0));
    CHECK(portB.validateBit(31));
    CHECK_THROWS_AS(portB.validateBit(-1), std::out_of_range);
    CHECK_THROWS_AS(portB.validateBit(33), std::out_of_range);
}

TEST_CASE("GPIO_port<AVR>: setDirection test")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinToSetDirection = 1;
    portB.setDirection(pinToSetDirection, true); // output
    CHECK((ddr & (1<<pinToSetDirection)) != 0);

    portB.setDirection(pinToSetDirection, false);
    CHECK((ddr & (1<<pinToSetDirection)) == 0);
}


TEST_CASE("GPIO_port<ARM>: serDirection test")
{
    volatile ARM ddr=0, port=0, pin=0;
    GPIO_port<ARM> portB(ddr, port, pin);
    AVR pinToSetDirection = 31;
    portB.setDirection(pinToSetDirection, true); // output
    CHECK((ddr & (1<<pinToSetDirection)) != 0);

    portB.setDirection(pinToSetDirection, false);
    CHECK((ddr & (1<<pinToSetDirection)) == 0);
}

TEST_CASE("GPIO_port<AVR>: setBit test")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinToSetBit = 2;
    portB.setBit(pinToSetBit, true); // high
    CHECK((port & (1<<pinToSetBit)) != 0);

    portB.setBit(pinToSetBit, false);
    CHECK((port & (1<<pinToSetBit)) == 0);
}

TEST_CASE("GPIO_port<AVR>: readBit test")
{
    volatile AVR ddr=0, port=0, pin=0x01;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR bitToRead_1 = 0;
    AVR bitToRead_2 = 7;
    AVR bitToRead_3 = 10;
    CHECK(portB.readBit(bitToRead_1));
    CHECK(portB.readBit(!bitToRead_2));
    CHECK_THROWS_AS(portB.readBit(bitToRead_3), std::out_of_range);
}

TEST_CASE("GPIO_port<AVR>: PullUpBit test")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR bitToPullUp = 1;
    portB.pullUpBit(bitToPullUp, true);
    CHECK((ddr & (1<<bitToPullUp)) == 0);
    CHECK((port & (1<<bitToPullUp)) != 0);
    
    portB.pullUpBit(bitToPullUp, false);
    CHECK(((port & (1<<bitToPullUp)) == 0));

    AVR bitToPullUp_2 = 30;
    CHECK_THROWS_AS(portB.pullUpBit(bitToPullUp_2, true), std::out_of_range);
}

TEST_CASE("GPIO_port<AVR>: pullUpBit")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinNumber = 3;
    GPIO_pin<AVR> pin_3(portB, pinNumber);

    portB.pullUpBit(pinNumber, true);
    CHECK((ddr & (1<<pinNumber)) == 0);
    CHECK((port & (1<<pinNumber)) != 0);
    CHECK((pin & (1<<pinNumber)) != 0);

    portB.pullUpBit(pinNumber, false);
    CHECK((port & (1<<pinNumber)) == 0);
    CHECK((pin & (1<<pinNumber)) == 0);
}

TEST_CASE("GPIO_pin<AVR>: setDirection")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinNumber = 3;
    GPIO_pin<AVR> pin_3(portB, pinNumber);

    pin_3.setDirection(OUTPUT);
    CHECK((ddr & (1<<pinNumber)) != 0);

}

TEST_CASE("GPIO_pin<AVR>: setPinState")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinNumber = 3;
    GPIO_pin<AVR> pin_3(portB, pinNumber);

    pin_3.setPinState(HIGH);
    CHECK((port & (1<<pinNumber)) != 0);

    pin_3.setPinState(LOW);
    CHECK((port & (1<<pinNumber)) == 0);
}

TEST_CASE("GPIO_pin<AVR>: setPinMode")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinNumber = 3;
    GPIO_pin<AVR> pin_3(portB, pinNumber);

    pin_3.setPinMode(INPUT_MODE);
    CHECK((ddr & (1<<pinNumber)) == 0);

    pin_3.setPinMode(OUTPUT_MODE);
    CHECK((ddr & (1<<pinNumber)) != 0);

    pin_3.setPinMode(INPUT_PULLUP_MODE);
    CHECK((ddr & (1<<pinNumber)) == 0);
    CHECK((port & (1<<pinNumber)) != 0);
}

TEST_CASE("GPIO_pin<AVR>: setPullMode")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinNumber = 3;
    GPIO_pin<AVR> pin_3(portB, pinNumber);

    pin_3.setPullMode(PULL_UP);
    CHECK((ddr & (1<<pinNumber)) == 0);
    CHECK((port & (1<<pinNumber)) != 0);

    pin_3.setPullMode(NO_PULL);
    CHECK((port & (1<<pinNumber)) == 0);
}

TEST_CASE("GPIO_pin<AVR>: read")
{
    volatile AVR ddr=0, port=0, pin=0;
    GPIO_port<AVR> portB(ddr, port, pin);
    AVR pinNumber = 3;
    GPIO_pin<AVR> pin_3(portB, pinNumber);

    bool state = pin_3.read();
    CHECK(state == false);

    pin_3.setPinState(HIGH);
    state = pin_3.read();
    CHECK(state == true);
}





