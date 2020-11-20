import os
import ROOT
import glob

files = glob.glob("higgsCombine_paramFit_Test*.root")

out_string=''

#file_name='higgsCombine_paramFit_Test_ff_tt_qcd_stat_unc1_njets0_dm10_2018.MultiDimFit.mH125.root'
#file_name='higgsCombine_paramFit_Test_ff_tt_qcd_stat_unc2_njets1_dm11_2017.MultiDimFit.mH125.root'

for file_name in files:

  f = ROOT.TFile(file_name)
  
  syst_name = '_'.join(file_name.split('.')[0].split('_')[3:])
  #print syst_name
  
  t = f.Get('limit')
  
  fail = False
  
  
  if t.GetEntries()<3:
    fail=True
  
  else:
    vals=[]
    for x in t:
      vals.append(getattr(x, '%(syst_name)s' % vars()))
      #print getattr(x, '%(syst_name)s' % vars())
    if vals[1] == vals[0] and vals[2] == vals[0]: fail=True
    #print fail
    
  #print vals
  
  if fail: out_string+='%(syst_name)s\n' % vars()

os.system('echo "%(out_string)s" > failed.dat' % vars())
