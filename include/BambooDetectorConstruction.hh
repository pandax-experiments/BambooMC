#pragma once

#include <G4Material.hh>
#include <G4VUserDetectorConstruction.hh>

#include "BambooControl.hh"
#include "BambooDetector.hh"

class G4VPhysicalVolume;

class BambooDetectorConstruction : public G4VUserDetectorConstruction {
  public:
    BambooDetectorConstruction(
        const std::string &m_name, const BambooParameters &m_pars,
        const BambooParameters &g_pars, const DetectorInfoVec &info,
        const std::map<std::string, BambooParameters> &pmaps)
        : materialName(m_name), materialParameters(m_pars),
          geometryParameters(g_pars), detectorInfo(info),
          detectorParameterMaps(pmaps) {}

    virtual G4VPhysicalVolume *Construct();

  private:
    void DefineMaterials();
    const std::string materialName;
    const BambooParameters &materialParameters;
    const BambooParameters &geometryParameters;
    const DetectorInfoVec &detectorInfo;
    const std::map<std::string, BambooParameters> &detectorParameterMaps;

    std::map<std::string, std::unique_ptr<BambooDetector>> detectorMap;
};
