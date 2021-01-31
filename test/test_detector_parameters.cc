#include "BambooControl.hh"
#include "BambooUtils.hh"

#include <G4SystemOfUnits.hh>

#include <iostream>
#include <typeinfo>

int main() {
    BambooParameters dp;
    dp.addParameter("x", "1");
    dp.addParameter("transparent", "0");
    dp.addParameter("z", "2.75");
    dp.addParameter("mass", "2.0 * kg");

    std::cout << dp.getParameter<int>("x") << std::endl;
    std::cout << std::boolalpha << dp.getParameter<bool>("transparent") << std::endl;
    std::cout << dp.getParameter<double>("z") << std::endl;
    std::cout << dp.getParameter("x") << std::endl;
    std::cout << dp.getParameter("mass") << std::endl;
    std::cout << dp.evaluateParameter("mass") << std::endl;
    std::cout << BambooUtils::evaluate("5*mm") << std::endl;
    std::cout << BambooUtils::evaluate("5*km") << std::endl;
    std::cout << BambooUtils::evaluate("20 * electronvolt") << std::endl;
    std::cout << BambooUtils::evaluate("20 * electronvoltx") << std::endl;
}
