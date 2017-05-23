if [ $2 == 1 ]; then
  for MODEL in nominal alternate_$1; do for TOYS in nominal alternate_$1; do \
    combineTool.py -M CollectLimits output/mssm_bias_${MODEL}/*/higgsCombine.bbH.toys.${TOYS}.* \
    --use-dirs -o mssm_bias_${MODEL}_model_${TOYS}_toys_bbH.json --toys; done; done
fi  

for TYPE in bb; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  mssm_bias_nominal_model_nominal_toys_${TYPE}H_${CHAN}.json \
  'mssm_bias_nominal_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Alt median"' \
  'mssm_bias_nominal_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp-1:MarkerSize=0.5,LineStyle=2,Title="Alt -1#sigma"' \
  'mssm_bias_nominal_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp+1:MarkerSize=0.5,LineStyle=2,Title="Alt +1#sigma"' \
  'mssm_bias_nominal_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp-2:MarkerSize=0.5,LineStyle=7,Title="Alt -2#sigma"' \
  'mssm_bias_nominal_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp+2:MarkerSize=0.5,LineStyle=7,Title="Alt +2#sigma"' \
  --cms-sub="Preliminary" -o mssm_bias_nominal_model_${TYPE}H_${CHAN}_$1'_bbH' --show exp \
  --ratio-to mssm_bias_nominal_model_nominal_toys_${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: nominal, Dashed lines: Alternate'; done;done
  
for TYPE in bb; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  'mssm_bias_nominal_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Nom median"' \
  'mssm_bias_nominal_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,LineStyle=2,Title="Alt median"' \
  --cms-sub="Preliminary" -o mssm_bias_nominal_model_${TYPE}H_${CHAN}_$1'_bbH_nouncerts' --show exp \
  --ratio-to mssm_bias_nominal_model_nominal_toys_${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: nominal, Dashed lines: Alternate'; done; done
  
for TYPE in bb; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  mssm_bias_alternate_$1_model_alternate_$1_toys_${TYPE}H_${CHAN}.json \
  'mssm_bias_alternate_'$1'_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Alt median"' \
  'mssm_bias_alternate_'$1'_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp-1:MarkerSize=0.5,LineStyle=2,Title="Alt -1#sigma"' \
  'mssm_bias_alternate_'$1'_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp+1:MarkerSize=0.5,LineStyle=2,Title="Alt +1#sigma"' \
  'mssm_bias_alternate_'$1'_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp-2:MarkerSize=0.5,LineStyle=7,Title="Alt -2#sigma"' \
  'mssm_bias_alternate_'$1'_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp+2:MarkerSize=0.5,LineStyle=7,Title="Alt +2#sigma"' \
  --cms-sub="Preliminary" -o mssm_bias_alternate_$1_model_${TYPE}H_${CHAN}_$1'_bbH' --show exp \
  --ratio-to mssm_bias_alternate_$1_model_alternate_$1_toys_${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: Alternate, Dashed lines: Nominal'; done;done
  
for TYPE in bb; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  'mssm_bias_alternate_'$1'_model_alternate_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Nom median"' \
  'mssm_bias_alternate_'$1'_model_nominal_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,LineStyle=2,Title="Alt median"' \
  --cms-sub="Preliminary" -o mssm_bias_alternate_$1_model_${TYPE}H_${CHAN}_$1'_bbH_nouncerts' --show exp \
  --ratio-to mssm_bias_alternate_$1_model_alternate_$1_toys_${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: Alternate, Dashed lines: Nominal'; done; done

