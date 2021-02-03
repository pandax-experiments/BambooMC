#pragma once

#include "simple_factory.hh"

#include "detector/BambooDetector.hh"

template <typename... Ts>
using DetectorFactory = SimpleFactory<BambooDetector, std::string, Ts...>;

template <typename DerivedDetector, typename... Ts>
using DetectorRegister =
    Register<BambooDetector, std::string, DerivedDetector, Ts...>;
