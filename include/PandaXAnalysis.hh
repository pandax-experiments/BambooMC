#pragma once

#include "BambooAnalysis.hh"
#include "BambooFactory.hh"

class PandaXAnalysis : public BambooAnalysis {
  public:
    PandaXAnalysis(const BambooParameters &pars);

    ~PandaXAnalysis() = default;

    static AnalysisRegister<PandaXAnalysis> reg;
};

