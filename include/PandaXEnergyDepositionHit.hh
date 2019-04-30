#pragma once

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>

#include <string>

using std::string;

class PandaXEnergyDepositionHit : public G4VHit {
  public:
    PandaXEnergyDepositionHit();
    ~PandaXEnergyDepositionHit() = default;

    const PandaXEnergyDepositionHit &
    operator=(const PandaXEnergyDepositionHit &right);

    int operator==(const PandaXEnergyDepositionHit &right) const;

    inline void *operator new(size_t);

    inline void operator delete(void *aHit);

    void Draw() {}
    void Print() {}

    int getTrackId() const;
    int getParentId() const;
    double getEnergy() const;
    double getX() const;
    double getY() const;
    double getZ() const;
    double getT() const;
    const string &getType() const;
    const string &getCreatorProcess() const;
    const string &getDepositionProcess() const;
    const string &getParent() const;

    void setTrackId(int id);
    void setParentId(int id);
    void setEnergy(double energy);
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setT(double t);
    void setType(const string &type);
    void setCreatorProcess(const string &process);
    void setDepositionProcess(const string &process);
    void setParent(const string &parent);

  private:
    int trackId_;
    int parentId_;
    double energy_;
    double x_;
    double y_;
    double z_;
    double t_;
    string type_;
    string creatorProcess_;
    string depositionProcess_;
    string parent_;
};

using PandaXEnergyDepositionHitsCollection =
    G4THitsCollection<PandaXEnergyDepositionHit>;

extern G4Allocator<PandaXEnergyDepositionHit>
    *pandaXEnergyDepositionHitAllocator;

inline void *PandaXEnergyDepositionHit::operator new(size_t) {
    if (!pandaXEnergyDepositionHitAllocator)
        pandaXEnergyDepositionHitAllocator =
            new G4Allocator<PandaXEnergyDepositionHit>;
    return (void *)pandaXEnergyDepositionHitAllocator->MallocSingle();
}

inline void PandaXEnergyDepositionHit::operator delete(void *aHit) {
    pandaXEnergyDepositionHitAllocator->FreeSingle(
        (PandaXEnergyDepositionHit *)aHit);
}
