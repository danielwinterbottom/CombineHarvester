{
f1 = new TFile("shapes_mt.root");
f2 = new TFile("shapes_mt_classic.root");

h1 = (TH1D*)f1->Get("htt_mt_8_13TeV_postfit/ggH");
h2 = (TH1D*)f2->Get("htt_mt_8_13TeV_postfit/ggH");


TCanvas c1;
c1.SetLogx();


h_div = (TH1D*)h1->Clone();
for(unsigned i=1; i<=h_div->GetNbinsX()+1;++i) h_div->SetBinError(i,0);
h1->Divide(h_div);
h2->Divide(h_div);


h1->SetStats(0);
h2->SetStats(0);


h1->GetXaxis()->SetTitle("M_{T}^{tot} (GeV)");
h1->GetYaxis()->SetTitle("1/(FF nominal)");


h1->GetYaxis()->SetRangeUser(0.55,1.85);

h1->SetFillColor(16);
h1->GetXaxis();
h1->DrawClone("E2");
h1->SetLineColor(0);
h1_leg=(TH1D*)h1->Clone();
h1->SetFillStyle(0);
h1->SetLineColor(kBlack);
h1->Draw("hist same");
h2->SetMarkerColor(h2->GetLineColor());
h2->Draw("same L");

legend = new TLegend(0.15, 0.8, 0.7, 0.85);
legend->SetNColumns(2);
legend->SetBorderSize(0);
legend->SetTextSize(0.02);
legend->AddEntry(h1, "FF nominal", "l");
legend->AddEntry(h1_leg, "FF total uncertainty", "f");
legend->AddEntry(h2, "classic nominal", "l");
legend->AddEntry(h2, "classic total uncertainty", "e");
legend->Draw();


c1.Print("ff_vs_classic_M3200.pdf");


}
