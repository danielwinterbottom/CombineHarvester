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
#include "CombineHarvester/HTTFits2016/interface/HttSystematics.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "TH2.h"
#include "TF1.h"

using namespace std;
using boost::starts_with;
namespace po = boost::program_options;

template <typename T>
void To1Bin(T* proc)
{
    std::unique_ptr<TH1> originalHist = proc->ClonedScaledShape();
    TH1F *hist = new TH1F("hist","hist",1,0,1);
    double err = 0;
    double rate =
    originalHist->IntegralAndError(0, originalHist->GetNbinsX() + 1, err);
    hist->SetDirectory(0);
    hist->SetBinContent(1, rate);
    hist->SetBinError(1, err);
    proc->set_shape(*hist, true);  // True means adjust the process rate to the
    // integral of the hist
}

bool BinIsControlRegion(ch::Object const* obj)
{
    return (boost::regex_search(obj->bin(),boost::regex{"_cr$"}) || (obj->channel() == std::string("mm")));
}

// Useful to have the inverse sometimes too
bool BinIsNotControlRegion(ch::Object const* obj)
{
    return !BinIsControlRegion(obj);
}


int main(int argc, char** argv) {

    string output_folder = "sm_run2";
    string input_folder_em="Imperial/CP/";
    string input_folder_et="Imperial/CP/";
    string input_folder_mt="Imperial/CP/";
    string input_folder_tt="Imperial/CP/";
    string input_folder_mm="USCMS/";
    string input_folder_ttbar="USCMS/";
    string postfix="";
    int control_region = 0;
    bool no_shape_systs = false;
    bool inc_btag = false;
    bool inc_jes = false;
    bool inc_met = true;
    po::variables_map vm;
    po::options_description config("configuration");
    config.add_options()
    ("input_folder_em", po::value<string>(&input_folder_em)->default_value("Imperial/CP"))
    ("input_folder_et", po::value<string>(&input_folder_et)->default_value("Imperial/CP"))
    ("input_folder_mt", po::value<string>(&input_folder_mt)->default_value("Imperial/CP"))
    ("input_folder_tt", po::value<string>(&input_folder_tt)->default_value("Imperial/CP"))
    ("input_folder_mm", po::value<string>(&input_folder_mm)->default_value("USCMS"))
    ("input_folder_ttbar", po::value<string>(&input_folder_ttbar)->default_value("USCMS"))
    ("postfix", po::value<string>(&postfix)->default_value(postfix))
    ("output_folder", po::value<string>(&output_folder)->default_value("sm_run2"))
    ("control_region", po::value<int>(&control_region)->default_value(0))
    ("no_shape_systs", po::value<bool>(&no_shape_systs)->default_value(no_shape_systs))
    ("inc_btag", po::value<bool>(&inc_btag)->default_value(inc_btag))
    ("inc_jes", po::value<bool>(&inc_jes)->default_value(inc_jes))
    ("inc_met", po::value<bool>(&inc_met)->default_value(inc_met));
    po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
    po::notify(vm);
    
    
    typedef vector<string> VString;
    typedef vector<pair<int, string>> Categories;
    //! [part1]
    // First define the location of the "auxiliaries" directory where we can
    // source the input files containing the datacard shapes
    //    string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/CombineTools/bin/AllROOT_20fb/";
    std::map<string, string> input_dir;
    input_dir["em"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/HTTFits2016/shapes/"+input_folder_em+"/";
    input_dir["mt"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/HTTFits2016/shapes/"+input_folder_mt+"/";
    input_dir["et"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/HTTFits2016/shapes/"+input_folder_et+"/";
    input_dir["tt"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/HTTFits2016/shapes/"+input_folder_tt+"/";
    input_dir["mm"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/HTTFits2016/shapes/"+input_folder_mm+"/";
    input_dir["ttbar"]  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/HTTFits2016/shapes/"+input_folder_ttbar+"/";    
    
    
    VString chns = {"mt","et","tt","em"};
    
    map<string, VString> bkg_procs;

    bkg_procs["et"] = {"ZTT",   "QCD", "ZL", "ZJ","TTT","TTJ", "VVT","VVJ", "EWKZ"};
    bkg_procs["mt"] = {"ZTT",   "QCD", "ZL", "ZJ","TTT","TTJ", "VVT","VVJ", "EWKZ"};
    bkg_procs["tt"] = {"ZTT",  "W", "QCD", "ZL", "ZJ","TTT","TTJ",  "VVT","VVJ", "EWKZ"};
    bkg_procs["em"] = {"ZTT", "W", "QCD", "ZLL", "TT", "VV", "EWKZ"};
    bkg_procs["mm"] = {"W", "ZL", "TT", "VV"};
    bkg_procs["ttbar"] = {"ZTT", "W", "QCD", "ZL", "TT", "VV", "EWKZ"};
    
    ch::CombineHarvester cb;
    
    map<string,Categories> cats;
    cats["et"] = {
        {1, "et_inclusive"}
    };
    
    cats["mt"] = {
        {1, "mt_inclusive"}
    }; 
    cats["em"] = {
        {1, "em_inclusive"}
    };
    
    cats["tt"] = {
        {1, "tt_inclusive"}
    };
    
    cats["mm"] = {
        {1, "mm_inclusive"}
    };
    cats["ttbar"] = {
        {1, "ttbar_inclusive"}
    };
    
    map<string,Categories> cats_cp;
    
    
    if (control_region > 0){
        // for each channel use the categories >= 10 for the control regions
        // the control regions are ordered in triples (10,11,12),(13,14,15)...
        for (auto chn : chns){
            // for em or tt or mm do nothing
            if (ch::contains({"mt", "et"}, chn)) {
                    
                Categories queue;
                int binid = 10;
                
                queue.push_back(make_pair(binid,chn+"_wjets_inclusive_cr"));
                queue.push_back(make_pair(binid+1,chn+"_antiiso_inclusive_cr"));
                
                cats[chn].insert(cats[chn].end(),queue.begin(),queue.end());
            }
            // Add tautau QCD CRs
            if (ch::contains({"tt"}, chn)) {
                    
                Categories queue;
                int binid = 10;
                queue.push_back(make_pair(binid,chn+"_inclusive_qcd_cr"));
                
                cats[chn].insert(cats[chn].end(),queue.begin(),queue.end());
            }
        }
    } // end CR et mt > 0

    
    map<string, VString> sig_procs;
    sig_procs["ggH"] = {"ggH_htt"};
    sig_procs["qqH"] = {"qqH_htt","WH_htt","ZH_htt"}; 
    vector<string> masses = {"125"};    

    using ch::syst::bin_id;
    
    //! [part2]
    for (auto chn : chns) {
        cb.AddObservations({"*"}, {"htt"}, {"13TeV"}, {chn}, cats[chn]);

        cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {chn}, bkg_procs[chn], cats[chn], false);

        cb.AddProcesses(masses,   {"htt"}, {"13TeV"}, {chn}, sig_procs["qqH"], cats[chn], true);
        cb.AddProcesses(masses,   {"htt"}, {"13TeV"}, {chn}, sig_procs["ggH"], cats[chn], true);
        
        //Needed to add ewkz and W as these are not not available/Negative in qcd cR
    }


    if (control_region > 0){
      cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {"et"}, {"W"}, {{1, "et_inclusive"},{10, "et_wjets_inclusive_cr"},{11, "et_antiiso_inclusive_cr"}}, false);
      cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {"mt"}, {"W"}, {{1, "mt_inclusive"},{10, "mt_wjets_inclusive_cr"},{11, "mt_antiiso_inclusive_cr"}}, false);
    }else{ 
      cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {"et"}, {"W"}, {{1, "et_inclusive"}}, false);
      cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {"mt"}, {"W"}, {{1, "mt_inclusive"}}, false);
    }
    
    
    
    
    if ((control_region > 0)){
        // Since we now account for QCD in the high mT region we only
        // need to filter signal processes
        cb.FilterAll([](ch::Object const* obj) {
            return (BinIsControlRegion(obj) && obj->signal());
        });
    }
    
    
    ch::AddSystematics(cb, control_region, inc_btag, inc_jes, inc_met);
    
    if(no_shape_systs){
      cb.FilterSysts([&](ch::Systematic *s){
        return s->type().find("shape") != std::string::npos;
      });
    }
    
            
    //! [part7]
    for (string chn:chns){
        cb.cp().channel({chn}).backgrounds().ExtractShapes(
                                                           input_dir[chn] + "htt_"+chn+".inputs-sm-13TeV"+postfix+".root",
                                                           "$BIN/$PROCESS",
                                                           "$BIN/$PROCESS_$SYSTEMATIC");
        if(chn == "em" || chn == "et" || chn == "mt" || chn == "tt"){
          cb.cp().channel({chn}).process(sig_procs["ggH"]).ExtractShapes(
                                                                  input_dir[chn] + "htt_"+chn+".inputs-sm-13TeV"+postfix+".root",
                                                                  "$BIN/$PROCESS$MASS",
                                                                  "$BIN/$PROCESS$MASS_$SYSTEMATIC");
          cb.cp().channel({chn}).process(sig_procs["qqH"]).ExtractShapes(
                                                                  input_dir[chn] + "htt_"+chn+".inputs-sm-13TeV"+postfix+".root",
                                                                  "$BIN/$PROCESS$MASS",
                                                                  "$BIN/$PROCESS$MASS_$SYSTEMATIC");
        }
    }
    

    
    //Now delete processes with 0 yield
    cb.FilterProcs([&](ch::Process *p) {
        bool null_yield = !(p->rate() > 0. || BinIsControlRegion(p));
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
        
    
    
    
    // And convert any shapes in the CRs to lnN:
    // Convert all shapes to lnN at this stage
    cb.cp().FilterSysts(BinIsNotControlRegion).syst_type({"shape"}).ForEachSyst([](ch::Systematic *sys) {
        sys->set_type("lnN");
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
    .SetAddThreshold(0.05)
    .SetMergeThreshold(0.8)
    .SetFixNorm(false);
    bbb.MergeBinErrors(cb.cp().backgrounds());
    bbb.AddBinByBin(cb.cp().backgrounds(), cb);
    
    
    
    // And now do bbb for the control region with a slightly different config:
    auto bbb_ctl = ch::BinByBinFactory()
    .SetPattern("CMS_$ANALYSIS_$BIN_$ERA_$PROCESS_bin_$#")
    .SetAddThreshold(0.)
    .SetMergeThreshold(0.8)
    .SetFixNorm(false)  // contrary to signal region, bbb *should* change yield here
    .SetVerbosity(1);
    // Will merge but only for non W and QCD processes, to be on the safe side
    bbb_ctl.MergeBinErrors(cb.cp().process({"QCD", "W"}, false).FilterProcs(BinIsNotControlRegion));
    bbb_ctl.AddBinByBin(cb.cp().process({"QCD", "W"}, false).FilterProcs(BinIsNotControlRegion), cb);
    cout << " done\n";
    
    
    ch::SetStandardBinNames(cb);

    string output_prefix = "output/";
    if(output_folder.compare(0,1,"/") == 0) output_prefix="";

    ch::CardWriter writer(output_prefix + output_folder + "/$TAG/$MASS/$BIN.txt",
                          output_prefix + output_folder + "/$TAG/common/htt_input.root");
    
    
    
    writer.WriteCards("cmb", cb);
    for (auto chn : chns) {
        if(chn == std::string("mm"))
        {
            continue;
        }
        // per-channel
        writer.WriteCards(chn, cb.cp().channel({chn, "mm"}));
        // And per-channel-category
        //  THERE IS A FLAW IN COMBINEHARVESTER
        writer.WriteCards("htt_"+chn+"_1_13TeV", cb.cp().channel({chn}).bin_id({1}));
        
        
        for (auto mmm : {"125"}){


            
            if (control_region > 0){
                
                if (ch::contains({"et", "mt"}, chn)) {
                    
                
                    cb.cp().channel({chn}).bin_id({10}).mass({"$MASS", "*"}).WriteDatacard(output_prefix + output_folder +"/"+chn+"/"+mmm+ "/htt_"+chn+"_10_13TeV.txt", output_prefix + output_folder +"/"+chn+ "/common/htt_input"+chn+"10.root");
                    cb.cp().channel({chn}).bin_id({11}).mass({"$MASS", "*"}).WriteDatacard(output_prefix + output_folder +"/"+chn+"/"+mmm+ "/htt_"+chn+"_11_13TeV.txt", output_prefix + output_folder +"/"+chn+ "/common/htt_input"+chn+"11.root");

                    cb.cp().channel({chn}).bin_id({10}).mass({"$MASS", "*"}).WriteDatacard(output_prefix + output_folder +"/cmb/"+mmm+ "/htt_"+chn+"_10_13TeV.txt", output_prefix + output_folder +"/"+chn+ "/common/htt_input"+chn+"10.root");
                    cb.cp().channel({chn}).bin_id({11}).mass({"$MASS", "*"}).WriteDatacard(output_prefix + output_folder +"/cmb/"+mmm+ "/htt_"+chn+"_11_13TeV.txt", output_prefix + output_folder +"/"+chn+ "/common/htt_input"+chn+"11.root");

                    
                } // end et mt
                if (ch::contains({"tt"}, chn)) {
                    
                    cb.cp().channel({chn}).bin_id({10}).mass({"$MASS", "*"}).WriteDatacard(output_prefix + output_folder +"/"+chn+"/"+mmm+ "/htt_"+chn+"_10_13TeV.txt", output_prefix + output_folder +"/"+chn+ "/common/htt_input"+chn+"10.root");
  
                    cb.cp().channel({chn}).bin_id({10}).mass({"$MASS", "*"}).WriteDatacard(output_prefix + output_folder +"/cmb/"+mmm+ "/htt_"+chn+"_10_13TeV.txt", output_prefix + output_folder +"/"+chn+ "/common/htt_input"+chn+"10.root");

                } // end tt
            } // end CR
        }
    }

    
    cb.PrintAll();
    cout << " done\n";
    
    
}
