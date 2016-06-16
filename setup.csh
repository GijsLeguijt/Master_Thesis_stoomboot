set gcc_config_version = 4.8.4
set LCGPLAT = x86_64-slc6
set LCG_lib_name = lib64
set LCG_arch = x86_64

set LCG_contdir = /cvmfs/sft.cern.ch/lcg/releases/LCG_84
set LCG_gcc_home = ${LCG_contdir}/gcc/${gcc_config_version}/${LCGPLAT}

setenv PATH ${LCG_gcc_home}/bin:${PATH}
setenv COMPILER_PATH ${LCG_gcc_home}/lib/gcc/${LCG_arch}-unknown-linux-gnu/${gcc_config_version}

if ($?LD_LIBRARY_PATH) then
setenv LD_LIBRARY_PATH ${LCG_gcc_home}/${LCG_lib_name}:${LD_LIBRARY_PATH}
else
setenv LD_LIBRARY_PATH ${LCG_gcc_home}/${LCG_lib_name}
endif

echo $PATH
echo $LCG_gcc_home
