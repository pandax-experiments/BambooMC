#include "PandaXOpticalAnalysis.hh"

#include "PandaXOpticalDataManager.hh"
#include "PandaXOpticalEventAction.hh"
#include "PandaXOpticalRunAction.hh"
#include "PandaXOpticalSteppingAction.hh"
#include "PandaXOpticalTrackingAction.hh"

AnalysisRegister<PandaXOpticalAnalysis>
    PandaXOpticalAnalysis::reg("PandaXOpticalAnalysis");

PandaXOpticalAnalysis::PandaXOpticalAnalysis(const BambooParameters &pars)
    : BambooAnalysis{pars} {
    bool enable_energy_deposition = true;
    if (analysisParameters.getParameters().find("EnableEnergyDeposition") !=
        analysisParameters.getParameters().end())
        enable_energy_deposition =
            analysisParameters.getParameter<bool>("EnableEnergyDeposition");

    auto &dm = PandaXOpticalDataManager::getInstance();
    dm.setRecordEnergyDeposition(enable_energy_deposition);
    auto sdName = analysisParameters.getParameter("OpticalSdName");

    runAction = new PandaXOpticalRunAction(
        analysisParameters.getParameter("DataFileName"));
    eventAction = new PandaXOpticalEventAction();

    steppingAction = new PandaXOpticalSteppingAction(sdName);

    trackingAction = new PandaXOpticalTrackingAction();
}
