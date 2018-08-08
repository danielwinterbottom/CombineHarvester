#FakeTauESMeasurement
#combineTool.py -M T2W -P CombineHarvester.CombinePdfs.FakeES:FakeES -i output/fake_es/{htt_mt_1_13TeV,htt_mt_2_13TeV}/* -o ws.root --parallel 8

# fit mt dm=0

combineTool.py -m 125 -M MultiDimFit --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=984,1020 --points 50 -d output/fake_es/htt_mt_1_13TeV/ws.root --algo grid --there -n .dm0 --preFitValue 1000

python scripts/plot1DScanFakeES.py --main=output/fake_es/htt_mt_1_13TeV/higgsCombine.dm0.MultiDimFit.mH125.root --POI=faketaues --output=mu_dm0_fit --improve --no-numbers --x_title="#mu#rightarrow#tau ES" --title="1-prong 0-#pi^{0}"


combineTool.py -M MaxLikelihoodFit --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=984,1050  -d output/fake_es/htt_mt_1_13TeV/ws.root --there --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad

PostFitShapesFromWorkspace -d output/fake_es/htt_mt_1_13TeV/htt_mt_1_13TeV.txt -w output/fake_es/htt_mt_1_13TeV/ws.root -o shapes_mu_dm0.root --print  --postfit --sampling -f output/fake_es/htt_mt_1_13TeV/mlfit.Test.root:fit_s

python scripts/postFitPlot.py --file=shapes_mu_dm0.root --ratio --no_signal --file_dir="htt_mt_1_" --ratio_range 0.7,1.3 --outname mu_dm0 --mode postfit --esplot --y_title="dN/dm_{#mu#tau} (1/GeV)" --x_title="m_{#mu#tau} (GeV)" --channel_label="#mu#tau_{h} 1-prong 0-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"

PostFitShapesFromWorkspace -d output/fake_es/htt_mt_1_13TeV/htt_mt_1_13TeV.txt -w output/fake_es/htt_mt_1_13TeV/ws.root -o shapes_mu_dm0_es1.root --print   --sampling -f output/fake_es/htt_mt_1_13TeV/multidimfit.dm0.plot.root:fit_mdf --freeze faketaues=1000

python scripts/postFitPlot.py --file=shapes_mu_dm0_es1.root --ratio --no_signal --file_dir="htt_mt_1_" --ratio_range 0.7,1.3 --outname mu_dm0 --mode prefit --esplot --y_title="dN/dm_{#mu#tau} (1/GeV)" --x_title="m_{#mu#tau} (GeV)" --channel_label="#mu#tau_{h} 1-prong 0-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"

# fit mt dm=1

combineTool.py -m 125 -M MultiDimFit --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=980,1080  -d output/fake_es/htt_mt_2_13TeV/ws.root --there -n .dm1 --algo grid --points 30 --preFitValue 1000

python scripts/plot1DScanFakeES.py --main=output/fake_es/htt_mt_2_13TeV/higgsCombine.dm1.MultiDimFit.mH125.root --POI=faketaues --output=mu_dm1_fit --improve --no-numbers --x_title="#mu#rightarrow#tau ES" --title="1-prong 1-#pi^{0}"

combineTool.py -M MaxLikelihoodFit --redefineSignalPOIs=faketaues --setPhysicsModelParameterRanges faketaues=980,1080  -d output/fake_es/htt_mt_2_13TeV/ws.root --there --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad

PostFitShapesFromWorkspace -d output/fake_es/htt_mt_2_13TeV/htt_mt_2_13TeV.txt -w output/fake_es/htt_mt_2_13TeV/ws.root -o shapes_mu_dm1.root --print  --postfit --sampling 1 -f output/fake_es/htt_mt_2_13TeV/mlfit.Test.root:fit_s

python scripts/postFitPlot.py --file=shapes_mu_dm1.root --ratio --no_signal --file_dir="htt_mt_2_" --ratio_range 0.7,1.3 --outname mu_dm1 --mode postfit --esplot --y_title="dN/dm_{#mu#tau} (1/GeV)" --x_title="m_{#mu#tau} (GeV)" --channel_label="#mu#tau_{h} 1-prong 1-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"

PostFitShapesFromWorkspace -d output/fake_es/htt_mt_2_13TeV/htt_mt_2_13TeV.txt -w output/fake_es/htt_mt_2_13TeV/ws.root -o shapes_mu_dm1_es1.root --print  --sampling 1 -f output/fake_es/htt_mt_2_13TeV/mlfit.Test.root:fit_s --freeze faketaues=1000

python scripts/postFitPlot.py --file=shapes_mu_dm1_es1.root --ratio --no_signal --file_dir="htt_mt_2_" --ratio_range 0.7,1.3 --outname mu_dm1 --mode prefit --esplot --y_title="dN/dm_{#mu#tau} (1/GeV)" --x_title="m_{#mu#tau} (GeV)" --channel_label="#mu#tau_{h} 1-prong 1-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"
