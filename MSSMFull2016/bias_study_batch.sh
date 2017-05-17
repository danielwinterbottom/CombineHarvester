MorphingMSSMFull2016 --output_folder=$1 -m MH --postfix="-mttot" --control_region=1 --manual_rebin=true --real_data=false --zmm_fit=false --input_folder_mt=$2 --input_folder_et=$2 

#combineTool.py -M T2W -o "ws.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/default/*
combineTool.py -M T2W -o "ws.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/$1/*

combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/default/cmb/ws.root
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 6

combineTool.py -m 90 -M GenerateOnly --boundlist input/mssm_boundaries.json \
  --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/$1/cmb/ws.root
  -n ".toys" -t 20 --there --saveToys -s 1:50:1 --parallel 6

for MODEL in default "$1"; do for TOYS in default "$1"; do \
  combineTool.py -m "90,140,250,450,800,1600,2300,3200" -M Asymptotic --boundlist input/mssm_boundaries.json \
    --setPhysicsModelParameters r_ggH=0,r_bbH=0 -d output/${MODEL}/cmb/ws.root --there \
    -n ".ggH.toys.${TOYS}.%(SEED)s" -t 20 -s 1:50:1 --run observed \
    --toysFile "../../${TOYS}/cmb/higgsCombine.toys.GenerateOnly.mH90.%(SEED)s.root" \
    --cminPreFit 1 --minimizerStrategy 0 --minimizerTolerance 0.1 --job-mode lxbatch --merge 5 \
    --task-name model-${MODEL}-toys-${TOYS}-ggH --sub-opts '-q 1nd' ;done; done
    
#for MODEL in default "$1"; do for TOYS in default "$1"; do \
#  combineTool.py -M CollectLimits output/${MODEL}/cmb/higgsCombine.ggH.toys.${TOYS}.* \
#  --use-dirs -o mssm_bias_${MODEL}_model_${TOYS}_toys_ggH.json --toys; done; done
#  combineTool.py -M CollectLimits output/${MODEL}/cmb/higgsCombine.bbH.toys.${TOYS}.* \
#  --use-dirs -o mssm_bias_${MODEL}_model_${TOYS}_toys_bbH.json --toys; done; done

#for TYPE in gg bb; do python scripts/plotMSSMLimits.py --logy --logx \
#  mssm_bias_nominal_model_nominal_toys_${TYPE}H_cmb.json \
#  'mssm_bias_default_model_'$1'_toys_'${TYPE}'H_cmb.json:exp0:MarkerSize=0.5,Title="Alt median"' \
#  'mssm_bias_default_model_'$1'_toys_'${TYPE}'H_cmb.json:exp-1:MarkerSize=0.5,LineStyle=2,Title="Alt -1#sigma"' \
#  'mssm_bias_default_model_'$1'_toys_'${TYPE}'H_cmb.json:exp+1:MarkerSize=0.5,LineStyle=2,Title="Alt +1#sigma"' \
#  'mssm_bias_default_model_'$1'_toys_'${TYPE}'H_cmb.json:exp-2:MarkerSize=0.5,LineStyle=7,Title="Alt -2#sigma"' \
#  'mssm_bias_default_model_'$1'_toys_'${TYPE}'H_cmb.json:exp+2:MarkerSize=0.5,LineStyle=7,Title="Alt +2#sigma"' \
#  --cms-sub="Preliminary" -o mssm_bias_nominal_model_${TYPE}H_cmb --show exp \
#  --ratio-to mssm_bias_nominal_model_nominal_toys_${TYPE}H_cmb.json:exp0 \
#  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
#  --title-left 'Model: nominal, Dashed lines: Alternate'; done
