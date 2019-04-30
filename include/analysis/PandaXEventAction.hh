#ifndef PANDAXEVENTACTION_H
#define PANDAXEVENTACTION_H

#include <G4UserEventAction.hh>

#include <memory>

using std::shared_ptr;

class G4Event;
class PandaXDataManager;

class PandaXEventAction : public G4UserEventAction {
  public:
    PandaXEventAction(shared_ptr<PandaXDataManager> manager);

    virtual ~PandaXEventAction();

    virtual void BeginOfEventAction(const G4Event *aEvent);

    virtual void EndOfEventAction(const G4Event *aEvent);

    void setUserSeed(long seed);

  private:
    shared_ptr<PandaXDataManager> _dataManager;
    long user_seed = -1;
};

inline void PandaXEventAction::setUserSeed(long seed) { user_seed = seed; }
#endif // PANDAXEVENTACTION_H
