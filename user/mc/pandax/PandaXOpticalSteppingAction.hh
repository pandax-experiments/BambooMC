#pragma once

#include <G4UserSteppingAction.hh>

#include <string>

class G4Step;

class PandaXOpticalSteppingAction : public G4UserSteppingAction {
  public:
    PandaXOpticalSteppingAction(const std::string &s) : sdName{s} {}

    ~PandaXOpticalSteppingAction() = default;

    virtual void UserSteppingAction(const G4Step *aStep);

  private:
    std::string sdName;
};
