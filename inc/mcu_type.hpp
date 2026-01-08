#pragma once 
#include "gpio.hpp"

template <typename T>
concept McuType = std::same_as<std::remove_cv_t<T>, uint8_t> || std::same_as<std::remove_cv_t<T>, uint32_t>;
// std::remove_cv_t jest po to aby akceptowalo volatile, const uint8_t, uint32_t i czyscilo


using ARM  = uint32_t;
using AVR =  uint8_t;


constexpr GPIO::Direction OUTPUT = GPIO::Direction::output;
constexpr GPIO::Direction INPUT  = GPIO::Direction::input;

constexpr GPIO::PinState HIGH = GPIO::PinState::high;
constexpr GPIO::PinState LOW  = GPIO::PinState::low;

constexpr GPIO::PinMode INPUT_PULLUP = GPIO::PinMode::inputPullUp;

constexpr GPIO::PullMode PULL_UP  = GPIO::PullMode::pull;
constexpr GPIO::PullMode NO_PULL  = GPIO::PullMode::noPull;
