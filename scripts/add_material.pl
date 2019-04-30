#!/usr/bin/perl -w

use strict;
use 5.010;
use File::Path qw(make_path);
say "Usage: $0 set_name material_name" and die unless $#ARGV==1;

my $set_name = $ARGV[0];
my $material_name = $ARGV[1];

say "creating new material \"$material_name\" in set \"$set_name\"";


# create the directory of set
make_path("user/detectors/$set_name", {mode => 0755}) unless -d "user/detectors/$set_name";

my $uc_name = $material_name;
$uc_name =~ tr/a-z/A-Z/;

my $sample_header = <<"EOF";
#pragma once

#include "BambooMaterial.hh"
#include "BambooFactory.hh"

class $material_name : public BambooMaterial {

  public:
    $material_name() = default;

    virtual void defineMaterials(const BambooParameters &pars);

    static MaterialRegister<$material_name> reg;

  private:
};

EOF

open my $header, ">user/detectors/${set_name}/${material_name}.hh"
  or die "Can't open file: $!";

print $header $sample_header;
close $header;

my $sample_source = <<"EOF";
#include "${material_name}.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

MaterialRegister<$material_name> ${material_name}::reg("$material_name");

void ${material_name}::defineMaterials(const BambooParameters &pars) {
}
EOF

open my $source, ">user/detectors/${set_name}/${material_name}.cc"
  or die "Can't open file: $!";

print $source $sample_source;
close $source;

# create the sources.cmake
my $cmake_file = "user/detectors/${set_name}/sources.cmake";
if ( -f $cmake_file ) {
  open my $fh_cmake, ">>$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(set(Detector \$\{Detector\} \$\{source_path\}/${material_name}.cc));
  close $fh_cmake;
} else {
  open my $fh_cmake, ">$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(message("enable $set_name"));
  say $fh_cmake qq(set(source_path \$\{PROJECT_SOURCE_DIR\}/user/detectors/${set_name}));
  say $fh_cmake qq(set(Detector \$\{Detector\} \$\{source_path\}/${material_name}.cc));
  close $fh_cmake;
}
