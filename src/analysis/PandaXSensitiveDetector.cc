#include "analysis/PandaXSensitiveDetector.hh"
#include "analysis/PandaXDataManager.hh"

#include <G4Box.hh>
#include <G4GeometryTolerance.hh>
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>

PandaXSensitiveDetector::PandaXSensitiveDetector(const G4String &name,
                                                 bool enableEnergyDeposition,
                                                 bool enableFlatSurfaceFlux)
    : G4VSensitiveDetector(name),
      _recordEnergyDeposition(enableEnergyDeposition),
      _recordFlatSurfaceFlux(enableFlatSurfaceFlux), _eDHitsCollection(0),
      _fsFluxHitsCollection(0) {
    // note: the collection name must be initialized in the constructor.
    collectionName.push_back("EnergyDepositionHits");
    collectionName.push_back("FlatSurfaceFluxHits");
}

PandaXSensitiveDetector::~PandaXSensitiveDetector() {}

void PandaXSensitiveDetector::Initialize(G4HCofThisEvent *hc) {
    _eDHitsCollection = new PandaXEnergyDepositionHitsCollection(
        SensitiveDetectorName, collectionName[0]);
    hc->AddHitsCollection(GetCollectionID(0), _eDHitsCollection);
    _fsFluxHitsCollection = new PandaXFlatSurfaceFluxHitsCollection(
        SensitiveDetectorName, collectionName[1]);
    hc->AddHitsCollection(GetCollectionID(1), _fsFluxHitsCollection);
}

void PandaXSensitiveDetector::EndOfEvent(G4HCofThisEvent *) {}

G4bool PandaXSensitiveDetector::ProcessHits(G4Step *aStep,
                                            G4TouchableHistory *) {
    bool isSuccess = false;
    if (_recordEnergyDeposition) {
        G4double edep = aStep->GetTotalEnergyDeposit();
        if (edep == 0.)
            goto END_ENERYG_DEPOSITION;
        PandaXEnergyDepositionHit *hit = new PandaXEnergyDepositionHit();
        int hitId = aStep->GetTrack()->GetTrackID();
        hit->setTrackId(hitId);
        std::map<int, std::string> trackMap =
            PandaXDataManager::Instance()->getTrackMap();
        int parentId = aStep->GetTrack()->GetParentID();
        hit->setParentId(parentId);
        if (parentId && trackMap.find(parentId) != trackMap.end()) {
            hit->setParent(trackMap[parentId]);
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
        _eDHitsCollection->insert(hit);
        isSuccess = true;
    }
END_ENERYG_DEPOSITION:

    if (_recordFlatSurfaceFlux) {
        G4StepPoint *preStep = aStep->GetPreStepPoint();
        std::map<int, std::string> trackMap =
            PandaXDataManager::Instance()->getTrackMap();
        if (preStep->GetStepStatus() == fGeomBoundary) {
            G4VSolid *solid =
                preStep->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
            G4Box *box = (G4Box *)solid;
            PandaXFlatSurfaceFluxHit *hit = new PandaXFlatSurfaceFluxHit();
            hit->setEnergy(preStep->GetKineticEnergy());
            hit->setTrackName(
                aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
            int parentId = aStep->GetTrack()->GetParentID();
            if (parentId && trackMap.find(parentId) != trackMap.end()) {
                hit->setParentName(trackMap[parentId]);
            }
            hit->setMomentum(preStep->GetMomentum());
            const auto &pos = preStep->GetPosition();
            hit->x = pos.x();
            hit->y = pos.y();
            hit->z = pos.z();
            hit->t = preStep->GetGlobalTime();
            hit->setSurface(determineBoxSurface(aStep, box));
            _fsFluxHitsCollection->insert(hit);
            isSuccess = true;
        }
    }
    return isSuccess;
}

int PandaXSensitiveDetector::determineBoxSurface(G4Step *aStep,
                                                 G4Box *boxSolid) {
    G4StepPoint *preStep = aStep->GetPreStepPoint();
    G4TouchableHandle touchable = preStep->GetTouchableHandle();

    G4double tolerance =
        G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
    G4ThreeVector stepPosition = preStep->GetPosition();
    G4ThreeVector localPosition =
        touchable->GetHistory()->GetTopTransform().TransformPoint(stepPosition);
    if (std::fabs(localPosition.x() + boxSolid->GetXHalfLength()) < tolerance)
        return -1;
    if (std::fabs(localPosition.x() - boxSolid->GetXHalfLength()) < tolerance)
        return 1;
    if (std::fabs(localPosition.y() + boxSolid->GetYHalfLength()) < tolerance)
        return -2;
    if (std::fabs(localPosition.y() - boxSolid->GetYHalfLength()) < tolerance)
        return 2;
    if (std::fabs(localPosition.z() + boxSolid->GetZHalfLength()) < tolerance)
        return -3;
    if (std::fabs(localPosition.z() - boxSolid->GetZHalfLength()) < tolerance)
        return 3;
    return 0;
}
