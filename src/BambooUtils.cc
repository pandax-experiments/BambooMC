#include "BambooUtils.hh"

#include <G4SystemOfUnits.hh>

#include <map>
#include <algorithm>

namespace BambooUtils {

std::map<std::string, double> unit_map{
    {"millimeter", millimeter},
    {"millimeter2", millimeter2},
    {"millimeter3", millimeter3},
    {"centimeter", centimeter},
    {"centimeter2", centimeter2},
    {"centimeter3", centimeter3},
    {"meter", meter},
    {"meter2", meter2},
    {"meter3", meter3},
    {"kilometer", kilometer},
    {"kilometer2", kilometer2},
    {"kilometer3", kilometer3},
    {"parsec", parsec},
    {"micrometer", micrometer},
    {"nanometer", nanometer},
    {"angstrom", angstrom},
    {"fermi", fermi},
    {"barn", barn},
    {"millibarn", millibarn},
    {"microbarn", microbarn},
    {"nanobarn", nanobarn},
    {"picobarn", picobarn},
    {"nm", nm},
    {"um", um},
    {"mm", mm},
    {"mm2", mm2},
    {"mm3", mm3},
    {"cm", cm},
    {"cm2", cm2},
    {"cm3", cm3},
    {"m", m},
    {"m2", m2},
    {"m3", m3},
    {"km", km},
    {"km2", km2},
    {"km3", km3},
    {"pc", pc},
    {"radian", radian},
    {"milliradian", milliradian},
    {"degree", degree},
    {"steradian", steradian},
    {"rad", rad},
    {"mrad", mrad},
    {"sr", sr},
    {"deg", deg},
    {"nanosecond", nanosecond},
    {"second", second},
    {"millisecond", millisecond},
    {"microsecond", microsecond},
    {"picosecond", picosecond},
    {"hertz", hertz},
    {"kilohertz", kilohertz},
    {"megahertz", megahertz},
    {"ns", ns},
    {"s", s},
    {"ms", ms},
    {"eplus", eplus},
    {"e_SI", e_SI},
    {"coulomb", coulomb},
    {"megaelectronvolt", megaelectronvolt},
    {"electronvolt", electronvolt},
    {"kiloelectronvolt", kiloelectronvolt},
    {"gigaelectronvolt", gigaelectronvolt},
    {"teraelectronvolt", teraelectronvolt},
    {"petaelectronvolt", petaelectronvolt},
    {"joule", joule},
    {"MeV", MeV},
    {"eV", eV},
    {"keV", keV},
    {"GeV", GeV},
    {"TeV", TeV},
    {"PeV", PeV},
    {"kilogram", kilogram},
    {"gram", gram},
    {"milligram", milligram},
    {"kg", kg},
    {"g", g},
    {"mg", mg},
    {"watt", watt},
    {"newton", newton},
    {"hep_pascal", hep_pascal},
    {"bar", bar},
    {"atmosphere", atmosphere},
    {"ampere", ampere},
    {"milliampere", milliampere},
    {"microampere", microampere},
    {"nanoampere", nanoampere},
    {"megavolt", megavolt},
    {"kilovolt", kilovolt},
    {"volt", volt},
    {"ohm", ohm},
    {"farad", farad},
    {"millifarad", millifarad},
    {"microfarad", microfarad},
    {"nanofarad", nanofarad},
    {"picofarad", picofarad},
    {"weber", weber},
    {"tesla", tesla},
    {"gauss", gauss},
    {"kilogauss", kilogauss},
    {"henry", henry},
    {"kelvin", kelvin},
    {"mole", mole},
    {"becquerel", becquerel},
    {"curie", curie},
    {"candela", candela},
    {"lumen", lumen},
    {"lux", lux},
    {"perCent", perCent},
    {"perThousand", perThousand},
    {"perMillion", perMillion},
};

double evaluate(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    auto n = str.find("*");
    double unit{0};
    if (n != std::string::npos) {
        auto it = unit_map.find(str.substr(n + 1));
        if (it != unit_map.end()) {
            unit = it->second;
        } else {
            unit = 0;
        }
        return std::stod(str.substr(0, n)) * unit;
    } else {
        if (str[0] >= '0' && str[0] <= '9') {
            return std::stod(str.substr(0, n));
        } else {
            auto it = unit_map.find(str);
            if (it != unit_map.end()) {
                return it->second;
            }
            return 0;
        }
    }
}
} // namespace BambooUtils
