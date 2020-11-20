import ROOT
import os
import math

ROOT.gROOT.SetBatch(ROOT.kTRUE)

syst='QCDscale_ggH_ACCEPT'

f = ROOT.TFile('cmb/common/htt_input.root')
c1=ROOT.TCanvas()

os.system('mkdir -p -v theory_syst_checks')

def MergeMVABins(hist, nxbins=12):
  histnew = ROOT.TH1D(hist.GetName()+'_merged','',nxbins,1,nxbins+1)
  nbins = hist.GetNbinsX()
  nybins = nbins/nxbins
  for i in range(1,nxbins+1):
    c=0.
    e=0.
    for j in range(0,nybins):
      c+=hist.GetBinContent(i+j*nxbins)
      e+=(hist.GetBinError(i+j*nxbins))**2
    e = math.sqrt(e)

    histnew.SetBinContent(i,c)
    histnew.SetBinError(i,e)

  return histnew

def Chi2Test(h1,h2, opt=True):
  if opt:
    h_1 = ROOT.TH1D('h_1','',6,1,7)
    h_2 = ROOT.TH1D('h_2','',6,1,7)
  else:
    h_1 = h1.Clone()
    h_2 = h2.Clone()
  if opt:
    for i in range(1,h_1.GetNbinsX()+1):
      h_1.SetBinError(i,0.)
      if i <= 6: 
        h_1.SetBinContent(i,h1.GetBinContent(i))
        h_1.SetBinError(i,h1.GetBinError(i))
        h_2.SetBinContent(i,h2.GetBinContent(i))
        h_2.SetBinError(i,h2.GetBinError(i))
  chi2 = h_2.Chi2Test(h_1,'WW')
  ks = h_2.KolmogorovTest(h_1,'WW')
  return (chi2, ks)
for i in [2,3,4,5,6]:
  for c in ['mt','et','tt','em']:
    for y in [2016,2017,2018]:
      dirname = 'htt_%(c)s_%(y)i_%(i)i_13TeV' % vars()
      for x in ['sm','ps']:
        h     = f.Get('%(dirname)s/ggH_%(x)s_htt125' % vars())
	hup   = f.Get('%(dirname)s/ggH_%(x)s_htt125_%(syst)sUp' % vars())
        hdown = f.Get('%(dirname)s/ggH_%(x)s_htt125_%(syst)sDown' % vars())

        print hup.GetBinError(32)/hup.GetBinContent(32), hdown.GetBinError(32)/hdown.GetBinContent(32), h.GetBinError(32)/h.GetBinContent(32)
        if i >2:
          h = MergeMVABins(h)
          hup = MergeMVABins(hup)
          hdown = MergeMVABins(hdown)
        else:
          nbins=10
          if c == 'tt': nbins=9
          h.Rebin(nbins)
          hup.Rebin(nbins)
          hdown.Rebin(nbins)

        h.Scale(1./h.Integral())
        hup.Scale(1./hup.Integral())
        hdown.Scale(1./hdown.Integral())
        miny = -1
        maxy=-1
        for j in range(1,hup.GetNbinsX()+1):
          n = h.GetBinContent(j)
          if n==0: n=1.
          hup.SetBinContent(j, hup.GetBinContent(j)/n)
          hup.SetBinError(j, hup.GetBinError(j)/n)
          hdown.SetBinContent(j, hdown.GetBinContent(j)/n)
          hdown.SetBinError(j, hdown.GetBinError(j)/n)

          hup.SetLineColor(ROOT.kBlue)
          hdown.SetLineColor(ROOT.kRed)
          yup_u = hup.GetBinContent(i)+hup.GetBinError(i)
          yup_d = hup.GetBinContent(i)-hup.GetBinError(i)
          ydown_u = hdown.GetBinContent(i)+hdown.GetBinError(i)
          ydown_d = hdown.GetBinContent(i)-hdown.GetBinError(i)
          if miny==-1:
            miny = min(yup_d,ydown_d)
          else:
            miny = min(miny,yup_d,ydown_d)

          if maxy==-1:
            maxy = max(yup_u,ydown_u)
          else:
            maxy = max(maxy,yup_u,ydown_u)

        hup.SetMaximum(1.2)
        hup.SetMinimum(0.8)
        hup.SetStats(0)
        hdown.SetStats(0)
        if i>2:
          hup.GetXaxis().SetRangeUser(1,7)
          hdown.GetXaxis().SetRangeUser(1,7)
        hup.Draw()
        hdown.Draw('same')
        name = 'theory_syst_checks/%(syst)s_%(i)i_%(c)s_%(y)s_%(x)s.pdf' % vars()
        (chi2, ks) = Chi2Test(hup,hdown, i!=2)
        print chi2, ks

        tex = 'chi2 = %(chi2).3f, ks = %(ks).3f' % vars()

        latex = ROOT.TLatex()
        latex.SetNDC()
        latex.SetTextAngle(0)
        latex.SetTextColor(ROOT.kBlack)
        latex.DrawLatex(0.15, 0.93 , tex)
        c1.Print(name)
