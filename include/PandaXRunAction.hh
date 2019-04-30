#pragma once

#include <G4UserRunAction.hh>

class G4Run;

class PandaXRunAction : public G4UserRunAction {
  public:
    PandaXRunAction(const std::string &name = "mc_out.root")
        : outputName{name} {};

    virtual ~PandaXRunAction();

    virtual void BeginOfRunAction(const G4Run *aRun);

    virtual void EndOfRunAction(const G4Run *aRun);

  private:
    std::string outputName;
    int round = 0;
};
