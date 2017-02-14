channels=("mt" "et")
sigs=("ggH" "bbH")


for i in "${channels[@]}"; do for j in "${sigs[@]}"; do
  channel=$i
  sig=$j
  if [ $channel == "mt" ];then
    chan_label="--channel_label=#mu#tau_{h}"
    title_left='--title-left=#mu#tau_{h}'
  else
    chan_label="--channel_label=e#tau_{h}"
    title_left='--title-left=e#tau_{h}'
  fi
  
  if [ $sig == "ggH" ];then
    process="--process=gg#phi"
    ytitle="--y-title=\"95% CL limit on gg#phi#rightarrow#tau#tau (pb)\""
    sig_string="gg#phi"
  else
    process="--process=bb#phi"
    sig_string="bb#phi"
  fi
  
 python scripts/MSSMlimitCompare.py --file="mssm_nobtag_limits_"$sig"_htt_"$channel"_30_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_38_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_32_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_48_13TeV.json" --labels="medium40,medium50,tight40,tight50" --expected_only --outname="mssm_"$channel"_mtcut_comparrisons_with_medium_nobtag_$sig" $process --log $chan_label" nobtag"  
  
 python scripts/MSSMlimitCompare.py --file="mssm_nobtag_limits_"$sig"_htt_"$channel"_24_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_32_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_40_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_48_13TeV.json" --labels="tight30,tight40,tight50,tight70" --expected_only --outname="mssm_"$channel"_mtcut_comparrisons_nobtag_$sig" $process --log $chan_label" nobtag" 
 
 python scripts/MSSMlimitCompare.py --file="mssm_btag_limits_"$sig"_htt_"$channel"_25_13TeV.json","mssm_btag_limits_"$sig"_htt_"$channel"_33_13TeV.json","mssm_btag_limits_"$sig"_htt_"$channel"_41_13TeV.json","mssm_btag_limits_"$sig"_htt_"$channel"_49_13TeV.json" --labels="tight30,tight40,tight50,tight70" --expected_only --outname="mssm_"$channel"_mtcut_comparrisons_btag_$sig" $process --log $chan_label" btag"

 python scripts/MSSMlimitCompare.py --file="mssm_nobtag_limits_"$sig"_htt_"$channel"_34_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_36_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_38_13TeV.json","mssm_nobtag_limits_"$sig"_htt_"$channel"_40_13TeV.json" --labels="vloose50,loose50,medium50,tight50" --expected_only --outname="mssm_"$channel"_isocut_comparrisons_nobtag_$sig" $process --log $chan_label" nobtag" 
  
 python scripts/MSSMlimitCompare.py -- file="mssm_btag_limits_"$sig"_htt_"$channel"_35_13TeV.json","mssm_btag_limits_"$sig"_htt_"$channel"_37_13TeV.json","mssm_btag_limits_"$sig"_htt_"$channel"_39_13TeV.json","mssm_btag_limits_"$sig"_htt_"$channel"_41_13TeV.json" --labels="vloose50,loose50,medium50,tight50" --expected_only --outname="mssm_"$channel"_isocut_comparrisons_btag_$sig" $process --log $chan_label" btag" 
   
  old_baseline_label="medium50"
  old_baseline_json="mssm_old_"$sig"_et_comb_38_39.json"
  if [ $channel == "mt" ];then
    old_baseline_label="medium40"
    old_baseline_json="mssm_old_"$sig"_mt_comb_30_31.json"
  fi

 
  
  #compare b-tag category breakdown
  python scripts/MSSMlimitCompare.py --file=mssm_6cats_$sig"_"$channel"_comb_51_53.json",mssm_6cats_$sig"_htt_"$channel"_51_13TeV.json",mssm_6cats_$sig"_htt_"$channel"_53_13TeV.json" --labels="btag_comb,btag_loose,btag_tight" --expected_only --outname="mssm_"$channel"_4cats_btag_$sig" $process --log $chan_label" btag" 
  
  #compare nob-tag category breakdown
  python scripts/MSSMlimitCompare.py --file=mssm_6cats_$sig"_"$channel"_comb_50_52.json",mssm_6cats_$sig"_htt_"$channel"_50_13TeV.json",mssm_6cats_$sig"_htt_"$channel"_52_13TeV.json" --labels="nobtag_comb,nobtag_loose,nobtag_tight" --expected_only --outname="mssm_"$channel"_4cats_nobtag_$sig" $process --log $chan_label" nobtag"
  
  #compare 6 cats vs 4 cats
  python scripts/plotMSSMLimits.py --logy --logx 'mssm_6cats_'$sig'_'$channel'_comb_50_51_52_53.json:exp0:LineStyle=1,LineWidth=3,MarkerColor=419,LineColor=419,Title="4_isocats"' 'mssm_6cats_'$sig'_'$channel'_comb_52_53_54_55_56_57.json:exp0:LineStyle=1,LineWidth=3,MarkerColor=632,LineColor=632,Title="6_isocats"'  --cms-sub="" -o mssm_$channel"_4cats_vs _6cats_comparison_"$sig"_with_ratio" --ratio-to 'mssm_6cats_'$sig'_'$channel'_comb_50_51_52_53.json:exp0' $title_left --title-right="12.8 fb^{-1} (13 TeV)" --y-title="95% CL limit on "$sig_string"#rightarrow#tau#tau (pb)"
  
   #compare 4 cats vs old baseline
  python scripts/plotMSSMLimits.py --logy --logx $old_baseline_json':exp0:LineStyle=1,LineWidth=3,MarkerColor=419,LineColor=419,Title="'$old_baseline_label'"' 'mssm_6cats_'$sig'_'$channel'_comb_50_51_52_53.json:exp0:LineStyle=1,LineWidth=3,MarkerColor=632,LineColor=632,Title="iso-cats(4)"'  --cms-sub="" -o mssm_$channel"_4cats_combination_comparison_"$sig"_with_ratio" --ratio-to $old_baseline_json':exp0' $title_left --title-right="12.8 fb^{-1} (13 TeV)" --y-title="95% CL limit on "$sig_string"#rightarrow#tau#tau (pb)"
  
  #compare 6 cats vs old baseline (medium 40/50)
  python scripts/plotMSSMLimits.py --logy --logx $old_baseline_json':exp0:LineStyle=1,LineWidth=3,MarkerColor=419,LineColor=419,Title="'$old_baseline_label'"' 'mssm_6cats_'$sig'_'$channel'_comb_52_53_54_55_56_57.json:exp0:LineStyle=1,LineWidth=3,MarkerColor=632,LineColor=632,Title="iso-cats(6)"'  --cms-sub="" -o mssm_$channel"_6cats_combination_comparison_"$sig"_with_ratio" --ratio-to $old_baseline_json':exp0' $title_left --title-right="12.8 fb^{-1} (13 TeV)" --y-title="95% CL limit on "$sig_string"#rightarrow#tau#tau (pb)"
  
  # plot 3 cat break down for nobtag
  python scripts/MSSMlimitCompare.py --file=mssm_6cats_$sig"_"$channel"_comb_52_54_56.json",mssm_6cats_$sig"_htt_"$channel"_52_13TeV.json",mssm_6cats_$sig"_htt_"$channel"_54_13TeV.json",mssm_6cats_$sig"_htt_"$channel"_56_13TeV.json" --labels="nobtag_comb,nobtag_tight,nobtag_looseiso,nobtag_loosemt" --expected_only --outname="mssm_"$channel"_6cats_nobtag_$sig" $process --log $chan_label
  
  # plot 3 cat break down for btag
  python scripts/MSSMlimitCompare.py --file=mssm_6cats_$sig"_"$channel"_comb_53_55_57.json",mssm_6cats_$sig"_htt_"$channel"_53_13TeV.json",mssm_6cats_$sig"_htt_"$channel"_55_13TeV.json",mssm_6cats_$sig"_htt_"$channel"_57_13TeV.json" --labels="btag_comb,btag_tight,btag_looseiso,btag_loosemt" --expected_only --outname="mssm_"$channel"_6cats_btag_$sig" $process --log $chan_label 
  
  #compare using 4/6 cats vs tight50 for both chices of additional cat
  # tight+looseiso first
  python scripts/plotMSSMLimits.py --logy --logx $old_baseline_json':exp0:LineStyle=1,LineWidth=3,MarkerColor=419,LineColor=419,Title="'$old_baseline_label'"' 'mssm_6cats_'$sig'_'$channel'_comb_52_53_54_55.json:exp0:LineStyle=1,LineWidth=3,MarkerColor=632,LineColor=632,Title="iso-cats(tight+looseiso)"'  --cms-sub="" -o mssm_$channel"_6cats_oldbaseline_vs_tightpluslooseiso_combination_comparison_"$sig"_with_ratio" --ratio-to $old_baseline_json':exp0' $title_left --title-right="12.8 fb^{-1} (13 TeV)" --y-title="95% CL limit on "$sig_string"#rightarrow#tau#tau (pb)"
  
  #tight+tightiso second
  python scripts/plotMSSMLimits.py --logy --logx $old_baseline_json':exp0:LineStyle=1,LineWidth=3,MarkerColor=419,LineColor=419,Title="'$old_baseline_label'"' 'mssm_6cats_'$sig'_'$channel'_comb_52_53_56_57.json:exp0:LineStyle=1,LineWidth=3,MarkerColor=632,LineColor=632,Title="iso-cats(tight+loosemt)"'  --cms-sub="" -o mssm_$channel"_6cats_oldbaseline_vs_tightplusloosemt_combination_comparison_"$sig"_with_ratio" --ratio-to $old_baseline_json':exp0' $title_left --title-right="12.8 fb^{-1} (13 TeV)" --y-title="95% CL limit on "$sig_string"#rightarrow#tau#tau (pb)"


done; done
