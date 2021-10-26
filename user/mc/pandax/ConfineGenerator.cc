#include "ConfineGenerator.hh"

GeneratorRegister<ConfineGenerator>
    ConfineGenerator::reg("ConfineGenerator");

ConfineGenerator::ConfineGenerator(const BambooParameters &pars)
    : BambooGenerator{pars} {}
