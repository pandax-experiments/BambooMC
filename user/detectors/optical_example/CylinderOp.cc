#include "CylinderOp.hh"
#include "PandaXOpticalSD.hh"

#include <G4LogicalSkinSurface.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>

DetectorRegister<CylinderOp, std::string, BambooParameters>
    CylinderOp::reg("CylinderOp");

CylinderOp::CylinderOp(const std::string &n, const BambooParameters &pars)
    : BambooDetector(n, pars) {
    G4cout << "create detector CylinderOp..." << G4endl;
}

bool CylinderOp::construct(const BambooParameters &, BambooDetector *parent) {
    // add construction code here
    // add construction code here
    if (parent == nullptr || parent->getContainerLV() == nullptr) {
        return false;
    }
    using namespace CLHEP;
    auto height = parameters.evaluateParameter("height");
    auto radius = parameters.evaluateParameter("radius");
    if (height == 0) {
        height = 2 * m;
    }
    if (radius == 0) {
        radius = 1 * m;
    }
    auto thickness = 1.0 * cm;

    defineMaterials();
    auto lXe = G4Material::GetMaterial("LXe");
    auto ptfe = G4Material::GetMaterial("PTFE");
    // the ptfe shell
    auto shellSolid = new G4Tubs("shellSolid", 0, radius + thickness,
                                 height / 2 + thickness, 0, 2 * M_PI);
    mainLV =
        new G4LogicalVolume(shellSolid, ptfe, "CylinderOpShellLV", 0, 0, 0);
    mainPV =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), mainLV, "CylinderOpShell",
                          parent->getContainerLV(), false, 0);
    auto vis_shell = new G4VisAttributes(G4Colour(0.95, 0.95, 0.9, 0.8));
    mainLV->SetVisAttributes(vis_shell);

    // create the optical surface
    G4double ptfe_EFF[] = {1., 1.}; // Enables 'detection' of photons
    G4double ptfe_R[] = {0., 0.};

    G4double energy[] = {7.0 * eV, 7.14 * eV};
    const G4int n = 2;
    auto mat_mt = new G4MaterialPropertiesTable();
    mat_mt->AddProperty("EFFICIENCY", energy, ptfe_EFF, n);
    mat_mt->AddProperty("REFLECTIVITY", energy, ptfe_R, n);

    auto mat_opsurf =
        new G4OpticalSurface("ptfe_opsurf", glisur, polished, dielectric_metal);
    mat_opsurf->SetMaterialPropertiesTable(mat_mt);

    // create logical skin surfaces
    new G4LogicalSkinSurface("ptfe_surf", mainLV, mat_opsurf);

    /////////////////////////////

    auto xenonSoild =
        new G4Tubs("xenonSoild", 0, radius, height / 2, 0, 2 * M_PI);
    containerLV = new G4LogicalVolume(xenonSoild, lXe, "CylinderOpLV", 0, 0, 0);
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), containerLV, "CylinderOp",
                      mainLV, false, 0);
    auto vis = new G4VisAttributes(G4Colour(0.9, 1.0, 0.9, 0.8));
    containerLV->SetVisAttributes(vis);

    auto optical_sd = new PandaXOpticalSD("OpticalSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(optical_sd);
    mainLV->SetSensitiveDetector(optical_sd);
    auto energy_sd = new PandaXOpticalSD("EnergySD", true);
    G4SDManager::GetSDMpointer()->AddNewDetector(energy_sd);
    containerLV->SetSensitiveDetector(energy_sd);
    return true;
}

void CylinderOp::defineMaterials() {
    using namespace CLHEP;
    G4double a, z, density;
    auto lXe = new G4Material("LXe", z = 54., a = 131.29 * g / mole,
                              density = 3.020 * g / cm3);

    auto nist = G4NistManager::Instance();
    auto C = nist->FindOrBuildElement(6);
    auto F = nist->FindOrBuildElement(9);
    auto ptfe = new G4Material("PTFE", density = 2.2 * g / cm3, 2);
    ptfe->AddElement(C, 2);
    ptfe->AddElement(F, 4);

    // material properties tables
    G4double lxe_energy[] = {
        // 3 scintillation + 50 cerenkov
        7.8 * eV,     7.15 * eV,    6.5 * eV,     6.6 * eV,     6.51224 * eV,
        6.42449 * eV, 6.33673 * eV, 6.24898 * eV, 6.16122 * eV, 6.07347 * eV,
        5.98571 * eV, 5.89796 * eV, 5.8102 * eV,  5.72245 * eV, 5.63469 * eV,
        5.54694 * eV, 5.45918 * eV, 5.37143 * eV, 5.28367 * eV, 5.19592 * eV,
        5.10816 * eV, 5.02041 * eV, 4.93265 * eV, 4.8449 * eV,  4.75714 * eV,
        4.66939 * eV, 4.58163 * eV, 4.49388 * eV, 4.40612 * eV, 4.31837 * eV,
        4.23061 * eV, 4.14286 * eV, 4.0551 * eV,  3.96735 * eV, 3.87959 * eV,
        3.79184 * eV, 3.70408 * eV, 3.61633 * eV, 3.52857 * eV, 3.44082 * eV,
        3.35306 * eV, 3.26531 * eV, 3.17755 * eV, 3.0898 * eV,  3.00204 * eV,
        2.91429 * eV, 2.82653 * eV, 2.73878 * eV, 2.65102 * eV, 2.56327 * eV,
        2.47551 * eV, 2.38776 * eV, 2.3 * eV};
    const G4int lxenum_scint = 3;
    const G4int lxenum_ceren = 53;
    G4double lxe_scint[] = {1.0, 1.0, 1.0};

    G4double lxe_rind[] = {
        // 3 + 50
        2.35,    1.75,    1.61,    1.62097, 1.60703, 1.59422, 1.58237, 1.57138,
        1.56114, 1.55158, 1.54261, 1.53419, 1.52625, 1.51876, 1.51168, 1.50497,
        1.49861, 1.49256, 1.4868,  1.48132, 1.47609, 1.4711,  1.46632, 1.46176,
        1.4574,  1.45322, 1.44921, 1.44537, 1.44168, 1.43815, 1.43475, 1.43149,
        1.42836, 1.42535, 1.42246, 1.41968, 1.41701, 1.41444, 1.41197, 1.40959,
        1.40731, 1.40512, 1.40301, 1.40099, 1.39905, 1.39718, 1.3954,  1.39368,
        1.39203, 1.39046, 1.38895, 1.38751, 1.38613};
    ;
    G4double lxe_absl[] = {
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm,
        1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm, 1000. * cm};
    ;

    // please note that rayleigh scattering is missing!

    auto lxe_mt = new G4MaterialPropertiesTable();
    lxe_mt->AddProperty("FASTCOMPONENT", lxe_energy, lxe_scint, lxenum_scint);
    lxe_mt->AddProperty("SLOWCOMPONENT", lxe_energy, lxe_scint, lxenum_scint);
    lxe_mt->AddProperty("RINDEX", lxe_energy, lxe_rind, lxenum_ceren);
    lxe_mt->AddProperty("ABSLENGTH", lxe_energy, lxe_absl, lxenum_ceren);
    lxe_mt->AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV);
    lxe_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    lxe_mt->AddConstProperty("FASTTIMECONSTANT", 20. * ns);
    lxe_mt->AddConstProperty("SLOWTIMECONSTANT", 45. * ns);
    lxe_mt->AddConstProperty("YIELDRATIO", 1.0);

    lXe->SetMaterialPropertiesTable(lxe_mt);
    lXe->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
}
