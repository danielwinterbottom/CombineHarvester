MorphingMSSMFull2016 --output_folder=$1 -m MH --postfix="-mttot" --control_region=1 --manual_rebin=true --real_data=false --zmm_fit=false --input_folder_mt=$2 --input_folder_et=$2 

combineTool.py -M T2W -o "ws.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/$1/*

combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/$1/et/ws.root
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 6
  
combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/$1/mt/ws.root
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 6

#set up workspace for nominal (default) datacards (only needs to be done once)
MorphingMSSMFull2016 --output_folder=default2 -m MH --postfix="-mttot" --control_region=1 --manual_rebin=true --real_data=false --zmm_fit=false --input_folder_mt=Imperial/Default/ --input_folder_et=Imperial/Default/
combineTool.py -M T2W -o "ws.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/default2/*

combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/default2/et/ws.root
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 6

combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/default2/mt/ws.root
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 6


for MODEL in default2 "$1"; do for TOYS in default2 "$1"; do \
  combineTool.py -m "90,140,250,450,800,1600,2300,3200" -M Asymptotic --boundlist input/mssm_boundaries.json \
    --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_bbH -d output/${MODEL}/et/ws.root --there \
    -n ".bbH.toys.${TOYS}.%(SEED)s" -t 20 -s 1:50:1 --run observed \
    --toysFile "../../${TOYS}/et/higgsCombine.toys.GenerateOnly.mH90.%(SEED)s.root" \
    --cminPreFit 1 --minimizerStrategy 0 --minimizerTolerance 0.1 --merge 2 --job-mode 'SGE'  --prefix-file ic  \
    --task-name model-${MODEL}-toys-${TOYS}-bbH-et --sub-opts '-q hep.q -l h_rt=0:180:0' ;done; done
    
for MODEL in default2 "$1"; do for TOYS in default2 "$1"; do \
  combineTool.py -m "90,140,250,450,800,1600,2300,3200" -M Asymptotic --boundlist input/mssm_boundaries.json \
    --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_bbH -d output/${MODEL}/mt/ws.root --there \
    -n ".bbH.toys.${TOYS}.%(SEED)s" -t 20 -s 1:50:1 --run observed \
    --toysFile "../../${TOYS}/mt/higgsCombine.toys.GenerateOnly.mH90.%(SEED)s.root" \
    --cminPreFit 1 --minimizerStrategy 0 --minimizerTolerance 0.1 --merge 2 --job-mode 'SGE'  --prefix-file ic  \
    --task-name model-${MODEL}-toys-${TOYS}-bbH-mt --sub-opts '-q hep.q -l h_rt=0:180:0' ;done; done
