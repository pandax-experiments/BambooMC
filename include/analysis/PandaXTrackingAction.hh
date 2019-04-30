#ifndef PANDAXTRACKINGACTION_HH
#define PANDAXTRACKINGACTION_HH

#include "analysis/PandaXDataManager.hh"

#include <G4UserTrackingAction.hh>

#include <memory>

using std::shared_ptr;

class G4Track;

class PandaXTrackingAction : public G4UserTrackingAction {
  public:
    PandaXTrackingAction(shared_ptr<PandaXDataManager> dm);

    ~PandaXTrackingAction();

    virtual void PreUserTrackingAction(const G4Track *aTrack);
    virtual void PostUserTrackingAction(const G4Track *aTrack);

    void setDataManager(PandaXDataManager *manager);

  private:
    shared_ptr<PandaXDataManager> _manager;
};

#endif // PANDAXTRACKINGACTION_HH
