require '/usr/local/openstudio-2.9.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new
sc = m.getShadowCalculation

m.save('test_vt_ShadowCalculation_default.osm', true)

sc.resetCalculationMethod
m.save('test_vt_ShadowCalculation_default_expanded.osm', true)

sc.setCalculationFrequency(19)
sc.setMaximumFiguresInShadowOverlapCalculations(14999)
sc.setPolygonClippingAlgorithm("ConvexWeilerAtherton")
sc.setSkyDiffuseModelingAlgorithm("DetailedSkyDiffuseModeling")
sc.setCalculationMethod("AverageOverDaysInFrequency")

m.save('test_vt_ShadowCalculation_nondefault.osm', true)


