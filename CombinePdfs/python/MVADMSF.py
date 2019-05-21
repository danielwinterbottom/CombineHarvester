from HiggsAnalysis.CombinedLimit.PhysicsModel import PhysicsModel
import math

class MVADMSF(PhysicsModel):
	def __init__(self):
	  PhysicsModel.__init__(self)
	#def setPhysicsOptions(self, physOptions):
        #        for po in physOptions:

	def doParametersOfInterest(self):
		"""Create POI and other parameters, and define the POI set."""
		# --- POI and other parameters ----
	   
                params = {
                  'rho_r1' : 1.,
                  'rho_r2' : 1.,
                  'pi_r1' : 1.,
                  'pi_r2' : 1.,
                  'a1_r1' : 1.,
                  'a1_r2' : 1., 
                }
     
                poiNames = []
            
                SFs = ['rho_SF1','rho_SF2','a1_SF1','a1_SF2','pi_SF1','pi_SF2']
                for SF in SFs:   
                  self.modelBuilder.doVar('%s[1,0.8,1.2]' % SF)
                  poiNames.append(SF)

                for i in ['rho','a1','pi']:                
                  self.modelBuilder.factory_(('expr::%s_SF3("(1-@0*{%s_r1} - @1*{%s_r2})", %s_SF1, %s_SF2)' % (i,i,i,i,i)).format(**params))

                self.modelBuilder.doSet('POI', ','.join(poiNames))
                
	def getYieldScale(self, bin, process):
                scalings = []
                if bin in ['htt_mt_1_13TeV']: 
                    if 'rho_' in process: scalings.append('rho_SF1')
                    if 'pi_' in process: scalings.append('pi_SF1')
                    if 'a1_' in process: scalings.append('a1_SF1')
                if bin in ['htt_mt_2_13TeV']:
                    if 'rho_' in process: scalings.append('rho_SF2')
                    if 'pi_' in process: scalings.append('pi_SF2')
                    if 'a1_' in process: scalings.append('a1_SF2')  
                if bin in ['htt_mt_3_13TeV']:
                    if 'rho_' in process: scalings.append('rho_SF3')
                    if 'pi_' in process: scalings.append('pi_SF3')
                    if 'a1_' in process: scalings.append('a1_SF3')

                if len(scalings)>0:
                  scaling = '_'.join(scalings)
                  print 'Scaling %s/%s as %s' % (bin, process, scaling)
                  return scaling
                else: 
                  return 1

MVADMSF = MVADMSF()


