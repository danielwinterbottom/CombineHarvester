from HiggsAnalysis.CombinedLimit.PhysicsModel import PhysicsModel
import math

class FakeES(PhysicsModel):
	def __init__(self):
	  PhysicsModel.__init__(self)

	#def doParametersOfInterest(self):
#		"""Create POI and other parameters, and define the POI set."""
#		# --- POI and other parameters ----
#	        
 #               poiNames = []
  #             
   #             #self.modelBuilder.doVar('faketaues[1000,9000,1100]') 
    #            #poiNames.append('faketaues')
     #           self.modelBuilder.doSet('POI', ','.join(poiNames))
                
	def getYieldScale(self, bin, process):
                return 1

FakeES = FakeES()


