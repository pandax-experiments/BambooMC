#include "PandaXOpticalAnalysis.hh"

#include "PandaXOpticalRunAction.hh"
#include "PandaXOpticalEventAction.hh"
#include "PandaXOpticalSteppingAction.hh"
#include "PandaXOpticalTrackingAction.hh"

AnalysisRegister<PandaXOpticalAnalysis>
    PandaXOpticalAnalysis::reg("PandaXOpticalAnalysis");

PandaXOpticalAnalysis::PandaXOpticalAnalysis(const BambooParameters &pars)
    : BambooAnalysis{pars} {
    auto sdName =analysisParameters.getParameter("sdName");

    runAction =
        new PandaXOpticalRunAction(analysisParameters.getParameter("DataFileName"));
    eventAction = new PandaXOpticalEventAction();

    steppingAction = new PandaXOpticalSteppingAction(sdName);

    trackingAction = new PandaXOpticalTrackingAction();
}
