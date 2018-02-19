
out="mjj"
Morphing --output_folder="$out" --postfix="-2D" --control_region=1 --inc_jes=true
combineTool.py -M T2W -i output/$out/{cmb,em,et,mt,tt}/* -o ws.root --parallel 8

combineTool.py -M MaxLikelihoodFit --setPhysicsModelParameters r=0 -d output/m_sv/mt/125/ws.root --there --minimizerTolerance 0.1 --minimizerStrategy

python scripts/postFitPlot.py --file=shapes_mt.root --ratio --extra_pad=0.6 --no_signal --file_dir="htt_mt_1_" --ratio_range 0.7,1.3  --outname test --mode postfit
