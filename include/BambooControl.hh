#ifndef BAMBOOCONTROL_H
#define BAMBOOCONTROL_H

#include <globals.hh>

class BambooControl {
  public:
    static BambooControl *getControl();

    virtual ~BambooControl();

    void setup(int argc, char *argv[]);

    static G4int DETECTOR_TYPE;
    static G4bool INTERACTIVE;
    static G4int NUM_EVENTS;
    static G4String MacroFileName;
    static G4String DataFileName;
    static G4String XmlFileName;

  private:
    BambooControl();

    static BambooControl *theControl;
};
#endif
