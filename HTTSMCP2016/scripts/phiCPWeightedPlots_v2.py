#propoganda_v5 has qqH as signal but may have buggs weights
#propoganda_v5 has qqH as background
#propoganda_v6 has qqH as signal

import numpy as np
import argparse
import ROOT
import math
import plotting2 as plot

ROOT.gROOT.SetBatch(ROOT.kTRUE)

parser = argparse.ArgumentParser()
parser.add_argument('--mode', '-m', default=1, type=int  ,help= 'File from which binning is to be optimized')
args = parser.parse_args()

mode=args.mode
# modes not needed at the moment

c='cmb'

if mode == 2: c = 'loosemjj_lowboost'
if mode == 3: c = 'loosemjj_boosted'
if mode == 4: c = 'tightmjj_lowboost'
if mode == 5: c = 'tightmjj_boosted'
if mode == 6: c = 'lowboost'
if mode == 7: c = 'boosted'
if mode == 8: c = 'loosemjj'
if mode == 9: c = 'tightmjj'
if mode == 100: c = 'cmb'

f1 = ROOT.TFile("hig20007_inputs_for_plots/shapes_propoganda_sm_%(c)s.root" % vars())
f2 = ROOT.TFile('hig20007_inputs_for_plots/shapes_propoganda_ps_%(c)s.root' % vars())


sm_hist = f1.Get('postfit/TotalSig')
ps_hist = f2.Get('postfit/TotalSig')
bkg_hist = f1.Get('postfit/TotalBkg')
data_hist = f1.Get('postfit/data_obs')

vbf=True
#vbf=False

if vbf:
  vbf_hist = f1.Get('postfit/qqH')

def Subtract(h1,h2):
  for i in range(1,h1.GetNbinsX()+1):
    diff = h1.GetBinContent(i) - h2.GetBinContent(i)
    h1.SetBinContent(i,diff)
  return h1

data_hist = Subtract(data_hist,bkg_hist)
bkg_hist = Subtract(bkg_hist,bkg_hist)

plot_name = 'phiCPWeighted_prefitW'
if mode >1 and mode !=100: plot_name+='_%(c)s' % vars()
if mode==100: plot_name+='_supp' % vars()

fout = ROOT.TFile('temp.root','RECREATE')
fout.cd()
data_hist.Write()
sm_hist.Write()

if vbf:
  plot.propoganda_plot_phicp(sm_hist,ps_hist,sm_hist,bkg_hist,data_hist,plot_name, mode, vbf=vbf_hist)
else: plot.propoganda_plot_phicp(sm_hist,ps_hist,sm_hist,bkg_hist,data_hist,plot_name, mode)


