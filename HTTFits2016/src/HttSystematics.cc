#include "CombineHarvester/HTTFits2016/interface/HttSystematics.h"
#include <vector>
#include <string>
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"

using namespace std;

namespace ch {
    
    using ch::syst::SystMap;
    using ch::syst::SystMapAsymm;
    using ch::syst::era;
    using ch::syst::channel;
    using ch::syst::bin_id;
    using ch::syst::process;
    using ch::syst::bin;
    using ch::JoinStr;
    
    void AddSystematics(CombineHarvester & cb, int control_region, bool inc_btag, bool inc_jes, bool inc_met) {

        
        
        std::vector<std::string> sig_procs = {"ggH_htt","qqH_htt","WH_htt","ZH_htt","ggHsm_htt", "ggHps_htt", "ggHmm_htt","qqHsm_htt", "qqHps_htt", "qqHmm_htt"};
        std::vector<std::string> ggH_sig_procs = {"ggH_htt","ggHsm_htt", "ggHps_htt", "ggHmm_htt"};
        std::vector<std::string> qqH_sig_procs = {"qqH_htt""qqHsm_htt", "qqHps_htt", "qqHmm_htt"};
        
        // N.B. when adding this list of backgrounds to a nuisance, only
        // the backgrounds that are included in the background process
        // defined in MorphingSM2016.cpp are included in the actual DCs
        // This is a list of all MC based backgrounds
        // QCD is explicitly excluded
        std::vector<std::string> all_mc_bkgs = {
            "ZLL","ZL","ZJ","ZTT","TTJ","TTT","TT",
            "W","VV","VVT","VVJ","EWKZ"};
        std::vector<std::string> all_mc_bkgs_no_W = {
            "ZLL","ZL","ZJ","ZTT","TTJ","TTT","TT","VV","VVT","VVJ","EWKZ"};

        //##############################################################################
        //  lumi
        //##############################################################################
        

        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs_no_W})).AddSyst(cb,
                                            "lumi_13TeV", "lnN", SystMap<>::init(1.025));
        
        //Add luminosity uncertainty for W in em, tt, ttbar and the mm region as norm is from MC
        cb.cp().process({"W"}).channel({"tt","em","mm","ttbar"}).AddSyst(cb,
                                            "lumi_13TeV", "lnN", SystMap<>::init(1.025));

        
        //##############################################################################
        //  trigger   ##FIXME   the values have been chosen rather arbitrarily
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs_no_W})).channel({"mt"}).AddSyst(cb,
                                             "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs_no_W})).channel({"et"}).AddSyst(cb,
                                             "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"em","ttbar"}).AddSyst(cb,
                                             // hard coding channel here keeps "em" and "ttbar" correlated
                                             "CMS_eff_trigger_em_$ERA", "lnN", SystMap<>::init(1.02));

        // New
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"tt"}).AddSyst(cb,
                                            "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.10));


        
        
        //##############################################################################
        //  Electron, muon and tau Id  efficiencies
        //##############################################################################
        cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
                        ({"mm"}, all_mc_bkgs,  1.02)
                        ({"mt"}, JoinStr({sig_procs, all_mc_bkgs_no_W}),  1.02)
                        ({"em","ttbar"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02));
        
        cb.cp().AddSyst(cb, "CMS_eff_e", "lnN", SystMap<channel, process>::init
                        ({"et"}, JoinStr({sig_procs, all_mc_bkgs_no_W}),  1.02)
                        ({"em","ttbar"}, JoinStr({sig_procs, all_mc_bkgs}),       1.02));
        

        // Tau Efficiency applied to all MC
        // in tautau channel the applied value depends on the number of taus which is determined by
        // gen match. WJets for example is assumed to have 1 real tau and 1 fake as is TTJ
        // compared to ZTT which has 2 real taus.
        // We also have channel specific components and fully correlated components
        //
        // ETau & MuTau
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt"}).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.045));
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt"}).AddSyst(cb,
                                             "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));

        // TauTau - 2 real taus
        cb.cp().process(JoinStr({sig_procs, {"ZTT","VV","VVT","TTT","EWKZ"}})).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.09));
        
        cb.cp().process(JoinStr({sig_procs, {"ZTT","VV","VVT","TTT","EWKZ"}})).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.04));

        // TauTau - 1+ jet to tau fakes
        cb.cp().process({"TTJ","ZJ","VVJ","W"}).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.06));
        
        cb.cp().process({"TTJ","ZJ","VVJ","W"}).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
        
        
        
        
        //##############################################################################
        //  b tag and mistag rate  efficiencies
        //##############################################################################
        if(inc_btag){
          cb.cp().process(JoinStr({sig_procs, all_mc_bkgs_no_W})).AddSyst(cb,
                                            "CMS_htt_eff_b_$ERA", "shape", SystMap<>::init(1.025));    
        }
        
        
        //##############################################################################
        //  Electron and tau energy Scale
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs, {"QCD"}})).channel({"em"}).AddSyst(cb,
                                             "CMS_scale_e_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));


        // Decay Mode based TES Settings
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).AddSyst(cb,
                                                "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).AddSyst(cb,
                                                "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).AddSyst(cb,
                                                "CMS_scale_t_3prong_$ERA", "shape", SystMap<>::init(1.00));
      

        //##############################################################################
        //  jet and met energy Scale
        //##############################################################################
 
        // MET Systematic shapes
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).bin_id({1}).channel({"et","mt","em"}).AddSyst(cb,
                                                "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).bin_id({1}).channel({"et","mt","em"}).AddSyst(cb,
                                                "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
        if(inc_met){
          cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"tt"}).AddSyst(cb,
                                                    "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
          cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"tt"}).AddSyst(cb,
                                                    "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
          
          if (control_region > 0) {
              
              cb.cp().process(all_mc_bkgs).bin_id({10, 11}).AddSyst(cb,
                                                              "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
              cb.cp().process(all_mc_bkgs).bin_id({10, 11}).AddSyst(cb,
                                                              "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
          }
        }
        
        if(inc_jes){
          cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).AddSyst(cb,"CMS_scale_j_$ERA", "shape", SystMap<>::init(1.00));
          cb.cp().process(JoinStr({{"QCD"}})).bin_id({1}).channel({"mt","et","em"}).AddSyst(cb,"CMS_scale_j_$ERA", "shape", SystMap<>::init(1.00));
        }
        
        
        //##############################################################################
        //  Background normalization uncertainties
        //##############################################################################
        
        //   Diboson  Normalisation - fully correlated
        cb.cp().process({"VV","VVT","VVJ"}).AddSyst(cb,
                                        "CMS_htt_vvXsec_13TeV", "lnN", SystMap<>::init(1.05));
        
	    cb.cp().process({"TT","TTT","TTJ"}).AddSyst(cb,
					  "CMS_htt_tjXsec_13TeV", "lnN", SystMap<>::init(1.06));
        
        cb.cp().process({"ZL","ZJ","ZTT","ZLL"}).AddSyst(cb,
                                             "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.04));
        
        cb.cp().process({"W"}).channel({"em","ttbar"}).AddSyst(cb,
                                                       "CMS_htt_jetFakeLep_13TeV", "lnN", SystMap<>::init(1.20));
        
        cb.cp().process({"W"}).channel({"tt","mm"}).AddSyst(cb,
                                                       "CMS_htt_wjXsec_13TeV", "lnN", SystMap<>::init(1.04));
        
        // QCD norm, just for em
        cb.cp().process({"QCD"}).channel({"em"}).bin_id({1}).AddSyst(cb,
                                             "CMS_htt_QCD_inclusive_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.10));
        
        // QCD norm, just for tt
        cb.cp().process({"QCD"}).channel({"tt"}).bin_id({1}).AddSyst(cb,
                                             "CMS_htt_QCD_inclusive_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.027));
        
        //Iso to antiiso extrapolation
        cb.cp().process({"QCD"}).channel({"mt"}).bin_id({1}).AddSyst(cb,
                                             "QCD_Extrap_Iso_nonIso_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.20));
        cb.cp().process({"QCD"}).channel({"et"}).bin_id({1}).AddSyst(cb,
                                             "QCD_Extrap_Iso_nonIso_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.20));
        
        
        //This should affect only shape (normalized to nominal values)
        cb.cp().process({"QCD"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                             "WSFUncert_$CHANNEL_inclusive_$ERA", "shape", SystMap<>::init(1.00));

        // based on the Ersatz study in Run1
        cb.cp().process({"W"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                             "WHighMTtoLowMT_inclusive_$ERA", "lnN", SystMap<>::init(1.10));

        
        
        //##############################################################################
        //  DY LO->NLO reweighting, Between no and twice the correc(on.
        //##############################################################################
        
        cb.cp().process( {"ZTT","ZJ","ZL","ZLL"}).AddSyst(cb,
                                             "CMS_htt_dyShape_$ERA", "shape", SystMap<>::init(1.00));
        
        //##############################################################################
        // Ttbar shape reweighting, Between no and twice the correction
        //##############################################################################
        
        cb.cp().process( {"TT","TTJ","TT"}).AddSyst(cb,
                                        "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));
        
        //##############################################################################
        // ZL shape  and electron/muon  to tau fake only in  mt and et channels (updated March 22)
        //##############################################################################
        
        cb.cp().process( {"ZL"}).channel({"mt","et"}).AddSyst(cb,
                                                         "CMS_ZLShape_$CHANNEL_1prong_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process( {"ZL"}).channel({"mt","et"}).AddSyst(cb,
                                                         "CMS_ZLShape_$CHANNEL_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        
        
        cb.cp().process( {"ZL"}).channel({"mt"}).AddSyst(cb,
                                                                     "CMS_mFakeTau_1prong_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process( {"ZL"}).channel({"mt"}).AddSyst(cb,
                                                                     "CMS_mFakeTau_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process( {"ZL"}).channel({"et"}).AddSyst(cb,
                                                                     "CMS_eFakeTau_1prong_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process( {"ZL"}).channel({"et"}).AddSyst(cb,
                                                                     "CMS_eFakeTau_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        

        
        //##############################################################################
        // jet  to tau fake only in tt, mt and et channels
        //##############################################################################
        
        cb.cp().process( {"TTJ","ZJ","VVJ","W"}).channel({"tt","mt","et"}).AddSyst(cb,
                                                                            "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));

       
            if (control_region == 1) {
            // Create rateParams for control regions:
            //  - [x] 1 rateParam for all W in every region
            //  - [x] 1 rateParam for QCD in low mT
            //  - [x] 1 rateParam for QCD in high mT
            //  - [x] lnNs for the QCD OS/SS ratio
            //         * should account for stat + syst
            //         * systs should account for: extrap. from anti-iso to iso region,
            //           possible difference between ratio in low mT and high mT (overkill?)
            //  - [x] lnNs for the W+jets OS/SS ratio
            //         * should account for stat only if not being accounted for with bbb,
            //           i.e. because the OS/SS ratio was measured with a relaxed selection
            //         * systs should account for: changes in low/high mT and OS/SS due to JES
            //           and btag (if relevant); OS/SS being wrong in the MC (from enriched data?);
            //           low/high mT being wrong in the MC (fake rate dependence?)
            
            // Going to use the regex filtering to select the right subset of
            // categories for each rateParam
            cb.SetFlag("filters-use-regex", true);
            //      for (auto bin : cb_sig.cp().channel({"et", "mt"}).bin_set()) {
            // Regex that matches, e.g. mt_nobtag or mt_nobtag_X
            
            
            
            cb.cp().bin({"mt_inclusive","mt_wjets_inclusive_cr"}).process({"W"}).AddSyst(cb, "rate_W_cr_inclusive_mt", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"et_inclusive","et_wjets_inclusive_cr"}).process({"W"}).AddSyst(cb, "rate_W_cr_inclusive_et", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"mt_inclusive","mt_antiiso_inclusive_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_inclusive_mt", "rateParam", SystMap<>::init(1.0));

            cb.cp().bin({"et_inclusive","et_antiiso_inclusive_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_inclusive_et", "rateParam", SystMap<>::init(1.0));

 
            cb.cp().bin({"tt_inclusive","tt_inclusive_qcd_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_inclusive_tt", "rateParam", SystMap<>::init(1.0));
            
            
            // Should set a sensible range for our rateParams
            for (auto sys : cb.cp().syst_type({"rateParam"}).syst_name_set()) {
                cb.GetParameter(sys)->set_range(0.0, 5.0);
            }
            cb.SetFlag("filters-use-regex", false);
        }
        
    }
}
