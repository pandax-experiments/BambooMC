#include "PandaXDataManager.hh"
#include "PandaXEnergyDepositionHit.hh"
#include "PandaXSurfaceFluxHit.hh"

#include <TTree.h>

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <G4PhysicalConstants.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4THitsMap.hh>

#include <iostream>

PandaXDataManager::~PandaXDataManager() {
    G4cout << "PandaXDataManager destructed." << G4endl;
}

void PandaXDataManager::book(const std::string &name) {
    rootFile = new TFile(name.c_str(), "RECREATE");
    if (rootFile->IsZombie()) {
        std::cerr << "PandaXDataManager::book: ";
        std::cerr << "problem creating ROOT file." << std::endl << std::endl;
        rootFile = nullptr;
        return;
    }
    mcTree = new TTree("mcTree", "Tree with MC info");
    mcTree->Branch("runId", &runId, "runId/I");
    mcTree->Branch("eventId", &eventId, "eventId/I");
    mcTree->Branch("seed", &seed, "seed/L");
    if (recordEnergyDeposition) {
        mcTree->Branch("nHits", &nHits, "nHits/I");
        mcTree->Branch("trackId", &trackId);
        mcTree->Branch("parentId", &parentId);
        mcTree->Branch("type", &type);
        mcTree->Branch("parentType", &parentType);
        mcTree->Branch("creatorProcess", &creatorProcess);
        mcTree->Branch("depositionProcess", &depositionProcess);
        mcTree->Branch("volume", &volume);
        mcTree->Branch("totalEnergy", &totalEnergy);
        mcTree->Branch("xd", &xd);
        mcTree->Branch("yd", &yd);
        mcTree->Branch("zd", &zd);
        mcTree->Branch("td", &td);
        mcTree->Branch("energy", &energy);
    }
    if (recordSurfaceFlux) {
        mcTree->Branch("nTracks", &nTracks);
        mcTree->Branch("trackEnergy", &trackEnergy);
        mcTree->Branch("trackName", &trackName);
        mcTree->Branch("trackParent", &trackParent);
        mcTree->Branch("px", &px);
        mcTree->Branch("py", &py);
        mcTree->Branch("pz", &pz);
        mcTree->Branch("trackTime", &track_time);
        mcTree->Branch("trackX", &track_x);
        mcTree->Branch("trackY", &track_y);
        mcTree->Branch("trackZ", &track_z);
    }
    if (recordPrimaryParticle) {
        mcTree->Branch("nPrimaries", &nPrimaries);
        mcTree->Branch("primaryType", &primaryType);
        mcTree->Branch("primaryId", &primaryId);
        mcTree->Branch("primaryEnergy", &primaryEnergy);
        mcTree->Branch("primaryPx", &primaryPx);
        mcTree->Branch("primaryPy", &primaryPy);
        mcTree->Branch("primaryPz", &primaryPz);
        mcTree->Branch("primaryX", &primaryX);
        mcTree->Branch("primaryY", &primaryY);
        mcTree->Branch("primaryZ", &primaryZ);
    }
}

void PandaXDataManager::save() {
    mcTree->Write("", TObject::kWriteDelete);
    G4cout << "PandaXDataManager: Write data to tree..." << G4endl;
}

void PandaXDataManager::close() {
    if (rootFile != nullptr) {
        rootFile->Close();
        delete rootFile;
        rootFile = nullptr;
    }
}

void PandaXDataManager::fillEvent(const G4Event *aEvent, bool partial) {
    if (!partial && partialSaved)
        return;
    G4HCofThisEvent *hCthis = aEvent->GetHCofThisEvent();
    int nHitCollections = hCthis->GetNumberOfCollections();
    resetData();
    if (recordEnergyDeposition) {
        for (int i = 0; i < nHitCollections; ++i) {
            G4VHitsCollection *hitsCollection = hCthis->GetHC(i);
            if (hitsCollection->GetName().contains("EnergyDepositionHits")) {
                PandaXEnergyDepositionHitsCollection *hC =
                    (PandaXEnergyDepositionHitsCollection *)hitsCollection;
                for (size_t j = 0; j < hitsCollection->GetSize(); ++j) {
                    PandaXEnergyDepositionHit *hit =
                        (PandaXEnergyDepositionHit *)hC->GetHit(j);
                    trackId.push_back(hit->getTrackId());
                    parentId.push_back(hit->getParentId());
                    type.push_back(hit->getType());
                    parentType.push_back(hit->getParent());
                    creatorProcess.push_back(hit->getCreatorProcess());
                    depositionProcess.push_back(hit->getDepositionProcess());
                    volume.push_back(hitsCollection->GetSDname());
                    xd.push_back(hit->getX() / mm);
                    yd.push_back(hit->getY() / mm);
                    zd.push_back(hit->getZ() / mm);
                    td.push_back(hit->getT() / s);
                    energy.push_back(hit->getEnergy() / keV);
                    totalEnergy += (hit->getEnergy()) / keV;
                    nHits++;
                }
            }
        }
    }
    if (recordSurfaceFlux) {
        for (int i = 0; i < nHitCollections; ++i) {
            G4VHitsCollection *hitsCollection = hCthis->GetHC(i);
            if (hitsCollection->GetName().contains("SurfaceFluxHits")) {
                PandaXSurfaceFluxHitsCollection *hC =
                    (PandaXSurfaceFluxHitsCollection *)hitsCollection;
                for (size_t j = 0; j < hitsCollection->GetSize(); ++j) {
                    PandaXSurfaceFluxHit *hit =
                        (PandaXSurfaceFluxHit *)hC->GetHit(j);
                    G4ThreeVector momentum = hit->getMomentum();
                    trackEnergy.push_back(hit->getEnergy() / keV);
                    trackName.push_back(hit->getTrackName());
                    trackParent.push_back(hit->getParentName());
                    px.push_back(momentum.x() / keV);
                    py.push_back(momentum.y() / keV);
                    pz.push_back(momentum.z() / keV);
                    track_time.push_back(hit->t / s);
                    track_x.push_back(hit->x / mm);
                    track_y.push_back(hit->y / mm);
                    track_z.push_back(hit->z / mm);
                    nTracks++;
                }
            }
        }
    }
    if (recordPrimaryParticle) {
        if (!partial || particles.size() == 0) {
            // loop over all primary particles...
            int nVertex = aEvent->GetNumberOfPrimaryVertex();
            for (int iV = 0; iV < nVertex; ++iV) {
                G4PrimaryVertex *vertex = aEvent->GetPrimaryVertex(iV);
                int nParticles = vertex->GetNumberOfParticle();
                double x = vertex->GetX0();
                double y = vertex->GetY0();
                double z = vertex->GetZ0();
                for (int ip = 0; ip < nParticles; ++ip) {
                    G4PrimaryParticle *particle = vertex->GetPrimary(ip);
                    auto pdef = particle->GetParticleDefinition();
                    if (pdef != nullptr) {
                        primaryType.push_back(pdef->GetParticleName());
                    } else {
                        primaryType.push_back("unknown");
                    }
                    primaryId.push_back(particle->GetTrackID());
                    primaryEnergy.push_back(particle->GetTotalEnergy() / keV);
                    primaryPx.push_back(particle->GetPx() / keV);
                    primaryPy.push_back(particle->GetPy() / keV);
                    primaryPz.push_back(particle->GetPz() / keV);
                    primaryX.push_back(x / mm);
                    primaryY.push_back(y / mm);
                    primaryZ.push_back(z / mm);
                    nPrimaries++;
                }
            }
        } else {
            for (size_t ip = 0; ip < particles.size(); ++ip) {
                TemporaryParticle &particle = particles[ip];
                primaryType.push_back(particle.getParticleType());
                primaryId.push_back(particle.getId());
                primaryEnergy.push_back(particle.getEnergy() / keV);
                primaryPx.push_back(particle.getPx() / keV);
                primaryPy.push_back(particle.getPy() / keV);
                primaryPz.push_back(particle.getPz() / keV);
                primaryX.push_back(particle.getX() / mm);
                primaryY.push_back(particle.getY() / mm);
                primaryZ.push_back(particle.getZ() / mm);
                nPrimaries++;
            }
        }
        if (partial) {
            // clear the temporary particles
            particles = tparticles;
            tparticles.clear();
        }
    }

    if ((recordEnergyDeposition && nHits > 0) ||
        (recordSurfaceFlux && nTracks > 0) ||
        (recordNullEvent && recordPrimaryParticle)) {
        mcTree->Fill();
    }
    if (partial) {
        resetPartialEvent(aEvent);
    }
    partialSaved = partial;
}

void PandaXDataManager::resetData() {
    if (recordEnergyDeposition) {
        nHits = 0;
        trackId.clear();
        parentId.clear();
        type.clear();
        parentType.clear();
        creatorProcess.clear();
        depositionProcess.clear();
        volume.clear();
        totalEnergy = 0;
        xd.clear();
        yd.clear();
        zd.clear();
        td.clear();
        energy.clear();
    }
    if (recordSurfaceFlux) {
        nTracks = 0;
        trackEnergy.clear();
        trackName.clear();
        trackParent.clear();
        px.clear();
        py.clear();
        pz.clear();
        track_time.clear();
        track_x.clear();
        track_y.clear();
        track_z.clear();
    }
    if (recordPrimaryParticle) {
        nPrimaries = 0;
        primaryType.clear();
        primaryId.clear();
        primaryEnergy.clear();
        primaryPx.clear();
        primaryPy.clear();
        primaryPz.clear();
        primaryX.clear();
        primaryY.clear();
        primaryZ.clear();
    }
}

void PandaXDataManager::resetPartialEvent(const G4Event *aEvent) {
    G4HCofThisEvent *hCthis = aEvent->GetHCofThisEvent();
    int nHitCollections = hCthis->GetNumberOfCollections();
    for (int i = 0; i < nHitCollections; ++i) {
        G4VHitsCollection *hitsCollection = hCthis->GetHC(i);
        if (hitsCollection->GetName().contains("EnergyDepositionHits")) {
            PandaXEnergyDepositionHitsCollection *hC =
                (PandaXEnergyDepositionHitsCollection *)hitsCollection;
            for (int j = 0; j < hC->entries(); ++j) {
                PandaXEnergyDepositionHit *hit = (*hC)[j];
                delete hit;
            }
            hC->GetVector()->clear();
        } else if (hitsCollection->GetName().contains("SurfaceFluxHits")) {
            PandaXSurfaceFluxHitsCollection *hC =
                (PandaXSurfaceFluxHitsCollection *)hitsCollection;
            for (int j = 0; j < hC->entries(); ++j) {
                PandaXSurfaceFluxHit *hit = (*hC)[j];
                delete hit;
            }
            hC->GetVector()->clear();
        }
    }
}

void PandaXDataManager::addTemporaryParticle(const TemporaryParticle &tp) {
    tparticles.push_back(tp);
}
