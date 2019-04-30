#pragma once

#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

#include <memory>

class EventLoader;

class EventLoaderMessenger : public G4UImessenger {
  public:
    EventLoaderMessenger(EventLoader *loader);
    virtual ~EventLoaderMessenger() = default;

    virtual void SetNewValue(G4UIcommand *command, G4String newValues);

  private:
    EventLoader *myLoader;
    std::unique_ptr<G4UIdirectory> loaderDir;
    std::unique_ptr<G4UIcmdWithAString> shapeCmd;
    std::unique_ptr<G4UIcmdWithAString> confineCmd;
    std::unique_ptr<G4UIcmdWith3VectorAndUnit> centerCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> radiusCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> heightCmd;
    std::unique_ptr<G4UIcmdWith3Vector> upVecCmd;
};
