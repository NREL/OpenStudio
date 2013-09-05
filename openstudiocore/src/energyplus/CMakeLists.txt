SET( target_name openstudio_energyplus )

SET( ${target_name}_src
  EnergyPlusAPI.hpp
  mainpage.hpp
  ErrorFile.hpp
  ErrorFile.cpp
  GeometryTranslator.hpp
  GeometryTranslator.cpp 
  MapFields.hpp
  MapFields.cpp

  ForwardTranslator.hpp
  ForwardTranslator.cpp
  ForwardTranslator/ForwardTranslateAirGap.cpp
  ForwardTranslator/ForwardTranslateAirLoopHVAC.cpp
  ForwardTranslator/ForwardTranslateAirLoopHVACOutdoorAirSystem.cpp
  ForwardTranslator/ForwardTranslateAirLoopHVACUnitaryHeatPumpAirToAir.cpp
  ForwardTranslator/ForwardTranslateAirLoopHVACZoneMixer.cpp
  ForwardTranslator/ForwardTranslateAirLoopHVACZoneSplitter.cpp
  ForwardTranslator/ForwardTranslateAirTerminalSingleDuctParallelPIUReheat.cpp
  ForwardTranslator/ForwardTranslateAirTerminalSingleDuctUncontrolled.cpp
  ForwardTranslator/ForwardTranslateAirTerminalSingleDuctVAVReheat.cpp
  ForwardTranslator/ForwardTranslateAirWallMaterial.cpp
  ForwardTranslator/ForwardTranslateBlind.cpp
  ForwardTranslator/ForwardTranslateBoilerHotWater.cpp
  ForwardTranslator/ForwardTranslateBoilerSteam.cpp
  ForwardTranslator/ForwardTranslateBuilding.cpp
  ForwardTranslator/ForwardTranslateCFactorUndergroundWallConstruction.cpp
  ForwardTranslator/ForwardTranslateChillerElectricEIR.cpp
  ForwardTranslator/ForwardTranslateCoilCoolingDXSingleSpeed.cpp
  ForwardTranslator/ForwardTranslateCoilCoolingDXTwoSpeed.cpp
  ForwardTranslator/ForwardTranslateCoilCoolingWater.cpp
  ForwardTranslator/ForwardTranslateCoilCoolingWaterToAirHeatPumpEquationFit.cpp
  ForwardTranslator/ForwardTranslateCoilHeatingDXSingleSpeed.cpp
  ForwardTranslator/ForwardTranslateCoilHeatingElectric.cpp
  ForwardTranslator/ForwardTranslateCoilHeatingGas.cpp
  ForwardTranslator/ForwardTranslateCoilHeatingWater.cpp
  ForwardTranslator/ForwardTranslateCoilHeatingWaterToAirHeatPumpEquationFit.cpp
  ForwardTranslator/ForwardTranslateConstruction.cpp
  ForwardTranslator/ForwardTranslateConstructionWithInternalSource.cpp
  ForwardTranslator/ForwardTranslateConvergenceLimits.cpp
  ForwardTranslator/ForwardTranslateControllerMechanicalVentilation.cpp
  ForwardTranslator/ForwardTranslateControllerOutdoorAir.cpp
  ForwardTranslator/ForwardTranslateControllerWaterCoil.cpp
  ForwardTranslator/ForwardTranslateCoolingTowerSingleSpeed.cpp
  ForwardTranslator/ForwardTranslateCurrencyType.cpp
  ForwardTranslator/ForwardTranslateCurveBicubic.cpp
  ForwardTranslator/ForwardTranslateCurveBiquadratic.cpp
  ForwardTranslator/ForwardTranslateCurveCubic.cpp
  ForwardTranslator/ForwardTranslateCurveDoubleExponentialDecay.cpp
  ForwardTranslator/ForwardTranslateCurveExponent.cpp
  ForwardTranslator/ForwardTranslateCurveExponentialDecay.cpp
  ForwardTranslator/ForwardTranslateCurveExponentialSkewNormal.cpp
  ForwardTranslator/ForwardTranslateCurveFanPressureRise.cpp
  ForwardTranslator/ForwardTranslateCurveFunctionalPressureDrop.cpp
  ForwardTranslator/ForwardTranslateCurveLinear.cpp
  ForwardTranslator/ForwardTranslateCurveQuadratic.cpp
  ForwardTranslator/ForwardTranslateCurveQuadraticLinear.cpp
  ForwardTranslator/ForwardTranslateCurveQuartic.cpp
  ForwardTranslator/ForwardTranslateCurveRectangularHyperbola1.cpp
  ForwardTranslator/ForwardTranslateCurveRectangularHyperbola2.cpp
  ForwardTranslator/ForwardTranslateCurveSigmoid.cpp
  ForwardTranslator/ForwardTranslateCurveTriquadratic.cpp
  ForwardTranslator/ForwardTranslateDaylightingControl.cpp
  ForwardTranslator/ForwardTranslateDaylightingDeviceShelf.cpp
  ForwardTranslator/ForwardTranslateDesignDay.cpp
  ForwardTranslator/ForwardTranslateDesignSpecificationOutdoorAir.cpp
  ForwardTranslator/ForwardTranslateDistrictCooling.cpp
  ForwardTranslator/ForwardTranslateDistrictHeating.cpp
  ForwardTranslator/ForwardTranslateElectricEquipment.cpp
  ForwardTranslator/ForwardTranslateEvaporativeCoolerDirectResearchSpecial.cpp
  ForwardTranslator/ForwardTranslateEvaporativeFluidCoolerSingleSpeed.cpp
  ForwardTranslator/ForwardTranslateExteriorLights.cpp
  ForwardTranslator/ForwardTranslateFanConstantVolume.cpp
  ForwardTranslator/ForwardTranslateFanOnOff.cpp
  ForwardTranslator/ForwardTranslateFanVariableVolume.cpp
  ForwardTranslator/ForwardTranslateFFactorGroundFloorConstruction.cpp
  ForwardTranslator/ForwardTranslateGas.cpp
  ForwardTranslator/ForwardTranslateGasEquipment.cpp
  ForwardTranslator/ForwardTranslateGasMixture.cpp
  ForwardTranslator/ForwardTranslateGroundHeatExchangerVertical.cpp
  ForwardTranslator/ForwardTranslateHeatBalanceAlgorithm.cpp
  ForwardTranslator/ForwardTranslateHeatExchangerAirToAirSensibleAndLatent.cpp
  ForwardTranslator/ForwardTranslateHotWaterEquipment.cpp
  ForwardTranslator/ForwardTranslateIlluminanceMap.cpp
  ForwardTranslator/ForwardTranslateInsideSurfaceConvectionAlgorithm.cpp
  ForwardTranslator/ForwardTranslateInfraredTransparentMaterial.cpp
  ForwardTranslator/ForwardTranslateInteriorPartitionSurface.cpp
  ForwardTranslator/ForwardTranslateInteriorPartitionSurfaceGroup.cpp
  ForwardTranslator/ForwardTranslateInternalMass.cpp
  ForwardTranslator/ForwardTranslateLifeCycleCost.cpp
  ForwardTranslator/ForwardTranslateLifeCycleCostParameters.cpp
  ForwardTranslator/ForwardTranslateLights.cpp
  ForwardTranslator/ForwardTranslateLuminaire.cpp
  ForwardTranslator/ForwardTranslateMasslessOpaqueMaterial.cpp
  ForwardTranslator/ForwardTranslateMeter.cpp
  ForwardTranslator/ForwardTranslateNode.cpp
  ForwardTranslator/ForwardTranslateOtherEquipment.cpp
  ForwardTranslator/ForwardTranslateOutsideSurfaceConvectionAlgorithm.cpp
  ForwardTranslator/ForwardTranslateOutputControlReportingTolerances.cpp
  ForwardTranslator/ForwardTranslateOutputVariable.cpp
  ForwardTranslator/ForwardTranslatePeople.cpp
  ForwardTranslator/ForwardTranslatePipeAdiabatic.cpp
  ForwardTranslator/ForwardTranslatePlantLoop.cpp
  ForwardTranslator/ForwardTranslatePortList.cpp
  ForwardTranslator/ForwardTranslatePumpConstantSpeed.cpp
  ForwardTranslator/ForwardTranslatePumpVariableSpeed.cpp
  ForwardTranslator/ForwardTranslateRefractionExtinctionGlazing.cpp
  ForwardTranslator/ForwardTranslateRoofVegetation.cpp
  ForwardTranslator/ForwardTranslateRunPeriod.cpp
  ForwardTranslator/ForwardTranslateRunPeriodControlDaylightSavingTime.cpp
  ForwardTranslator/ForwardTranslateRunPeriodControlSpecialDays.cpp
  ForwardTranslator/ForwardTranslateScreen.cpp
  ForwardTranslator/ForwardTranslateScheduleCompact.cpp
  ForwardTranslator/ForwardTranslateScheduleConstant.cpp
  ForwardTranslator/ForwardTranslateScheduleDay.cpp
  ForwardTranslator/ForwardTranslateScheduleFixedInterval.cpp
  ForwardTranslator/ForwardTranslateScheduleRuleset.cpp
  ForwardTranslator/ForwardTranslateScheduleTypeLimits.cpp
  ForwardTranslator/ForwardTranslateScheduleVariableInterval.cpp
  ForwardTranslator/ForwardTranslateScheduleWeek.cpp
  ForwardTranslator/ForwardTranslateScheduleYear.cpp
  ForwardTranslator/ForwardTranslateSetpointManagerFollowOutdoorAirTemperature.cpp
  ForwardTranslator/ForwardTranslateSetpointManagerMixedAir.cpp
  ForwardTranslator/ForwardTranslateSetpointManagerOutdoorAirReset.cpp
  ForwardTranslator/ForwardTranslateSetpointManagerScheduled.cpp
  ForwardTranslator/ForwardTranslateSetpointManagerSingleZoneReheat.cpp
  ForwardTranslator/ForwardTranslateSetpointManagerWarmest.cpp
  ForwardTranslator/ForwardTranslateShade.cpp
  ForwardTranslator/ForwardTranslateShadowCalculation.cpp
  ForwardTranslator/ForwardTranslateShadingControl.cpp
  ForwardTranslator/ForwardTranslateShadingSurface.cpp
  ForwardTranslator/ForwardTranslateShadingSurfaceGroup.cpp
  ForwardTranslator/ForwardTranslateSimpleGlazing.cpp
  ForwardTranslator/ForwardTranslateSimulationControl.cpp
  ForwardTranslator/ForwardTranslateSite.cpp
  ForwardTranslator/ForwardTranslateSiteGroundReflectance.cpp
  ForwardTranslator/ForwardTranslateSiteGroundTemperatureBuildingSurface.cpp
  ForwardTranslator/ForwardTranslateSiteWaterMainsTemperature.cpp
  ForwardTranslator/ForwardTranslateSkyTemperature.cpp
  ForwardTranslator/ForwardTranslateSizingParameters.cpp
  ForwardTranslator/ForwardTranslateSizingPlant.cpp
  ForwardTranslator/ForwardTranslateSizingSystem.cpp
  ForwardTranslator/ForwardTranslateSizingZone.cpp
  ForwardTranslator/ForwardTranslateSpace.cpp
  ForwardTranslator/ForwardTranslateSpaceType.cpp
  ForwardTranslator/ForwardTranslateSpaceInfiltrationDesignFlowRate.cpp
  ForwardTranslator/ForwardTranslateSpaceInfiltrationEffectiveLeakageArea.cpp
  ForwardTranslator/ForwardTranslateStandardGlazing.cpp
  ForwardTranslator/ForwardTranslateStandardOpaqueMaterial.cpp
  ForwardTranslator/ForwardTranslateSteamEquipment.cpp
  ForwardTranslator/ForwardTranslateSubSurface.cpp
  ForwardTranslator/ForwardTranslateSurface.cpp
  ForwardTranslator/ForwardTranslateThermalZone.cpp
  ForwardTranslator/ForwardTranslateThermostatSetpointDualSetpoint.cpp
  ForwardTranslator/ForwardTranslateTimestep.cpp
  ForwardTranslator/ForwardTranslateVersion.cpp
  ForwardTranslator/ForwardTranslateWaterHeaterMixed.cpp
  ForwardTranslator/ForwardTranslateWaterUseConnections.cpp
  ForwardTranslator/ForwardTranslateWaterUseEquipment.cpp
  ForwardTranslator/ForwardTranslateZoneAirHeatBalanceAlgorithm.cpp
  ForwardTranslator/ForwardTranslateZoneHVACBaseboardConvectiveElectric.cpp
  ForwardTranslator/ForwardTranslateZoneHVACBaseboardConvectiveWater.cpp
  ForwardTranslator/ForwardTranslateZoneHVACEquipmentList.cpp
  ForwardTranslator/ForwardTranslateZoneHVACFourPipeFanCoil.cpp    
  ForwardTranslator/ForwardTranslateZoneHVACIdealLoadsAirSystem.cpp
  ForwardTranslator/ForwardTranslateZoneHVACLowTemperatureRadiantElectric.cpp
  ForwardTranslator/ForwardTranslateZoneHVACLowTempRadiantConstFlow.cpp
  ForwardTranslator/ForwardTranslateZoneHVACLowTempRadiantVarFlow.cpp
  ForwardTranslator/ForwardTranslateZoneHVACPackagedTerminalHeatPump.cpp
  ForwardTranslator/ForwardTranslateZoneHVACPackagedTerminalAirConditioner.cpp
  ForwardTranslator/ForwardTranslateZoneHVACWaterToAirHeatPump.cpp 
  ForwardTranslator/ForwardTranslateZoneHVACUnitHeater.cpp
  ForwardTranslator/CreateAirLoopHVACSupplyPath.cpp
  ForwardTranslator/CreateAirLoopHVACReturnPath.cpp

  ReverseTranslator.hpp
  ReverseTranslator.cpp
  ReverseTranslator/ReverseTranslateAirLoopHVAC.cpp
  ReverseTranslator/ReverseTranslateAirLoopHVACOutdoorAirSystem.cpp
  ReverseTranslator/ReverseTranslateAirTerminalSingleDuctUncontrolled.cpp
  ReverseTranslator/ReverseTranslateAirTerminalSingleDuctVAVReheat.cpp
  ReverseTranslator/ReverseTranslateBuilding.cpp
  ReverseTranslator/ReverseTranslateBuildingSurfaceDetailed.cpp
  ReverseTranslator/ReverseTranslateControllerOutdoorAir.cpp
  ReverseTranslator/ReverseTranslateCoilCoolingDXSingleSpeed.cpp
  ReverseTranslator/ReverseTranslateCoilHeatingGas.cpp
  ReverseTranslator/ReverseTranslateCoilSystemCoolingDX.cpp
  ReverseTranslator/ReverseTranslateConstruction.cpp
  ReverseTranslator/ReverseTranslateConvergenceLimits.cpp
  ReverseTranslator/ReverseTranslateCurveBicubic.cpp
  ReverseTranslator/ReverseTranslateCurveBiquadratic.cpp
  ReverseTranslator/ReverseTranslateCurveCubic.cpp
  ReverseTranslator/ReverseTranslateCurveDoubleExponentialDecay.cpp
  ReverseTranslator/ReverseTranslateCurveExponent.cpp
  ReverseTranslator/ReverseTranslateCurveExponentialDecay.cpp
  ReverseTranslator/ReverseTranslateCurveExponentialSkewNormal.cpp
  ReverseTranslator/ReverseTranslateCurveFanPressureRise.cpp
  ReverseTranslator/ReverseTranslateCurveFunctionalPressureDrop.cpp
  ReverseTranslator/ReverseTranslateCurveLinear.cpp
  ReverseTranslator/ReverseTranslateCurveQuadratic.cpp
  ReverseTranslator/ReverseTranslateCurveQuadraticLinear.cpp
  ReverseTranslator/ReverseTranslateCurveQuartic.cpp
  ReverseTranslator/ReverseTranslateCurveRectangularHyperbola1.cpp
  ReverseTranslator/ReverseTranslateCurveRectangularHyperbola2.cpp
  ReverseTranslator/ReverseTranslateCurveSigmoid.cpp
  ReverseTranslator/ReverseTranslateCurveTriquadratic.cpp
  ReverseTranslator/ReverseTranslateDaylightingControls.cpp
  ReverseTranslator/ReverseTranslateDesignSpecificationOutdoorAir.cpp
  ReverseTranslator/ReverseTranslateElectricEquipment.cpp
  ReverseTranslator/ReverseTranslateEvaporativeCoolerDirectResearchSpecial.cpp
  ReverseTranslator/ReverseTranslateEvaporativeFluidCoolerSingleSpeed.cpp
  ReverseTranslator/ReverseTranslateExteriorLights.cpp
  ReverseTranslator/ReverseTranslateFanConstantVolume.cpp
  ReverseTranslator/ReverseTranslateFenestrationSurfaceDetailed.cpp 
  ReverseTranslator/ReverseTranslateGasEquipment.cpp
  ReverseTranslator/ReverseTranslateGroundHeatExchangerVertical.cpp
  ReverseTranslator/ReverseTranslateHeatBalanceAlgorithm.cpp
  ReverseTranslator/ReverseTranslateHotWaterEquipment.cpp
  ReverseTranslator/ReverseTranslateInternalMass.cpp
  ReverseTranslator/ReverseTranslateLights.cpp
  ReverseTranslator/ReverseTranslateMaterial.cpp
  ReverseTranslator/ReverseTranslateMaterialAirGap.cpp
  ReverseTranslator/ReverseTranslateMaterialNoMass.cpp
  ReverseTranslator/ReverseTranslateOtherEquipment.cpp
  ReverseTranslator/ReverseTranslateOutputIlluminanceMap.cpp
  ReverseTranslator/ReverseTranslateOutputMeter.cpp
  ReverseTranslator/ReverseTranslateOutputMeterCumulative.cpp
  ReverseTranslator/ReverseTranslateOutputMeterCumulativeMeterFileOnly.cpp
  ReverseTranslator/ReverseTranslateOutputMeterMeterFileOnly.cpp
  ReverseTranslator/ReverseTranslateOutputVariable.cpp
  ReverseTranslator/ReverseTranslatePeople.cpp
  ReverseTranslator/ReverseTranslateRunPeriod.cpp
  ReverseTranslator/ReverseTranslateRunPeriodControlDaylightSavingTime.cpp
  ReverseTranslator/ReverseTranslateRunPeriodControlSpecialDays.cpp
  ReverseTranslator/ReverseTranslateScheduleCompact.cpp
  ReverseTranslator/ReverseTranslateScheduleConstant.cpp
  ReverseTranslator/ReverseTranslateScheduleDayHourly.cpp
  ReverseTranslator/ReverseTranslateScheduleDayInterval.cpp
  ReverseTranslator/ReverseTranslateScheduleTypeLimits.cpp
  ReverseTranslator/ReverseTranslateScheduleWeekDaily.cpp
  ReverseTranslator/ReverseTranslateScheduleYear.cpp
  ReverseTranslator/ReverseTranslateSetpointManagerMixedAir.cpp
  ReverseTranslator/ReverseTranslateSetpointManagerScheduled.cpp
  ReverseTranslator/ReverseTranslateSetpointManagerSingleZoneReheat.cpp
  ReverseTranslator/ReverseTranslateShadingBuildingDetailed.cpp
  ReverseTranslator/ReverseTranslateShadingSiteDetailed.cpp
  ReverseTranslator/ReverseTranslateShadingZoneDetailed.cpp
  ReverseTranslator/ReverseTranslateShadowCalculation.cpp
  ReverseTranslator/ReverseTranslateSimulationControl.cpp
  ReverseTranslator/ReverseTranslateSiteGroundReflectance.cpp
  ReverseTranslator/ReverseTranslateSiteGroundTemperatureBuildingSurface.cpp
  ReverseTranslator/ReverseTranslateSiteLocation.cpp
  ReverseTranslator/ReverseTranslateSiteWaterMainsTemperature.cpp
  ReverseTranslator/ReverseTranslateSizingParameters.cpp
  ReverseTranslator/ReverseTranslateSizingSystem.cpp
  ReverseTranslator/ReverseTranslateSizingZone.cpp
  ReverseTranslator/ReverseTranslateSizingPeriodDesignDay.cpp
  ReverseTranslator/ReverseTranslateSteamEquipment.cpp
  ReverseTranslator/ReverseTranslateSurfaceConvectionAlgorithmInside.cpp
  ReverseTranslator/ReverseTranslateSurfaceConvectionAlgorithmOutside.cpp
  ReverseTranslator/ReverseTranslateThermostatSetpointDualSetpoint.cpp
  ReverseTranslator/ReverseTranslateTimestep.cpp
  ReverseTranslator/ReverseTranslateVersion.cpp
  ReverseTranslator/ReverseTranslateWindowMaterialGas.cpp
  ReverseTranslator/ReverseTranslateWindowMaterialGlazing.cpp
  ReverseTranslator/ReverseTranslateWindowMaterialSimpleGlazingSystem.cpp
  ReverseTranslator/ReverseTranslateZoneAirHeatBalanceAlgorithm.cpp
  ReverseTranslator/ReverseTranslateZone.cpp
  ReverseTranslator/ReverseTranslateZoneHVACEquipmentList.cpp 
  ReverseTranslator/ReverseTranslateZoneHVACIdealLoadsAirSystem.cpp
  ReverseTranslator/ReverseTranslateZoneInfiltrationDesignFlowRate.cpp
  ReverseTranslator/ReverseTranslateZoneInfiltrationEffectiveLeakageArea.cpp
  ReverseTranslator/ReverseTranslateZoneList.cpp
  ReverseTranslator/ReverseTranslateZoneVentilationDesignFlowRate.cpp
)

SET( ${target_name}_test_src
  Test/EnergyPlusFixture.hpp
  Test/EnergyPlusFixture.cpp

  Test/ErrorFile_GTest.cpp
  Test/Translator_GTest.cpp
  Test/GeometryTranslator_GTest.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/ReverseTranslator_GTest.cpp
  
  Test/AirWallMaterial_GTest.cpp
  Test/Building_GTest.cpp
  Test/Construction_GTest.cpp
  Test/DaylightingControl_GTest.cpp
  Test/DaylightingDeviceShelf_GTest.cpp
  Test/DesignSpecificationOutdoorAir_GTest.cpp
  Test/ElectricEquipment_GTest.cpp
  Test/ExteriorLights_GTest.cpp
  Test/GasEquipment_GTest.cpp
  Test/HotWaterEquipment_GTest.cpp
  Test/IlluminanceMap_GTest.cpp
  Test/InteriorPartitionSurface_GTest.cpp
  Test/InternalMass_GTest.cpp
  Test/LifeCycleCost_GTest.cpp
  Test/LifeCycleCostParameters_GTest.cpp
  Test/Lights_GTest.cpp
  Test/Luminaire_GTest.cpp
  Test/Meter_GTest.cpp
  Test/People_GTest.cpp
  Test/ScheduleInterval_GTest.cpp
  Test/ScheduleRuleset_GTest.cpp
  Test/ShadingSurface_GTest.cpp
  Test/Space_GTest.cpp
  Test/SpaceInfiltrationDesignFlowRate_GTest.cpp
  Test/SpaceInfiltrationEffectiveLeakageArea_GTest.cpp
  Test/SubSurface_GTest.cpp
  Test/Surface_GTest.cpp
  Test/ThermalZone_GTest.cpp
  Test/ZoneHVACLowTemperatureRadiantConstFlow_GTest.cpp
  Test/ZoneHVACLowTempRadiantElectric_GTest.cpp
  Test/ZoneHVACLowTempRadiantVarFlow_GTest.cpp
)

SET( ${target_name}_swig_src
  EnergyPlus.i
)

## resource files
SET( ${target_name}_qrc
  EnergyPlus.qrc
)

# generate rules for building source files from the resources
qt4_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

SET( ${target_name}_depends
  openstudio_model
)


ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} ${${target_name}_qrcs} )
TARGET_LINK_LIBRARIES( ${target_name} ${${target_name}_depends} )

AddPCH(${target_name})

IF( NOT APPLE )
INSTALL(TARGETS ${target_name} 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_src}" )
#CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
#CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
IF(BUILD_TESTING)

  ADD_DEPENDENCIES(${target_name}_tests 
    ResultsViewer_resources
    openstudio_energyplus_resources)
    
ENDIF()

MAKE_SWIG_TARGET( OpenStudioEnergyPlus EnergyPlus "${CMAKE_CURRENT_SOURCE_DIR}/EnergyPlus.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel )
