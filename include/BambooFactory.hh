#pragma once

#include "simple_factory.hh"

#include "detector/BambooDetector.hh"
#include "detector/BambooMaterial.hh"

template <typename... Ts>
using DetectorFactory = SimpleFactory<BambooDetector, std::string, Ts...>;

template <typename DerivedDetector, typename... Ts>
using DetectorRegister =
    Register<BambooDetector, std::string, DerivedDetector, Ts...>;

using MaterialFactory = SimpleFactory<BambooMaterial, std::string>;
template <typename DerivedMaterial>
using MaterialRegister =
    Register<BambooMaterial, std::string, DerivedMaterial>;
