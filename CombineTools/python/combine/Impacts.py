#!/usr/bin/env python

import sys
import re
import json
import ROOT
import CombineHarvester.CombineTools.combine.utils as utils

from CombineHarvester.CombineTools.combine.CombineToolBase import CombineToolBase
from HiggsAnalysis.CombinedLimit.RooAddPdfFixer import FixAll


class Impacts(CombineToolBase):
    description = 'Calculate nuisance parameter impacts'
    requires_root = True

    def __init__(self):
        CombineToolBase.__init__(self)

    def attach_intercept_args(self, group):
        CombineToolBase.attach_intercept_args(self, group)
        group.add_argument('-m', '--mass', required=True)
        group.add_argument('-d', '--datacard', required=True)
        group.add_argument('--redefineSignalPOIs')
        group.add_argument('--setPhysicsModelParameters')
        group.add_argument('--setParameters')
        group.add_argument('--name', '-n', default='Test')

    def attach_args(self, group):
        CombineToolBase.attach_args(self, group)
        group.add_argument('--named', metavar='PARAM1,PARAM2,...', help=""" By
            default the list of nuisance parameters will be loaded from the
            input workspace. Use this option to specify a different list""")
        group.add_argument('--exclude', metavar='PARAM1,PARAM2,...', help=""" Skip
            these nuisances. Also accepts regexp with syntax 'rgx{<my regexp>}'""")
        group.add_argument('--doInitialFit', action='store_true', help="""Find
            the crossings of all the POIs. Must have the output from this
            before running with --doFits""")
        group.add_argument('--splitInitial', action='store_true', help="""In
            the initial fits generate separate jobs for each POI""")
        group.add_argument('--doFits', action='store_true', help="""Actually
            run the fits for the nuisance parameter impacts, otherwise just
            looks for the results""")
        group.add_argument('--allPars', action='store_true', help="""Run the
            impacts for all free parameters of the model, not just those
            listed as nuisance parameters""")
        group.add_argument('--output', '-o', help="""write output json to a
            file""")
        group.add_argument('--approx', default=None, choices=['hesse', 'robust'],
            help="""Calculate impacts using the covariance matrix instead""")

    def run_method(self):
        if self.args.allPars:
            print 'Info: the behaviour of --allPars is now always enabled and the option will be removed in a future update'
        passthru = self.passthru
        mh = self.args.mass
        ws = self.args.datacard
        name = self.args.name if self.args.name is not None else ''
        named = []
        if self.args.named is not None:
            named = self.args.named.split(',')
        # Put intercepted args back
        passthru.extend(['-m', mh])
        passthru.extend(['-d', ws])
        if self.args.setPhysicsModelParameters is not None:
            passthru.extend(['--setPhysicsModelParameters', self.args.setPhysicsModelParameters])
        if self.args.setParameters is not None:
            passthru.extend(['--setParameters', self.args.setParameters])
            self.args.setPhysicsModelParameters = self.args.setParameters
        pass_str = ' '.join(passthru)

        paramList = []
        if self.args.redefineSignalPOIs is not None:
            poiList = self.args.redefineSignalPOIs.split(',')
        else:
            poiList = utils.list_from_workspace(ws, 'w', 'ModelConfig_POI')
        print 'Have POIs: ' + str(poiList)
        poistr = ','.join(poiList)

        if self.args.approx == 'hesse' and self.args.doFits:
            self.job_queue.append(
                'combine -M MultiDimFit -n _approxFit_%(name)s --algo none --redefineSignalPOIs %(poistr)s --floatOtherPOIs 1 --saveInactivePOI 1 --saveFitResult %(pass_str)s' % {
                    'name': name,
                    'poistr': poistr,
                    'pass_str': pass_str
                })
            self.flush_queue()
            sys.exit(0)
        elif self.args.approx == 'robust' and self.args.doFits:
            self.job_queue.append(
                'combine -M MultiDimFit -n _approxFit_%(name)s --algo none --redefineSignalPOIs %(poistr)s --floatOtherPOIs 1 --saveInactivePOI 1 --robustHesse 1 %(pass_str)s' % {
                    'name': name,
                    'poistr': poistr,
                    'pass_str': pass_str
                })
            self.flush_queue()
            sys.exit(0)

        ################################################
        # Generate the initial fit(s)
        ################################################
        if self.args.doInitialFit and self.args.approx is not None:
            print 'No --initialFit needed with --approx, use --output directly'
            sys.exit(0)
        if self.args.doInitialFit:
            if self.args.splitInitial:
                for poi in poiList:
                    self.job_queue.append(
                        'combine -M MultiDimFit -n _initialFit_%(name)s_POI_%(poi)s --algo singles --redefineSignalPOIs %(poistr)s --floatOtherPOIs 1 --saveInactivePOI 1 -P %(poi)s %(pass_str)s' % {
                            'name': name,
                            'poi': poi,
                            'poistr': poistr,
                            'pass_str': pass_str
                        })
            else:
                self.job_queue.append(
                    'combine -M MultiDimFit -n _initialFit_%(name)s --algo singles --redefineSignalPOIs %(poistr)s %(pass_str)s' % {
                        'name': name,
                        'poistr': poistr,
                        'pass_str': pass_str
                    })
            self.flush_queue()
            sys.exit(0)

        # Read the initial fit results
        initialRes = {}
        if self.args.approx is not None:
            if self.args.approx == 'hesse':
                fResult = ROOT.TFile('multidimfit_approxFit_%(name)s.root' % {'name': name})
                rfr = fResult.Get('fit_mdf')
                fResult.Close()
                initialRes = utils.get_roofitresult(rfr, poiList, poiList)
            elif self.args.approx == 'robust':
                fResult = ROOT.TFile('robustHesse_approxFit_%(name)s.root' % {'name': name})
                floatParams = fResult.Get('floatParsFinal')
                rfr = fResult.Get('h_correlation')
                rfr.SetDirectory(0)
                fResult.Close()
                initialRes = utils.get_robusthesse(floatParams, rfr, poiList, poiList)
        elif self.args.splitInitial:
            for poi in poiList:
                initialRes.update(utils.get_singles_results(
                'higgsCombine_initialFit_%(name)s_POI_%(poi)s.MultiDimFit.mH%(mh)s.root' % vars(), [poi], poiList))
        else:
            initialRes = utils.get_singles_results(
                'higgsCombine_initialFit_%(name)s.MultiDimFit.mH%(mh)s.root' % vars(), poiList, poiList)

        ################################################
        # Build the parameter list
        ################################################
        if len(named) > 0:
            paramList = named
        else:
            paramList = self.all_free_parameters(ws, 'w', 'ModelConfig', poiList)
        # else:
        #     paramList = utils.list_from_workspace(
        #         ws, 'w', 'ModelConfig_NuisParams')

        # Exclude some parameters
        if self.args.exclude is not None:
            exclude = self.args.exclude.split(',')            
            expExclude = []
            for exParam in exclude:
                if 'rgx{' in exParam:
                    pattern = exParam.replace("'rgx{","").replace("}'","")
                    pattern = pattern.replace("rgx{","").replace("}","")
                    for param in paramList:
                        if re.search(pattern, param):
                            expExclude.append(param)
                else:
                    expExclude.append(exParam)
            paramList = [x for x in paramList if x not in expExclude]

        print 'Have parameters: ' + str(len(paramList))

        prefit = utils.prefit_from_workspace(ws, 'w', paramList, self.args.setPhysicsModelParameters)
        res = {}
        res["POIs"] = []
        res["params"] = []
        for poi in poiList:
            res["POIs"].append({"name": poi, "fit": initialRes[poi][poi]})

        missing = []
        for param in paramList:
            pres = {'name': param}
            pres.update(prefit[param])
            # print 'Doing param ' + str(counter) + ': ' + param
            if self.args.doFits:
                self.job_queue.append(
                    'combine -M MultiDimFit -n _paramFit_%(name)s_%(param)s --algo impact --redefineSignalPOIs %(poistr)s -P %(param)s --floatOtherPOIs 1 --saveInactivePOI 1 %(pass_str)s' % vars())
            else:
                if self.args.approx == 'hesse':
                    paramScanRes = utils.get_roofitresult(rfr, [param], poiList + [param])
                elif self.args.approx == 'robust':
                    if floatParams.find(param):
                        paramScanRes = utils.get_robusthesse(floatParams, rfr, [param], poiList + [param])
                    else:
                        paramScanRes = None
                else:
                    paramScanRes = utils.get_singles_results(
                        'higgsCombine_paramFit_%(name)s_%(param)s.MultiDimFit.mH%(mh)s.root' % vars(), [param], poiList + [param])
                if paramScanRes is None:
                    missing.append(param)
                    continue
                pres["fit"] = paramScanRes[param][param]
                for p in poiList:
                    pres.update({p: paramScanRes[param][p], 'impact_' + p: max(map(abs, (x - paramScanRes[
                                param][p][1] for x in (paramScanRes[param][p][2], paramScanRes[param][p][0]))))})
            res['params'].append(pres)
        self.flush_queue()

        if self.args.approx == 'hesse':
                res['method'] = 'hesse'
        elif self.args.approx == 'robust':
                res['method'] = 'robust'
        else:
                res['method'] = 'default'
        jsondata = json.dumps(
            res, sort_keys=True, indent=2, separators=(',', ': '))
        # print jsondata
        if self.args.output is not None:
            with open(self.args.output, 'w') as out_file:
                out_file.write(jsondata)
        if len(missing) > 0:
            print 'Missing inputs: ' + ','.join(missing)

    def all_free_parameters(self, file, wsp, mc, pois):
        res = []
        wsFile = ROOT.TFile.Open(file)
        w = wsFile.Get(wsp)
        FixAll(w)
        config = w.genobj(mc)
        pdfvars = config.GetPdf().getParameters(config.GetObservables())
        it = pdfvars.createIterator()
        var = it.Next()
        while var:
            if var.GetName() not in pois and (not var.isConstant()) and var.InheritsFrom("RooRealVar"):
                res.append(var.GetName())
            var = it.Next()
        return res
