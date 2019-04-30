#ifndef PANDAXANALYSIS_H
#define PANDAXANALYSIS_H

#include "analysis/BambooAnalysis.hh"
#include "analysis/PandaXDataManager.hh"

#include "analysis/BambooAnalysisFactory.hh"

#include <memory>

using std::shared_ptr;

class PandaXAnalysis : public BambooAnalysis {
  public:
    PandaXAnalysis(const G4String &name);

    ~PandaXAnalysis();

    static AnalysisRegister<PandaXAnalysis> reg;

  private:
    shared_ptr<PandaXDataManager> dm;
};

#endif // PANDAXANALYSIS_H
