#pragma once 

template <typename T>
concept McuType = std::same_as<std::remove_cv_t<T>, uint8_t> || std::same_as<std::remove_cv_t<T>, uint32_t>;
// std::remove_cv_t jest po to aby akceptowalo volatile, const uint8_t, uint32_t i czyscilo


using ARM  = uint32_t;
using AVR =  uint8_t;
