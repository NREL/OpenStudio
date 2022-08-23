require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

def test_vt_AvailabilityManagerHybridVentilation()
  # Generate test for #3369
  # Added two required numeric fields at end of object

  m = Model.new
  avm = AvailabilityManagerHybridVentilation.new(m)

  avm.minimumOutdoorVentilationAirSchedule.setName("Min OA Schedule")

  opening_factor_curve = CurveLinear.new(m)
  opening_factor_curve.setName("Opening Factor Function of Wind Speed Curve")
  avm.setOpeningFactorFunctionofWindSpeedCurve(opening_factor_curve)

  m.save('test_vt_AvailabilityManagerHybridVentilation.osm', true)
end


test_vt_AvailabilityManagerHybridVentilation()
