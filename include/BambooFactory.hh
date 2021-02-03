#pragma once

#include "simple_factory.hh"

#include "detector/BambooDetector.hh"
#include "detector/BambooMaterial.hh"

#include "physics/BambooPhysics.hh"
#include "generator/BambooGenerator.hh"

template <typename... Ts>
using DetectorFactory = SimpleFactory<BambooDetector, std::string, Ts...>;

template <typename DerivedDetector, typename... Ts>
using DetectorRegister =
    Register<BambooDetector, std::string, DerivedDetector, Ts...>;

using MaterialFactory = SimpleFactory<BambooMaterial, std::string>;
template <typename DerivedMaterial>
using MaterialRegister =
    Register<BambooMaterial, std::string, DerivedMaterial>;

using PhysicsFactory = SimpleFactory<BambooPhysics, std::string, BambooParameters>;
template <typename DerivedPhysics>
using PhysicsRegister =
    Register<BambooPhysics, std::string, DerivedPhysics, BambooPhysics>;

using GeneratorFactory = SimpleFactory<BambooGenerator, std::string, BambooParameters>;
template <typename DerivedGenerator>
using GeneratorRegister =
    Register<BambooGenerator, std::string, DerivedGenerator, BambooGenerator>;
