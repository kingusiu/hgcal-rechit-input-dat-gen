# determine the location if this file
if [ ! -z "$ZSH_VERSION" ]; then
    this_file="${(%):-%x}"
else
    this_file="${BASH_SOURCE[0]}"
fi
this_dir="$( cd "$( dirname "$this_file" )" && pwd )"

OLDDIR=`pwd`

#DAnalysis
DANALYSISPATH=/afs/cern.ch/user/j/jkiesele/public/DAnalysis_framework/special_HGCal/DAnalysis

#CMSSW and Delphes
cd $DANALYSISPATH/../
cd CMSSW_8_0_4
eval `scramv1 runtime -sh`
cd ..
cd delphes
source DelphesEnv.sh
export DELPHES_PATH=`pwd`
cd $OLDDIR


export PYTHIA8=$CMSSW_RELEASE_BASE/../../../external/pythia8/212-ikhhed3
export LD_LIBRARY_PATH=$PYTHIA8/lib:$LD_LIBRARY_PATH
export DANALYSISPATH=$DANALYSISPATH
export LD_LIBRARY_PATH=$DANALYSISPATH:$LD_LIBRARY_PATH
export PATH=$PATH:$DANALYSISPATH
export LD_LIBRARY_PATH=$this_dir:$LD_LIBRARY_PATH
export PATH=$this_dir:$PATH
