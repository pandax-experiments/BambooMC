#pragma once

#include <string>

#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>

class BambooDetector {
  public:
    BambooDetector(const std::string &n = "", const std::string &p = "")
        : name{n}, parent{p} {};

    virtual ~BambooDetector() = default;

    virtual bool construct(BambooDetector *parent = nullptr) = 0;

    auto getMainLV() { return mainLV; }
    auto getMainPV() { return mainPV; }
    auto getContainerLV() { return containerLV; }

  private:
    std::string name;
    std::string parent;
    G4LogicalVolume *mainLV = nullptr;
    G4LogicalVolume *containerLV = nullptr;
    G4VPhysicalVolume *mainPV = nullptr;
};
