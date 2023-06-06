/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"
#include "../SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl.hpp"

#include "../SurfacePropertyOtherSideConditionsModel.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SolarCollectorPerformancePhotovoltaicThermalBIPVT_GettersSetters) {
  Model m;
  SolarCollectorPerformancePhotovoltaicThermalBIPVT solarCollectorPerformancePhotovoltaicThermalBIPVT(m);

  solarCollectorPerformancePhotovoltaicThermalBIPVT.setName("My SolarCollectorPerformancePhotovoltaicThermalBIPVT");

  // Boundary Conditions Model Name: Required Object
  {
    SurfacePropertyOtherSideConditionsModel oscm(m);
    EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setBoundaryConditionsModel(oscm));
    EXPECT_EQ(oscm, solarCollectorPerformancePhotovoltaicThermalBIPVT.boundaryConditionsModel());
  }

  // Availability Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setAvailabilitySchedule(sch));
    EXPECT_EQ(sch, solarCollectorPerformancePhotovoltaicThermalBIPVT.availabilitySchedule());
  }

  // Effective Plenum Gap Thickness Behind PV Modules: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setEffectivePlenumGapThicknessBehindPVModules(0.5));
  EXPECT_EQ(0.5, solarCollectorPerformancePhotovoltaicThermalBIPVT.effectivePlenumGapThicknessBehindPVModules());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setEffectivePlenumGapThicknessBehindPVModules(-10.0));
  EXPECT_EQ(0.5, solarCollectorPerformancePhotovoltaicThermalBIPVT.effectivePlenumGapThicknessBehindPVModules());

  // PV Cell Normal Transmittance-Absorptance Product: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVCellNormalTransmittanceAbsorptanceProduct(0.857));
  EXPECT_EQ(0.857, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVCellNormalTransmittanceAbsorptanceProduct());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVCellNormalTransmittanceAbsorptanceProduct(-10.0));
  EXPECT_EQ(0.857, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVCellNormalTransmittanceAbsorptanceProduct());

  // Backing Material Normal Transmittance-Absorptance Product: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setBackingMaterialNormalTransmittanceAbsorptanceProduct(0.875));
  EXPECT_EQ(0.875, solarCollectorPerformancePhotovoltaicThermalBIPVT.backingMaterialNormalTransmittanceAbsorptanceProduct());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setBackingMaterialNormalTransmittanceAbsorptanceProduct(-10.0));
  EXPECT_EQ(0.875, solarCollectorPerformancePhotovoltaicThermalBIPVT.backingMaterialNormalTransmittanceAbsorptanceProduct());

  // Cladding Normal Transmittance-Absorptance Product: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setCladdingNormalTransmittanceAbsorptanceProduct(0.889));
  EXPECT_EQ(0.889, solarCollectorPerformancePhotovoltaicThermalBIPVT.claddingNormalTransmittanceAbsorptanceProduct());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setCladdingNormalTransmittanceAbsorptanceProduct(-10.0));
  EXPECT_EQ(0.889, solarCollectorPerformancePhotovoltaicThermalBIPVT.claddingNormalTransmittanceAbsorptanceProduct());

  // Fraction of Collector Gross Area Covered by PV Module: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setFractionofCollectorGrossAreaCoveredbyPVModule(0.9));
  EXPECT_EQ(0.9, solarCollectorPerformancePhotovoltaicThermalBIPVT.fractionofCollectorGrossAreaCoveredbyPVModule());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setFractionofCollectorGrossAreaCoveredbyPVModule(-10.0));
  EXPECT_EQ(0.9, solarCollectorPerformancePhotovoltaicThermalBIPVT.fractionofCollectorGrossAreaCoveredbyPVModule());

  // Fraction of PV Cell Area to PV Module Area: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setFractionofPVCellAreatoPVModuleArea(0.909));
  EXPECT_EQ(0.909, solarCollectorPerformancePhotovoltaicThermalBIPVT.fractionofPVCellAreatoPVModuleArea());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setFractionofPVCellAreatoPVModuleArea(-10.0));
  EXPECT_EQ(0.909, solarCollectorPerformancePhotovoltaicThermalBIPVT.fractionofPVCellAreatoPVModuleArea());

  // PV Module Top Thermal Resistance: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleTopThermalResistance(1.1));
  EXPECT_EQ(1.1, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleTopThermalResistance());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleTopThermalResistance(-10.0));
  EXPECT_EQ(1.1, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleTopThermalResistance());

  // PV Module Bottom Thermal Resistance: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleBottomThermalResistance(1.2));
  EXPECT_EQ(1.2, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleBottomThermalResistance());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleBottomThermalResistance(-10.0));
  EXPECT_EQ(1.2, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleBottomThermalResistance());

  // PV Module Front Longwave Emissivity: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleFrontLongwaveEmissivity(0.929));
  EXPECT_EQ(0.929, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleFrontLongwaveEmissivity());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleFrontLongwaveEmissivity(-10.0));
  EXPECT_EQ(0.929, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleFrontLongwaveEmissivity());

  // PV Module Back Longwave Emissivity: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleBackLongwaveEmissivity(0.933));
  EXPECT_EQ(0.933, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleBackLongwaveEmissivity());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setPVModuleBackLongwaveEmissivity(-10.0));
  EXPECT_EQ(0.933, solarCollectorPerformancePhotovoltaicThermalBIPVT.pVModuleBackLongwaveEmissivity());

  // Glass Thickness: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setGlassThickness(0.009));
  EXPECT_EQ(0.009, solarCollectorPerformancePhotovoltaicThermalBIPVT.glassThickness());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setGlassThickness(-10.0));
  EXPECT_EQ(0.009, solarCollectorPerformancePhotovoltaicThermalBIPVT.glassThickness());

  // Glass Refraction Index: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setGlassRefractionIndex(9.471));
  EXPECT_EQ(9.471, solarCollectorPerformancePhotovoltaicThermalBIPVT.glassRefractionIndex());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setGlassRefractionIndex(-9.0));
  EXPECT_EQ(9.471, solarCollectorPerformancePhotovoltaicThermalBIPVT.glassRefractionIndex());

  // Glass Extinction Coefficient: Required Double
  EXPECT_TRUE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setGlassExtinctionCoefficient(94.444));
  EXPECT_EQ(94.444, solarCollectorPerformancePhotovoltaicThermalBIPVT.glassExtinctionCoefficient());
  // Bad Value
  EXPECT_FALSE(solarCollectorPerformancePhotovoltaicThermalBIPVT.setGlassExtinctionCoefficient(-10.0));
  EXPECT_EQ(94.444, solarCollectorPerformancePhotovoltaicThermalBIPVT.glassExtinctionCoefficient());
}
