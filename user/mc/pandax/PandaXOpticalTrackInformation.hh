#pragma once

#include <G4ThreeVector.hh>
#include <G4VUserTrackInformation.hh>

class PandaXOpticalTrackInformation : public G4VUserTrackInformation {
  public:
    PandaXOpticalTrackInformation()
        : G4VUserTrackInformation("PandaXOpticalTrackInformation") {}

    const G4String &GetParent() const { return parent; }

    void SetParent(const G4String &p) { parent = p; }

    const G4ThreeVector &GetSourcePos() const { return source_pos; }

    void SetSourcePos(const G4ThreeVector &v) { source_pos = v; }

  private:
    G4String parent = "";
    G4ThreeVector source_pos = {0, 0, 0};
};
