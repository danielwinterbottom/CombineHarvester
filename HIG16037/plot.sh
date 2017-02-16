channels=("mt" "et" "tt")
sigs=("ggH" "bbH")


for i in "${channels[@]}"; do for j in "${sigs[@]}"; do
  channel=$i
  sig=$j
  if [ $channel == "mt" ];then
    chan_label="--channel_label=#mu#tau_{h}"
    title_left='--title-left=#mu#tau_{h}'
  elif [ $channel == "tt" ];then
    chan_label="--channel_label=#tau_{h}#tau_{h}"
    title_left='--title-left=#tau_{h}#tau_{h}'
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
  
 python scripts/MSSMlimitCompare.py --file=oldplotting_$sig"_cmb.json",newplotting_$sig"_cmb.json" --labels="old_plotting,new_plotting" --outname="old_vs_new_plotting_"$sig $process --log --relative --channel_label="e#tau,#mu#tau,#tau#tau combination"
 
 python scripts/MSSMlimitCompare.py --file=oldplotting_$sig"_"$channel$".json",newplotting_$sig"_"$channel$".json" --labels="old_plotting,new_plotting" --outname="old_vs_new_plotting_"$sig"_"$channel $process --log --relative $chan_label

   
  old_baseline_label="medium50"
  old_baseline_json="mssm_old_"$sig"_et_comb_38_39.json"
  if [ $channel == "mt" ];then
    old_baseline_label="medium40"
    old_baseline_json="mssm_old_"$sig"_mt_comb_30_31.json"
  fi 

done; done
