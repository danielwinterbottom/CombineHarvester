for MODEL in default2 "$1"; do for TOYS in default2 "$1"; do \
  combineTool.py -M CollectLimits output/${MODEL}/mt/higgsCombine.bbH.toys.${TOYS}.* \
  --use-dirs -o mssm_bias_${MODEL}_model_${TOYS}_toys_bbH.json --toys
  combineTool.py -M CollectLimits output/${MODEL}/et/higgsCombine.bbH.toys.${TOYS}.* \
  --use-dirs -o mssm_bias_${MODEL}_model_${TOYS}_toys_bbH.json --toys
done; done
  
  
for TYPE in bb ; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  'mssm_bias_default2_model_default2_toys_'${TYPE}H_${CHAN}.json \
  'mssm_bias_default2_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Alt median"' \
  'mssm_bias_default2_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp-1:MarkerSize=0.5,LineStyle=2,Title="Alt -1#sigma"' \
  'mssm_bias_default2_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp+1:MarkerSize=0.5,LineStyle=2,Title="Alt +1#sigma"' \
  'mssm_bias_default2_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp-2:MarkerSize=0.5,LineStyle=7,Title="Alt -2#sigma"' \
  'mssm_bias_default2_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp+2:MarkerSize=0.5,LineStyle=7,Title="Alt +2#sigma"' \
  --cms-sub="Preliminary" -o mssm_bias_nominal_model_${TYPE}H_${CHAN}_$1 --show exp \
  --ratio-to mssm_bias_default2_model_default2_toys_${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: nominal, Dashed lines: Alternate'; done; done
  
for TYPE in bb; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  'mssm_bias_default2_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Nom median"' \
  'mssm_bias_default2_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,LineStyle=2,Title="Alt median"' \
  --cms-sub="Preliminary" -o mssm_bias_nominal_model_${TYPE}H_${CHAN}_$1'_nouncerts' --show exp \
  --ratio-to mssm_bias_default2_model_default2_toys_${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: nominal, Dashed lines: Alternate'; done; done
  
for TYPE in bb ; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}H_${CHAN}.json \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Alt median"' \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp-1:MarkerSize=0.5,LineStyle=2,Title="Alt -1#sigma"' \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp+1:MarkerSize=0.5,LineStyle=2,Title="Alt +1#sigma"' \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp-2:MarkerSize=0.5,LineStyle=7,Title="Alt -2#sigma"' \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp+2:MarkerSize=0.5,LineStyle=7,Title="Alt +2#sigma"' \
  --cms-sub="Preliminary" -o mssm_bias_alternative_model_${TYPE}H_${CHAN}_$1 --show exp \
  --ratio-to mssm_bias_$1'_model_'$1'_toys_'${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: nominal, Dashed lines: Alternate'; done; done
  
for TYPE in bb; do for CHAN in mt et; do python scripts/plotMSSMLimits.py --logy --logx \
  'mssm_bias_'$1'_model_'$1'_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,Title="Nom median"' \
  'mssm_bias_'$1'_model_default2_toys_'${TYPE}'H_'${CHAN}'.json:exp0:MarkerSize=0.5,LineStyle=2,Title="Alt median"' \
  --cms-sub="Preliminary" -o mssm_bias_alternative_model_${TYPE}H_${CHAN}_$1'_nouncerts' --show exp \
  --ratio-to mssm_bias_$1'_model_'$1'_toys_'${TYPE}H_${CHAN}.json:exp0 \
  --y-title "95% CL limit on ${TYPE}#phi#rightarrow#tau#tau (pb)" \
  --title-left 'Model: nominal, Dashed lines: Alternate'; done; done
