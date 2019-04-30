#include "BambooControl.hh"
#include "BambooGlobalVariables.hh"

#include <G4UImanager.hh>
#include <sstream>
#include <string>
#include <unistd.h>

G4int BambooControl::DETECTOR_TYPE = 0;
G4bool BambooControl::INTERACTIVE = false;
G4int BambooControl::NUM_EVENTS = 0;
G4String BambooControl::MacroFileName = "";
G4String BambooControl::DataFileName = "";
G4String BambooControl::XmlFileName = "";

BambooControl *BambooControl::theControl = 0;

BambooControl *BambooControl::getControl() {
    if (theControl == 0) {
        theControl = new BambooControl();
    }
    return theControl;
}

BambooControl::BambooControl() {}

BambooControl::~BambooControl() {}

void BambooControl::setup(int argc, char *argv[]) {
    extern char *optarg;
    std::stringstream ss;
    while (true) {
        const int option = getopt(argc, argv, "f:m:n:o:x:i");
        if (option == -1)
            break;
        switch (option) {
        case 'f':
        case 'm':
            BambooControl::MacroFileName = optarg;
            break;
        case 'n':
            ss.clear();
            ss.str(optarg);
            ss >> BambooControl::NUM_EVENTS;
            break;
        case 'o':
            BambooControl::DataFileName = optarg;
            BambooGlobalVariables::Instance()->setOutDataName(optarg);
            break;
        case 'x':
            BambooControl::XmlFileName = optarg;
            if (!BambooGlobalVariables::Instance()->loadXMLFile(
                    BambooControl::XmlFileName))
                exit(1);
            break;
        case 'i':
            INTERACTIVE = true;
            break;
        }
    }

    if (XmlFileName.empty()) {
        G4cerr << "An XML file must be provided! Try '-x xml_file'." << G4endl;
        exit(1);
    }

    if (optind <= argc - 1) {
        G4cerr << optind << " vs " << argc - 1 << G4endl;
        G4cerr << "Unrecognized arguments!" << G4endl;
        exit(1);
    }
}
