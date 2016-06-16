#!/bin/csh

#--------------------------------------------------------------------------------------------

#
# Setup script for GEANT4 + ROOT6 with /cvmfs/
#
# 1) setup your geant4 version
# 2) use Nikhef setup for ROOT - NOTE: should be improved for other sites to use it as well
#
# A.P.Colijn 
setenv BASEDIR `pwd`

#cd /cvmfs/sft.cern.ch/lcg/external/gcc/4.8.1/x86_64-slc6-gcc48-opt
source setup.csh

#--------------------------------------------------------------------------------------------
# setup GEANT4
#source /cvmfs/geant4.cern.ch/etc/login.csh
cd /cvmfs/geant4.cern.ch/geant4/10.2.p01/x86_64-slc6-gcc48-opt/bin/
source geant4.csh
#cd /cvmfs/geant4.cern.ch/geant4/10.1.p02/x86_64-slc6-gcc48-opt/bin/
#cd /cvmfs/geant4.cern.ch/geant4/9.5.p02/x86_64-slc5-gcc43-opt/bin
cd ${BASEDIR}

#--------------------------------------------------------------------------------------------
# setup root
### source /project/atlas/nikhef/cvmfs/setup.sh
### setupATLAS
#localSetupROOT 5.34.19-x86_64-slc6-gcc47-opt
#localSetupROOT 5.34.19-x86_64-slc5-gcc43-opt
### localSetupROOT 6.04.10-x86_64-slc6-gcc48-opt
 
#source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.04.12/x86_64-slc6-gcc48-opt/root/bin/thisroot.csh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_84/ROOT/6.06.02/x86_64-slc6-gcc48-opt/bin/thisroot.csh

#--------------------------------------------------------------------------------------------

# setup gcc to make sure you still use the right version of gcc and g++
#source /cvmfs/sft.cern.ch/lcg/external/gcc/4.8.1/x86_64-slc6-gcc48-opt/setup.csh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_84/gcc/4.8.4/x86_64-slc6/setup.csh

#export CXX=`which g++`
#export CC=`which gcc`
setenv CXX `which g++`
setenv CC  `which gcc`
cd $BASEDIR
