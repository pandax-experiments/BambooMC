#pragma once

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4VHit.hh>

#include <string>

using std::string;

class PandaXOpticalHit : public G4VHit {
  public:
    PandaXOpticalHit() = default;
    ~PandaXOpticalHit() = default;

    const PandaXOpticalHit &operator=(const PandaXOpticalHit &right);
    G4int operator==(const PandaXOpticalHit &right) const;

    inline void *operator new(size_t);

    inline void operator delete(void *aHit);

    void Draw() {}
    void Print() {}

    void setPMTNumber(int n) { pmt_number = n; }
    int getPMTNumber() const { return pmt_number; }

    void setEnergy(double v) { energy = v; }
    double getEnergy() const { return energy; }

    void setPosition(const G4ThreeVector &v) { pos = v; }
    const G4ThreeVector &getPosition() const { return pos; }

    void setCreatorProcess(const string &s) { creatorProcess = s; }
    const string &getCreatorProcess() const { return creatorProcess; }

    void setGlobalTime(double v) { t_global = v; }
    double getGlobalTime() const { return t_global; }

    void setLocalTime(double v) { t_local = v; }
    double getLocalTime() const { return t_local; }

    void setProperTime(double v) { t_proper = v; }
    double getProperTime() const { return t_proper; }

    void setVelocity(double v) { velocity = v; }
    double getVelocity() const { return velocity; }

    void setSourcePos(const G4ThreeVector &v) { source_pos = v; }
    const G4ThreeVector &getSourcePos() const { return source_pos; }

    void setParent(const std::string &s) { parent = s; }
    const std::string &getParent() { return parent; }

    void setParentId(int n) { parentId = n; }
    int getParentId() { return parentId; }

  private:
    int pmt_number = -1;
    G4ThreeVector pos = {0, 0, 0};
    double energy = 0;
    string creatorProcess = "";
    double t_global = 0;
    double t_local = 0;
    double t_proper = 0;
    double velocity = 0;

    G4ThreeVector source_pos = {0, 0, 0};
    string parent = "";
    int parentId = -1;
};

using PandaXOpticalHitsCollection = G4THitsCollection<PandaXOpticalHit>;

extern G4ThreadLocal G4Allocator<PandaXOpticalHit> *pandaXOpticalHitAllocator;

inline void *PandaXOpticalHit::operator new(size_t) {
    if (!pandaXOpticalHitAllocator)
        pandaXOpticalHitAllocator = new G4Allocator<PandaXOpticalHit>;
    return (void *)pandaXOpticalHitAllocator->MallocSingle();
}

inline void PandaXOpticalHit::operator delete(void *aHit) {
    pandaXOpticalHitAllocator->FreeSingle((PandaXOpticalHit *)aHit);
}
