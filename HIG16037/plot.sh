channels=("mt" "et")
sigs=("ggH" "bbH")


for i in "${channels[@]}"; do for j in "${sigs[@]}"; do
  channel=$i
  sig=$j
  if [ $channel == "mt" ];then
    chan_label="--channel_label=#mu#tau_{h}"
  else
    chan_label="--channel_label=e#tau_{h}"
  fi
  
  if [ $sig == "ggH" ];then
    process="--process=gg#phi"
  else
    process="--process=bb#phi"
  fi
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_medium_$sig_htt_$channel"_8_13TeV.json",mssm_isostudy_medium_$sig_htt_$channel"_9_13TeV.json",mssm_isostudy_medium_$sig_htt_$channel"_10_13TeV.json",mssm_isostudy_medium_$sig_htt_$channel"_11_13TeV".json --labels="medium30,medium40,medium50,medium70" --expected_only --outname="mssm_isostudy_"$channel"_$sig" --process="gg#phi" --log
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_loose_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_loose_$sig"_htt_"$channel"_9_13TeV.json",mssm_isostudy_loose_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_loose_$sig"_htt_"$channel"_11_13TeV",mssm_isostudy_medium_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_9_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_11_13TeV".json,mssm_isostudy_tight_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_9_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_tight_$sig"_htt"_$channel"_11_13TeV".json --labels="loose30,loose40,loose50,loose70, medium30,medium40,medium50,medium70,tight30,tight40,tight50,tight70" --expected_only --outname="mssm_isostudy_"$channel"_$sig" --process="gg#phi" --log 
  #mssm_isostudy_loose_gg_htt_mt_8_13TeV.json,mssm_isostudy_loose_gg_htt_mt_9_13TeV.json,mssm_isostudy_loose_gg_htt_mt_10_13TeV.json,mssm_isostudy_loose_gg_htt_mt_11_13TeV.json
  
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_loose_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_loose_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_loose_$sig"_htt_"$channel"_11_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_9_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_11_13TeV".json,mssm_isostudy_tight_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_tight_$sig"_htt"_$channel"_11_13TeV".json --labels="loose30,loose50,loose70, medium30,medium40,medium50,medium70,tight30,tight50,tight70" --expected_only --outname="mssm_isostudy_"$channel"_$sig" --process="gg#phi" --log
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isocats5_$sig"_"$channel.json,mssm_isostudy_tight50_$sig"_"$channel".json" --labels="isocats5_comb,tight50" --expected_only --outname="mssm_isocats_vs_tight50_"$channel"_$sig" $process --log $chan_label 
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isocats1_$sig"_"$channel.json,mssm_isocats2_$sig"_"$channel.json,mssm_isocats3_$sig"_"$channel.json,mssm_isocats4_$sig"_"$channel.json,mssm_isocats5_$sig"_"$channel.json,mssm_isocats6_$sig"_"$channel.json --labels="isocats1,isocats2,isocats3,isocats4,isocats5,isocats6" --expected_only --outname="mssm_isocats_"$channel"_$sig" $process --log $chan_label 
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_loose_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_10_13TeV.json" --labels="loose50,medium50,tight50" --expected_only --outname="mssm_isostudy_mt50_"$channel"_$sig" $process --log $chan_label
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_tight_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_9_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_tight_$sig"_htt_"$channel"_11_13TeV.json" --labels="tight30,tight40,tight50,tight70" --expected_only --outname="mssm_isostudy_tight_"$channel"_$sig" $process --log $chan_label
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_tight_$sig"_htt_"$channel"_8_13TeV.json",mssm_isostudy_medium_$sig"_htt_"$channel"_10_13TeV.json",mssm_isostudy_loose_$sig"_htt_"$channel"_11_13TeV.json" --labels="tight30,medium50,loose70" --expected_only --outname="mssm_isostudy_mixed_"$channel"_$sig" $process --log $chan_label
  
  #python scripts/MSSMlimitCompare.py --file=mssm_isostudy_tight50_$sig"_"$channel".json",mssm_isocats5_$sig"_"$channel.json --labels="tight50,isocats5_comb" --expected_only --outname="mssm_isocats_vs_tight50_ratio_"$channel"_$sig" $process $chan_label --ratio --custom_y_range --y_axis_min=0.5 --y_axis_max=1.5
  
  python scripts/MSSMlimitCompare.py --file=mssm_isocats5_$sig"_"$channel.json,mssm_isostudy_tight50_$sig"_"$channel".json" --labels="isocats5_comb,tight50" --expected_only --outname="mssm_isocats_vs_tight50_"$channel"_$sig" $process --log $chan_label

done; done
