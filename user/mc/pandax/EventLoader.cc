#include "EventLoader.hh"
#include "EventLoaderMessenger.hh"

#include <G4Event.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4TrackingManager.hh>
#include <G4TransportationManager.hh>
#include <Randomize.hh>

#include <exception>

GeneratorRegister<EventLoader> EventLoader::reg("EventLoader");

EventLoader::EventLoader(const BambooParameters &pars)
    : BambooGenerator{pars}, messenger{new EventLoaderMessenger(this)},
      source{pars.getParameter("source")}, source_stream{source} {
    if (!source_stream.is_open())
        throw std::runtime_error("EventLoader: Cannot open source file " +
                                 source);
}

void EventLoader::GeneratePrimaries(G4Event *anEvent) {
    // load data from file
    int event_number, n_particles;
    using CLHEP::GeV;
    if (source_stream >> event_number >> n_particles) {
        int ip, code;
        double px, py, pz;
        // create new vertex
        auto pos = generatePosition();
        auto vertex = new G4PrimaryVertex(pos, 0);
        for (int il = 0; il < n_particles; ++il) {
            source_stream >> ip >> code >> px >> py >> pz;
            if (source_stream.fail())
                throw std::runtime_error("EventLoader: error read data");
            auto particle =
                new G4PrimaryParticle(code, px * GeV, py * GeV, pz * GeV);
            vertex->SetPrimary(particle);
        }
        anEvent->AddPrimaryVertex(vertex);
    } else {
        throw std::runtime_error("EventLoader: error read data");
    }
}

void EventLoader::setConfineVolume(const G4String &s) {
    // check confine
    auto pvStore = G4PhysicalVolumeStore::GetInstance();
    for (auto pv : *pvStore) {
        auto name = pv->GetName();
        if (s == name) {
            confineVolume = s;
            return;
        }
    }
    G4cout << "Confine volume " << s << " is not found!" << G4endl;
}

G4ThreeVector EventLoader::generatePosition() {
    if (shape == "point")
        return center;
    if (shape != "cylinder")
        return center;
    if (radius == 0 || height == 0)
        return center;

    auto pos = sampleCylinderPosition();
    if (confineVolume == "")
        return pos;

    constexpr int max_count = 10000;
    int count = 0;
    while (count < max_count && !isSourceConfined(pos)) {
        pos = sampleCylinderPosition();
        ++count;
    }
    if (count == max_count) {
        G4cout << "max confine count reached! Give up!" << G4endl;
    }
    return pos;
}

G4ThreeVector EventLoader::sampleCylinderPosition() {
    // sample in height direction
    double zp = G4UniformRand() * height - height / 2;

    // sample in radius
    double rp = sqrt(G4UniformRand()) * radius;
    double phi = G4UniformRand() * 2 * M_PI;

    G4ThreeVector raw(rp * cos(phi), rp * sin(phi), zp);

    // rotate uz
    raw.rotateUz(upVec);

    // translation
    raw += center;
    return raw;
}

G4bool EventLoader::isSourceConfined(const G4ThreeVector &pos) {
    G4ThreeVector null(0, 0, 0);
    auto navigator = G4TransportationManager::GetTransportationManager()
                         ->GetNavigatorForTracking();
    auto volume = navigator->LocateGlobalPointAndSetup(pos, &null, true);
    if (!volume)
        return false;
    auto name = volume->GetName();
    if (name == confineVolume)
        return true;
    return false;
}
