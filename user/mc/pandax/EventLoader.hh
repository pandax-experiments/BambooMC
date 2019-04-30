#pragma once

#include "BambooFactory.hh"
#include "BambooGenerator.hh"

#include <fstream>
#include <memory>

class EventLoaderMessenger;

class G4Event;

class EventLoader : public BambooGenerator {
  public:
    EventLoader(const BambooParameters &pars);

    ~EventLoader() = default;

    virtual void GeneratePrimaries(G4Event *anEvent);

    static GeneratorRegister<EventLoader> reg;

    void setConfineVolume(const G4String &s);

    void setShape(const G4String &s) { shape = s; }

    void setCenter(const G4ThreeVector &v) { center = v; }

    void setRadius(double v) { radius = v; }

    void setHeight(double v) { height = v; }

    void setUpVec(const G4ThreeVector &v) { upVec = v; }

    const G4String &getShape() { return shape; }

    const G4String &getConfineVolume() { return confineVolume; }

  private:
    G4ThreeVector generatePosition();
    G4ThreeVector sampleCylinderPosition();
    G4bool isSourceConfined(const G4ThreeVector &pos);
    G4String shape;

    G4String confineVolume;

    G4ThreeVector center{0, 0, 0};

    double radius{0};
    double height{0};

    G4ThreeVector upVec{0, 0, 1};

    std::unique_ptr<EventLoaderMessenger> messenger;

    std::string source;
    std::ifstream source_stream;
};
