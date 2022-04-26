#include "ConfineGenerator.hh"
#include "ConfineGeneratorMessenger.hh"

#include <G4Event.hh>
#include <G4IonTable.hh>
#include <G4Ions.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Navigator.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4PrimaryParticle.hh>
#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4TransportationManager.hh>
#include <G4VPhysicalVolume.hh>
#include <Randomize.hh>

#include <exception>

using namespace CLHEP;

GeneratorRegister<ConfineGenerator> ConfineGenerator::reg("ConfineGenerator");

ConfineGenerator::ConfineGenerator(const BambooParameters &pars)
    : BambooGenerator{pars}, messenger{new ConfineGeneratorMessenger(this)} {}

void ConfineGenerator::GeneratePrimaries(G4Event *anEvent) {

    auto pos = generateRealPosition();
    auto vertex = new G4PrimaryVertex(pos, 0);

    // need to update in future
    G4PrimaryParticle *particle = new G4PrimaryParticle(particle_definition);
    particle->SetKineticEnergy(monoEnergy);
    particle->SetMomentumDirection(direction);
    vertex->SetPrimary(particle);
    anEvent->AddPrimaryVertex(vertex);
}

void ConfineGenerator::confineSourceToVolume(const G4String &v) {
    std::set<G4String> confine_names;
    G4String confine_name;
    {
        std::istringstream iss{v};
        while (iss >> confine_name) {
            confine_names.insert(confine_name);
        }
    }
    // checks if the selected volumes exist and store all volumes that match
    auto pv_store = G4PhysicalVolumeStore::GetInstance();

    volume_names.clear();
    for (auto name : confine_names) {
        if (name.back() == '*') {
            name.pop_back();
            // loop over all physical volumes
            for (auto pv : *pv_store) {
                auto pv_name = pv->GetName();
                if (pv_name.size() < name.size())
                    continue;
                if (pv_name.substr(0, name.size()) == name) {
                    volume_names.insert(pv_name);
                }
            }
        } else {
            // exact match
            auto pv = pv_store->GetVolume(name);
            if (pv != nullptr) {
                volume_names.insert(name);
            }
        }
    }
    if (!volume_names.empty()) {
        confine = true;
    } else {
        G4cerr << "The provided confine volumes are not found!" << G4endl;
        confine = false;
    }
}

void ConfineGenerator::confineSourceToMaterial(const G4String &m) {
    std::set<G4String> confine_names;
    G4String confine_name;
    {
        std::istringstream iss{m};
        while (iss >> confine_name) {
            confine_names.insert(confine_name);
        }
    }

    auto *pv_store = G4PhysicalVolumeStore::GetInstance();
    volume_names.clear();
    for (const auto &name : confine_names) {
        for (auto pv : *pv_store) {
            auto m_name = pv->GetLogicalVolume()->GetMaterial()->GetName();
            if (name == m_name) {
                volume_names.insert(pv->GetName());
            }
        }
    }
    if (!volume_names.empty()) {
        confine = true;
    } else {
        G4cerr << "The provided confine materials are not found!" << G4endl;
        confine = false;
    }
}

G4ThreeVector ConfineGenerator::generateRealPosition() {
    if (shape == "Point")
        return center;

    if (!confine) {
        return generatePointsInVolume();
    }
    // source is confined
    constexpr int max_count = 10000;
    int count = 0;
    while (count < max_count) {
        auto pos = generatePointsInVolume();
        if (isSourceConfined(pos)) {
            return pos;
        }
        ++count;
    }
    if (count == max_count) {
        G4cerr << "Maximum counts for confine particle reached!"
               << " Give up! Generate event in center!" << G4endl;
    }
    return center;
}

void ConfineGenerator::setParticleType(const G4String &t) {
    particle_type = t;
    if (particle_type != "ion") {
	auto table = G4ParticleTable::GetParticleTable();
	auto def = table->FindParticle(particle_type);
        if (def != 0) {
	    setParticleDefinition(def);
        }
    }
}


G4ThreeVector ConfineGenerator::generatePointsInVolume() {

    G4double x{0}, y{0}, z{0};

    if (shape == "Cylinder") {
        double param_x = G4UniformRand();
        double r = sqrt(param_x * radius * radius);
        double theta = 2 * M_PI * G4UniformRand();
        x = r * cos(theta);
        y = r * sin(theta);
        z = G4UniformRand();
        z = (z * 2. * halfz) - halfz;
    } else if (shape == "Box") {
        x = 2 * (G4UniformRand() - 0.5) * halfx;
        y = 2 * (G4UniformRand() - 0.5) * halfy;
        z = 2 * (G4UniformRand() - 0.5) * halfz;
    } else {
        G4cerr << "Error: Volume Shape Does Not Exist" << G4endl;
    }
    return center + G4ThreeVector{x, y, z};
}

G4bool ConfineGenerator::isSourceConfined(const G4ThreeVector &pos) {
    // Method to check point is within the volume specified
    auto navigator = G4TransportationManager::GetTransportationManager()
                         ->GetNavigatorForTracking();
    // Check particlePosition is within a volume in our list
    auto pv = navigator->LocateGlobalPointAndSetup(pos);
    if (volume_names.find(pv->GetName()) != volume_names.end()) {
        return true;
    }
    return false;
}
