#include "PandaXOpticalSD.hh"
#include "PandaXOpticalTrackInformation.hh"

#include <G4OpticalPhoton.hh>
#include <G4VProcess.hh>

PandaXOpticalSD::PandaXOpticalSD(const G4String &name)
    : G4VSensitiveDetector(name) {
    collectionName.push_back("OpticalHits");
}

void PandaXOpticalSD::Initialize(G4HCofThisEvent *hc) {
    hitsCollection = new PandaXOpticalHitsCollection(SensitiveDetectorName,
                                                     collectionName[0]);
    hc->AddHitsCollection(GetCollectionID(0), hitsCollection);
}

void PandaXOpticalSD::EndOfEvent(G4HCofThisEvent *) {}

G4bool PandaXOpticalSD::ProcessHits(G4Step *, G4TouchableHistory *) {
    return false;
}

G4bool PandaXOpticalSD::ProcessHits_constStep(const G4Step *aStep,
                                              G4TouchableHistory *) {
    auto track = aStep->GetTrack();
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
        return false;
    auto pmt_number =
        aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);

    auto info = static_cast<PandaXOpticalTrackInformation *>(track->GetUserInformation());
    auto hit = new PandaXOpticalHit();
    hit->setPMTNumber(pmt_number);
    hit->setEnergy(track->GetKineticEnergy());
    hit->setPosition(aStep->GetPostStepPoint()->GetPosition());
    if (track->GetCreatorProcess())
        hit->setCreatorProcess(track->GetCreatorProcess()->GetProcessName());
    else
        hit->setCreatorProcess("unknown");
    hit->setVelocity(track->GetVelocity());
    hit->setGlobalTime(track->GetGlobalTime());
    hit->setLocalTime(track->GetLocalTime());
    hit->setProperTime(track->GetProperTime());
    if (info) {
        hit->setSourcePos(info->GetSourcePos());
        hit->setParent(info->GetParent());
    }
    hitsCollection->insert(hit);
    return true;
}
