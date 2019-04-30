#include "BambooUtils.hh"

#include <G4SystemOfUnits.hh>

#include <QString>
#include <map>

namespace BambooUtils {

static std::map<std::string, double> createUnitMap() {
    std::map<std::string, double> uMap;
    uMap["millimeter"] = millimeter;
    uMap["millimeter2"] = millimeter2;
    uMap["millimeter3"] = millimeter3;
    uMap["centimeter"] = centimeter;
    uMap["centimeter2"] = centimeter2;
    uMap["centimeter3"] = centimeter3;
    uMap["meter"] = meter;
    uMap["meter2"] = meter2;
    uMap["meter3"] = meter3;
    uMap["kilometer"] = kilometer;
    uMap["kilometer2"] = kilometer2;
    uMap["kilometer3"] = kilometer3;
    uMap["parsec"] = parsec;
    uMap["micrometer"] = micrometer;
    uMap["nanometer"] = nanometer;
    uMap["angstrom"] = angstrom;
    uMap["fermi"] = fermi;
    uMap["barn"] = barn;
    uMap["millibarn"] = millibarn;
    uMap["microbarn"] = microbarn;
    uMap["nanobarn"] = nanobarn;
    uMap["picobarn"] = picobarn;
    uMap["nm"] = nm;
    uMap["um"] = um;
    uMap["mm"] = mm;
    uMap["mm2"] = mm2;
    uMap["mm3"] = mm3;
    uMap["cm"] = cm;
    uMap["cm2"] = cm2;
    uMap["cm3"] = cm3;
    uMap["m"] = m;
    uMap["m2"] = m2;
    uMap["m3"] = m3;
    uMap["km"] = km;
    uMap["km2"] = km2;
    uMap["km3"] = km3;
    uMap["pc"] = pc;
    uMap["radian"] = radian;
    uMap["milliradian"] = milliradian;
    uMap["degree"] = degree;
    uMap["steradian"] = steradian;
    uMap["rad"] = rad;
    uMap["mrad"] = mrad;
    uMap["sr"] = sr;
    uMap["deg"] = deg;
    uMap["nanosecond"] = nanosecond;
    uMap["second"] = second;
    uMap["millisecond"] = millisecond;
    uMap["microsecond"] = microsecond;
    uMap["picosecond"] = picosecond;
    uMap["hertz"] = hertz;
    uMap["kilohertz"] = kilohertz;
    uMap["megahertz"] = megahertz;
    uMap["ns"] = ns;
    uMap["s"] = s;
    uMap["ms"] = ms;
    uMap["eplus"] = eplus;
    uMap["e_SI"] = e_SI;
    uMap["coulomb"] = coulomb;
    uMap["megaelectronvolt"] = megaelectronvolt;
    uMap["electronvolt"] = electronvolt;
    uMap["kiloelectronvolt"] = kiloelectronvolt;
    uMap["gigaelectronvolt"] = gigaelectronvolt;
    uMap["teraelectronvolt"] = teraelectronvolt;
    uMap["petaelectronvolt"] = petaelectronvolt;
    uMap["joule"] = joule;
    uMap["MeV"] = MeV;
    uMap["eV"] = eV;
    uMap["keV"] = keV;
    uMap["GeV"] = GeV;
    uMap["TeV"] = TeV;
    uMap["PeV"] = PeV;
    uMap["kilogram"] = kilogram;
    uMap["gram"] = gram;
    uMap["milligram"] = milligram;
    uMap["kg"] = kg;
    uMap["g"] = g;
    uMap["mg"] = mg;
    uMap["watt"] = watt;
    uMap["newton"] = newton;
    uMap["hep_pascal"] = hep_pascal;
    uMap["bar"] = bar;
    uMap["atmosphere"] = atmosphere;
    uMap["ampere"] = ampere;
    uMap["milliampere"] = milliampere;
    uMap["microampere"] = microampere;
    uMap["nanoampere"] = nanoampere;
    uMap["megavolt"] = megavolt;
    uMap["kilovolt"] = kilovolt;
    uMap["volt"] = volt;
    uMap["ohm"] = ohm;
    uMap["farad"] = farad;
    uMap["millifarad"] = millifarad;
    uMap["microfarad"] = microfarad;
    uMap["nanofarad"] = nanofarad;
    uMap["picofarad"] = picofarad;
    uMap["weber"] = weber;
    uMap["tesla"] = tesla;
    uMap["gauss"] = gauss;
    uMap["kilogauss"] = kilogauss;
    uMap["henry"] = henry;
    uMap["kelvin"] = kelvin;
    uMap["mole"] = mole;
    uMap["becquerel"] = becquerel;
    uMap["curie"] = curie;
    uMap["candela"] = candela;
    uMap["lumen"] = lumen;
    uMap["lux"] = lux;
    uMap["perCent"] = perCent;
    uMap["perThousand"] = perThousand;
    uMap["perMillion"] = perMillion;
    return uMap;
}

static std::map<std::string, double> unitMap = BambooUtils::createUnitMap();

double evaluate(std::string str) {
    std::string::size_type n = str.find("*");
    double unit(0), value(0);
    std::map<std::string, double>::iterator it;
    if (n != std::string::npos) {
        it = unitMap.find(str.substr(n + 1));
        if (it != unitMap.end()) {
            unit = it->second;
        }
        value = QString(str.substr(0, n).c_str()).toDouble() * unit;
    } else {
        if (str.c_str()[0] >= '0' && str.c_str()[0] <= '9') {
            value = QString(str.substr(0, n).c_str()).toDouble();
        } else {
            it = unitMap.find(str);
            if (it != unitMap.end()) {
                unit = it->second;
            }
            value = unit;
        }
    }
    return value;
}
}
