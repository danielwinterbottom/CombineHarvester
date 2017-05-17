
#MorphingMSSMFull2016 --output_folder=$1 -m MH --postfix="-mttot" --control_region=$6 --auto_rebin=true --real_data=false --zmm_fit=false --input_folder_mt=Imperial/$4 --input_folder_et=Imperial/$4 --input_folder_tt=Imperial/$4 --do_old_cats=$5 > $1"_binning.txt"
MorphingMSSMFull2016 --output_folder=$1 -m MH --postfix="-mttot" --control_region=$5 --auto_rebin=true --real_data=false --zmm_fit=false --input_folder_mt=$4 --input_folder_et=$4 --input_folder_tt=$4 -- --jetfakes=false  > $1"_binning.txt"
#--auto_rebin=true
combineTool.py -M T2W -o "ws.root" -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO '"map=^.*/ggH$:r_ggH[0,0,200]"' --PO '"map=^.*/bbH$:r_bbH[0,0,200]"' -i output/$1/*

if [ $2 == 1 ];then
    combineTool.py -m "90,100,110,120,130,140,160,180,200,250,350,400,450,500,700,800,900,1000,1200,1400,1600,1800,2000,2300,2600,2900,3200" -M Asymptotic --rAbsAcc 0 --rRelAcc 0.0005 --boundlist input/mssm_boundaries.json  --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_ggH -d output/$1/*/ws.root --there -n ".ggH" -t -1 --parallel=6 $7
    #--job-mode 'lxbatch' --task-name $1_ggH --sub-opts '-q 1nh' --merge=2
fi    
if [ $3 == 1 ];then
    combineTool.py -m "90,100,110,120,130,140,160,180,200,250,350,400,450,500,700,800,900,1000,1200,1400,1600,1800,2000,2300,2600,2900,3200" -M Asymptotic --rAbsAcc 0 --rRelAcc 0.0005 --boundlist input/mssm_boundaries.json  --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_bbH -d output/$1/*/ws.root --there -n ".bbH" -t -1 --parallel=6 $7
    #--job-mode 'lxbatch' --task-name $1_bbH --sub-opts '-q 1nh' --merge=2
fi


