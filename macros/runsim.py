#!/usr/bin/python

#
# runsim.py <args>
#
# Run GEANT4 simulations for the Modulation setup
#
#
#
# A.P. Colijn - colijn@nikhef.nl
#

#--------------------------------------------------------------------------------------------

# where is the modusim code
simulation_basedir = "/user/z37/Modulation/modusim"
# where should the output go
output_basedir = "/data/atlas/users/acolijn/Modulation/simulation"
# where should the scripts for running go?
run_dir = output_basedir + "/macros"

# temporary variables
ran_seed        = 12345
source_name     = "Ti44"
output_basename = source_name + "_"+ str(ran_seed)

output_root     = output_basename + ".root"

preinit_script = ''
run_script = ''

#--------------------------------------------------------------------------------------------
def make_preinit_script():
    #
    # make the preinit_script
    #
    print('make_preinit_script:: start generating script')
    preinit_script = run_dir+'/preinit'+run+'.mac'
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
def make_run_script():
    #
    # make the run_script
    #
    print('make_run_script:: start generating script')
    preinit_script = run_dir+'/preinit'+run+'.mac'
    fout = open(preinit_script,'w')
    
    fout.write('# GEANT4 - G4simu run macro (AUTOMATICALLY GENERATED) \n')
    fout.write('/control/verbose  0 \n')
    fout.write('/run/verbose      0\n')
    fout.write('/event/verbose    0\n')
    fout.write('/tracking/verbose 0\n')
    
    fout.write('/run/random/setRandomSeed 12346\n')
    
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
# MAIN CODE

make_preinit_script()

make_run_script()
