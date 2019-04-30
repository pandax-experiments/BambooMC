#include "detector/SampleMaterial.hh"
#include "BambooGlobalVariables.hh"
#include "detector/BambooMaterialFactory.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

#include <vector>

using std::vector;

MaterialRegister<SampleMaterial> SampleMaterial::reg("SampleMaterial");

SampleMaterial::SampleMaterial(const G4String &name) : BambooMaterial(name) {
    G4cout << "SampleMaterial found..." << G4endl;
}

void SampleMaterial::defineMaterials() {
    G4NistManager *pNistManager = G4NistManager::Instance();

    vector<G4Element *> elementVec;
    vector<G4Material *> materialVec;
    G4Element *H = pNistManager->FindOrBuildElement(1);
    elementVec.push_back(H);
    G4Element *C = pNistManager->FindOrBuildElement(6);
    elementVec.push_back(C);
    G4Element *N = pNistManager->FindOrBuildElement(7);
    elementVec.push_back(N);
    G4Element *O = pNistManager->FindOrBuildElement(8);
    elementVec.push_back(O);
    G4Element *F = pNistManager->FindOrBuildElement(9);
    elementVec.push_back(F);
    G4Element *Mg = pNistManager->FindOrBuildElement(12);
    elementVec.push_back(Mg);
    G4Element *Al = pNistManager->FindOrBuildElement(13);
    elementVec.push_back(Al);
    G4Element *Si = pNistManager->FindOrBuildElement(14);
    elementVec.push_back(Si);
    G4Element *P = pNistManager->FindOrBuildElement(15);
    elementVec.push_back(P);
    G4Element *S = pNistManager->FindOrBuildElement(16);
    elementVec.push_back(S);
    G4Element *Cl = pNistManager->FindOrBuildElement(17);
    elementVec.push_back(Cl);
    G4Element *K = pNistManager->FindOrBuildElement(19);
    elementVec.push_back(K);
    G4Element *Ca = pNistManager->FindOrBuildElement(20);
    elementVec.push_back(Ca);
    G4Element *Cr = pNistManager->FindOrBuildElement(24);
    elementVec.push_back(Cr);
    G4Element *Mn = pNistManager->FindOrBuildElement(25);
    elementVec.push_back(Mn);
    G4Element *Fe = pNistManager->FindOrBuildElement(26);
    elementVec.push_back(Fe);
    G4Element *Ni = pNistManager->FindOrBuildElement(28);
    elementVec.push_back(Ni);
    G4Element *Cu = pNistManager->FindOrBuildElement(29);
    elementVec.push_back(Cu);
    G4Element *Pb = pNistManager->FindOrBuildElement(82);
    elementVec.push_back(Pb);
    G4Element *Sn = pNistManager->FindOrBuildElement(50);
    elementVec.push_back(Sn);
    G4cout << "Avaliable elements: " << G4endl;
    for (size_t i = 0; i < elementVec.size(); ++i) {
        G4cout << elementVec[i]->GetName() << " ";
    }
    G4cout << G4endl;

    G4Material *concrete = pNistManager->FindOrBuildMaterial("G4_CONCRETE");
    materialVec.push_back(concrete);

    G4Material *marble =
        new G4Material("MARBLE", 2.71 * g / cm3, 7, kStateSolid);
    marble->AddElement(C, 11.88 * 0.01);
    marble->AddElement(O, 47.91 * 0.01);
    marble->AddElement(Mg, 5.58 * 0.01);
    marble->AddElement(Al, 1.535 * 0.01);
    marble->AddElement(Si, 1.27 * 0.01);
    marble->AddElement(K, 1.535 * 0.01);
    marble->AddElement(Ca, 30.29 * 0.01);
    materialVec.push_back(marble);

    G4double vacuumAtomicNumber = 1.;
    G4double vacuumMassOfMole = 1.008 * g / mole;
    G4double vacuumDensity = 1.e-25 * g / cm3;
    G4double vacuumTemperature = 2.73 * kelvin;
    G4double vacuumPressure = 3.0e-18 * pascal;
    G4Material *vacuum = new G4Material(
        "vacuum", vacuumAtomicNumber, vacuumMassOfMole, vacuumDensity,
        kStateGas, vacuumTemperature, vacuumPressure);
    materialVec.push_back(vacuum);

    G4Material *air = pNistManager->FindOrBuildMaterial("G4_AIR");
    materialVec.push_back(air);

    G4Material *water = pNistManager->FindOrBuildMaterial("G4_WATER");
    materialVec.push_back(water);

    G4Material *polyEthylene =
        pNistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
    materialVec.push_back(polyEthylene);

    G4Material *teflon = pNistManager->FindOrBuildMaterial("G4_TEFLON");
    materialVec.push_back(teflon);

    G4Material *copper = pNistManager->FindOrBuildMaterial("G4_Cu");
    materialVec.push_back(copper);

    G4Material *liquidXenon = pNistManager->FindOrBuildMaterial("G4_lXe");
    materialVec.push_back(liquidXenon);

    G4Material *stainlessSteel =
        pNistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    materialVec.push_back(stainlessSteel);

    G4cout << "Available materials: " << G4endl;
    for (size_t i = 0; i < materialVec.size(); ++i) {
        G4cout << materialVec[i]->GetName() << " ";
    }
    G4cout << G4endl;
}
