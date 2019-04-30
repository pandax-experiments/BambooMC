#pragma once

#include <G4UserRunAction.hh>

class G4Run;

class PandaXOpticalRunAction : public G4UserRunAction {
  public:
    PandaXOpticalRunAction(const std::string &name = "mc_out.root")
        : outputName{name} {};

    virtual ~PandaXOpticalRunAction();

    virtual void BeginOfRunAction(const G4Run *aRun);

    virtual void EndOfRunAction(const G4Run *aRun);

  private:
    std::string outputName;
    int round = 0;
};
