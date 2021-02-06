#include "SimpleGPSGenerator.hh"

#include <G4Event.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>

GeneratorRegister<SimpleGPSGenerator>
    SimpleGPSGenerator::reg("SimpleGPSGenerator");

SimpleGPSGenerator::SimpleGPSGenerator(const BambooParameters &pars)
    : BambooGenerator{pars}, gun{new G4GeneralParticleSource} {}

void SimpleGPSGenerator::GeneratePrimaries(G4Event *anEvent) {
    gun->GeneratePrimaryVertex(anEvent);
}
