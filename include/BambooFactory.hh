#pragma once

#include "simple_factory.hh"

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooMaterial.hh"

#include "BambooPhysics.hh"
#include "BambooGenerator.hh"
#include "BambooAnalysis.hh"

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
    Register<BambooPhysics, std::string, DerivedPhysics, BambooParameters>;

using GeneratorFactory = SimpleFactory<BambooGenerator, std::string, BambooParameters>;
template <typename DerivedGenerator>
using GeneratorRegister =
    Register<BambooGenerator, std::string, DerivedGenerator, BambooParameters>;

using AnalysisFactory = SimpleFactory<BambooAnalysis, std::string, BambooParameters>;
template <typename DerivedAnalysis>
using AnalysisRegister =
    Register<BambooAnalysis, std::string, DerivedAnalysis, BambooParameters>;
