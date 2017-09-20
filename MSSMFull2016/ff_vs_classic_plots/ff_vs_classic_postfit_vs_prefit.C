{
f1 = new TFile("shapes_mt.root");
f2 = new TFile("shapes_mt_classic.root");

h1 = (TH1D*)f1->Get("htt_mt_8_13TeV_postfit/ggH");
h2 = (TH1D*)f1->Get("htt_mt_8_13TeV_prefit/ggH");
h3 = (TH1D*)f2->Get("htt_mt_8_13TeV_postfit/ggH");
h4 = (TH1D*)f2->Get("htt_mt_8_13TeV_prefit/ggH");

h5 = (TH1D*)f1->Get("htt_mt_8_13TeV_postfit/TotalBkg");
h6 = (TH1D*)f1->Get("htt_mt_8_13TeV_prefit/TotalBkg");
h7 = (TH1D*)f2->Get("htt_mt_8_13TeV_postfit/TotalBkg");
h8 = (TH1D*)f2->Get("htt_mt_8_13TeV_prefit/TotalBkg");



TCanvas c1;
c1.SetLogx();
gPad->SetBottomMargin(0.15);
c1.Divide(0,2,0,2);


h_div = (TH1D*)h1->Clone();
for(unsigned i=1; i<=h2->GetNbinsX()+1;++i) h2->SetBinError(i,0);
for(unsigned i=1; i<=h4->GetNbinsX()+1;++i) h4->SetBinError(i,0);
for(unsigned i=1; i<=h6->GetNbinsX()+1;++i) h6->SetBinError(i,0);
for(unsigned i=1; i<=h8->GetNbinsX()+1;++i) h8->SetBinError(i,0);
h1->Divide(h2);
//h2->Divide(h2);
h3->Divide(h4);
h5->Divide(h6);
h7->Divide(h8);



h1->SetStats(0);
h2->SetStats(0);
h3->SetStats(0);
h4->SetStats(0);

h5->SetStats(0);
h6->SetStats(0);
h7->SetStats(0);
h8->SetStats(0);

h5->GetXaxis()->SetTitle("M_{T}^{tot} (GeV)");
h5->GetXaxis()->SetTitleSize(h5->GetXaxis()->GetTitleSize()*1.5);
h5->GetXaxis()->SetLabelSize(h5->GetXaxis()->GetLabelSize()*1.3);
h1->GetYaxis()->SetTitle("post-fit/pre-fit");
h1->GetYaxis()->SetTitleSize(h1->GetYaxis()->GetTitleSize()*1.8);
h1->GetYaxis()->SetTitleOffset(0.8);
h1->GetYaxis()->SetLabelSize(h1->GetYaxis()->GetLabelSize()*1.2);


//h1->GetYaxis()->SetRangeUser(0.05,1.85);
h5->GetYaxis()->SetRangeUser(0.2,1.85);

//c1.Divide(0,3,0,3);
c1.cd(1);
c1.cd(1)->SetLogx();
c1.cd(1)->SetFrameBorderMode(0);
c1.cd(1)->SetBorderMode(0);
c1.cd(1)->SetBorderSize(0);
c1.cd(1)->SetFillColor(0);
h1->SetFillColor(16);
h1->GetXaxis();
h1->DrawClone("E2");
h1->SetLineColor(0);
h1_leg=(TH1D*)h1->Clone();
h1->SetFillStyle(0);
h1->SetLineColor(kBlack);
h1->Draw("hist same");
h3->SetMarkerColor(h2->GetLineColor());
h3->Draw("same L");

legend = new TLegend(0.15, 0.85, 0.7, 0.95);
legend->SetNColumns(2);
legend->SetBorderSize(0);
legend->SetTextSize(0.04);
legend->AddEntry(h1, "FF signal nominal", "l");
legend->AddEntry(h1_leg, "FF total uncertainty", "f");
legend->AddEntry(h3, "classic signal nominal", "l");
legend->AddEntry(h3, "classic total uncertainty", "e");
legend->Draw();

c1.cd(2);
c1.cd(2)->SetLogx();
c1.cd(2)->SetFrameBorderMode(0);
c1.cd(2)->SetBorderMode(0);
c1.cd(2)->SetBorderSize(0);
c1.cd(2)->SetFillColor(0);
h5->SetFillColor(16);
h5->GetXaxis();
h5->DrawClone("E2");
h5->SetLineColor(0);
h5_leg=(TH1D*)h5->Clone();
h5->SetFillStyle(0);
h5->SetLineColor(kBlack);
h5->Draw("hist same");
h7->SetMarkerColor(h2->GetLineColor());
h7->Draw("same L");

legend = new TLegend(0.15, 0.85, 0.8, 0.95);
legend->SetNColumns(2);
legend->SetBorderSize(0);
legend->SetTextSize(0.03);
legend->AddEntry(h1, "FF background nominal", "l");
legend->AddEntry(h1_leg, "FF total uncertainty", "f");
legend->AddEntry(h3, "classic background nominal", "l");
legend->AddEntry(h3, "classic total uncertainty", "e");
legend->Draw();

//c1.cd(2);
//c1.cd(2)->SetLogx();
//c1.cd(2)->SetFrameBorderMode(0);
//c1.cd(2)->SetBorderMode(0);
//c1.cd(2)->SetBorderSize(0);
//c1.cd(2)->SetFillColor(0);
//h3->SetFillColor(16);
//h3->DrawClone("E2");
//h3->SetFillStyle(0);
//h3->SetLineColor(kBlack);
//h3->Draw("hist same");
//h4->SetMarkerColor(h4->GetLineColor());
//h4->Draw("same L");
//
//legend = new TLegend(0.15, 0.7, 0.7, 0.9);
//legend->SetNColumns(2);
//legend->SetBorderSize(0);
//legend->SetTextSize(0.06);
//legend->AddEntry(h1, "FF nominal", "l");
//legend->AddEntry(h1_leg, "FF total uncertainty", "f");
//legend->AddEntry(h2, "classic background nominal", "l");
//legend->AddEntry(h2, "classic background total uncertainty", "e");
//legend->Draw();


c1.Print("ff_vs_classic_postfit_vs_prefit.pdf");


}
