#include "HEPEvtLoader.hh"

#include <G4Event.hh>

GeneratorRegister<HEPEvtLoader>
HEPEvtLoader::reg("HEPEvtLoader");

HEPEvtLoader::HEPEvtLoader(const BambooParameters &pars):
    BambooGenerator{pars}, evt{new G4HEPEvtInterface(pars.getParameter("source").c_str())} {}

void HEPEvtLoader::GeneratePrimaries(G4Event *anEvent) {
    evt->SetParticlePosition(G4ThreeVector(0, 0, 0));
    evt->GeneratePrimaryVertex(anEvent);
}
