#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "boost/algorithm/string/predicate.hpp"
#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"
#include "CombineHarvester/CombineTools/interface/AutoRebin.h"
#include "CombineHarvester/CombinePdfs/interface/MorphFunctions.h"
#include "CombineHarvester/CombineTools/interface/HttSystematics.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "TH2.h"

using namespace std;
using boost::starts_with;
namespace po = boost::program_options;
using ch::syst::SystMap;
using ch::syst::SystMapAsymm;
using ch::syst::era;
using ch::syst::channel;
using ch::syst::bin_id;
using ch::syst::process;
using ch::JoinStr;
using namespace ch;


int main(int argc, char** argv) {
  // First define the location of the "auxiliaries" directory where we can
  // source the input files containing the datacard shapes
  string mass = "mA";
  string output_folder = "fake_es";
  string input_folder="shapes/fake_es/";
  po::variables_map vm;
  po::options_description config("configuration");
  config.add_options()
    ("mass,m", po::value<string>(&mass)->default_value(mass))
    ("input_folder", po::value<string>(&input_folder)->default_value("shapes/fake_es/"))
    ("output_folder", po::value<string>(&output_folder)->default_value("fake_es"));

  po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
  po::notify(vm);
  
  typedef vector<string> VString;
  typedef vector<pair<int, string>> Categories;
  string input_dir =
      "./"+input_folder;


  RooRealVar faketaues("faketaues", "faketaues", 1000, 900, 1200);

  VString chns =
      //{"tt"};
      {/*"et",*/"mt"};
   //   {"mt","et"};


  map<string, VString> bkg_procs;
  bkg_procs["et"] = {"QCD","W","ZTT","ZJ","TTT","TTJ","VVT","VVJ"};
  bkg_procs["mt"] = {"QCD","W","ZTT","ZJ","TTT","TTJ","VVT","VVJ"};

  map<string,Categories> cats;
  

  cats["mt_13TeV"] = {
    {1, "mt_mu_0pi0"},
    {2, "mt_mu_1pi0"}
    };
  cats["et_13TeV"] = {
    {1, "et_e_0pi0"},
    {2, "et_e_1pi0"}
    };  
 
  map<string, vector<double> > binning;

  binning["mt_0pi0"] = {50,55,60,65,70,75,80,82,84,86,88,90,92,94,96,98,100,105,110,115,120,130,140};
  binning["mt_1pi0"] = {50,55,60,65,70,75,80,82,84,86,88,90,92,94,96,98,100,105,110,115,120,130,140};


  // Create an empty CombineHarvester instance that will hold all of the
  // datacard configuration and histograms etc.
  ch::CombineHarvester cb;
  
  map<string, vector<string> > energy_scales; 
  energy_scales["mt_mu_0pi0"] = {"900","910","920","930","940","950","960","970","980","982","984","986","988","990","992","994","996","998","1000","1002","1004","1006","1008","1010","1012","1020","1030","1040","1050","1060","1070","1080","1090","1100"};
  energy_scales["mt_mu_1pi0"] = {"900","910","920","930","940","950","960","970","980","990","1000","1004","1006","1008","1010","1012","1014","1016","1018","1020","1022","1024","1026","1028","1030","1035","1040","1045","1050","1060","1070","1080","1090","1100"};
  energy_scales["et_e_0pi0"] = {"900","910","920","930","940","950","960","970","980","990","1000","1010","1020","1030","1040","1050","1060","1070","1080","1090","1100"}; 
  energy_scales["et_e_1pi0"] = {"900","910","920","930","940","950","960","970","980","990","1000","1010","1020","1030","1040","1050","1060","1070","1080","1090","1100"};

  for(auto chn : chns){
    cb.AddObservations({"*"}, {"htt"}, {"13TeV"}, {chn}, cats[chn+"_13TeV"]);

    cb.AddProcesses({"*"}, {"htt"}, {"13TeV"}, {chn}, bkg_procs[chn], cats[chn+"_13TeV"], false);
   
    if(chn == "mt"){ 
      cb.AddProcesses({energy_scales["mt_mu_0pi0"]}, {"htt"}, {"13TeV"}, {chn}, {"ZL"}, {{1,"mt_mu_0pi0"}}, true);
      cb.AddProcesses({energy_scales["mt_mu_1pi0"]}, {"htt"}, {"13TeV"}, {chn}, {"ZL"}, {{2,"mt_mu_1pi0"}}, true);
    }
    if(chn == "et"){ 
      cb.AddProcesses({energy_scales["et_e_0pi0"]}, {"htt"}, {"13TeV"}, {chn}, {"ZL"}, {{1,"et_e_0pi0"}}, true);
      cb.AddProcesses({energy_scales["et_e_1pi0"]}, {"htt"}, {"13TeV"}, {chn}, {"ZL"}, {{2,"et_e_1pi0"}}, true);
    }

    }


  auto signal = Set2Vec(cb.cp().signals().SetFromProcs(
      std::mem_fn(&Process::process)));

     // Add all systematics here
  
    std::vector<std::string> real_tau = {"TTT","VVT","ZTT"};
    std::vector<std::string> jetfake = {"W","VVJ","TTJ","ZJ"};
    std::vector<std::string> jetfake_noW = {"VVJ","TTJ","ZJ"};
    std::vector<std::string> zl = {"ZL"};
    
    cb.cp().process(JoinStr({real_tau,jetfake_noW,zl,{"ZTT","ZTTpass","ZTTfail"}})).AddSyst(cb,
                                            "lumi_13TeV", "lnN", SystMap<>::init(1.025));

    
    cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
                        ({"mt"}, JoinStr({real_tau,jetfake_noW,zl}),  1.02));
    cb.cp().AddSyst(cb, "CMS_eff_e", "lnN", SystMap<channel, process>::init
                        ({"et"}, JoinStr({real_tau,jetfake_noW,zl}),  1.02));   

    
    //##############################################################################
    //  trigger   
    //##############################################################################
    
    cb.cp().process(JoinStr({real_tau,jetfake_noW,zl})).channel({"mt"}).AddSyst(cb,
                                         "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
    
    cb.cp().process(JoinStr({real_tau,jetfake_noW,zl})).channel({"et"}).AddSyst(cb,
                                         "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));

    cb.cp().process(real_tau).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.07));
    cb.cp().process(real_tau).AddSyst(cb,
                                           "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));

    //cb.cp().process(real_tau).AddSyst(cb,
    //                                         "CMS_eff_t_$BIN_$ERA", "lnN", SystMap<>::init(1.05));

    cb.cp().process({"W"}).AddSyst(cb,
                                             "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));  
    cb.cp().process(jetfake_noW).AddSyst(cb,
                                                      "CMS_htt_jFakeTau_13TeV", "lnN", SystMap<>::init(1.20));

    cb.cp().process(zl).channel({"mt"}).AddSyst(cb,
                                                      "CMS_htt_mFakeTau_13TeV", "lnN", SystMap<>::init(1.20));

    cb.cp().process(zl).channel({"et"}).AddSyst(cb,
                                                        "CMS_htt_eFakeTau_13TeV", "lnN", SystMap<>::init(1.20));
    
    cb.cp().process(real_tau).bin_id({1}).AddSyst(cb,
                                            "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(real_tau).bin_id({2}).AddSyst(cb,
                                            "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
    
    
    cb.cp().process(JoinStr({real_tau,jetfake_noW})).AddSyst(cb,"CMS_scale_j_$ERA", "shape", SystMap<>::init(1.00));

    cb.cp().process({"ZL"}).channel({"et"}).bin_id({1}).AddSyst(cb,"CMS_scale_j_$ERA", "lnN", SystMapAsymm<>::init(0.972,1.015));
    cb.cp().process({"ZL"}).channel({"et"}).bin_id({2}).AddSyst(cb,"CMS_scale_j_$ERA", "lnN", SystMapAsymm<>::init(0.976,1.012));

    cb.cp().process({"ZL"}).channel({"mt"}).bin_id({1}).AddSyst(cb,"CMS_scale_j_$ERA", "lnN", SystMapAsymm<>::init(0.976,1.018));
    cb.cp().process({"ZL"}).channel({"mt"}).bin_id({2}).AddSyst(cb,"CMS_scale_j_$ERA", "lnN", SystMapAsymm<>::init(0.967,1.016));
    
    cb.cp().process({"VVT","VVJ"}).AddSyst(cb,
                                        "CMS_htt_vvXsec_13TeV", "lnN", SystMap<>::init(1.05));
    
    cb.cp().process({"TTT","TTJ"}).AddSyst(cb,
					  "CMS_htt_tjXsec_13TeV", "lnN", SystMap<>::init(1.06));


    cb.cp().process({"ZTT","ZL","ZJ"}).AddSyst(cb,
                                                       "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.04));
    

    cb.cp().process({"ZTT","ZL","ZJ"}).AddSyst(cb,
                                             "CMS_htt_dyShape_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process({"TTJ","TTT"}).AddSyst(cb,
                                        "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));
    
    cb.cp().process({"QCD"}).AddSyst(cb,
                                        "CMS_QCD_OSSS_$CHANNEL_$BIN_$ERA", "lnN", SystMap<>::init(1.10));
    
    cb.cp().process({"W"}).AddSyst(cb,
                                        "CMS_W_Extrap_$CHANNEL_$BIN_$ERA", "lnN", SystMap<>::init(1.10));
    
  
  //! [part7]
  for (string chn:chns){
    cb.cp().channel({chn}).backgrounds().ExtractShapes(
        input_dir + "htt_"+chn+".inputs-sm-13TeV-2D.root",
        "$BIN/$PROCESS",
        "$BIN/$PROCESS_$SYSTEMATIC");
    cb.cp().channel({chn}).process({"ZL"}).ExtractShapes(
        input_dir + "htt_"+chn+".inputs-sm-13TeV-2D.root",
        "$BIN/ZL_$MASS",
        "$BIN/ZL_$MASS_$SYSTEMATIC");
   }

  auto bins = cb.cp().bin_set();
  //bool manual_rebin = false;  
  //bool auto_rebin = true;  
  //std::map<std::string, TH1F> before_rebin;
  //std::map<std::string, TH1F> after_rebin;
  //std::map<std::string, TH1F> after_rebin_neg;
  //auto rebin = ch::AutoRebin()
  //  .SetBinThreshold(0.)
  //  .SetBinUncertFraction(0.9)
  //  .SetRebinMode(1)
  //  .SetPerformRebin(true)
  //  .SetVerbosity(1);
  //if(auto_rebin) rebin.Rebin(cb, cb);
  //
  //if(manual_rebin) {
  //    for(auto b : bins) {
  //      std::cout << "Rebinning by hand for bin: " << b <<  std::endl;
  //      cb.cp().bin({b}).VariableRebin(binning[b]);    
  //    }
  //}
  
    // At this point we can fix the negative bins
    std::cout << "Fixing negative bins\n";
    cb.ForEachProc([](ch::Process *p) {
      if (ch::HasNegativeBins(p->shape())) {
         std::cout << "[Negative bins] Fixing negative bins for " << p->bin()
                   << "," << p->process() << "\n";
        auto newhist = p->ClonedShape();
        ch::ZeroNegativeBins(newhist.get());
        // Set the new shape but do not change the rate, we want the rate to still
        // reflect the total integral of the events
        p->set_shape(std::move(newhist), false);
      }
    });


    cb.ForEachSyst([](ch::Systematic *s) {
      if (s->type().find("shape") == std::string::npos) return;
      if (ch::HasNegativeBins(s->shape_u()) || ch::HasNegativeBins(s->shape_d())) {
         std::cout << "[Negative bins] Fixing negative bins for syst" << s->bin()
               << "," << s->process() << "," << s->name() << "\n";
        auto newhist_u = s->ClonedShapeU();
        auto newhist_d = s->ClonedShapeD();
        ch::ZeroNegativeBins(newhist_u.get());
        ch::ZeroNegativeBins(newhist_d.get());
        // Set the new shape but do not change the rate, we want the rate to still
        // reflect the total integral of the events
        s->set_shapes(std::move(newhist_u), std::move(newhist_d), nullptr);
      }
  });
  
  cout << "Generating bbb uncertainties...";
  auto bbb = ch::BinByBinFactory()
    .SetAddThreshold(0.)
    .SetMergeThreshold(00)
    .SetFixNorm(false);
  bbb.MergeAndAdd(cb.cp().process({"ZTT","W", "ZJ", "QCD", "TTT", "VVT", "VVJ","TTT", "TTJ"}), cb); 
  
  auto bbb_sig = ch::BinByBinFactory()
    .SetAddThreshold(0.0)
    .SetMergeThreshold(0.0)
    .SetFixNorm(false);
  bbb_sig.AddBinByBin(cb.cp().signals(), cb); 

  cb.ForEachSyst([](ch::Systematic *s) {
      if (s->type().find("shape") == std::string::npos) return;
      if (ch::HasNegativeBins(s->shape_u()) || ch::HasNegativeBins(s->shape_d())) {
         std::cout << "[Negative bins] Fixing negative bins for syst" << s->bin()
               << "," << s->process() << "," << s->name() << "\n";
        auto newhist_u = s->ClonedShapeU();
        auto newhist_d = s->ClonedShapeD();
        ch::ZeroNegativeBins(newhist_u.get());
        ch::ZeroNegativeBins(newhist_d.get());
        // Set the new shape but do not change the rate, we want the rate to still
        // reflect the total integral of the events
        s->set_shapes(std::move(newhist_u), std::move(newhist_d), nullptr);
      }
  });

  cb.cp().syst_name({"CMS_scale_j_13TeV"}).ForEachSyst([](ch::Systematic *sys) { sys->set_type("lnN");});

  cout << " done\n";

  // This function modifies every entry to have a standardised bin name of
  
  // This function modifies every entry to have a standardised bin name of
  // the form: {analysis}_{channel}_{bin_id}_{era}
  // which is commonly used in the htt analyses
  ch::SetStandardBinNames(cb);
  //! [part8]
  
  //! [part9]
  // First we generate a set of bin names:
  RooWorkspace ws("htt", "htt");

  TFile demo("htt_mssm_demo.root", "RECREATE");

  bool do_morphing = true;
  if (do_morphing) {
    auto bins = cb.bin_set();
    for (auto b : bins) {
      auto procs = cb.cp().bin({b}).signals().process_set();
      for (auto p : procs) {
        ch::BuildRooMorphing(ws, cb, b, p, faketaues,
                             "norm", true, true, false, &demo);
      }
    }
  }
  demo.Close();
  cb.AddWorkspace(ws);
  cb.cp().process({"ZL"}).ExtractPdfs(cb, "htt", "$BIN_$PROCESS_morph");
  cb.PrintAll();
  
  string folder = "output/"+output_folder+"/cmb";
  boost::filesystem::create_directories(folder);
 
 //Write out datacards. Naming convention important for rest of workflow. We
 //make one directory per chn-cat, one per chn and cmb. In this code we only
 //store the individual datacards for each directory to be combined later, but
 //note that it's also possible to write out the full combined card with CH
 
 cout << "Writing datacards ...";

  //Individual channel-cats  
  for (string chn : chns) {
     string folderchn = "output/"+output_folder+"/"+chn;
     auto bins = cb.cp().channel({chn}).bin_set();
      for (auto b : bins) {
        string folderchncat = "output/"+output_folder+"/"+b;
        boost::filesystem::create_directories(folderchn);
        boost::filesystem::create_directories(folderchncat);
        TFile output((folder + "/"+b+"_input.root").c_str(), "RECREATE");
        TFile outputchn((folderchn + "/"+b+"_input.root").c_str(), "RECREATE");
        TFile outputchncat((folderchncat + "/"+b+"_input.root").c_str(), "RECREATE");
        cb.cp().channel({chn}).bin({b}).mass({"*"}).WriteDatacard(folderchn + "/" + b + ".txt", outputchn);
        cb.cp().channel({chn}).bin({b}).mass({"*"}).WriteDatacard(folderchncat + "/" + b + ".txt", outputchncat);
        cb.cp().channel({chn}).bin({b}).mass({"*"}).WriteDatacard(folder + "/" + b + ".txt", output);
        output.Close();
        outputchn.Close();
        outputchncat.Close();
    }
  }
     
  cout << " done\n";


}

