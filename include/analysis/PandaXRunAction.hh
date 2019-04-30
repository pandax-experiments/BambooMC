#ifndef PANDAXRUNACTION_H
#define PANDAXRUNACTION_H

#include <G4UserRunAction.hh>

#include <memory>

using std::shared_ptr;

class G4Run;
class PandaXDataManager;

class PandaXRunAction : public G4UserRunAction {
  public:
    PandaXRunAction(shared_ptr<PandaXDataManager> manager);

    virtual ~PandaXRunAction();

    virtual void BeginOfRunAction(const G4Run *aRun);

    virtual void EndOfRunAction(const G4Run *aRun);

  private:
    shared_ptr<PandaXDataManager> _dataManager;
};

#endif // PANDAXRUNACTION_H
