#pragma once

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4VHit.hh>

#include <string>

using std::string;

class PandaXSurfaceFluxHit : public G4VHit {
  public:
    PandaXSurfaceFluxHit() = default;
    ~PandaXSurfaceFluxHit() = default;

    const PandaXSurfaceFluxHit &operator=(const PandaXSurfaceFluxHit &right);

    int operator==(const PandaXSurfaceFluxHit &right) const;

    inline void *operator new(size_t);

    inline void operator delete(void *aHit);

    void Draw() {}
    void Print() {}

    double getEnergy() const;
    std::string getTrackName() const;
    std::string getParentName() const;
    G4ThreeVector getMomentum() const;

    void setEnergy(double energy);
    void setTrackName(const std::string &name);
    void setParentName(const std::string &name);
    void setMomentum(const G4ThreeVector &momentum);

    double x;
    double y;
    double z;
    double t;

  private:
    double energy_ = 0;
    std::string trackName_ = "";
    std::string parentName_ = "";
    G4ThreeVector momentum_;
};

using PandaXSurfaceFluxHitsCollection = G4THitsCollection<PandaXSurfaceFluxHit>;

extern G4Allocator<PandaXSurfaceFluxHit> *pandaXSurfaceFluxHitAllocator;

inline void *PandaXSurfaceFluxHit::operator new(size_t) {
    if (!pandaXSurfaceFluxHitAllocator)
        pandaXSurfaceFluxHitAllocator = new G4Allocator<PandaXSurfaceFluxHit>;
    return (void *)pandaXSurfaceFluxHitAllocator->MallocSingle();
}

inline void PandaXSurfaceFluxHit::operator delete(void *aHit) {
    pandaXSurfaceFluxHitAllocator->FreeSingle((PandaXSurfaceFluxHit *)aHit);
}
