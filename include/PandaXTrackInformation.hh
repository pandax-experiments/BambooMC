#pragma once

#include <G4VUserTrackInformation.hh>

class PandaXTrackInformation : public G4VUserTrackInformation {
public:
    PandaXTrackInformation() : G4VUserTrackInformation("PandaXTrackInformation") {}

    const G4String & GetParent() const { return parent; }

    void SetParent(const G4String &p) { parent = p; }
private:
    G4String parent = "";
};
