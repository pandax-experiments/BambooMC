#include "SampleMaterial.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

#include <vector>

using std::vector;

MaterialRegister<SampleMaterial> SampleMaterial::reg("SampleMaterial");

void SampleMaterial::defineMaterials(const BambooParameters &) {
    using namespace CLHEP;
    auto *pNistManager = G4NistManager::Instance();

    vector<G4Element *> elements;
    vector<G4Material *> materials;
    auto H = pNistManager->FindOrBuildElement(1);
    elements.push_back(H);
    auto C = pNistManager->FindOrBuildElement(6);
    elements.push_back(C);
    auto N = pNistManager->FindOrBuildElement(7);
    elements.push_back(N);
    auto O = pNistManager->FindOrBuildElement(8);
    elements.push_back(O);
    auto F = pNistManager->FindOrBuildElement(9);
    elements.push_back(F);
    auto Mg = pNistManager->FindOrBuildElement(12);
    elements.push_back(Mg);
    auto Al = pNistManager->FindOrBuildElement(13);
    elements.push_back(Al);
    auto Si = pNistManager->FindOrBuildElement(14);
    elements.push_back(Si);
    auto P = pNistManager->FindOrBuildElement(15);
    elements.push_back(P);
    auto S = pNistManager->FindOrBuildElement(16);
    elements.push_back(S);
    auto Cl = pNistManager->FindOrBuildElement(17);
    elements.push_back(Cl);
    auto K = pNistManager->FindOrBuildElement(19);
    elements.push_back(K);
    auto Ca = pNistManager->FindOrBuildElement(20);
    elements.push_back(Ca);
    auto Cr = pNistManager->FindOrBuildElement(24);
    elements.push_back(Cr);
    auto Mn = pNistManager->FindOrBuildElement(25);
    elements.push_back(Mn);
    auto Fe = pNistManager->FindOrBuildElement(26);
    elements.push_back(Fe);
    auto Ni = pNistManager->FindOrBuildElement(28);
    elements.push_back(Ni);
    auto Cu = pNistManager->FindOrBuildElement(29);
    elements.push_back(Cu);
    auto Pb = pNistManager->FindOrBuildElement(82);
    elements.push_back(Pb);
    auto Sn = pNistManager->FindOrBuildElement(50);
    elements.push_back(Sn);
    G4cout << "Avaliable elements: " << G4endl;
    for (auto pe : elements) {
        G4cout << pe->GetName() << " ";
    }
    G4cout << G4endl;

    auto concrete = pNistManager->FindOrBuildMaterial("G4_CONCRETE");
    materials.push_back(concrete);

    auto marble = new G4Material("MARBLE", 2.71 * g / cm3, 7, kStateSolid);
    marble->AddElement(C, 11.88 * 0.01);
    marble->AddElement(O, 47.91 * 0.01);
    marble->AddElement(Mg, 5.58 * 0.01);
    marble->AddElement(Al, 1.535 * 0.01);
    marble->AddElement(Si, 1.27 * 0.01);
    marble->AddElement(K, 1.535 * 0.01);
    marble->AddElement(Ca, 30.29 * 0.01);
    materials.push_back(marble);

    G4double vacuumAtomicNumber = 1.;
    G4double vacuumMassOfMole = 1.008 * g / mole;
    G4double vacuumDensity = 1.e-25 * g / cm3;
    G4double vacuumTemperature = 2.73 * kelvin;
    G4double vacuumPressure = 3.0e-18 * pascal;
    auto vacuum = new G4Material("vacuum", vacuumAtomicNumber, vacuumMassOfMole,
                                 vacuumDensity, kStateGas, vacuumTemperature,
                                 vacuumPressure);
    materials.push_back(vacuum);

    auto air = pNistManager->FindOrBuildMaterial("G4_AIR");
    materials.push_back(air);

    auto water = pNistManager->FindOrBuildMaterial("G4_WATER");
    materials.push_back(water);

    auto polyEthylene = pNistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
    materials.push_back(polyEthylene);

    auto teflon = pNistManager->FindOrBuildMaterial("G4_TEFLON");
    materials.push_back(teflon);

    auto copper = pNistManager->FindOrBuildMaterial("G4_Cu");
    materials.push_back(copper);

    auto liquidXenon = pNistManager->FindOrBuildMaterial("G4_lXe");
    materials.push_back(liquidXenon);

    auto stainlessSteel =
        pNistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    materials.push_back(stainlessSteel);

    G4cout << "Available materials: " << G4endl;
    for (auto pm : materials) {
        G4cout << pm->GetName() << " ";
    }
    G4cout << G4endl;
}
