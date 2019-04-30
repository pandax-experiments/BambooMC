#include "PandaXAnalysis.hh"
#include "BambooUtils.hh"
#include "PandaXDataManager.hh"
#include "PandaXEventAction.hh"
#include "PandaXRunAction.hh"
#include "PandaXStackingAction.hh"
#include "PandaXSteppingAction.hh"
#include "PandaXTrackingAction.hh"

AnalysisRegister<PandaXAnalysis> PandaXAnalysis::reg("PandaXAnalysis");

PandaXAnalysis::PandaXAnalysis(const BambooParameters &pars)
    : BambooAnalysis(pars) {
    bool enable_energy_deposition = true;
    if (analysisParameters.getParameters().find("EnableEnergyDeposition") !=
        analysisParameters.getParameters().end())
        enable_energy_deposition =
            analysisParameters.getParameter<bool>("EnableEnergyDeposition");
    bool enable_surface_flux =
        analysisParameters.getParameter<bool>("EnableSurfaceFlux");
    bool enable_primary_particle =
        analysisParameters.getParameter<bool>("EnablePrimaryParticle");
    bool save_null_events =
        analysisParameters.getParameter<bool>("SaveNullEvents");
    bool enable_decay_chain_splitting =
        analysisParameters.getParameter<bool>("EnableDecayChainSplitting");
    long user_seed = analysisParameters.getParameter<long>("UserSeed");

    auto &dm = PandaXDataManager::getInstance();
    dm.setRecordEnergyDeposition(enable_energy_deposition);
    dm.setRecordSurfaceFlux(enable_surface_flux);
    dm.setRecordPrimaryParticle(enable_primary_particle);
    dm.setRecordNullEvent(save_null_events);

    runAction =
        new PandaXRunAction(analysisParameters.getParameter("DataFileName"));
    eventAction = new PandaXEventAction();
    if (user_seed > 0) {
        dynamic_cast<PandaXEventAction *>(eventAction)->setUserSeed(user_seed);
    }
    trackingAction = new PandaXTrackingAction();
    if (enable_decay_chain_splitting) {
        double chainSplittingLifeTime = 300 * CLHEP::microsecond;
        const auto &pmap = analysisParameters.getParameters();
        if (pmap.find("ChainSplittingLifeTime") != pmap.end()) {
            chainSplittingLifeTime = BambooUtils::evaluate(
                analysisParameters.getParameter("ChainSplittingLifeTime"));
        }
        if (chainSplittingLifeTime == 0) {
            chainSplittingLifeTime = 300 * CLHEP::microsecond;
        }
        steppingAction = new PandaXSteppingAction(chainSplittingLifeTime);
        stackingAction = new PandaXStackingAction(chainSplittingLifeTime);
    }
}
