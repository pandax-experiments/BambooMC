#include <G4PrimaryParticle.hh>
#include <G4Event.hh>
#include <G4TransportationManager.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4IonTable.hh>
#include <G4Ions.hh>
#include <Randomize.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>

#include "ConfineGenerator.hh"
#include "ConfineGeneratorMessenger.hh"

#include <exception>

using namespace CLHEP;

GeneratorRegister<ConfineGenerator> ConfineGenerator::reg("ConfineGenerator");

ConfineGenerator::ConfineGenerator(const BambooParameters &pars)
    : BambooGenerator{ pars },
      messenger{ new ConfineGeneratorMessenger(this) } {}

void ConfineGenerator::GeneratePrimaries(G4Event *anEvent) {

    auto pos = GenerateRealPosition();

    auto vertex = new G4PrimaryVertex(pos, 0);

    G4PrimaryParticle *particle =
        new G4PrimaryParticle(particleDefinitionVec[0], 0, 0, 0);
    vertex->SetPrimary(particle);

    anEvent->AddPrimaryVertex(vertex);
}


void ConfineGenerator::ConfineSourceToVolume(G4String hVolumeList) {
    std::stringstream hStream;
    hStream.str(hVolumeList);
    G4String hVolumeName;

    // store all the volume names
    while (!hStream.eof()) {
        hStream >> hVolumeName;
        m_hVolumeNames.insert(hVolumeName);
    }

    // checks if the selected volumes exist and store all volumes that match
    G4PhysicalVolumeStore *PVStore = G4PhysicalVolumeStore::GetInstance();
    G4bool bFoundAll = true;

    std::set<G4String> hActualVolumeNames;
    for (auto pIt = m_hVolumeNames.begin(); pIt != m_hVolumeNames.end();
         pIt++) {
        G4String hRequiredVolumeName = *pIt;
        G4bool bMatch = false;

        if ((hRequiredVolumeName.substr(hRequiredVolumeName.length() - 1))
                .compare("*") == 0) {
            hRequiredVolumeName =
                hRequiredVolumeName.substr(0, hRequiredVolumeName.length() - 1);
            bMatch = true;
        }

        G4bool bFoundOne = false;
        for (G4int iIndex = 0; iIndex < (G4int)PVStore->size(); iIndex++) {
            G4String hName = (*PVStore)[iIndex]->GetName();

            if ((bMatch && (hName.substr(0, hRequiredVolumeName.size())) ==
                               hRequiredVolumeName) ||
                hName == hRequiredVolumeName) {
                hActualVolumeNames.insert(hName);
                bFoundOne = true;
            }
        }

        bFoundAll = bFoundAll && bFoundOne;
    }

    if (bFoundAll) {
        m_hVolumeNames = hActualVolumeNames;
        m_bConfine = true;
    } else if (m_hVolumeNames.empty()) {
        G4cout << "Empty confine volume list. :(" << G4endl;
        m_bConfine = false;
    } else {
        G4cout << " **** Error: One or more volumes do not exist **** "
               << G4endl;
        G4cout << " Ignoring confine condition" << G4endl;
        m_hVolumeNames.clear();
        m_bConfine = false;
    }

    int cou = 0;

    auto pvStore = G4PhysicalVolumeStore::GetInstance();
    for (auto pv : *pvStore) {
        auto name = pv->GetName();
        for (auto pvreal = m_hVolumeNames.begin();
             pvreal != m_hVolumeNames.end(); pvreal++) {
            G4String s = *pvreal;

            if (s == name) {
                confineVolume = s;
                cou = cou + 1;
                G4cout << "Confine volume " << s << " IS FOUND!" << G4endl;
            }
        }
    }
    if (cou == 0)
        G4cout << "Confine volume " << s << " is not found!" << G4endl;
}

void ConfineGenerator::ConfineSourceToMaterial(G4String material_confine) {
    std::stringstream hStream1;
    hStream1.str(material_confine);
    G4String hMaterialName;

    // store all the volume names
    while (!hStream1.eof()) {
        hStream1 >> hMaterialName;
        m_hMaterialNames.insert(hMaterialName);
    }

    // checks if the selected volumes exist and store all volumes that match
    //   G4LogicalVolumeStore *LVStore = G4LogicalVolumeStore::GetInstance();
    G4PhysicalVolumeStore *PVStore = G4PhysicalVolumeStore::GetInstance();
    G4bool bFoundAll = true;
    G4bool bFoundOne = false;

    std::set<G4String> hNeedPVNames;
    for (auto pIt = m_hMaterialNames.begin(); pIt != m_hMaterialNames.end();
         pIt++) {
        hMaterialName = *pIt;
    for (G4int iIndex1 = 0; iIndex1 < (G4int)PVStore->size(); iIndex1++) {
if ( hMaterialName.compare ((*PVStore)[iIndex1]->GetLogicalVolume()->GetMaterial()->GetName()) == 0 ){    
//        if (hMaterialName.compare((*PVStore)[iIndex1] ->GetLogicalVolume() ->GetMaterial() ->GetName()) == 0) {
                G4String hPVName = (*PVStore)[iIndex1]->GetName();
                hNeedPVNames.insert(hPVName);
                bFoundOne = true;
            }
        }
        bFoundAll = bFoundAll && bFoundOne;
    }

    if (bFoundAll) {
        m_hMaterialNames = hNeedPVNames;
        m_bConfine_Material = true;
    } else if (m_hMaterialNames.empty()) {
        G4cout << "Empty confine Material volume list. :(" << G4endl;
        m_bConfine_Material = false;
    } else {
        G4cout << " **** Error: Confine Material --- One or more volumes do "
                  "not exist **** " << G4endl;
        G4cout << " Ignoring confine condition" << G4endl;
        m_hMaterialNames.clear();
        m_bConfine_Material = false;
    }

    int cou = 0;

    auto pvStore = G4PhysicalVolumeStore::GetInstance();
    for (auto pv : *pvStore) {
        auto name = pv->GetName();
        for (auto pvreal = m_hMaterialNames.begin();
             pvreal != m_hMaterialNames.end(); pvreal++) {
            G4String s = *pvreal;

            if (s == name) {
                confineVolume = s;
                cou = cou + 1;
                G4cout << "Confine volume " << s << " IS FOUND!" << G4endl;
            }
        }
    }
    if (cou == 0)
        G4cout << "Confine volume " << s << " is not found!" << G4endl;
}


G4ThreeVector ConfineGenerator::GenerateRealPosition() {
    if (m_hShape == "Point")
        return m_hCenterCoords;

    auto pos = GeneratePointsInVolume();

    constexpr int max_count = 10000;
    int count = 0;
    while (count < max_count && !IsSourceConfined(pos)) {
        pos = GeneratePointsInVolume();
        ++count;
    }
    if (count == max_count) {
        G4cout << "max confine count reached! Give up!" << G4endl;
    }
    return pos;
}

G4ThreeVector ConfineGenerator::GeneratePointsInVolume() {

    G4ThreeVector RandPos;
    G4double x = 0., y = 0., z = 0.;

    if (m_hShape == "Cylinder") {
        double param_x = G4UniformRand();
        double r = sqrt(param_x * m_dRadius * m_dRadius);
        double theta = 2 * 3.1415926 * G4UniformRand();
        x = r * cos(theta);
        y = r * sin(theta);
        z = G4UniformRand();
        z = (z * 2. * m_dHalfz) - m_dHalfz;
    } else if (m_hShape == "Box") {

        x = 2 * (G4UniformRand() - 0.5) * m_dHalfx;
        y = 2 * (G4UniformRand() - 0.5) * m_dHalfy;
        z = 2 * (G4UniformRand() - 0.5) * m_dHalfz;
    } else
        G4cout << "Error: Volume Shape Does Not Exist" << G4endl;

    RandPos.setX(x);
    RandPos.setY(y);
    RandPos.setZ(z);
    auto ParticlePosition = m_hCenterCoords + RandPos;
    return ParticlePosition;
}

G4bool ConfineGenerator::IsSourceConfined(const G4ThreeVector &pos) {
    // Method to check point is within the volume specified
    G4ThreeVector null(0., 0., 0.);
    G4ThreeVector *ptr;

    ptr = &null;

    m_pNavigator = G4TransportationManager::GetTransportationManager()
                       ->GetNavigatorForTracking();

    // Check m_hParticlePosition is within a volume in our list
    G4VPhysicalVolume *theVolume;

    theVolume = m_pNavigator->LocateGlobalPointAndSetup(pos, ptr, true);
    G4String theVolName = theVolume->GetName();

    std::set<G4String>::iterator pIt;
    if ((pIt = m_hVolumeNames.find(theVolName)) != m_hVolumeNames.end() ||
        (pIt = m_hMaterialNames.find(theVolName)) != m_hMaterialNames.end()) {
        return (true);
    } else
        return (false);
}

void ConfineGenerator::SetParticleDefinition(
    G4ParticleDefinition *aParticleDefinition) {
    particleDefinitionVec.push_back(aParticleDefinition);
    m_dParticleCharge = particleDefinitionVec[0]->GetPDGCharge();
}
