
out="mjj"
Morphing --output_folder="$out" --postfix="-2D" --control_region=1 --inc_jes=true
combineTool.py -M T2W -i output/$out/{cmb,em,et,mt,tt}/* -o ws.root --parallel 8
