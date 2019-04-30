#pragma once

#include <string>

#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>

class BambooDetector {
  public:
    BambooDetector(const std::string &n, const BambooParameters &pars)
        : name{n}, parameters{pars} {};

    virtual ~BambooDetector() = default;

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent = nullptr) = 0;

    auto getMainLV() { return mainLV; }
    auto getMainPV() { return mainPV; }
    auto getContainerLV() { return containerLV; }

  protected:
    std::string name;
    BambooParameters parameters;
    G4LogicalVolume *mainLV = nullptr;
    G4LogicalVolume *containerLV = nullptr;
    G4VPhysicalVolume *mainPV = nullptr;
};
