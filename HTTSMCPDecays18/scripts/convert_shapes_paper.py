# you can run the combined shapes for all years using commands like the example below:
#PostFitShapesFromWorkspace -m 125 -d output/pas_2706/htt_tt_3_only_13TeV/125/combined.txt.cmb -w output/pas_2706/htt_tt_3_only_13TeV/125/ws.root -o shapes_tt_cmb_3.root --print --postfit --sampling -f output/pas_2406_v2/cmb/125/multidimfit.bestfit.root:fit_mdf --total-shapes-bin=true --total-shapes=true

import ROOT

bini=3

#f2 = ROOT.TFile('shapes_bestfit.root')
#f3 = ROOT.TFile('shapes_sm_bestfit.root')
#f4 = ROOT.TFile('shapes_ps_bestfit.root')

fout = ROOT.TFile('shapes_all_cmb.root','RECREATE')

chans = ['mt','tt','et']
bins = {'mt': [1,2,3,4,5,6], 'tt': [1,2,3,4,5,6,7,8,9,10,11], 'et': [1,2,3,4,5,6]}

bkgs = {}

bkgs['mt'] = [
 'EmbedZTT',	
 'TTT',
 'VVT',	
 'ZL',	
 'jetFakes'
]

bkgs['et'] = [
 'EmbedZTT',
 'TTT',
 'VVT',
 'ZL',
 'jetFakes'
]

bkgs['tt'] = [
 'EmbedZTT',     
 'TTT',
 'VVT',  
 'ZL',   
 'jetFakes',      
 'Wfakes'      
]	

for chan in chans:
  for bini in bins[chan]:

    dirname = 'htt_%(chan)s_2018_%(bini)i_13TeV_postfit' % vars()
    fout.cd()
    if not ROOT.gDirectory.GetDirectory(dirname): ROOT.gDirectory.mkdir(dirname)
   
    if bini>2:
      #shapes_htt_%(chan)s_%(bini)i_only_13TeV.root 
      f1 = ROOT.TFile('shapes_htt_%(chan)s_%(bini)i_only_13TeV.root' % vars())
      f2 = ROOT.TFile('shapes_htt_%(chan)s_%(bini)i_only_13TeV_ps.root' % vars())
    else:
      f1 = ROOT.TFile('shapes_htt_%(chan)s_%(bini)i_13TeV.root' % vars())
      f2 = ROOT.TFile('shapes_htt_%(chan)s_%(bini)i_13TeV_ps.root' % vars())
 
    #f1 = ROOT.TFile('shapes_%(chan)s_cmb_%(bini)i.root' % vars()) 
    directory = f1.Get('postfit')
    for key in directory.GetListOfKeys():
      histo = directory.Get(key.GetName()).Clone()
      fout.cd(dirname)
      histo.Write()


    year_dirname = 'htt_%(chan)s_201X_%(bini)i_13TeV_postfit' % vars()
    for x in bkgs[chan]:
      d1 = f1.Get(year_dirname.replace('X','6'))
      d2 = f1.Get(year_dirname.replace('X','7'))
      d3 = f1.Get(year_dirname.replace('X','8'))
      exists = d1.FindKey(x)
      if d1.FindKey(x): h1 = f1.Get(year_dirname.replace('X','6')+'/'+x).Clone()
      else: 
        h1 = f1.Get(year_dirname.replace('X','6')+'/TotalBkg').Clone()
        h1.Scale(0.) 
        h1.SetName(x)
      if d2.FindKey(x): h2 = f1.Get(year_dirname.replace('X','7')+'/'+x).Clone()
      else:
        h2 = f1.Get(year_dirname.replace('X','7')+'/TotalBkg').Clone()
        h2.Scale(0.)
        h2.SetName(x)
      if d3.FindKey(x): h3 = f1.Get(year_dirname.replace('X','8')+'/'+x).Clone()
      else:
        h3 = f1.Get(year_dirname.replace('X','8')+'/TotalBkg').Clone()
        h3.Scale(0.)
        h3.SetName(x)
      h1.Add(h2)
      h1.Add(h3)
      fout.cd(dirname)
      h1.Write()

    x = 'TotalSig'
    #h1 = f1.Get(year_dirname.replace('X','6')+'/'+x).Clone()
    #h2 = f1.Get(year_dirname.replace('X','7')+'/'+x).Clone()
    #h3 = f1.Get(year_dirname.replace('X','8')+'/'+x).Clone()
    #h1.Add(h2)
    #h1.Add(h3)
    #fout.cd(dirname)
    #h1.Write('TotalSigSM')

    h1 = f2.Get(year_dirname.replace('X','6')+'/'+x).Clone()
    h2 = f2.Get(year_dirname.replace('X','7')+'/'+x).Clone()
    h3 = f2.Get(year_dirname.replace('X','8')+'/'+x).Clone()
    h1.Add(h2)
    h1.Add(h3)
    fout.cd(dirname)
    h1.Write('TotalSigPS')
