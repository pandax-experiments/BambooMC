#include "analysis/PandaXStackingAction.hh"
#include <G4EventManager.hh>
#include <G4StackManager.hh>
#include <G4Track.hh>
#include <G4Version.hh>

PandaXStackingAction::PandaXStackingAction(shared_ptr<PandaXDataManager> dm)
    : _manager{dm} {}

PandaXStackingAction::~PandaXStackingAction() {}

G4ClassificationOfNewTrack
PandaXStackingAction::ClassifyNewTrack(const G4Track *aTrack) {
    const G4ParticleDefinition *def = aTrack->GetParticleDefinition();
#if G4VERSION_NUMBER < 1020
    if (def->GetParticleType() == "nucleus" &&
        (aTrack->GetTrackStatus() == fPostponeToNextEvent ||
         (def->GetParticleName().contains("[0.0]") &&
          aTrack->GetKineticEnergy() <= 0.0 && aTrack->GetTrackID() > 1))) {
#else // G4VERSION_NUMBER>=1020
    if (def->GetParticleType() == "nucleus" &&
        (aTrack->GetTrackStatus() == fPostponeToNextEvent ||
         ((!def->GetParticleName().contains("[")) &&
          (!def->GetParticleName().contains("alpha")) &&
          aTrack->GetKineticEnergy() <= 0.0 && aTrack->GetTrackID() > 1))) {
#endif
        // the postponed nucleus set in stepping,
        // or the nucleus at ground state with 0 kinetic energy
        // (except the first track),
        // will be pushed to the waiting stack.
        TemporaryParticle tp;
        tp.setParticleType(def->GetParticleName());
        tp.setId(aTrack->GetTrackID());
        tp.setEnergy(aTrack->GetTotalEnergy());
        tp.setPx(aTrack->GetMomentum().x());
        tp.setPy(aTrack->GetMomentum().y());
        tp.setPz(aTrack->GetMomentum().z());
        tp.setX(aTrack->GetPosition().x());
        tp.setY(aTrack->GetPosition().y());
        tp.setZ(aTrack->GetPosition().z());
        _manager->addTemporaryParticle(tp);
        return fWaiting;
    }
    return fUrgent;
}

void PandaXStackingAction::NewStage() {
    // all the urgent tracks have been processed. So we can save the results.
    _manager->fillEvent(
        G4EventManager::GetEventManager()->GetConstCurrentEvent(), true);
}

void PandaXStackingAction::PrepareNewEvent() {}
