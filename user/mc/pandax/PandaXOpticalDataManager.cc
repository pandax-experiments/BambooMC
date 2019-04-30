#include "PandaXOpticalDataManager.hh"
#include "PandaXOpticalHit.hh"

#include <TTree.h>

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4THitsMap.hh>

PandaXOpticalDataManager::~PandaXOpticalDataManager() {
    G4cout << "PandaXOpticalDataManager destructed." << G4endl;
}

void PandaXOpticalDataManager::book(const std::string &name) {
    rootFile = new TFile(name.c_str(), "RECREATE");
    if (rootFile->IsZombie()) {
        std::cerr << "PandaXOpticalDataManager::book: ";
        std::cerr << "problem creating ROOT file." << std::endl << std::endl;
        rootFile = nullptr;
        return;
    }
    mcTree = new TTree("mcTree", "Tree with Optical MC info");
    mcTree->Branch("runId", &runId, "runId/I");
    mcTree->Branch("eventId", &eventId, "eventId/I");
    mcTree->Branch("nHits", &nHits, "nHits/I");
    mcTree->Branch("pmtNumber", &pmtNumber);
    mcTree->Branch("creatorProcess", &creatorProcess);
    mcTree->Branch("x", &x);
    mcTree->Branch("y", &y);
    mcTree->Branch("z", &z);
    mcTree->Branch("tGlobal", &tGlobal);
    mcTree->Branch("tLocal", &tLocal);
    mcTree->Branch("tProper", &tProper);
    mcTree->Branch("energy", &energy);
    mcTree->Branch("ox", &ox);
    mcTree->Branch("oy", &oy);
    mcTree->Branch("oz", &oz);
    mcTree->Branch("parent", &parent);
}

void PandaXOpticalDataManager::save() {
    mcTree->Write("", TObject::kWriteDelete);
    G4cout << "PandaXOpticalDataManager: Write data to tree..." << G4endl;
}

void PandaXOpticalDataManager::close() {
    if (rootFile != nullptr) {
        rootFile->Close();
        delete rootFile;
        rootFile = nullptr;
    }
}

void PandaXOpticalDataManager::fillEvent(const G4Event *aEvent) {
    auto hCthis = aEvent->GetHCofThisEvent();
    int nHitCollections = hCthis->GetNumberOfCollections();
    resetData();
    for (int i = 0; i < nHitCollections; ++i) {
        auto hC = hCthis->GetHC(i);
        if (hC->GetName().contains("OpticalHits")) {
            auto hCo = static_cast<PandaXOpticalHitsCollection *>(hC);
            for (auto j = 0u; j < hC->GetSize(); ++j) {
                auto hit = static_cast<PandaXOpticalHit *>(hCo->GetHit(j));
                pmtNumber.push_back(hit->getPMTNumber());
                x.push_back(hit->getPosition().x() / mm);
                y.push_back(hit->getPosition().y() / mm);
                z.push_back(hit->getPosition().z() / mm);
                energy.push_back(hit->getEnergy() / keV);
                velocity.push_back(hit->getVelocity() / c_light);
                creatorProcess.push_back(hit->getCreatorProcess());
                tGlobal.push_back(hit->getGlobalTime() / ns);
                tLocal.push_back(hit->getLocalTime() / ns);
                tProper.push_back(hit->getProperTime() / ns);
                ox.push_back(hit->getSourcePos().x() / mm);
                oy.push_back(hit->getSourcePos().y() / mm);
                oz.push_back(hit->getSourcePos().z() / mm);
                parent.push_back(hit->getParent());
                nHits++;
            }
        }
    }
    if (nHits > 0)
        mcTree->Fill();
}

void PandaXOpticalDataManager::resetData() {
    nHits = 0;
    x.clear();
    y.clear();
    z.clear();
    energy.clear();
    creatorProcess.clear();
    tGlobal.clear();
    tLocal.clear();
    tProper.clear();
    pmtNumber.clear();
    x.clear();
    y.clear();
    z.clear();
    parent.clear();
}
