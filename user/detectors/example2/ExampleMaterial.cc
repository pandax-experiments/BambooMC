#include "ExampleMaterial.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

MaterialRegister<ExampleMaterial> ExampleMaterial::reg("ExampleMaterial");

void ExampleMaterial::defineMaterials(const BambooParameters &pars) {
}
