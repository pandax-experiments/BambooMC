#include "analysis/PandaXAnalysis.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "analysis/BambooAnalysisFactory.hh"
#include "analysis/PandaXEventAction.hh"
#include "analysis/PandaXRunAction.hh"
#include "analysis/PandaXStackingAction.hh"
#include "analysis/PandaXSteppingAction.hh"
#include "analysis/PandaXTrackingAction.hh"

AnalysisRegister<PandaXAnalysis> PandaXAnalysis::reg("PandaXAnalysis");

PandaXAnalysis::PandaXAnalysis(const G4String &name)
    : BambooAnalysis(name), dm{nullptr} {
    int dEnergyDeposition =
        BambooGlobalVariables::Instance()->getAnalysisParameterAsInt(
            "EnableEnergyDeposition");
    int dFlatSurfaceFlux =
        BambooGlobalVariables::Instance()->getAnalysisParameterAsInt(
            "EnableFlatSurfaceFlux");
    int dPrimaryParticle =
        BambooGlobalVariables::Instance()->getAnalysisParameterAsInt(
            "enable_primary_particle");
    int dNullEvent =
        BambooGlobalVariables::Instance()->getAnalysisParameterAsInt(
            "save_null_event");
    int dDecayChainSplitting =
        BambooGlobalVariables::Instance()->getAnalysisParameterAsInt(
            "enable_decay_chain_splitting");
    std::string user_seed_string =
        BambooGlobalVariables::Instance()->getAnalysisParameterAsString(
            "user_seed");
    bool enableEnergyDeposition(true);
    bool enableFlatSurfaceFlux(false);
    bool enablePrimaryParticle(false);
    bool enableDecayChainSplitting(false);

    if (dEnergyDeposition == 0) {
        enableEnergyDeposition = false;
    }
    if (dFlatSurfaceFlux == 1) {
        enableFlatSurfaceFlux = true;
    }
    if (dPrimaryParticle == 1) {
        enablePrimaryParticle = true;
    }
    if (dDecayChainSplitting == 1) {
        enableDecayChainSplitting = true;
    }
    if (dEnergyDeposition != 0 && dEnergyDeposition != 1) {
        G4cout << "EnableEnergyDeposition can only be 0(false) or 1(true). 1 "
                  "will be used in the simulation."
               << G4endl;
    }
    if (dFlatSurfaceFlux != 0 && dFlatSurfaceFlux != 1) {
        G4cout << "EnableFlatSurfaceFlux can only be 0(false) or 1(true). 0 "
                  "will be used in the simulation."
               << G4endl;
    }
    if (dPrimaryParticle != 0 && dPrimaryParticle != 1) {
        G4cout << "enable_primary_particle can only be 0(false) or 1(true). 0 "
                  "will be used in the simulation."
               << G4endl;
    }

    if (dDecayChainSplitting != 0 && dDecayChainSplitting != 1) {
        G4cout << "enable_decay_chain_splitting can only be 0(false) or "
                  "1(true). 0 will be used in the simulation."
               << G4endl;
    }

    long user_seed = -1;
    if (user_seed_string.size() != 0) {
        G4cout << "User seed provided " << user_seed_string << G4endl;
        user_seed = stol(user_seed_string);
    }

    dm = shared_ptr<PandaXDataManager>{new PandaXDataManager(
        enableEnergyDeposition, enableFlatSurfaceFlux, enablePrimaryParticle)};
    if (dNullEvent == 1) {
        dm->saveNullEvent(true);
    }
    _runAction = new PandaXRunAction(dm);
    _eventAction = new PandaXEventAction(dm);
    if (user_seed != -1) {
        dynamic_cast<PandaXEventAction *>(_eventAction)->setUserSeed(user_seed);
    }
    _trackingAction = new PandaXTrackingAction(dm);
    if (enableDecayChainSplitting) {
        _steppingAction = new PandaXSteppingAction(dm);
        _stackingAction = new PandaXStackingAction(dm);
    }
}

PandaXAnalysis::~PandaXAnalysis() {
    G4cout << "Destructing PandaXAnalysis..." << G4endl;
}
