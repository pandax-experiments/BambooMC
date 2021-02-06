#include "PandaXPhysics.hh"

#include <G4EmLivermorePhysics.hh>
#include <QString>
#include <Shielding.hh>

PhysicsRegister<PandaXPhysics>
    PandaXPhysics::reg("PandaXPhysics");

PandaXPhysics::PandaXPhysics(const BambooParameters &pars)
    : BambooPhysics(pars) {

    auto cut_length = physicsParameters.evaluateParameter("cutlength");

    if (cut_length == 0) {
        cut_length = 0.1 * mm;
    }
    defaultCutValue = cut_length;
    G4cout << "default cut length: " << cut_length / mm << " mm" << G4endl;

    G4int verbose = 1;

    // EM Physics
    RegisterPhysics(new G4EmLivermorePhysics(verbose));

    // Synchroton Radiation & GN Physics
    RegisterPhysics(new G4EmExtraPhysics(verbose));

    // Decays
    RegisterPhysics(new G4DecayPhysics(verbose));

    RegisterPhysics(new G4RadioactiveDecayPhysics(verbose));

    // hadron physics
    RegisterPhysics(new G4HadronElasticPhysicsHP(verbose));

    // shielding, changed api for different version of geant4.
    RegisterPhysics(new G4HadronPhysicsShielding(verbose));

    // stopping physics
    RegisterPhysics(new G4StoppingPhysics(verbose));

    // ion physics
    RegisterPhysics(new G4IonQMDPhysics(verbose));
}

