#ifndef BAMBOOMATERIAL_H
#define BAMBOOMATERIAL_H

#include <globals.hh>

class BambooMaterial {
  public:
    BambooMaterial(const G4String &name);

    virtual ~BambooMaterial() {}

    G4String getName() const { return _name; }

    virtual void defineMaterials() = 0;

  protected:
    G4String _name;
};
#endif
