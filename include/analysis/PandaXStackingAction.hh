#ifndef PANDAXSTACKINGACTION_HH
#define PANDAXSTACKINGACTION_HH

#include "analysis/PandaXDataManager.hh"

#include <G4UserStackingAction.hh>

#include <memory>

using std::shared_ptr;

class G4Track;

class PandaXStackingAction : public G4UserStackingAction {
  public:
    PandaXStackingAction(shared_ptr<PandaXDataManager> dm);

    ~PandaXStackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *aTrack);

    virtual void NewStage();

    virtual void PrepareNewEvent();

    void setDataManager(PandaXDataManager *manager);

  private:
    shared_ptr<PandaXDataManager> _manager;
};

#endif // PANDAXSTACKINGACTION_HH
