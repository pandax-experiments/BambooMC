#ifndef PANDAXENERGYDEPOSITIONHIT_H
#define PANDAXENERGYDEPOSITIONHIT_H

#include <G4Allocator.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>

#include <string>

using std::string;

class PandaXEnergyDepositionHit : public G4VHit {
  public:
    PandaXEnergyDepositionHit();
    ~PandaXEnergyDepositionHit();

    const PandaXEnergyDepositionHit &
    operator=(const PandaXEnergyDepositionHit &right);

    int operator==(const PandaXEnergyDepositionHit &right) const;

    inline void *operator new(size_t);

    inline void operator delete(void *aHit);

    void Draw() { ; }
    void Print() { ; }

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
    int _trackId;
    int _parentId;
    double _energy;
    double _x;
    double _y;
    double _z;
    double _t;
    string _type;
    string _creatorProcess;
    string _depositionProcess;
    string _parent;
};

typedef G4THitsCollection<PandaXEnergyDepositionHit>
    PandaXEnergyDepositionHitsCollection;

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
#endif // PANDAXENERGYDEPOSITIONHIT_H
