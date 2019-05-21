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
#include "boost/regex.hpp"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/CardWriter.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"
#include "CombineHarvester/CombineTools/interface/Algorithm.h"
#include "CombineHarvester/CombineTools/interface/AutoRebin.h"
#include "CombineHarvester/CombinePdfs/interface/MorphFunctions.h"
#include "CombineHarvester/CombineTools/interface/CopyTools.h"
#include "CombineHarvester/CombineTools/interface/JsonTools.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"

using namespace std;
using ch::JoinStr;
using ch::syst::SystMap;
using ch::syst::SystMapAsymm;
using ch::syst::era;
using ch::syst::channel;
using ch::syst::bin_id;
using ch::syst::process;
using ch::syst::bin;
using boost::starts_with;
namespace po = boost::program_options;

void ConvertShapesToLnN (ch::CombineHarvester& cb, string name) {
  auto cb_syst = cb.cp().syst_name({name});
  cb_syst.ForEachSyst([&](ch::Systematic *syst) {
    if (syst->type().find("shape") != std::string::npos) {
      std::cout << "Converting systematic " << syst->name() << " for process " << syst->process() << " in bin " << syst->bin() << " to lnN." <<std::endl;
      syst->set_type("lnN");
      return;
    }
  });
}


int main(int argc, char** argv) {

    string output_folder = "sm_run2";
    string input_folder_mt="";
    string postfix="-2D";
    bool do_embedded = false;
    po::variables_map vm;
    po::options_description config("configuration");
    config.add_options()
    ("do_embedded", po::value<bool>(&do_embedded)->default_value(false)) 
    ("input_folder_mt", po::value<string>(&input_folder_mt)->default_value(""))
    ("postfix", po::value<string>(&postfix)->default_value(postfix))
    ("output_folder", po::value<string>(&output_folder)->default_value("sm_run2"));

    po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
    po::notify(vm);
    
    
    typedef vector<string> VString;
    typedef vector<pair<int, string>> Categories;
    //! [part1]
    // First define the location of the "auxiliaries" directory where we can
    // source the input files containing the datacard shapes
    std::map<string, string> input_dir;
    input_dir["mt"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/MVADM2016/shapes/"+input_folder_mt+"/";  
    
    
    VString chns = {"mt"};
    
    map<string, VString> bkg_procs;
    VString sig_procs;
    
    bkg_procs["mt"] = {"QCD","W","ZL","ZJ","TTJ","VVJ","TTT_other","VVT_other","ZZT_other"};
    sig_procs = {"ZTT_pi","ZTT_a1","ZTT_rho","TTT_pi","TTT_a1","TTT_rho","VVT_pi","VVT_a1","VVT_rho"};
    if(do_embedded) {
      bkg_procs["mt"] = {"QCD","W","ZL","ZJ","TTJ","VVJ","TTT_other","VVT_other","ZZT_other"};
      sig_procs = {"EmbedZTT_pi","EmbedZTT_a1","EmbedZTT_rho","TTT_pi","TTT_a1","TTT_rho","VVT_pi","VVT_a1","VVT_rho"};
    }

    ch::CombineHarvester cb;
    
    map<string,Categories> cats;
    
    cats["mt"] = {
        {1, "mvadm_rho"},
        {2, "mvadm_pi"},
        {3, "mvadm_a1"}
    }; 
    
    using ch::syst::bin_id;
    
    //! [part2]
    for (auto chn : chns) {
        cb.AddObservations({"*"}, {"htt"}, {"13TeV"}, {chn}, cats[chn]);
        cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {chn}, bkg_procs[chn], cats[chn], false);
        cb.AddProcesses(   {"*"},   {"htt"}, {"13TeV"}, {chn}, sig_procs, cats[chn], true);
    }

    std::vector<std::string> embed = {"EmbedZTT_pi","EmbedZTT_a1","EmbedZTT_rho","EmbedZTT_other"};
    std::vector<std::string> real_tau = {"ZTT_pi","ZTT_a1","ZTT_rho","ZTT_other","TTT_pi","TTT_a1","TTT_rho","TTT_other","VVT_pi","VVT_a1","VVT_rho","VVT_other"};
    std::vector<std::string> jetfake = {"W","VVJ","TTJ","ZJ"};
    std::vector<std::string> jetfake_noW = {"VVJ","TTJ","ZJ"};
    std::vector<std::string> zl = {"ZL"};
    
    // Add all systematics here
    
    cb.cp().process(JoinStr({real_tau,jetfake_noW,zl})).AddSyst(cb,
                                            "lumi_13TeV", "lnN", SystMap<>::init(1.025));

    
    cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
                        ({"mt"}, JoinStr({real_tau,jetfake_noW,zl}),  1.02));
    cb.cp().AddSyst(cb, "CMS_eff_embedded_m", "lnN", SystMap<channel, process>::init
                        ({"mt"}, embed,  1.02));
    cb.cp().AddSyst(cb, "CMS_embed_norm_13TeV", "lnN", SystMap<channel, process>::init
                        ({"mt"}, embed,  1.04));
   
 
    //##############################################################################
    //  trigger   
    //##############################################################################
    
    cb.cp().process(JoinStr({real_tau,jetfake_noW,zl})).channel({"mt"}).AddSyst(cb,
                                         "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
    cb.cp().process(embed).channel({"mt"}).AddSyst(cb,
                                         "CMS_eff_embedded_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
   

    cb.cp().process(real_tau).channel({"mt"}).AddSyst(cb,
                                               "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.05)); 
    cb.cp().process(embed).channel({"mt"}).AddSyst(cb,
                                               "CMS_eff_embedded_t_$ERA", "lnN", SystMap<>::init(1.05));


    cb.cp().process({"W"}).AddSyst(cb,
                                               "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(jetfake_noW).AddSyst(cb,
                                                      "CMS_htt_jFakeTau_13TeV", "lnN", SystMap<>::init(1.20));
    cb.cp().process(zl).AddSyst(cb,
                                                      "CMS_htt_mFakeTau_13TeV", "lnN", SystMap<>::init(1.20));
    
    cb.cp().process(JoinStr({real_tau,embed})).AddSyst(cb,
                                            "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.0));
    cb.cp().process(JoinStr({real_tau,embed})).AddSyst(cb,
                                            "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.0));
    cb.cp().process(JoinStr({real_tau,embed})).AddSyst(cb,
                                            "CMS_scale_t_3prong_$ERA", "shape", SystMap<>::init(1.0));
    


    cb.cp().process({"TTT_pi","TTT_a1","TTT_rho","TTT_other","VVT_pi","VVT_a1","VVT_rho","VVT_other","VVJ","TTJ"}).AddSyst(cb,
                                                  "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
    
    cb.cp().process({"TTT_pi","TTT_a1","TTT_rho","TTT_other","VVT_pi","VVT_a1","VVT_rho","VVT_other","VVJ","TTJ"}).AddSyst(cb,"CMS_scale_j_$ERA", "shape", SystMap<>::init(1.00));

    cb.cp().process(JoinStr({sig_procs, {"ZTT_pi","ZTT_a1","ZTT_rho","ZTT_other","ZL","ZJ"}})).AddSyst(cb,
                                                  "CMS_htt_boson_reso_met_13TeV", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({sig_procs, {"ZTT_pi","ZTT_a1","ZTT_rho","ZTT_other","ZL","ZJ"}})).AddSyst(cb,
                                                  "CMS_htt_boson_scale_met_13TeV", "shape", SystMap<>::init(1.00));


    cb.cp().process(zl).channel({"mt"}).AddSyst(cb,
                                                         "CMS_ZLShape_$CHANNEL_1prong_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(zl).channel({"mt"}).AddSyst(cb,
                                                         "CMS_ZLShape_$CHANNEL_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
    
    cb.cp().process({"VVJ,""VVT_pi","VVT_a1","VVT_rho","VVT_other"}).AddSyst(cb,
                                        "CMS_htt_vvXsec_13TeV", "lnN", SystMap<>::init(1.05));
    
    cb.cp().process({"TTJ","TTT_pi","TTT_a1","TTT_rho","TTT_other"}).AddSyst(cb,
					  "CMS_htt_tjXsec_13TeV", "lnN", SystMap<>::init(1.06));


    cb.cp().process({"ZTT_pi","ZTT_a1","ZTT_rho","ZTT_other","ZL","ZJ"}).AddSyst(cb,
                                                       "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.04));
    

    cb.cp().process({"ZTT_pi","ZTT_a1","ZTT_rho","ZTT_other","ZJ","ZL"}).AddSyst(cb,
                                             "CMS_htt_dyShape_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process({"TTJ","TTT_pi","TTT_a1","TTT_rho","TTT_other"}).AddSyst(cb,
                                        "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));
    
    cb.cp().process({"QCD"}).AddSyst(cb,
                                           "CMS_QCD_OSSS_$CHANNEL_$BIN_$ERA", "lnN", SystMap<>::init(1.10));
   
    cb.cp().process({"W"}).AddSyst(cb,
                                                 "CMS_htt_W_Extrap_$CHANNEL_$BIN_13TeV", "lnN", SystMap<>::init(1.10));    
    
           
    cb.cp().process({"TTT_other","VVT_other","ZTT_other"}).AddSyst(cb,
                                                 "CMS_otheryield_$CHANNEL_$BIN_13TeV", "lnN", SystMap<>::init(1.10));
    cb.cp().process({"EmbedZTT_other"}).AddSyst(cb,
                                                 "CMS_otheryield_embedded_$CHANNEL_$BIN_13TeV", "lnN", SystMap<>::init(1.10));
 
    //! [part7]
    for (string chn : cb.channel_set()){
        string channel = chn;
        if(chn == "ttbar") channel = "em"; 
        cb.cp().channel({chn}).backgrounds().ExtractShapes(
                                                           input_dir[chn] + "htt_"+channel+".inputs-sm-13TeV"+postfix+".root",
                                                           "$BIN/$PROCESS",
                                                           "$BIN/$PROCESS_$SYSTEMATIC");
        cb.cp().channel({chn}).process(sig_procs).ExtractShapes(
                                                                  input_dir[chn] + "htt_"+chn+".inputs-sm-13TeV"+postfix+".root",
                                                                  "$BIN/$PROCESS",
                                                                  "$BIN/$PROCESS_$SYSTEMATIC");

    }

    // convert MET uncertainties to lnN
    ConvertShapesToLnN(cb, "CMS_htt_boson_reso_met_13TeV");
    ConvertShapesToLnN(cb, "CMS_htt_boson_scale_met_13TeV");
    ConvertShapesToLnN(cb, "CMS_scale_met_unclustered_13TeV");
    ConvertShapesToLnN(cb, "CMS_scale_j_13TeV");
    

    vector<string> es_uncerts = {
      "t_1prong_13TeV",
      "t_1prong1pizero_13TeV",
      "t_3prong_13TeV",
    };
    for (auto name : es_uncerts) {
      auto cb_syst = cb.cp().syst_name({"CMS_scale_"+name});
      ch::CloneSysts(cb.cp().process({"EmbedZTT"}).syst_name({"CMS_scale_"+name}), cb, [&](ch::Systematic *s) {
        s->set_name("CMS_scale_embedded_"+name);
        if (s->type().find("shape") != std::string::npos) {
          s->set_scale(s->scale() * 0.707);
        }
      });

      ch::CloneSysts(cb.cp().process({"EmbedZTT"},false).syst_name({"CMS_scale_"+name}), cb, [&](ch::Systematic *s) {
        s->set_name("CMS_scale_mc_"+name);
        if (s->type().find("shape") != std::string::npos) {
          s->set_scale(s->scale() * 0.707);
        }
      });

      cb_syst.ForEachSyst([](ch::Systematic *syst) {
        if (syst->type().find("shape") != std::string::npos) {
          syst->set_scale(syst->scale() * 0.707);
        }
      });

    }


    
    //Now delete processes with 0 yield
    cb.FilterProcs([&](ch::Process *p) {
        bool null_yield = !(p->rate() > 0.);
        if (null_yield){
            std::cout << "[Null yield] Removing process with null yield: \n ";
            std::cout << ch::Process::PrintHeader << *p << "\n";
            cb.FilterSysts([&](ch::Systematic *s){
                bool remove_syst = (MatchingProcess(*p,*s));
                return remove_syst;
            });
        }
        return null_yield;
    });
        
  
  
    // At this point we can fix the negative bins
    std::cout << "Fixing negative bins\n";
    cb.ForEachProc([](ch::Process *p) {
      if (ch::HasNegativeBins(p->shape())) {
         std::cout << "[Negative bins] Fixing negative bins for " << p->bin()
                   << "," << p->process() << "\n";
         std::cout << "[Negative bins] Before:\n";
         p->shape()->Print("range");
        auto newhist = p->ClonedShape();
        ch::ZeroNegativeBins(newhist.get());
        // Set the new shape but do not change the rate, we want the rate to still
        // reflect the total integral of the events
        p->set_shape(std::move(newhist), false);
         std::cout << "[Negative bins] After:\n";
         p->shape()->Print("range");
      }
    });
  
    cb.ForEachSyst([](ch::Systematic *s) {
      if (s->type().find("shape") == std::string::npos) return;            
      if (ch::HasNegativeBins(s->shape_u()) || ch::HasNegativeBins(s->shape_d())) {
         std::cout << "[Negative bins] Fixing negative bins for syst" << s->bin()
               << "," << s->process() << "," << s->name() << "\n";
         std::cout << "[Negative bins] Before:\n";
         s->shape_u()->Print("range");
         s->shape_d()->Print("range");
        auto newhist_u = s->ClonedShapeU();
        auto newhist_d = s->ClonedShapeD();
        ch::ZeroNegativeBins(newhist_u.get());
        ch::ZeroNegativeBins(newhist_d.get());
        // Set the new shape but do not change the rate, we want the rate to still
        // reflect the total integral of the events
        s->set_shapes(std::move(newhist_u), std::move(newhist_d), nullptr);
         std::cout << "[Negative bins] After:\n";
         s->shape_u()->Print("range");
         s->shape_d()->Print("range");
      }
    });
      
    
    ////! [part8]
    auto bbb = ch::BinByBinFactory()
    .SetAddThreshold(0.)
    .SetMergeThreshold(0.4)
    .SetFixNorm(false);
    bbb.MergeBinErrors(cb.cp().backgrounds());
    bbb.AddBinByBin(cb.cp().backgrounds(), cb);
    
    auto bbb_sig = ch::BinByBinFactory()
    .SetAddThreshold(0.)
    .SetMergeThreshold(0.0)
    .SetFixNorm(false);
    bbb_sig.AddBinByBin(cb.cp().signals(), cb); 


    ch::SetStandardBinNames(cb);
    
    string output_prefix = "output/";
    if(output_folder.compare(0,1,"/") == 0) output_prefix="";
    ch::CardWriter writer(output_prefix + output_folder + "/$TAG/125/$BIN.txt",
                          output_prefix + output_folder + "/$TAG/common/htt_input.root");
    // We're not using mass as an identifier - which we need to tell the CardWriter
    // otherwise it will see "*" as the mass value for every object and skip it
    writer.SetWildcardMasses({});
    writer.SetVerbosity(1);
    writer.WriteCards("mt_mvadm_rho", cb.cp().channel({"mt"}).bin_id({1}));     
    writer.WriteCards("mt_mvadm_pi", cb.cp().channel({"mt"}).bin_id({2}));
    writer.WriteCards("mt_mvadm_a1", cb.cp().channel({"mt"}).bin_id({3}));
    writer.WriteCards("mt_combined", cb.cp().channel({"mt"}).bin_id({1,2,3}));
  

    cb.PrintAll();
    cout << " done\n";
    
    
}
