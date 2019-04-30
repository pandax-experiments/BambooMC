#ifndef BAMBOODETECTORFACTORY_H
#define BAMBOODETECTORFACTORY_H

#include "BambooDetectorPart.hh"

#include "simple_factory.hh"

using BambooDetectorFactory =
    SimpleFactory<BambooDetectorPart, std::string, const G4String &>;

template <typename DerivedDetector>
using DetectorRegister = Register<BambooDetectorPart, std::string,
                                  DerivedDetector, const G4String &>;

#endif // BAMBOODETECTORFACTORY_H
