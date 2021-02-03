#pragma once

#include <G4Material.hh>
#include <G4VUserDetectorConstruction.hh>

#include "BambooControl.hh"

class G4VPhysicalVolume;

class BambooDetectorConstruction : public G4VUserDetectorConstruction {
  public:
    BambooDetectorConstruction(
        const DetectorInfoVec &info,
        const std::map<std::string, BambooParameters> &pmaps)
        : detectorInfo(info), detectorParameterMaps(pmaps) {}

    virtual G4VPhysicalVolume *Construct();

  private:
    //    void DefineMaterials();
    const DetectorInfoVec &detectorInfo;
    const std::map<std::string, BambooParameters> &detectorParameterMaps;

    std::map<std::string, std::unique_ptr<BambooDetector>> detectorMap;
};
