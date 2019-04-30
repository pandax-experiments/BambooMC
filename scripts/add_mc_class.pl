#!/usr/bin/perl -w

use strict;
use 5.010;
use File::Path qw(make_path);
say "Usage: $0 user_mc_name class_type class_name\n\n - class_type could be: physics, generator, analysis\n" and die unless $#ARGV==2;

my $user_mc_name = $ARGV[0];
my $class_type = $ARGV[1];
my $class_name = $ARGV[2];

my $type = $class_type;
$type =~ tr/A-Z/a-z/;
if ($type ne 'physics' && $type ne 'generator' && $type ne 'analysis') {
  say "Usage: $0 user_mc_name class_type class_name\n\n - class_type could be: physics, generator, analysis\n" and die;
}

$type = ucfirst($type);

say "creating new user mc class \"$class_name\" of type \"$class_type\" in \"$user_mc_name\"";

# create the directory of user mc
make_path("user/mc/$user_mc_name", {mode => 0755}) unless -d "user/mc/$user_mc_name";

# create header

my $sample_header = <<"EOF";
#pragma once

#include "BambooFactory.hh"
#include "Bamboo${type}.hh"

class $class_name : public Bamboo${type} {
  public:
    $class_name(const BambooParameters &pars);

    ~$class_name() = default;

    static ${type}Register<$class_name> reg;
};
EOF

open my $header, ">user/mc/${user_mc_name}/${class_name}.hh"
  or die "Can't open file: $!";

print $header $sample_header;
close $header;

# create the source file

my $sample_source = <<"EOF";
#include "${class_name}.hh"

${type}Register<${class_name}>
    ${class_name}::reg("${class_name}");

${class_name}::${class_name}(const BambooParameters &pars)
    : Bamboo${type}{pars} {}
EOF

open my $source, ">user/mc/${user_mc_name}/${class_name}.cc"
  or die "Can't open file: $!";

print $source $sample_source;
close $source;

# create the sources.cmake
my $cmake_file = "user/mc/${user_mc_name}/sources.cmake";
if ( -f $cmake_file ) {
  open my $fh_cmake, ">>$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(set(UserMC \$\{UserMC\} \$\{source_path\}/${class_name}.cc));
  close $fh_cmake;
} else {
  open my $fh_cmake, ">$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(message("enable $user_mc_name"));
  say $fh_cmake qq(set(source_path \$\{PROJECT_SOURCE_DIR\}/user/mc/${user_mc_name}));
  say $fh_cmake qq(set(UserMC \$\{UserMC\} \$\{source_path\}/${class_name}.cc));
  close $fh_cmake;
}
