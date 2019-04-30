#include "PandaXOpticalPhysics.hh"

#include <G4DecayPhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4EmLivermorePhysics.hh>
#include <G4EmParameters.hh>
#include <G4EmStandardPhysics.hh>
#include <G4EmStandardPhysics_option4.hh>
#include <G4HadronElasticPhysicsHP.hh>
#include <G4HadronElasticPhysicsLEND.hh>
#include <G4HadronPhysicsShielding.hh>
#include <G4IonElasticPhysics.hh>
#include <G4IonQMDPhysics.hh>
#include <G4OpticalPhysics.hh>
#include <G4ParticleHPManager.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4StoppingPhysics.hh>
#include <G4SystemOfUnits.hh>
#include <QString>
#include <Shielding.hh>

PhysicsRegister<PandaXOpticalPhysics>
    PandaXOpticalPhysics::reg("PandaXOpticalPhysics");

PandaXOpticalPhysics::PandaXOpticalPhysics(const BambooParameters &pars)
    : BambooPhysics(pars) {

    auto cut_length = physicsParameters.evaluateParameter("cutlength");

    if (cut_length == 0) {
        cut_length = 0.1 * mm;
    }
    defaultCutValue = cut_length;
    G4cout << "default cut length: " << cut_length / mm << " mm" << G4endl;

    auto verbose = physicsParameters.getParameter<int>("verbose");

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

    // Optical Physics
    G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
    RegisterPhysics(opticalPhysics);

    opticalPhysics->SetWLSTimeProfile("delta");

    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);

    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);

    opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
    opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
}
