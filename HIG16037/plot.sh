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
  
  python scripts/MSSMlimitCompare.py --file=mssm_$sig"_"$channel"_comb_16_17.json",mssm_$sig"_"$channel"_comb_19_21.json" --labels="tight50_comb,isocats_tight_comb" --expected_only --outname="mssm_isocatsnoloose_vs_tight50_"$channel"_$sig" $process --log $chan_label
  
  python scripts/MSSMlimitCompare.py --file=mssm_$sig"_"$channel"_comb_18_16_20_21.json",mssm_$sig"_"$channel"_comb_18_19_20_21.json" --labels="mix,isocats_only" --expected_only --outname="mssm_mis_comb_vs_isocats_comb_"$channel"_$sig" $process --log $chan_label
  
  python scripts/MSSMlimitCompare.py --file=mssm_$sig"_htt_"$channel"_16_13TeV.json",mssm_$sig"_htt_"$channel"_19_13TeV.json" --labels="16,19" --expected_only --outname="mssm_16_vs_19_"$channel"_$sig" $process --log $chan_label
  
  python scripts/MSSMlimitCompare.py --file=mssm_$sig"_htt_"$channel"_17_13TeV.json",mssm_$sig"_htt_"$channel"_21_13TeV.json" --labels="17,21" --expected_only --outname="mssm_17_vs_21_"$channel"_$sig" $process --log $chan_label
  
  python scripts/MSSMlimitCompare.py --file=mssm_$sig"_"$channel"_comb_16_17.json",mssm_$sig"_"$channel"_comb_18_16_20_21.json" --labels="tight50,isocats" --expected_only --outname="mssm_tight50_vs_isocats_"$channel"_$sig" $process --log $chan_label

done; done
