#include "GdmlLoader.hh"

#include <G4GDMLParser.hh>

DetectorRegister<GdmlLoader, std::string, BambooParameters> GdmlLoader::reg("GdmlLoader");

GdmlLoader::GdmlLoader (const std::string &n, const BambooParameters &pars)
  : BambooDetector(n, pars) {
    G4cout << "create detector GdmlLoader..." << G4endl;
}

bool GdmlLoader::construct (const BambooParameters &, BambooDetector *) {
    // add construction code here
    using namespace CLHEP;
    const auto &file_name = parameters.getParameter("file_name");
    if (file_name == "")
        return false;
    G4GDMLParser parser;
    parser.Read(file_name);
    mainPV = parser.GetWorldVolume();
    mainLV = mainPV->GetLogicalVolume();
    return true;
}

