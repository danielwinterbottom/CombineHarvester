FakeTauESMeasurement
combineTool.py -M T2W -P CombineHarvester.CombinePdfs.FakeES:FakeES -i output/fake_es/{htt_et_1_13TeV,htt_et_2_13TeV}/* -o ws.root --parallel 8

# fit mt dm=0

combineTool.py -m 125 -M MultiDimFit --redefineSignalPOIs=efaketauesdm0 --setPhysicsModelParameterRanges efaketauesdm0=980,1080 --points 20 -d output/fake_es/htt_et_1_13TeV/125/ws.root --algo grid --there -n .dm0 --preFitValue 1000

python scripts/plot1DScanFakeES.py --main=output/fake_es/htt_et_1_13TeV/125/higgsCombine.dm0.MultiDimFit.mH125.root --POI=efaketauesdm0 --output=e_dm0_fit --improve --no-numbers --x_title="e#rightarrow#tau ES" --title="1-prong 0-#pi^{0}"

combineTool.py -M MaxLikelihoodFit --redefineSignalPOIs=efaketauesdm0 --setPhysicsModelParameterRanges efaketauesdm0=1015,1016  -d output/fake_es/htt_et_1_13TeV/125/ws.root --there --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad

PostFitShapesFromWorkspace -d output/fake_es/htt_et_1_13TeV/125/htt_et_1_13TeV.txt -w output/fake_es/htt_et_1_13TeV/125/ws.root -o shapes_e_dm0.root --print  --postfit --sampling -f output/fake_es/htt_et_1_13TeV/125/mlfit.Test.root:fit_s

python scripts/postFitPlot.py --file=shapes_e_dm0.root --ratio --no_signal --file_dir="htt_et_1_" --ratio_range 0.7,1.3 --outname e_dm0 --mode postfit --esplot --y_title="dN/dm_{e#tau} (1/GeV)" --x_title="m_{e#tau} (GeV)" --channel_label="e#tau_{h} 1-prong 0-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"

PostFitShapesFromWorkspace -d output/fake_es/htt_et_1_13TeV/125/htt_et_1_13TeV.txt -w output/fake_es/htt_et_1_13TeV/125/ws.root -o shapes_e_dm0_es1.root --print   --sampling -f output/fake_es/htt_et_1_13TeV/125/multidimfit.dm0.plot.root:fit_mdf --freeze efaketauesdm0=1000

python scripts/postFitPlot.py --file=shapes_e_dm0_es1.root --ratio --no_signal --file_dir="htt_et_1_" --ratio_range 0.7,1.3 --outname e_dm0 --mode prefit --esplot --y_title="dN/dm_{e#tau} (1/GeV)" --x_title="m_{e#tau} (GeV)" --channel_label="e#tau_{h} 1-prong 0-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"


# fit et dm=1

combineTool.py -m 125 -M MultiDimFit --redefineSignalPOIs=efaketauesdm1 --setPhysicsModelParameterRanges efaketauesdm1=1000,1100 --points 20 -d output/fake_es/htt_et_2_13TeV/125/ws.root --algo grid --there -n .dm1 --preFitValue 1000 

python scripts/plot1DScanFakeES.py --main=output/fake_es/htt_et_2_13TeV/125/higgsCombine.dm1.MultiDimFit.mH125.root --POI=efaketauesdm1 --output=e_dm1_fit --improve --no-numbers --x_title="e#rightarrow#tau ES" --title="1-prong 1-#pi^{0}"

combineTool.py -M MaxLikelihoodFit --redefineSignalPOIs=efaketauesdm1 --setPhysicsModelParameterRanges efaketauesdm1=1039,1041  -d output/fake_es/htt_et_2_13TeV/125/ws.root --there --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migra

PostFitShapesFromWorkspace -d output/fake_es/htt_et_2_13TeV/125/htt_et_2_13TeV.txt -w output/fake_es/htt_et_2_13TeV/125/ws.root -o shapes_e_dm1.root --print  --postfit --sampling 1 -f output/fake_es/htt_et_2_13TeV/125/mlfit.Test.root:fit_s


python scripts/postFitPlot.py --file=shapes_e_dm1.root --ratio --no_signal --file_dir="htt_et_2_" --ratio_range 0.7,1.3 --outname e_dm1 --mode postfit --esplot --y_title="dN/dm_{e#tau} (1/GeV)" --x_title="m_{e#tau} (GeV)" --channel_label="e#tau_{h} 1-prong 1-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"

PostFitShapesFromWorkspace -d output/fake_es/htt_et_2_13TeV/125/htt_et_2_13TeV.txt -w output/fake_es/htt_et_2_13TeV/125/ws.root -o shapes_e_dm1_es1.root --print  --sampling 1 -f output/fake_es/htt_et_2_13TeV/125/mlfit.Test.root:fit_s --freeze efaketauesdm1=1000

python scripts/postFitPlot.py --file=shapes_e_dm1_es1.root --ratio --no_signal --file_dir="htt_et_2_" --ratio_range 0.7,1.3 --outname e_dm1 --mode prefit --esplot --y_title="dN/dm_{e#tau} (1/GeV)" --x_title="m_{e#tau} (GeV)" --channel_label="e#tau_{h} 1-prong 1-#pi^{0}" --lumi="41.9 fb^{-1} (13 TeV)"

