#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model


def test_vt_TableMultiVariableLookup_oneDim()
  m = Model.new

  capModFuncOfWaterFlow = OpenStudio::Model::TableMultiVariableLookup.new(m, 1)
  capModFuncOfWaterFlow.setName('CapModFuncOfWaterFlow')

  capModFuncOfWaterFlow.setCurveType('Quadratic')
  capModFuncOfWaterFlow.setInterpolationMethod('EvaluateCurveToLimits')
  capModFuncOfWaterFlow.setMinimumValueofX1(0)
  capModFuncOfWaterFlow.setMaximumValueofX1(1.33)
  capModFuncOfWaterFlow.setInputUnitTypeforX1('Dimensionless')
  capModFuncOfWaterFlow.setOutputUnitType('Dimensionless')

  # The goal being to test the NormalizationReference feature since the E+ 9.2.0
  # change to TableLookup, we pick one.
  norm_ref = 2.4
  capModFuncOfWaterFlow.setNormalizationReference(norm_ref)

  # Quoting I/O ref for 9.1:
  # > Both the output values and minimum/maximum curve limits
  # > are normalized as applicable.
  capModFuncOfWaterFlow.setMinimumTableOutput(0.0 * norm_ref)
  capModFuncOfWaterFlow.setMaximumTableOutput(1.04 * norm_ref)

  capModFuncOfWaterFlow.addPoint(0.0, 0.0 * norm_ref)
  capModFuncOfWaterFlow.addPoint(0.05, 0.001 * norm_ref)
  capModFuncOfWaterFlow.addPoint(0.33333, 0.71 * norm_ref)
  capModFuncOfWaterFlow.addPoint(0.5, 0.85 * norm_ref)
  capModFuncOfWaterFlow.addPoint(0.666667, 0.92 * norm_ref)
  capModFuncOfWaterFlow.addPoint(0.833333, 0.97 * norm_ref)
  capModFuncOfWaterFlow.addPoint(1.0, 1.0 * norm_ref) # <= RATING POINT
  capModFuncOfWaterFlow.addPoint(1.333333, 1.04 * norm_ref)
  m.save('test_vt_TableMultiVariableLookup_oneDim.osm')
end


def test_vt_TableMultiVariableLookup_twoDims()
# Make one with two independent variables
  m = Model.new
  table = OpenStudio::Model::TableMultiVariableLookup.new(m, 2)
  table.setName("Table With 2 dims")
  table.setCurveType('Biquadratic')
  table.setInterpolationMethod('LagrangeInterpolationLinearExtrapolation')
  table.setNumberofInterpolationPoints(3)
  table.setMinimumValueofX1(60)
  table.setMaximumValueofX1(80)
  table.setMinimumValueofX2(30)
  table.setMaximumValueofX2(100)
  table.setInputUnitTypeforX1('Temperature')
  table.setInputUnitTypeforX2('Temperature')
  table.setOutputUnitType('Dimensionless')
  table.setMinimumTableOutput(0.05)
  table.setMaximumTableOutput(1.25)
  table.resetNormalizationReference

  table.addPoint(70, 32, 0.1)
  table.addPoint(72, 32, 0.3)
  table.addPoint(74, 32, 0.5)
  table.addPoint(76, 32, 0.7)
  table.addPoint(78, 32, 0.9)

  table.addPoint(70, 45, 0.2)
  table.addPoint(72, 45, 0.4)
  table.addPoint(74, 45, 0.6)
  table.addPoint(76, 45, 0.8)
  table.addPoint(78, 45, 1.0)

  table.addPoint(70, 68, 0.3)
  table.addPoint(72, 68, 0.5)
  table.addPoint(74, 68, 0.7)
  table.addPoint(76, 68, 0.9)
  table.addPoint(78, 68, 1.1)

  table.addPoint(70, 81, 0.4)
  table.addPoint(72, 81, 0.6)
  table.addPoint(74, 81, 0.8)
  table.addPoint(76, 81, 1.0)
  table.addPoint(78, 81, 1.2)

  table.addPoint(70, 94, 0.5)
  table.addPoint(72, 94, 0.7)
  table.addPoint(74, 94, 0.9)
  table.addPoint(76, 94, 1.1)
  table.addPoint(78, 94, 1.3)

  table.addPoint(70, 107, 0.6)
  table.addPoint(72, 107, 0.8)
  table.addPoint(74, 107, 1.0)
  table.addPoint(76, 107, 1.2)
  table.addPoint(78, 107, 1.4)

  m.save('test_vt_TableMultiVariableLookup_twoDims.osm', true)
end

test_vt_TableMultiVariableLookup_oneDim()
test_vt_TableMultiVariableLookup_twoDims()
