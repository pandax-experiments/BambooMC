#ifndef BAMBOOANALYSISFACTORY_H
#define BAMBOOANALYSISFACTORY_H

#include "BambooAnalysis.hh"
#include "simple_factory.hh"

using BambooAnalysisFactory =
    SimpleFactory<BambooAnalysis, std::string, const G4String &>;

template <typename DerivedAnalysis>
using AnalysisRegister =
    Register<BambooAnalysis, std::string, DerivedAnalysis, const G4String &>;

#endif // BAMBOOANALYSISFACTORY_H
