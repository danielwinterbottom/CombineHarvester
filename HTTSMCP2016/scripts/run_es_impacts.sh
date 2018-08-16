
#cd output/fake_es/htt_et_1_13TeV/
#
#combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=980,1080 --setPhysicsModelParameters faketaues=1016 --doInitialFit --parallel 8 
#
#combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=980,1080 --setPhysicsModelParameters faketaues=1016 --doFits --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --job-mode 'SGE'  --prefix-file ic --sub-opts "-q hep.q -l h_rt=0:180:0" --merge=2
#
#cd ../../../
#
#cd output/fake_es/htt_et_2_13TeV/
#
#combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=1000,1100 --setPhysicsModelParameters faketaues=1040 --doInitialFit  --parallel 8 
#
#combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=1000,1100 --setPhysicsModelParameters faketaues=1040 --doFits --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --job-mode 'SGE'  --prefix-file ic --sub-opts "-q hep.q -l h_rt=0:180:0" --merge=2
#
#cd ../../../

cd output/fake_es/htt_mt_1_13TeV/

combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=990,1010 --setPhysicsModelParameters faketaues=999 --doInitialFit --parallel 8 

combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=990,1010  --setPhysicsModelParameters faketaues=999 --doFits --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --job-mode 'SGE'  --prefix-file ic --sub-opts "-q hep.q -l h_rt=0:180:0" --merge=2

cd ../../../

cd output/fake_es/htt_mt_2_13TeV/

combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=970,1060 --setPhysicsModelParameters faketaues=1011 --doInitialFit --parallel 8 

combineTool.py -M Impacts -d ws.root -m 125 --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=970,1060 --setPhysicsModelParameters faketaues=1011  --doFits --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --job-mode 'SGE'  --prefix-file ic --sub-opts "-q hep.q -l h_rt=0:180:0" --merge=2

cd ../../../
