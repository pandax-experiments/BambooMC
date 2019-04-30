#include "detector/BambooDetectorPart.hh"
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>

BambooDetectorPart::BambooDetectorPart(const G4String &partName)
    : partName_{partName}, partLogicalVolume_{0},
      partContainerLogicalVolume_{0}, partPhysicalVolume_{0}, parentPart_{0} {}

void BambooDetectorPart::setParent(BambooDetectorPart *parent) {
    parentPart_ = parent;
}

G4LogicalVolume *BambooDetectorPart::getLogicalVolume() {
    return partLogicalVolume_;
}

G4VPhysicalVolume *BambooDetectorPart::getPhysicalVolume() {
    return partPhysicalVolume_;
}

G4LogicalVolume *BambooDetectorPart::getContainerLogicalVolume() {
    return partContainerLogicalVolume_;
}
