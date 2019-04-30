#include "BambooDetectorConstruction.hh"
#include "BambooFactory.hh"

#include <G4GeometryManager.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>
#include <G4MaterialTable.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4RunManager.hh>
#include <G4SolidStore.hh>
#include <G4VPhysicalVolume.hh>

#include <G4Box.hh>
#include <G4PVPlacement.hh>

G4VPhysicalVolume *BambooDetectorConstruction::Construct() {

    DefineMaterials();

    using factory = DetectorFactory<std::string, BambooParameters>;
    G4VPhysicalVolume *world = nullptr;
    for (const auto &di : detectorInfo) {
        const auto &name = std::get<0>(di);
        const auto &type = std::get<1>(di);
        const auto &parent = std::get<2>(di);
        auto pd = factory::create(type, name, detectorParameterMaps.at(name));
        if (pd.get() == nullptr) {
            std::cerr << "detector " << name << " is not properly registered!"
                      << std::endl;
            exit(1);
        }
        if (parent == "") {
            pd->construct(geometryParameters);
            world = pd->getMainPV();
        } else {
            pd->construct(geometryParameters, detectorMap.at(parent).get());
        }
        detectorMap.emplace(name, std::move(pd));
    }
    return world;
}

void BambooDetectorConstruction::DefineMaterials() {
    if (materialName != "") {
        auto material = MaterialFactory::create(materialName);
        material->defineMaterials(materialParameters);
    }
}
