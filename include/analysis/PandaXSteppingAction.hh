#ifndef PANDAXSTEPPINGACTION_HH
#define PANDAXSTEPPINGACTION_HH

#include "analysis/PandaXDataManager.hh"

#include <G4UserSteppingAction.hh>

#include <memory>

using std::shared_ptr;

class G4Step;

class PandaXSteppingAction : public G4UserSteppingAction {
  public:
    PandaXSteppingAction(shared_ptr<PandaXDataManager> dm);

    ~PandaXSteppingAction();

    virtual void UserSteppingAction(const G4Step *aStep);

    void setDataManager(PandaXDataManager *manager);

  private:
    shared_ptr<PandaXDataManager> _manager;
};

#endif // PANDAXSTEPPINGACTION_HH
