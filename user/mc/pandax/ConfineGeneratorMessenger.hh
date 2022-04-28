#pragma once

#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

#include <memory>

#include <set>
#include <vector>

class ConfineGenerator;

class ConfineGeneratorMessenger : public G4UImessenger {
  public:
    ConfineGeneratorMessenger(ConfineGenerator *gen);
    virtual ~ConfineGeneratorMessenger() = default;

    virtual void SetNewValue(G4UIcommand *command, G4String newValues);

  private:
    ConfineGenerator *myGen;
    std::unique_ptr<G4UIdirectory> cgsDir;
    std::unique_ptr<G4UIcmdWithAString> shapeCmd;
    std::unique_ptr<G4UIcmdWithAString> confineCmd;
    std::unique_ptr<G4UIdirectory> confineDir;
    std::unique_ptr<G4UIcmdWithAString> confineMaterialCmd;
    std::unique_ptr<G4UIcmdWithAString> particleCmd;
    std::unique_ptr<G4UIcommand> ionCmd;
    std::unique_ptr<G4UIcmdWith3VectorAndUnit> centerCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> radiusCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> halfXCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> halfYCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> halfZCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> energyCmd;
    std::unique_ptr<G4UIcmdWith3Vector> directionCmd;
    std::unique_ptr<G4UIdirectory> angularDir;
    std::unique_ptr<G4UIcmdWithAString> angularTypeCmd;
};
