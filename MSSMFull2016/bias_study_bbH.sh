MorphingMSSMFull2016 --output_folder=mssm_bias_alternate_$1 -m MH --postfix="-mttot" --control_region=1 --manual_rebin=true --real_data=true --zmm_fit=false --input_folder_mt=$2 --input_folder_et=$2
#MorphingMSSMFull2016 --output_folder=mssm_bias_nominal -m MH --postfix="-mttot" --control_region=1 --manual_rebin=true --real_data=true --zmm_fit=false --input_folder_mt=Imperial/Default/ --input_folder_et=Imperial/Default/
combineTool.py -M T2W -o "bbPhi.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/mssm_bias_alternate_$1/*
#combineTool.py -M T2W -o "bbPhi.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/mssm_bias_nominal/*

# Nominal toys
#combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
#  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/mssm_bias_nominal/*/bbPhi.root \
#  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 4
# Alternate toys
combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/mssm_bias_alternate_$1/*/bbPhi.root \
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 4
  
for MODEL in nominal alternate_$1; do for TOYS in nominal alternate_$1; do \
combineTool.py -m 90,140,250,450,800,1600,2300,3200 -M Asymptotic --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_bbH -d output/mssm_bias_${MODEL}/et/bbPhi.root --there \
  -n ".bbH.toys.${TOYS}.%(SEED)s" -t 20 -s 1:50:1 --run observed \
  --toysFile "../../mssm_bias_${TOYS}/et/higgsCombine.toys.GenerateOnly.mH90.%(SEED)s.root" \
  --cminPreFit 1 --minimizerStrategy 0 --minimizerTolerance 0.1 --job-mode 'SGE'  --prefix-file ic  --merge 8 \
  --task-name model-${MODEL}-toys-${TOYS}-bbH-et --sub-opts '-q hep.q -l h_rt=0:180:0';done; done
  
for MODEL in nominal alternate_$1; do for TOYS in nominal alternate_$1; do \
combineTool.py -m 90,140,250,450,800,1600,2300,3200 -M Asymptotic --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_bbH -d output/mssm_bias_${MODEL}/mt/bbPhi.root --there \
  -n ".bbH.toys.${TOYS}.%(SEED)s" -t 20 -s 1:50:1 --run observed \
  --toysFile "../../mssm_bias_${TOYS}/mt/higgsCombine.toys.GenerateOnly.mH90.%(SEED)s.root" \
  --cminPreFit 1 --minimizerStrategy 0 --minimizerTolerance 0.1 --job-mode 'SGE'  --prefix-file ic  --merge 8 \
  --task-name model-${MODEL}-toys-${TOYS}-bbH-mt --sub-opts '-q hep.q -l h_rt=0:180:0';done; done
    
    
