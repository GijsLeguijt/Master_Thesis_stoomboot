#!/usr/bin/python

#
# runsim.py <args>
#
# Run GEANT4 simulations for the Modulation setup
#
#
# A.P. Colijn - colijn@nikhef.nl
#
import sys,os
#--------------------------------------------------------------------------------------------

# where is the modusim code
simulation_basedir = "/user/z37/Modulation/modusim"
# where should the output go
output_basedir = "/data/atlas/users/acolijn/Modulation/simulation"
# where should the scripts for running go?
run_dir = output_basedir + "/macros"

# ROOT VERSION
ROOT_version = "5.34.25-x86_64-slc6-gcc48-opt"
# GEANT4 VERSION
GEANT4_version = "10.1.p02"


# temporary variables
ran_seed        = 12345
source_name     = "Ti44"

#--------------------------------------------------------------------------------------------
def make_G4preinit_script():
    #
    # make the preinit_script
    #
    print('make_preinit_script:: macro ='+preinit_script)
    
    fout = open(preinit_script,'w')
    
    fout.write('# GEANT4 - G4simu pre_init macro (AUTOMATICALLY GENERATED) \n')
    # EM physics list to select: emlivermore, empenelope
    fout.write('/run/physics/setEMlowEnergyModel emlivermore \n')
    # HAD physics list to select: G4QGSP_BERT or G4QGSP_BERT_HP
    fout.write('/run/physics/setHadronicModel    G4QGSP_BERT_HP \n')
    
    # do we like to have the physics histograms or not? true OR false
    fout.write('/run/physics/setHistograms false \n')
    
    # format of the output ntuple = compact OR raw
    fout.write('/G4simu/analysis/setTreeType compact \n')
    
    fout.close()
#--------------------------------------------------------------------------------------------
def make_G4run_script():
    #
    # make the run_script
    #
    print('make_run_script:: macro ='+run_script)

    fout = open(run_script,'w')
    
    fout.write('# GEANT4 - G4simu run macro (AUTOMATICALLY GENERATED) \n')
    fout.write('/control/verbose  0 \n')
    fout.write('/run/verbose      0\n')
    fout.write('/event/verbose    0\n')
    fout.write('/tracking/verbose 0\n')
    
    fout.write('/run/random/setRandomSeed '+str(ran_seed)+'\n')
    
    fout.write('/gps/pos/type    Volume\n')
    fout.write('/gps/pos/shape   Sphere\n')
    fout.write('/gps/pos/centre  0. 0. 0. cm\n')
    fout.write('/gps/pos/radius  1 cm\n')
    fout.write('/gps/pos/confine SourceCore\n')
    fout.write('/gps/ang/type   iso\n')
    
    fout.write('/grdm/BRbias 0\n')
    fout.write('/grdm/fBeta 0\n')
    fout.write('/grdm/verbose 0\n')
    fout.write('/grdm/analogueMC 1\n')
    fout.write('/gps/energy 0. eV\n')
    
    if   source_name == 'Ti44':
        fout.write('/gps/ion 22 44 \n')
    elif source_name == 'Co60':
        fout.write('/gps/ion 27 60 \n')
    elif source_name == 'Cs137':
        fout.write('/gps/ion 55 137 \n')
    
    fout.close()
#--------------------------------------------------------------------------------------------
def make_shell_script():
    #
    # make the shell script for running on stoomboot
    #

    print('make_run_script:: macro ='+run_script)
    fout = open(run_script,'w')
    fout.write('#!/bin/sh\n')

    fout.write('# setup root\n')
    fout.write('source /project/atlas/nikhef/cvmfs/setup.csh\n')
    fout.write('setupATLAS\n')
    fout.write('localSetupROOT '+ROOT_version+'\n')

    fout.write('# setup GEANT4\n')
    fout.write('setenv BASEDIR `pwd`\n')
    fout.write('source /cvmfs/geant4.cern.ch/etc/login.csh\n')
    fout.write('cd /cvmfs/geant4.cern.ch/geant4/'+GEANT4_version+'/x86_64-slc6-gcc48-opt/bin/\n')
    fout.write('source geant4.csh\n')
    fout.write('cd ${BASEDIR} \n')

    fout.write(simulation_basedir+'/../modusim-build/G4simu -p '+preinit_script+' -f '+run_script+' -n '+str(numberOfEvents))

    fout.close()

    cmd = 'chmod +x '+run_script
    os.system(cmd)
#--------------------------------------------------------------------------------------------
# MAIN CODE

numberOfEvents = 123

# output structure and files
output_basename = source_name + "_"+ str(ran_seed)
output_root     = output_basename + ".root"

# macros and scripts
preinit_script = run_dir+'/preinit_'+output_basename+'.mac'
run_script     = run_dir+'/run_'+output_basename+'.mac'
shell_script   = run_dir+'/simu_'+output_basename+'.sh'


# generate the GEANT4 preinit script
make_G4preinit_script()

# generate the GEANT4 run script
make_G4run_script()

# generate run shell script
make_shell_script()



