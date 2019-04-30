#ifndef SAMPLEMATERIAL_H
#define SAMPLEMATERIAL_H

#include "detector/BambooMaterial.hh"
#include "detector/BambooMaterialFactory.hh"

class SampleMaterial : public BambooMaterial {

  public:
    SampleMaterial(const G4String &name);

    virtual void defineMaterials();

    static MaterialRegister<SampleMaterial> reg;

  private:
};

#endif // SAMPLEMATERIAL_H
