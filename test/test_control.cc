#include "BambooControl.hh"
#include "BambooUtils.hh"

#include <G4SystemOfUnits.hh>

#include <iostream>
#include <typeinfo>

int main(int argc, char *argv[]) {
    BambooControl control;
    control.setup(argc, argv);
}
