#pragma once

#include "BambooFactory.hh"
#include "BambooAnalysis.hh"

class PandaXOpticalAnalysis : public BambooAnalysis {
  public:
    PandaXOpticalAnalysis(const BambooParameters &pars);

    ~PandaXOpticalAnalysis() = default;

    static AnalysisRegister<PandaXOpticalAnalysis> reg;
};
