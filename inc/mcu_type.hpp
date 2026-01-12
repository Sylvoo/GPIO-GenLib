/**
 * @file mcu_type.hpp
 * @brief MCU type constraints and GPIO definitions.
 *
 * @details
 * Defines the @ref ss::McuType concept for supported MCU register-width types.
 */
#pragma once
#include <cstdint>
#include <type_traits>
#include <concepts>

#include "gpio.hpp"

namespace ss {

/**
 * @concept McuType
 * @brief Compile-time constraint for supported MCU register-width types.
 *
 * @details
 * Accepts @c uint8_t and @c uint32_t (after removing @c const / @c volatile qualifiers),
 *
 * @tparam T Candidate type to validate.
 */
template <typename T>
concept McuType =
    std::same_as<std::remove_cv_t<T>, uint8_t> ||
    std::same_as<std::remove_cv_t<T>, uint32_t>;


/** @brief alias representing 32-bit MCU register width (typical for ARM). */
using ARM = uint32_t;

/** @brief alias representing 8-bit MCU register width (typical for AVR). */
using AVR = uint8_t;

/** @brief GPIO direction constant mapping to @ref GPIO::Direction::output. */
constexpr GPIO::Direction OUTPUT = GPIO::Direction::output;

/** @brief GPIO direction constant mapping to @ref GPIO::Direction::input. */
constexpr GPIO::Direction INPUT  = GPIO::Direction::input;


/** @brief GPIO state constant mapping to @ref GPIO::PinState::high. */
constexpr GPIO::PinState HIGH = GPIO::PinState::high;

/** @brief GPIO state constant mapping to @ref GPIO::PinState::low. */
constexpr GPIO::PinState LOW  = GPIO::PinState::low;


/** @brief GPIO mode constant mapping to @ref GPIO::PinMode::inputPullUp. */
constexpr GPIO::PinMode INPUT_PULLUP_MODE = GPIO::PinMode::inputPullUp;

/** @brief GPIO mode constant mapping to @ref GPIO::PinMode::input. */
constexpr GPIO::PinMode INPUT_MODE = GPIO::PinMode::input;

/** @brief GPIO mode constant mapping to @ref GPIO::PinMode::output. */
constexpr GPIO::PinMode OUTPUT_MODE = GPIO::PinMode::output;


/** @brief GPIO pull configuration constant mapping to @ref GPIO::PullMode::pull. */
constexpr GPIO::PullMode PULL_UP = GPIO::PullMode::pull;

/** @brief GPIO pull configuration constant mapping to @ref GPIO::PullMode::noPull. */
constexpr GPIO::PullMode NO_PULL = GPIO::PullMode::noPull;

} // namespace ss