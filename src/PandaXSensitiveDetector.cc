#include "PandaXSensitiveDetector.hh"
#include "PandaXTrackInformation.hh"

#include <G4Box.hh>
#include <G4GeometryTolerance.hh>
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>

PandaXSensitiveDetector::PandaXSensitiveDetector(const G4String &name,
                                                 bool enableEnergyDeposition,
                                                 bool enableSurfaceFlux)
    : G4VSensitiveDetector(name),
      recordEnergyDeposition(enableEnergyDeposition),
      recordSurfaceFlux(enableSurfaceFlux) {
    // note: the collection name must be initialized in the constructor.
    collectionName.push_back("EnergyDepositionHits");
    collectionName.push_back("SurfaceFluxHits");
}

void PandaXSensitiveDetector::Initialize(G4HCofThisEvent *hc) {
    eDHitsCollection = new PandaXEnergyDepositionHitsCollection(
        SensitiveDetectorName, collectionName[0]);
    hc->AddHitsCollection(GetCollectionID(0), eDHitsCollection);
    fsFluxHitsCollection = new PandaXSurfaceFluxHitsCollection(
        SensitiveDetectorName, collectionName[1]);
    hc->AddHitsCollection(GetCollectionID(1), fsFluxHitsCollection);
}

void PandaXSensitiveDetector::EndOfEvent(G4HCofThisEvent *) {}

G4bool PandaXSensitiveDetector::ProcessHits(G4Step *aStep,
                                            G4TouchableHistory *) {
    bool isSuccess = false;
    if (recordEnergyDeposition) {
        G4double edep = aStep->GetTotalEnergyDeposit();
        if (edep == 0.)
            goto END_ENERYG_DEPOSITION;
        PandaXEnergyDepositionHit *hit = new PandaXEnergyDepositionHit();
        auto track = aStep->GetTrack();
        int hitId = track->GetTrackID();
        hit->setTrackId(hitId);
        int parentId = track->GetParentID();
        hit->setParentId(parentId);
        if (parentId != 0) {
            auto trackInfo = static_cast<PandaXTrackInformation *>(
                track->GetUserInformation());
            hit->setParent(trackInfo->GetParent());
        }
        hit->setEnergy(edep);
        hit->setX(aStep->GetPostStepPoint()->GetPosition().x());
        hit->setY(aStep->GetPostStepPoint()->GetPosition().y());
        hit->setZ(aStep->GetPostStepPoint()->GetPosition().z());
        hit->setT(aStep->GetPostStepPoint()->GetGlobalTime());
        hit->setType(
            aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
        if (aStep->GetTrack()->GetCreatorProcess()) {
            hit->setCreatorProcess(
                aStep->GetTrack()->GetCreatorProcess()->GetProcessName());
        }
        hit->setDepositionProcess(aStep->GetPostStepPoint()
                                      ->GetProcessDefinedStep()
                                      ->GetProcessName());
        eDHitsCollection->insert(hit);
        isSuccess = true;
    }
END_ENERYG_DEPOSITION:

    if (recordSurfaceFlux) {
        G4StepPoint *preStep = aStep->GetPreStepPoint();
        if (preStep->GetStepStatus() == fGeomBoundary) {
            PandaXSurfaceFluxHit *hit = new PandaXSurfaceFluxHit();
            hit->setEnergy(preStep->GetKineticEnergy());
            hit->setTrackName(
                aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
            auto track = aStep->GetTrack();
            if (track->GetParentID() != 0) {
                auto trackInfo = static_cast<PandaXTrackInformation *>(
                    track->GetUserInformation());
                hit->setParentName(trackInfo->GetParent());
            }
            hit->setMomentum(preStep->GetMomentum());
            const auto &pos = preStep->GetPosition();
            hit->x = pos.x();
            hit->y = pos.y();
            hit->z = pos.z();
            hit->t = preStep->GetGlobalTime();
            fsFluxHitsCollection->insert(hit);
            isSuccess = true;
        }
    }
    return isSuccess;
}
