/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "Component.hpp"
#include "ComponentWatcher_Impl.hpp"
#include "Connection.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"

// central list of all concrete ModelObject header files (_Impl and non-_Impl)
// needed here for ::createObject
#include "ConcreteModelObjects.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Version_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"

#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/idd/IddObject_Impl.hpp"
#include "../utilities/idd/IddField_Impl.hpp"
#include "../utilities/idd/IddFile_Impl.hpp"
#include "../utilities/idf/Workspace_Impl.hpp" // needed for serialization

#include "../utilities/idf/IdfFile.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/plot/ProgressBar.hpp"

#include "../utilities/sql/SqlFile.hpp"

#include <boost/regex.hpp>

using openstudio::IddObjectType;
using openstudio::detail::WorkspaceObject_Impl;

using std::dynamic_pointer_cast;

struct ModelResourceInitializer{
  ModelResourceInitializer()
  {
    Q_INIT_RESOURCE(Model);
  }
};
static ModelResourceInitializer __modelResourceInitializer__;

namespace openstudio {
namespace model {

namespace detail {

  // default constructor
  Model_Impl::Model_Impl()
    : Workspace_Impl(StrictnessLevel::Draft, IddFileType::OpenStudio)
  {
    // careful not to call anything that calls shared_from_this here, this is not yet constructed
  }

  Model_Impl::Model_Impl(const IdfFile& idfFile)
    : Workspace_Impl(idfFile,StrictnessLevel(StrictnessLevel::Draft))
  {
    // careful not to call anything that calls shared_from_this here, this is not yet constructed
    if (iddFileType() != IddFileType::OpenStudio) {
      LOG_AND_THROW("Models must be constructed with the OpenStudio Idd as the underlying "
          << "data schema. (Attempted construction from IdfFile with IddFileType "
          << idfFile.iddFileType().valueDescription() << ".)");
    }
  }

  Model_Impl::Model_Impl(const openstudio::detail::Workspace_Impl& workspace,
                         bool keepHandles)
    : openstudio::detail::Workspace_Impl(workspace,keepHandles)
  {
    // careful not to call anything that calls shared_from_this here, this is not yet constructed
    if (iddFileType() != IddFileType::OpenStudio) {
      LOG_AND_THROW("Models must be constructed with the OpenStudio Idd as the underlying "
        << "data schema. (Attempted construction from Workspace with IddFileType "
        << workspace.iddFileType().valueDescription() << ".)");
    }
  }

  // copy constructor, used for clone
  Model_Impl::Model_Impl(const Model_Impl& other, bool keepHandles)
    : Workspace_Impl(other, keepHandles),
      m_sqlFile((other.m_sqlFile)?(std::shared_ptr<SqlFile>(new SqlFile(*other.m_sqlFile))):(other.m_sqlFile)),
      m_workflowJSON(WorkflowJSON(other.m_workflowJSON))
  {
    // notice we are cloning the workflow and sqlfile too, if necessary
    // careful not to call anything that calls shared_from_this here, this is not yet constructed
  }

  // copy constructor used for cloneSubset
  Model_Impl::Model_Impl(const Model_Impl& other,
                         const std::vector<Handle>& hs,
                         bool keepHandles,
                         StrictnessLevel level)
    : Workspace_Impl(other,hs,keepHandles,level),
      m_sqlFile((other.m_sqlFile)?(std::shared_ptr<SqlFile>(new SqlFile(*other.m_sqlFile))):(other.m_sqlFile)),
      m_workflowJSON(WorkflowJSON(other.m_workflowJSON))
  {
    // notice we are cloning the workflow and sqlfile too, if necessary
  }
  Workspace Model_Impl::clone(bool keepHandles) const {
    // copy everything but objects
    std::shared_ptr<Model_Impl> cloneImpl(new Model_Impl(*this,keepHandles));
    // clone objects
    createAndAddClonedObjects(model().getImpl<Model_Impl>(),cloneImpl,keepHandles);
    cloneImpl->createComponentWatchers();
    // wrap impl and return
    Model result(cloneImpl);
    return result.cast<Workspace>();
  }

  Workspace Model_Impl::cloneSubset(const std::vector<Handle>& handles,
                                    bool keepHandles,
                                    StrictnessLevel level) const
  {
    // copy everything but objects
    std::shared_ptr<Model_Impl> cloneImpl(new Model_Impl(*this,handles,keepHandles,level));
    // clone objects
    createAndAddSubsetClonedObjects(model().getImpl<Model_Impl>(),cloneImpl,handles,keepHandles);
    // wrap impl and return
    Model result(cloneImpl);
    return result.cast<Model>();
  }

  void Model_Impl::swap(Workspace& other) {
    // Workspace::swap guarantees that other is a Model

    // swap Workspace-level data
    openstudio::detail::Workspace_Impl::swap(other);

    // swap Model-level data
    std::shared_ptr<Model_Impl> otherImpl = other.getImpl<detail::Model_Impl>();

    WorkflowJSON twf = m_workflowJSON;
    setWorkflowJSON(otherImpl->workflowJSON());
    otherImpl->setWorkflowJSON(twf);

    std::shared_ptr<SqlFile> tsf = m_sqlFile;
    m_sqlFile = otherImpl->m_sqlFile;
    otherImpl->m_sqlFile = tsf;

    ComponentWatcherVector tcw = m_componentWatchers;
    m_componentWatchers = otherImpl->m_componentWatchers;
    otherImpl->m_componentWatchers = tcw;

    clearCachedData();
    otherImpl->clearCachedData();
  }

  void Model_Impl::createComponentWatchers() {
    ComponentDataVector componentDataObjects = castVector<ComponentData>(getObjectsByType(ComponentData::iddObjectType()));
    if (!m_componentWatchers.empty()) {
      OS_ASSERT(m_componentWatchers.size() == componentDataObjects.size());
      return;
    }
    for (ComponentData& object : componentDataObjects) {
      mf_createComponentWatcher(object);
    }
  }

  // Overriding this from WorkspaceObject_Impl is how all objects in the model end up
  // as model objects
  std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> Model_Impl::createObject(
      const IdfObject& object,
      bool keepHandle)
  {
    std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> result;
    IddObjectType typeToCreate = object.iddObject().type();

#define REGISTER_CONSTRUCTOR(_className) \
if (_className::iddObjectType() == typeToCreate) { \
  result = std::shared_ptr<_className##_Impl>(new _className##_Impl(object,this,keepHandle)); \
}

    REGISTER_CONSTRUCTOR(AirConditionerVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(AirGap);
    REGISTER_CONSTRUCTOR(AirLoopHVAC);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitaryHeatPumpAirToAir);
    REGISTER_CONSTRUCTOR(AirLoopHVACOutdoorAirSystem);
    REGISTER_CONSTRUCTOR(AirLoopHVACReturnPlenum);
    REGISTER_CONSTRUCTOR(AirLoopHVACSupplyPlenum);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitarySystem);
    REGISTER_CONSTRUCTOR(AirLoopHVACZoneMixer);
    REGISTER_CONSTRUCTOR(AirLoopHVACZoneSplitter);
    REGISTER_CONSTRUCTOR(AirTerminalDualDuctVAV);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctInletSideMixer);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeCooledBeam);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeFourPipeInduction);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctParallelPIUReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctSeriesPIUReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctUncontrolled);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVNoReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVHeatAndCoolNoReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVHeatAndCoolReheat);
    REGISTER_CONSTRUCTOR(AirWallMaterial);
    REGISTER_CONSTRUCTOR(AvailabilityManagerNightCycle);
    REGISTER_CONSTRUCTOR(AvailabilityManagerOptimumStart);
    REGISTER_CONSTRUCTOR(AvailabilityManagerHybridVentilation);
    REGISTER_CONSTRUCTOR(AvailabilityManagerDifferentialThermostat);
    REGISTER_CONSTRUCTOR(AvailabilityManagerNightVentilation);
    REGISTER_CONSTRUCTOR(Blind);
    REGISTER_CONSTRUCTOR(BoilerHotWater);
    REGISTER_CONSTRUCTOR(BoilerSteam);
    REGISTER_CONSTRUCTOR(Building);
    REGISTER_CONSTRUCTOR(BuildingStory);
    REGISTER_CONSTRUCTOR(BuildingUnit);
    REGISTER_CONSTRUCTOR(CentralHeatPumpSystem);
    REGISTER_CONSTRUCTOR(CentralHeatPumpSystemModule);
    REGISTER_CONSTRUCTOR(CFactorUndergroundWallConstruction);
    REGISTER_CONSTRUCTOR(ChillerAbsorption);
    REGISTER_CONSTRUCTOR(ChillerAbsorptionIndirect);
    REGISTER_CONSTRUCTOR(ChillerElectricEIR);
    REGISTER_CONSTRUCTOR(ChillerHeaterPerformanceElectricEIR);
    REGISTER_CONSTRUCTOR(ClimateZones);
    REGISTER_CONSTRUCTOR(CoilCoolingCooledBeam);
    REGISTER_CONSTRUCTOR(CoilCoolingDXMultiSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXMultiSpeedStageData);
    REGISTER_CONSTRUCTOR(CoilCoolingDXSingleSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXTwoSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXTwoStageWithHumidityControlMode);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(CoilCoolingLowTempRadiantConstFlow);
    REGISTER_CONSTRUCTOR(CoilCoolingLowTempRadiantVarFlow);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableSpeedSpeedData);
    REGISTER_CONSTRUCTOR(CoilCoolingWater);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterToAirHeatPumpEquationFit);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_CONSTRUCTOR(CoilHeatingDesuperheater);
    REGISTER_CONSTRUCTOR(CoilHeatingDXMultiSpeed);
    REGISTER_CONSTRUCTOR(CoilHeatingDXMultiSpeedStageData);
    REGISTER_CONSTRUCTOR(CoilHeatingDXSingleSpeed);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(CoilHeatingElectric);
    REGISTER_CONSTRUCTOR(CoilHeatingGas);
    REGISTER_CONSTRUCTOR(CoilHeatingGasMultiStage);
    REGISTER_CONSTRUCTOR(CoilHeatingGasMultiStageStageData);
    REGISTER_CONSTRUCTOR(CoilHeatingLowTempRadiantConstFlow);
    REGISTER_CONSTRUCTOR(CoilHeatingLowTempRadiantVarFlow);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableSpeed);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableSpeedSpeedData);
    REGISTER_CONSTRUCTOR(CoilHeatingWater);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterToAirHeatPumpEquationFit);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterBaseboard);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterBaseboardRadiant);
    REGISTER_CONSTRUCTOR(CoilPerformanceDXCooling);
    REGISTER_CONSTRUCTOR(CoilSystemCoolingWaterHeatExchangerAssisted);
    REGISTER_CONSTRUCTOR(CoilSystemCoolingDXHeatExchangerAssisted);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingAirToWaterHeatPump);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingAirToWaterHeatPumpWrapped);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingDesuperheater);
    REGISTER_CONSTRUCTOR(ComponentCostAdjustments);
    REGISTER_CONSTRUCTOR(ComponentData);
    REGISTER_CONSTRUCTOR(Connection);
    REGISTER_CONSTRUCTOR(ConnectorMixer);
    REGISTER_CONSTRUCTOR(ConnectorSplitter);
    REGISTER_CONSTRUCTOR(Construction);
    REGISTER_CONSTRUCTOR(ConstructionWithInternalSource);
    REGISTER_CONSTRUCTOR(ControllerMechanicalVentilation);
    REGISTER_CONSTRUCTOR(ControllerOutdoorAir);
    REGISTER_CONSTRUCTOR(ControllerWaterCoil);
    REGISTER_CONSTRUCTOR(ConvergenceLimits);
    REGISTER_CONSTRUCTOR(CoolingTowerPerformanceCoolTools);
    REGISTER_CONSTRUCTOR(CoolingTowerPerformanceYorkCalc);
    REGISTER_CONSTRUCTOR(CoolingTowerSingleSpeed);
    REGISTER_CONSTRUCTOR(CoolingTowerTwoSpeed);
    REGISTER_CONSTRUCTOR(CoolingTowerVariableSpeed);
    REGISTER_CONSTRUCTOR(CurrencyType);
    REGISTER_CONSTRUCTOR(CurveBicubic);
    REGISTER_CONSTRUCTOR(CurveBiquadratic);
    REGISTER_CONSTRUCTOR(CurveCubic);
    REGISTER_CONSTRUCTOR(CurveDoubleExponentialDecay);
    REGISTER_CONSTRUCTOR(CurveExponent);
    REGISTER_CONSTRUCTOR(CurveExponentialDecay);
    REGISTER_CONSTRUCTOR(CurveExponentialSkewNormal);
    REGISTER_CONSTRUCTOR(CurveFanPressureRise);
    REGISTER_CONSTRUCTOR(CurveFunctionalPressureDrop);
    REGISTER_CONSTRUCTOR(CurveLinear);
    REGISTER_CONSTRUCTOR(CurveQuadratic);
    REGISTER_CONSTRUCTOR(CurveQuadraticLinear);
    REGISTER_CONSTRUCTOR(CurveQuartic);
    REGISTER_CONSTRUCTOR(CurveRectangularHyperbola1);
    REGISTER_CONSTRUCTOR(CurveRectangularHyperbola2);
    REGISTER_CONSTRUCTOR(CurveSigmoid);
    REGISTER_CONSTRUCTOR(CurveTriquadratic);
    REGISTER_CONSTRUCTOR(DaylightingControl);
    REGISTER_CONSTRUCTOR(DaylightingDeviceShelf);
    REGISTER_CONSTRUCTOR(DaylightRedirectionDevice);
    REGISTER_CONSTRUCTOR(DefaultConstructionSet);
    REGISTER_CONSTRUCTOR(DefaultScheduleSet);
    REGISTER_CONSTRUCTOR(DefaultSubSurfaceConstructions);
    REGISTER_CONSTRUCTOR(DefaultSurfaceConstructions);
    REGISTER_CONSTRUCTOR(DesignDay);
    REGISTER_CONSTRUCTOR(DesignSpecificationOutdoorAir);
    REGISTER_CONSTRUCTOR(DesignSpecificationZoneAirDistribution);
    REGISTER_CONSTRUCTOR(DistrictCooling);
    REGISTER_CONSTRUCTOR(DistrictHeating);
    REGISTER_CONSTRUCTOR(Duct);
    REGISTER_CONSTRUCTOR(ElectricEquipment);
    REGISTER_CONSTRUCTOR(ElectricEquipmentDefinition);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterDistribution);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterInverterLookUpTable);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterInverterSimple);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterStorageSimple);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterStorageConverter);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemActuator);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemConstructionIndexVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemCurveOrTableIndexVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemGlobalVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemInternalVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemMeteredOutputVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemOutputVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemProgram);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemProgramCallingManager);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemSensor);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemSubroutine);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemTrendVariable);
    REGISTER_CONSTRUCTOR(EvaporativeCoolerDirectResearchSpecial);
    REGISTER_CONSTRUCTOR(EvaporativeCoolerIndirectResearchSpecial);
    REGISTER_CONSTRUCTOR(EvaporativeFluidCoolerSingleSpeed);
    REGISTER_CONSTRUCTOR(EvaporativeFluidCoolerTwoSpeed);
    REGISTER_CONSTRUCTOR(ExteriorLights);
    REGISTER_CONSTRUCTOR(ExteriorLightsDefinition);
    REGISTER_CONSTRUCTOR(Facility);
    REGISTER_CONSTRUCTOR(FanConstantVolume);
    REGISTER_CONSTRUCTOR(FanOnOff);
    REGISTER_CONSTRUCTOR(FanVariableVolume);
    REGISTER_CONSTRUCTOR(FanZoneExhaust);
    REGISTER_CONSTRUCTOR(FFactorGroundFloorConstruction);
    REGISTER_CONSTRUCTOR(FluidCoolerSingleSpeed);
    REGISTER_CONSTRUCTOR(FluidCoolerTwoSpeed);
    REGISTER_CONSTRUCTOR(Gas);
    REGISTER_CONSTRUCTOR(GasEquipment);
    REGISTER_CONSTRUCTOR(GasEquipmentDefinition);
    REGISTER_CONSTRUCTOR(GasMixture);
    REGISTER_CONSTRUCTOR(GeneratorFuelCell);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellAirSupply);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellAuxiliaryHeater);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellElectricalStorage);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellExhaustGasToWaterHeatExchanger);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellInverter);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellPowerModule);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellStackCooler);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellWaterSupply);
    REGISTER_CONSTRUCTOR(GeneratorFuelSupply);
    REGISTER_CONSTRUCTOR(GeneratorMicroTurbine);
    REGISTER_CONSTRUCTOR(GeneratorMicroTurbineHeatRecovery);
    REGISTER_CONSTRUCTOR(GeneratorPhotovoltaic);
    REGISTER_CONSTRUCTOR(GlareSensor);
    REGISTER_CONSTRUCTOR(GroundHeatExchangerHorizontalTrench);
    REGISTER_CONSTRUCTOR(GroundHeatExchangerVertical);
    REGISTER_CONSTRUCTOR(HeaderedPumpsConstantSpeed);
    REGISTER_CONSTRUCTOR(HeaderedPumpsVariableSpeed);
    REGISTER_CONSTRUCTOR(HeatBalanceAlgorithm);
    REGISTER_CONSTRUCTOR(HeatExchangerAirToAirSensibleAndLatent);
    REGISTER_CONSTRUCTOR(HeatExchangerFluidToFluid);
    REGISTER_CONSTRUCTOR(HeatPumpWaterToWaterEquationFitCooling);
    REGISTER_CONSTRUCTOR(HeatPumpWaterToWaterEquationFitHeating);
    REGISTER_CONSTRUCTOR(HotWaterEquipment);
    REGISTER_CONSTRUCTOR(HotWaterEquipmentDefinition);
    REGISTER_CONSTRUCTOR(HumidifierSteamElectric);
    REGISTER_CONSTRUCTOR(IlluminanceMap);
    REGISTER_CONSTRUCTOR(InfraredTransparentMaterial);
    REGISTER_CONSTRUCTOR(InsideSurfaceConvectionAlgorithm);
    REGISTER_CONSTRUCTOR(InteriorPartitionSurface);
    REGISTER_CONSTRUCTOR(InteriorPartitionSurfaceGroup);
    REGISTER_CONSTRUCTOR(InternalMass);
    REGISTER_CONSTRUCTOR(InternalMassDefinition);
    REGISTER_CONSTRUCTOR(LoadProfilePlant);
    REGISTER_CONSTRUCTOR(LifeCycleCost);
    REGISTER_CONSTRUCTOR(LifeCycleCostParameters);
    REGISTER_CONSTRUCTOR(LifeCycleCostUsePriceEscalation);
    REGISTER_CONSTRUCTOR(LightingDesignDay);
    REGISTER_CONSTRUCTOR(LightingSimulationControl);
    REGISTER_CONSTRUCTOR(LightingSimulationZone);
    REGISTER_CONSTRUCTOR(Lights);
    REGISTER_CONSTRUCTOR(LightsDefinition)
    REGISTER_CONSTRUCTOR(Luminaire);
    REGISTER_CONSTRUCTOR(LuminaireDefinition);
    REGISTER_CONSTRUCTOR(MaterialPropertyGlazingSpectralData);
    REGISTER_CONSTRUCTOR(MaterialPropertyMoisturePenetrationDepthSettings);
    REGISTER_CONSTRUCTOR(MasslessOpaqueMaterial);
    REGISTER_CONSTRUCTOR(MeterCustom);
    REGISTER_CONSTRUCTOR(MeterCustomDecrement);
    REGISTER_CONSTRUCTOR(ModelObjectList);
    REGISTER_CONSTRUCTOR(Node);
    REGISTER_CONSTRUCTOR(OtherEquipment);
    REGISTER_CONSTRUCTOR(OtherEquipmentDefinition);
    REGISTER_CONSTRUCTOR(OutputControlReportingTolerances);
    REGISTER_CONSTRUCTOR(OutputEnergyManagementSystem);
    REGISTER_CONSTRUCTOR(OutputMeter);
    REGISTER_CONSTRUCTOR(OutputVariable);
    REGISTER_CONSTRUCTOR(OutsideSurfaceConvectionAlgorithm);
    REGISTER_CONSTRUCTOR(People);
    REGISTER_CONSTRUCTOR(PeopleDefinition);
    REGISTER_CONSTRUCTOR(PhotovoltaicPerformanceEquivalentOneDiode);
    REGISTER_CONSTRUCTOR(PhotovoltaicPerformanceSimple);
    REGISTER_CONSTRUCTOR(PipeAdiabatic);
    REGISTER_CONSTRUCTOR(PipeIndoor);
    REGISTER_CONSTRUCTOR(PipeOutdoor);
    REGISTER_CONSTRUCTOR(PlantComponentTemperatureSource);
    REGISTER_CONSTRUCTOR(PlantLoop);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationCoolingLoad);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationHeatingLoad);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDryBulb);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorWetBulb);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDewpoint);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorRelativeHumidity);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDryBulbDifference);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorWetBulbDifference);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDewpointDifference);
    REGISTER_CONSTRUCTOR(PortList);
    REGISTER_CONSTRUCTOR(ProgramControl);
    REGISTER_CONSTRUCTOR(PumpConstantSpeed);
    REGISTER_CONSTRUCTOR(PumpVariableSpeed);
    REGISTER_CONSTRUCTOR(RadianceParameters);
    REGISTER_CONSTRUCTOR(RefractionExtinctionGlazing);
    REGISTER_CONSTRUCTOR(RefrigerationAirChiller);
    REGISTER_CONSTRUCTOR(RefrigerationCase);
    REGISTER_CONSTRUCTOR(RefrigerationCompressor);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserAirCooled);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserCascade);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserEvaporativeCooled);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserWaterCooled);
    REGISTER_CONSTRUCTOR(RefrigerationDefrostCycleParameters);
    REGISTER_CONSTRUCTOR(RefrigerationGasCoolerAirCooled);
    REGISTER_CONSTRUCTOR(RefrigerationSubcoolerMechanical);
    REGISTER_CONSTRUCTOR(RefrigerationSubcoolerLiquidSuction);
    REGISTER_CONSTRUCTOR(RefrigerationSecondarySystem);
    REGISTER_CONSTRUCTOR(RefrigerationSystem);
    REGISTER_CONSTRUCTOR(RefrigerationTranscriticalSystem);
    REGISTER_CONSTRUCTOR(RefrigerationWalkIn);
    REGISTER_CONSTRUCTOR(RefrigerationWalkInZoneBoundary);
    REGISTER_CONSTRUCTOR(RenderingColor);
    REGISTER_CONSTRUCTOR(RoofVegetation);
    REGISTER_CONSTRUCTOR(RunPeriod);
    REGISTER_CONSTRUCTOR(RunPeriodControlDaylightSavingTime);
    REGISTER_CONSTRUCTOR(RunPeriodControlSpecialDays);
    REGISTER_CONSTRUCTOR(ScheduleCompact);
    REGISTER_CONSTRUCTOR(ScheduleConstant);
    REGISTER_CONSTRUCTOR(ScheduleDay);
    REGISTER_CONSTRUCTOR(ScheduleFixedInterval);
    REGISTER_CONSTRUCTOR(ScheduleTypeLimits);
    REGISTER_CONSTRUCTOR(ScheduleVariableInterval);
    REGISTER_CONSTRUCTOR(ScheduleRule);
    REGISTER_CONSTRUCTOR(ScheduleRuleset);
    REGISTER_CONSTRUCTOR(ScheduleWeek);
    REGISTER_CONSTRUCTOR(ScheduleYear);
    REGISTER_CONSTRUCTOR(Screen);
    REGISTER_CONSTRUCTOR(SetpointManagerColdest);
    REGISTER_CONSTRUCTOR(SetpointManagerFollowGroundTemperature);
    REGISTER_CONSTRUCTOR(SetpointManagerFollowOutdoorAirTemperature);
    REGISTER_CONSTRUCTOR(SetpointManagerFollowSystemNodeTemperature);
    REGISTER_CONSTRUCTOR(SetpointManagerMixedAir);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneCoolingAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneHeatingAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneHumidityMaximum);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneHumidityMinimum);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneMaximumHumidityAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneMinimumHumidityAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerOutdoorAirPretreat);
    REGISTER_CONSTRUCTOR(SetpointManagerOutdoorAirReset);
    REGISTER_CONSTRUCTOR(SetpointManagerScheduled);
    REGISTER_CONSTRUCTOR(SetpointManagerScheduledDualSetpoint);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneCooling);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneHeating);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneHumidityMaximum);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneHumidityMinimum);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneOneStageCooling);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneOneStageHeating);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneReheat);
    REGISTER_CONSTRUCTOR(SetpointManagerWarmest);
    REGISTER_CONSTRUCTOR(SetpointManagerWarmestTemperatureFlow);
    REGISTER_CONSTRUCTOR(Shade);
    REGISTER_CONSTRUCTOR(ShadingControl);
    REGISTER_CONSTRUCTOR(ShadingSurface);
    REGISTER_CONSTRUCTOR(ShadingSurfaceGroup);
    REGISTER_CONSTRUCTOR(ShadowCalculation);
    REGISTER_CONSTRUCTOR(SimpleGlazing);
    REGISTER_CONSTRUCTOR(SimulationControl);
    REGISTER_CONSTRUCTOR(Site);
    REGISTER_CONSTRUCTOR(SiteGroundReflectance);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureBuildingSurface);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureDeep);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureShallow);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureFCfactorMethod);
    REGISTER_CONSTRUCTOR(SiteWaterMainsTemperature);
    REGISTER_CONSTRUCTOR(SizingParameters);
    REGISTER_CONSTRUCTOR(SizingPlant);
    REGISTER_CONSTRUCTOR(SizingSystem);
    REGISTER_CONSTRUCTOR(SizingZone);
    REGISTER_CONSTRUCTOR(SkyTemperature);
    REGISTER_CONSTRUCTOR(SolarCollectorFlatPlatePhotovoltaicThermal);
    REGISTER_CONSTRUCTOR(SolarCollectorFlatPlateWater);
    REGISTER_CONSTRUCTOR(SolarCollectorIntegralCollectorStorage);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformanceFlatPlate);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformanceIntegralCollectorStorage);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformancePhotovoltaicThermalSimple);
    REGISTER_CONSTRUCTOR(Space);
    REGISTER_CONSTRUCTOR(SpaceInfiltrationDesignFlowRate);
    REGISTER_CONSTRUCTOR(SpaceInfiltrationEffectiveLeakageArea);
    REGISTER_CONSTRUCTOR(SpaceType);
    REGISTER_CONSTRUCTOR(StandardGlazing);
    REGISTER_CONSTRUCTOR(StandardOpaqueMaterial);
    REGISTER_CONSTRUCTOR(StandardsInformationConstruction);
    REGISTER_CONSTRUCTOR(StandardsInformationMaterial);
    REGISTER_CONSTRUCTOR(SteamEquipment);
    REGISTER_CONSTRUCTOR(SteamEquipmentDefinition);
    REGISTER_CONSTRUCTOR(SubSurface);
    REGISTER_CONSTRUCTOR(Surface);
    REGISTER_CONSTRUCTOR(SurfacePropertyConvectionCoefficients)
    REGISTER_CONSTRUCTOR(SurfacePropertyConvectionCoefficientsMultipleSurface);
    REGISTER_CONSTRUCTOR(SurfacePropertyOtherSideCoefficients);
    REGISTER_CONSTRUCTOR(SurfacePropertyOtherSideConditionsModel);
    REGISTER_CONSTRUCTOR(TableMultiVariableLookup);
    REGISTER_CONSTRUCTOR(TemperingValve);
    REGISTER_CONSTRUCTOR(ThermochromicGlazing);
    REGISTER_CONSTRUCTOR(ThermostatSetpointDualSetpoint);
    REGISTER_CONSTRUCTOR(ThermalZone);
    REGISTER_CONSTRUCTOR(ThermalStorageChilledWaterStratified);
    REGISTER_CONSTRUCTOR(ThermalStorageIceDetailed);
    REGISTER_CONSTRUCTOR(Timestep);
    REGISTER_CONSTRUCTOR(UnitarySystemPerformanceMultispeed);
    REGISTER_CONSTRUCTOR(UtilityBill);
    REGISTER_CONSTRUCTOR(UtilityCost_Charge_Block);
    REGISTER_CONSTRUCTOR(UtilityCost_Charge_Simple);
    REGISTER_CONSTRUCTOR(UtilityCost_Computation);
    REGISTER_CONSTRUCTOR(UtilityCost_Qualify);
    REGISTER_CONSTRUCTOR(UtilityCost_Ratchet);
    REGISTER_CONSTRUCTOR(UtilityCost_Tariff);
    REGISTER_CONSTRUCTOR(UtilityCost_Variable);
    REGISTER_CONSTRUCTOR(Version);
    REGISTER_CONSTRUCTOR(WaterHeaterMixed);
    REGISTER_CONSTRUCTOR(WaterHeaterHeatPump);
    REGISTER_CONSTRUCTOR(WaterHeaterHeatPumpWrappedCondenser);
    REGISTER_CONSTRUCTOR(WaterHeaterStratified);
    REGISTER_CONSTRUCTOR(WaterUseConnections);
    REGISTER_CONSTRUCTOR(WaterUseEquipment);
    REGISTER_CONSTRUCTOR(WaterUseEquipmentDefinition);
    REGISTER_CONSTRUCTOR(WeatherFile);
    REGISTER_CONSTRUCTOR(WeatherFileConditionType);
    REGISTER_CONSTRUCTOR(WeatherFileDays);
    REGISTER_CONSTRUCTOR(WindowDataFile);
    REGISTER_CONSTRUCTOR(WindowPropertyFrameAndDivider);
    REGISTER_CONSTRUCTOR(YearDescription);
    REGISTER_CONSTRUCTOR(ZoneAirContaminantBalance);
    REGISTER_CONSTRUCTOR(ZoneAirHeatBalanceAlgorithm);
    REGISTER_CONSTRUCTOR(ZoneAirMassFlowConservation);
    REGISTER_CONSTRUCTOR(ZoneCapacitanceMultiplierResearchSpecial);
    REGISTER_CONSTRUCTOR(ZoneControlContaminantController);
    REGISTER_CONSTRUCTOR(ZoneControlHumidistat);
    REGISTER_CONSTRUCTOR(ZoneControlThermostatStagedDualSetpoint);
    REGISTER_CONSTRUCTOR(ZoneHVACEquipmentList);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardConvectiveElectric);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardConvectiveWater);
    REGISTER_CONSTRUCTOR(ZoneHVACIdealLoadsAirSystem);
    REGISTER_CONSTRUCTOR(ZoneHVACFourPipeFanCoil);
    REGISTER_CONSTRUCTOR(ZoneHVACHighTemperatureRadiant);
    REGISTER_CONSTRUCTOR(ZoneHVACLowTemperatureRadiantElectric);
    REGISTER_CONSTRUCTOR(ZoneHVACLowTempRadiantConstFlow);
    REGISTER_CONSTRUCTOR(ZoneHVACLowTempRadiantVarFlow);
    REGISTER_CONSTRUCTOR(ZoneHVACPackagedTerminalHeatPump);
    REGISTER_CONSTRUCTOR(ZoneHVACPackagedTerminalAirConditioner);
    REGISTER_CONSTRUCTOR(ZoneHVACTerminalUnitVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(ZoneHVACWaterToAirHeatPump);
    REGISTER_CONSTRUCTOR(ZoneHVACUnitHeater);
    REGISTER_CONSTRUCTOR(ZoneHVACDehumidifierDX);
    REGISTER_CONSTRUCTOR(ZoneHVACEnergyRecoveryVentilatorController);
    REGISTER_CONSTRUCTOR(ZoneHVACEnergyRecoveryVentilator);
    REGISTER_CONSTRUCTOR(ZoneHVACUnitVentilator);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardRadiantConvectiveWater);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardRadiantConvectiveElectric);
    REGISTER_CONSTRUCTOR(ZoneMixing);
    REGISTER_CONSTRUCTOR(ZoneVentilationDesignFlowRate);

    if (!result) {
      LOG(Warn,"Creating GenericModelObject for IddObjectType '"
          << object.iddObject().type().valueName() << "'.");
      result = std::shared_ptr<GenericModelObject_Impl>(new GenericModelObject_Impl(object, this, keepHandle));
    }

    return result;
  }

  std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> Model_Impl::createObject(
      const std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>& originalObjectImplPtr,
      bool keepHandle) {

    OS_ASSERT(originalObjectImplPtr);
    // perhaps also assert that originalObjectImplPtr is initialized?

    std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> result;
    IddObjectType typeToCreate = originalObjectImplPtr->iddObject().type();



#define REGISTER_COPYCONSTRUCTORS(_className) \
if (_className::iddObjectType() == typeToCreate) { \
  if (dynamic_pointer_cast<_className##_Impl>(originalObjectImplPtr)) { \
    result = std::shared_ptr<_className##_Impl>(new _className##_Impl( \
        *dynamic_pointer_cast<_className##_Impl>(originalObjectImplPtr),this,keepHandle)); \
  } \
  else { \
    OS_ASSERT(!dynamic_pointer_cast<ModelObject_Impl>(originalObjectImplPtr)); \
    result = std::shared_ptr<_className##_Impl>(new _className##_Impl( \
        *originalObjectImplPtr,this,keepHandle)); \
  } \
}

    REGISTER_COPYCONSTRUCTORS(AirConditionerVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(AirGap);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVAC);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitaryHeatPumpAirToAir);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACOutdoorAirSystem);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACReturnPlenum);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACSupplyPlenum);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitarySystem);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACZoneMixer);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACZoneSplitter);
    REGISTER_COPYCONSTRUCTORS(AirTerminalDualDuctVAV);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctInletSideMixer);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeCooledBeam);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeFourPipeInduction);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctParallelPIUReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctSeriesPIUReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctUncontrolled);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVNoReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVHeatAndCoolNoReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVHeatAndCoolReheat);
    REGISTER_COPYCONSTRUCTORS(AirWallMaterial);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerNightCycle);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerOptimumStart);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerHybridVentilation);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerDifferentialThermostat);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerNightVentilation);
    REGISTER_COPYCONSTRUCTORS(Blind);
    REGISTER_COPYCONSTRUCTORS(BoilerHotWater);
    REGISTER_COPYCONSTRUCTORS(BoilerSteam);
    REGISTER_COPYCONSTRUCTORS(Building);
    REGISTER_COPYCONSTRUCTORS(BuildingStory);
    REGISTER_COPYCONSTRUCTORS(BuildingUnit);
    REGISTER_COPYCONSTRUCTORS(CentralHeatPumpSystem);
    REGISTER_COPYCONSTRUCTORS(CentralHeatPumpSystemModule);
    REGISTER_COPYCONSTRUCTORS(CFactorUndergroundWallConstruction);
    REGISTER_COPYCONSTRUCTORS(ClimateZones);
    REGISTER_COPYCONSTRUCTORS(ChillerAbsorption);
    REGISTER_COPYCONSTRUCTORS(ChillerAbsorptionIndirect);
    REGISTER_COPYCONSTRUCTORS(ChillerElectricEIR);
    REGISTER_COPYCONSTRUCTORS(ChillerHeaterPerformanceElectricEIR);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingCooledBeam);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXMultiSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXMultiSpeedStageData);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXTwoStageWithHumidityControlMode);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingLowTempRadiantConstFlow);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingLowTempRadiantVarFlow);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableSpeedSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWater);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterToAirHeatPumpEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDesuperheater);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXMultiSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXMultiSpeedStageData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingElectric);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingGas);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingGasMultiStage);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingGasMultiStageStageData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingLowTempRadiantConstFlow);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingLowTempRadiantVarFlow);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableSpeedSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWater);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterToAirHeatPumpEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterBaseboard);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterBaseboardRadiant);
    REGISTER_COPYCONSTRUCTORS(CoilPerformanceDXCooling);
    REGISTER_COPYCONSTRUCTORS(CoilSystemCoolingWaterHeatExchangerAssisted);
    REGISTER_COPYCONSTRUCTORS(CoilSystemCoolingDXHeatExchangerAssisted);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingAirToWaterHeatPump);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingAirToWaterHeatPumpWrapped);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingDesuperheater);
    REGISTER_COPYCONSTRUCTORS(ComponentCostAdjustments);
    REGISTER_COPYCONSTRUCTORS(ComponentData);
    REGISTER_COPYCONSTRUCTORS(Connection);
    REGISTER_COPYCONSTRUCTORS(ConnectorMixer);
    REGISTER_COPYCONSTRUCTORS(ConnectorSplitter);
    REGISTER_COPYCONSTRUCTORS(Construction);
    REGISTER_COPYCONSTRUCTORS(ConstructionWithInternalSource);
    REGISTER_COPYCONSTRUCTORS(ControllerMechanicalVentilation);
    REGISTER_COPYCONSTRUCTORS(ControllerOutdoorAir);
    REGISTER_COPYCONSTRUCTORS(ControllerWaterCoil);
    REGISTER_COPYCONSTRUCTORS(ConvergenceLimits);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerPerformanceCoolTools);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerPerformanceYorkCalc);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CurrencyType);
    REGISTER_COPYCONSTRUCTORS(CurveBicubic);
    REGISTER_COPYCONSTRUCTORS(CurveBiquadratic);
    REGISTER_COPYCONSTRUCTORS(CurveCubic);
    REGISTER_COPYCONSTRUCTORS(CurveDoubleExponentialDecay);
    REGISTER_COPYCONSTRUCTORS(CurveExponent);
    REGISTER_COPYCONSTRUCTORS(CurveExponentialDecay);
    REGISTER_COPYCONSTRUCTORS(CurveExponentialSkewNormal);
    REGISTER_COPYCONSTRUCTORS(CurveFanPressureRise);
    REGISTER_COPYCONSTRUCTORS(CurveFunctionalPressureDrop);
    REGISTER_COPYCONSTRUCTORS(CurveLinear);
    REGISTER_COPYCONSTRUCTORS(CurveQuadratic);
    REGISTER_COPYCONSTRUCTORS(CurveQuadraticLinear);
    REGISTER_COPYCONSTRUCTORS(CurveQuartic);
    REGISTER_COPYCONSTRUCTORS(CurveRectangularHyperbola1);
    REGISTER_COPYCONSTRUCTORS(CurveRectangularHyperbola2);
    REGISTER_COPYCONSTRUCTORS(CurveSigmoid);
    REGISTER_COPYCONSTRUCTORS(CurveTriquadratic);
    REGISTER_COPYCONSTRUCTORS(DaylightingControl);
    REGISTER_COPYCONSTRUCTORS(DaylightingDeviceShelf);
    REGISTER_COPYCONSTRUCTORS(DaylightRedirectionDevice);
    REGISTER_COPYCONSTRUCTORS(DefaultConstructionSet);
    REGISTER_COPYCONSTRUCTORS(DefaultScheduleSet);
    REGISTER_COPYCONSTRUCTORS(DefaultSubSurfaceConstructions);
    REGISTER_COPYCONSTRUCTORS(DefaultSurfaceConstructions);
    REGISTER_COPYCONSTRUCTORS(DesignDay);
    REGISTER_COPYCONSTRUCTORS(DesignSpecificationOutdoorAir);
    REGISTER_COPYCONSTRUCTORS(DesignSpecificationZoneAirDistribution);
    REGISTER_COPYCONSTRUCTORS(DistrictCooling);
    REGISTER_COPYCONSTRUCTORS(DistrictHeating);
    REGISTER_COPYCONSTRUCTORS(Duct);
    REGISTER_COPYCONSTRUCTORS(ElectricEquipment);
    REGISTER_COPYCONSTRUCTORS(ElectricEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterDistribution);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterInverterLookUpTable);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterInverterSimple);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterStorageSimple);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterStorageConverter);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemActuator);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemConstructionIndexVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemCurveOrTableIndexVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemGlobalVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemInternalVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemMeteredOutputVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemOutputVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemProgram);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemProgramCallingManager);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemSensor);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemSubroutine);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemTrendVariable);
    REGISTER_COPYCONSTRUCTORS(EvaporativeCoolerDirectResearchSpecial);
    REGISTER_COPYCONSTRUCTORS(EvaporativeCoolerIndirectResearchSpecial);
    REGISTER_COPYCONSTRUCTORS(EvaporativeFluidCoolerSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(EvaporativeFluidCoolerTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(ExteriorLights);
    REGISTER_COPYCONSTRUCTORS(ExteriorLightsDefinition);
    REGISTER_COPYCONSTRUCTORS(Facility);
    REGISTER_COPYCONSTRUCTORS(FanConstantVolume);
    REGISTER_COPYCONSTRUCTORS(FanOnOff);
    REGISTER_COPYCONSTRUCTORS(FanVariableVolume);
    REGISTER_COPYCONSTRUCTORS(FanZoneExhaust);
    REGISTER_COPYCONSTRUCTORS(FFactorGroundFloorConstruction);
    REGISTER_COPYCONSTRUCTORS(FluidCoolerSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(FluidCoolerTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(Gas);
    REGISTER_COPYCONSTRUCTORS(GasEquipment);
    REGISTER_COPYCONSTRUCTORS(GasEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(GasMixture);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCell);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellAirSupply);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellAuxiliaryHeater);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellElectricalStorage);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellExhaustGasToWaterHeatExchanger);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellInverter);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellPowerModule);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellStackCooler);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellWaterSupply);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelSupply);
    REGISTER_COPYCONSTRUCTORS(GeneratorMicroTurbine);
    REGISTER_COPYCONSTRUCTORS(GeneratorMicroTurbineHeatRecovery);
    REGISTER_COPYCONSTRUCTORS(GeneratorPhotovoltaic);
    REGISTER_COPYCONSTRUCTORS(GlareSensor);
    REGISTER_COPYCONSTRUCTORS(GroundHeatExchangerHorizontalTrench);
    REGISTER_COPYCONSTRUCTORS(GroundHeatExchangerVertical);
    REGISTER_COPYCONSTRUCTORS(HeaderedPumpsConstantSpeed);
    REGISTER_COPYCONSTRUCTORS(HeaderedPumpsVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(HeatBalanceAlgorithm);
    REGISTER_COPYCONSTRUCTORS(HeatExchangerAirToAirSensibleAndLatent);
    REGISTER_COPYCONSTRUCTORS(HeatExchangerFluidToFluid);
    REGISTER_COPYCONSTRUCTORS(HeatPumpWaterToWaterEquationFitCooling);
    REGISTER_COPYCONSTRUCTORS(HeatPumpWaterToWaterEquationFitHeating);
    REGISTER_COPYCONSTRUCTORS(HotWaterEquipment);
    REGISTER_COPYCONSTRUCTORS(HotWaterEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(HumidifierSteamElectric);
    REGISTER_COPYCONSTRUCTORS(InfraredTransparentMaterial);
    REGISTER_COPYCONSTRUCTORS(InsideSurfaceConvectionAlgorithm);
    REGISTER_COPYCONSTRUCTORS(InteriorPartitionSurface);
    REGISTER_COPYCONSTRUCTORS(InteriorPartitionSurfaceGroup);
    REGISTER_COPYCONSTRUCTORS(InternalMass);
    REGISTER_COPYCONSTRUCTORS(InternalMassDefinition);
    REGISTER_COPYCONSTRUCTORS(IlluminanceMap);
    REGISTER_COPYCONSTRUCTORS(LoadProfilePlant);
    REGISTER_COPYCONSTRUCTORS(LifeCycleCost);
    REGISTER_COPYCONSTRUCTORS(LifeCycleCostParameters);
    REGISTER_COPYCONSTRUCTORS(LifeCycleCostUsePriceEscalation);
    REGISTER_COPYCONSTRUCTORS(LightingDesignDay);
    REGISTER_COPYCONSTRUCTORS(LightingSimulationControl);
    REGISTER_COPYCONSTRUCTORS(LightingSimulationZone);
    REGISTER_COPYCONSTRUCTORS(Lights);
    REGISTER_COPYCONSTRUCTORS(LightsDefinition);
    REGISTER_COPYCONSTRUCTORS(Luminaire);
    REGISTER_COPYCONSTRUCTORS(LuminaireDefinition);
    REGISTER_COPYCONSTRUCTORS(MaterialPropertyGlazingSpectralData);
    REGISTER_COPYCONSTRUCTORS(MaterialPropertyMoisturePenetrationDepthSettings);
    REGISTER_COPYCONSTRUCTORS(MasslessOpaqueMaterial);
    REGISTER_COPYCONSTRUCTORS(MeterCustom);
    REGISTER_COPYCONSTRUCTORS(MeterCustomDecrement);
    REGISTER_COPYCONSTRUCTORS(ModelObjectList);
    REGISTER_COPYCONSTRUCTORS(Node);
    REGISTER_COPYCONSTRUCTORS(OtherEquipment);
    REGISTER_COPYCONSTRUCTORS(OtherEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(OutputControlReportingTolerances);
    REGISTER_COPYCONSTRUCTORS(OutputEnergyManagementSystem);
    REGISTER_COPYCONSTRUCTORS(OutputMeter);
    REGISTER_COPYCONSTRUCTORS(OutputVariable);
    REGISTER_COPYCONSTRUCTORS(OutsideSurfaceConvectionAlgorithm);
    REGISTER_COPYCONSTRUCTORS(People);
    REGISTER_COPYCONSTRUCTORS(PeopleDefinition);
    REGISTER_COPYCONSTRUCTORS(PhotovoltaicPerformanceEquivalentOneDiode);
    REGISTER_COPYCONSTRUCTORS(PhotovoltaicPerformanceSimple);
    REGISTER_COPYCONSTRUCTORS(PipeAdiabatic);
    REGISTER_COPYCONSTRUCTORS(PipeIndoor);
    REGISTER_COPYCONSTRUCTORS(PipeOutdoor);
    REGISTER_COPYCONSTRUCTORS(PlantComponentTemperatureSource);
    REGISTER_COPYCONSTRUCTORS(PlantLoop);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationCoolingLoad);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationHeatingLoad);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDryBulb);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorWetBulb);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDewpoint);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorRelativeHumidity);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDryBulbDifference);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorWetBulbDifference);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDewpointDifference);
    REGISTER_COPYCONSTRUCTORS(PortList);
    REGISTER_COPYCONSTRUCTORS(ProgramControl);
    REGISTER_COPYCONSTRUCTORS(PumpConstantSpeed);
    REGISTER_COPYCONSTRUCTORS(PumpVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(RadianceParameters);
    REGISTER_COPYCONSTRUCTORS(RefractionExtinctionGlazing);
    REGISTER_COPYCONSTRUCTORS(RefrigerationAirChiller);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCase);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCompressor);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserAirCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserCascade);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserEvaporativeCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserWaterCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationDefrostCycleParameters);
    REGISTER_COPYCONSTRUCTORS(RefrigerationGasCoolerAirCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSubcoolerMechanical);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSubcoolerLiquidSuction);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSecondarySystem);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSystem);
    REGISTER_COPYCONSTRUCTORS(RefrigerationTranscriticalSystem);
    REGISTER_COPYCONSTRUCTORS(RefrigerationWalkIn);
    REGISTER_COPYCONSTRUCTORS(RefrigerationWalkInZoneBoundary);
    REGISTER_COPYCONSTRUCTORS(RenderingColor);
    REGISTER_COPYCONSTRUCTORS(RoofVegetation);
    REGISTER_COPYCONSTRUCTORS(RunPeriod);
    REGISTER_COPYCONSTRUCTORS(RunPeriodControlDaylightSavingTime);
    REGISTER_COPYCONSTRUCTORS(RunPeriodControlSpecialDays);
    REGISTER_COPYCONSTRUCTORS(ScheduleCompact);
    REGISTER_COPYCONSTRUCTORS(ScheduleConstant);
    REGISTER_COPYCONSTRUCTORS(ScheduleDay);
    REGISTER_COPYCONSTRUCTORS(ScheduleFixedInterval);
    REGISTER_COPYCONSTRUCTORS(ScheduleTypeLimits);
    REGISTER_COPYCONSTRUCTORS(ScheduleVariableInterval);
    REGISTER_COPYCONSTRUCTORS(ScheduleRule);
    REGISTER_COPYCONSTRUCTORS(ScheduleRuleset);
    REGISTER_COPYCONSTRUCTORS(ScheduleWeek);
    REGISTER_COPYCONSTRUCTORS(ScheduleYear);
    REGISTER_COPYCONSTRUCTORS(Screen);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerColdest);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerFollowGroundTemperature);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerFollowOutdoorAirTemperature);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerFollowSystemNodeTemperature);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMixedAir);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneCoolingAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneHeatingAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneHumidityMaximum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneHumidityMinimum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneMaximumHumidityAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneMinimumHumidityAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerOutdoorAirPretreat);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerOutdoorAirReset);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerScheduled);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerScheduledDualSetpoint);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneCooling);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneHeating);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneHumidityMaximum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneHumidityMinimum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneOneStageCooling);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneOneStageHeating);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneReheat);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerWarmest);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerWarmestTemperatureFlow);
    REGISTER_COPYCONSTRUCTORS(Shade);
    REGISTER_COPYCONSTRUCTORS(ShadingControl);
    REGISTER_COPYCONSTRUCTORS(ShadingSurface);
    REGISTER_COPYCONSTRUCTORS(ShadingSurfaceGroup);
    REGISTER_COPYCONSTRUCTORS(ShadowCalculation);
    REGISTER_COPYCONSTRUCTORS(SimpleGlazing);
    REGISTER_COPYCONSTRUCTORS(SimulationControl);
    REGISTER_COPYCONSTRUCTORS(Site);
    REGISTER_COPYCONSTRUCTORS(SiteGroundReflectance);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureBuildingSurface);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureDeep);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureShallow);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureFCfactorMethod);
    REGISTER_COPYCONSTRUCTORS(SiteWaterMainsTemperature);
    REGISTER_COPYCONSTRUCTORS(SizingParameters);
    REGISTER_COPYCONSTRUCTORS(SizingPlant);
    REGISTER_COPYCONSTRUCTORS(SizingSystem);
    REGISTER_COPYCONSTRUCTORS(SizingZone);
    REGISTER_COPYCONSTRUCTORS(SkyTemperature);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorFlatPlatePhotovoltaicThermal);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorFlatPlateWater);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorIntegralCollectorStorage);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformanceFlatPlate);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformanceIntegralCollectorStorage);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformancePhotovoltaicThermalSimple);
    REGISTER_COPYCONSTRUCTORS(Space);
    REGISTER_COPYCONSTRUCTORS(SpaceInfiltrationDesignFlowRate);
    REGISTER_COPYCONSTRUCTORS(SpaceInfiltrationEffectiveLeakageArea);
    REGISTER_COPYCONSTRUCTORS(SpaceType);
    REGISTER_COPYCONSTRUCTORS(StandardGlazing);
    REGISTER_COPYCONSTRUCTORS(StandardOpaqueMaterial);
    REGISTER_COPYCONSTRUCTORS(StandardsInformationConstruction);
    REGISTER_COPYCONSTRUCTORS(StandardsInformationMaterial);
    REGISTER_COPYCONSTRUCTORS(SteamEquipment);
    REGISTER_COPYCONSTRUCTORS(SteamEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(SubSurface);
    REGISTER_COPYCONSTRUCTORS(Surface);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyConvectionCoefficients);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyConvectionCoefficientsMultipleSurface);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyOtherSideCoefficients);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyOtherSideConditionsModel);
    REGISTER_COPYCONSTRUCTORS(TableMultiVariableLookup);
    REGISTER_COPYCONSTRUCTORS(TemperingValve);
    REGISTER_COPYCONSTRUCTORS(ThermochromicGlazing);
    REGISTER_COPYCONSTRUCTORS(ThermostatSetpointDualSetpoint);
    REGISTER_COPYCONSTRUCTORS(ThermalZone);
    REGISTER_COPYCONSTRUCTORS(ThermalStorageChilledWaterStratified);
    REGISTER_COPYCONSTRUCTORS(ThermalStorageIceDetailed);
    REGISTER_COPYCONSTRUCTORS(Timestep);
    REGISTER_COPYCONSTRUCTORS(UnitarySystemPerformanceMultispeed);
    REGISTER_COPYCONSTRUCTORS(UtilityBill);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Charge_Block);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Charge_Simple);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Computation);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Qualify);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Ratchet);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Tariff);
    REGISTER_COPYCONSTRUCTORS(UtilityCost_Variable);
    REGISTER_COPYCONSTRUCTORS(Version);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterMixed);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterHeatPump);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterHeatPumpWrappedCondenser);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterStratified);
    REGISTER_COPYCONSTRUCTORS(WaterUseConnections);
    REGISTER_COPYCONSTRUCTORS(WaterUseEquipment);
    REGISTER_COPYCONSTRUCTORS(WaterUseEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(WeatherFile);
    REGISTER_COPYCONSTRUCTORS(WeatherFileConditionType);
    REGISTER_COPYCONSTRUCTORS(WeatherFileDays);
    REGISTER_COPYCONSTRUCTORS(WindowDataFile);
    REGISTER_COPYCONSTRUCTORS(WindowPropertyFrameAndDivider);
    REGISTER_COPYCONSTRUCTORS(YearDescription);
    REGISTER_COPYCONSTRUCTORS(ZoneAirContaminantBalance);
    REGISTER_COPYCONSTRUCTORS(ZoneAirHeatBalanceAlgorithm);
    REGISTER_COPYCONSTRUCTORS(ZoneAirMassFlowConservation);
    REGISTER_COPYCONSTRUCTORS(ZoneCapacitanceMultiplierResearchSpecial);
    REGISTER_COPYCONSTRUCTORS(ZoneControlContaminantController);
    REGISTER_COPYCONSTRUCTORS(ZoneControlHumidistat);
    REGISTER_COPYCONSTRUCTORS(ZoneControlThermostatStagedDualSetpoint);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEquipmentList);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardConvectiveElectric);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardConvectiveWater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACIdealLoadsAirSystem);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACFourPipeFanCoil);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACHighTemperatureRadiant);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACLowTemperatureRadiantElectric);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACLowTempRadiantConstFlow);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACLowTempRadiantVarFlow);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACPackagedTerminalHeatPump);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACPackagedTerminalAirConditioner);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACTerminalUnitVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACWaterToAirHeatPump);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACUnitHeater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACDehumidifierDX);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEnergyRecoveryVentilatorController);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEnergyRecoveryVentilator);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACUnitVentilator);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardRadiantConvectiveWater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardRadiantConvectiveElectric);
    REGISTER_COPYCONSTRUCTORS(ZoneMixing);
    REGISTER_COPYCONSTRUCTORS(ZoneVentilationDesignFlowRate);

    if (!result) {
      LOG(Warn,"Creating GenericModelObject for IddObjectType '"
          << originalObjectImplPtr->iddObject().type().valueName() << "'.");
      if (dynamic_pointer_cast<GenericModelObject_Impl>(originalObjectImplPtr)) {
        result = std::shared_ptr<GenericModelObject_Impl>(new GenericModelObject_Impl(
            *dynamic_pointer_cast<GenericModelObject_Impl>(originalObjectImplPtr), this, keepHandle));
      }
      else {
        if (dynamic_pointer_cast<ModelObject_Impl>(originalObjectImplPtr)) {
          std::cout << "Please register copy constructors for IddObjectType '"
                    << originalObjectImplPtr->iddObject().type().valueName() << "'." << std::endl;
          LOG_AND_THROW("Trying to copy a ModelObject, but the copy constructors are not "
              << "registered for IddObjectType '"
              << originalObjectImplPtr->iddObject().type().valueName() << "'.");
        }
        result = std::shared_ptr<GenericModelObject_Impl>(
            new GenericModelObject_Impl(*originalObjectImplPtr,this,keepHandle));
      }
    }

    return result;
  }

  Model Model_Impl::model() const
  {
    // const cast looks pretty bad but is justified here as this operation does not
    // modify the model, this is similar to a copy constructor, don't abuse it though
    return Model(std::dynamic_pointer_cast<Model_Impl>(std::const_pointer_cast<openstudio::detail::Workspace_Impl>(this->shared_from_this())));
  }

  bool Model_Impl::setIddFile(IddFileType iddFileType) {
    OS_ASSERT(iddFileType == IddFileType::OpenStudio);
    return false;
  }

  boost::optional<Building> Model_Impl::building() const
  {
    if (m_cachedBuilding){
      return m_cachedBuilding;
    }

    boost::optional<Building> result = this->model().getOptionalUniqueModelObject<Building>();
    if (result){
      m_cachedBuilding = result;
      result->getImpl<Building_Impl>().get()->Building_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedBuilding>(const_cast<openstudio::model::detail::Model_Impl *>(this));
    }

    return m_cachedBuilding;
  }

  boost::optional<LifeCycleCostParameters> Model_Impl::lifeCycleCostParameters() const
  {
    if (m_cachedLifeCycleCostParameters){
      return m_cachedLifeCycleCostParameters;
    }

    boost::optional<LifeCycleCostParameters> result = this->model().getOptionalUniqueModelObject<LifeCycleCostParameters>();
    if (result){
      m_cachedLifeCycleCostParameters = result;
      result->getImpl<LifeCycleCostParameters_Impl>().get()->LifeCycleCostParameters_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedLifeCycleCostParameters>(const_cast<openstudio::model::detail::Model_Impl *>(this));
    }

    return m_cachedLifeCycleCostParameters;
  }

  boost::optional<RunPeriod> Model_Impl::runPeriod() const
  {
    if (m_cachedRunPeriod){
      return m_cachedRunPeriod;
    }

    boost::optional<RunPeriod> result = this->model().getOptionalUniqueModelObject<RunPeriod>();
    if (result){
      m_cachedRunPeriod = result;
      result->getImpl<RunPeriod_Impl>().get()->RunPeriod_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedRunPeriod>(const_cast<openstudio::model::detail::Model_Impl *>(this));
    }

    return m_cachedRunPeriod;
  }

  boost::optional<YearDescription> Model_Impl::yearDescription() const
  {
    if (m_cachedYearDescription){
      return m_cachedYearDescription;
    }

    boost::optional<YearDescription> result = this->model().getOptionalUniqueModelObject<YearDescription>();
    if (result){
      m_cachedYearDescription = result;
      result->getImpl<YearDescription_Impl>().get()->YearDescription_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedYearDescription>(const_cast<openstudio::model::detail::Model_Impl *>(this));
    }

    return m_cachedYearDescription;
  }

  boost::optional<int> Model_Impl::calendarYear() const
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->calendarYear();
  }

  std::string Model_Impl::dayofWeekforStartDay() const
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->dayofWeekforStartDay();
  }

  bool Model_Impl::isDayofWeekforStartDayDefaulted() const
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->isDayofWeekforStartDayDefaulted();
  }

  bool Model_Impl::isLeapYear() const
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->isLeapYear();
  }

  bool Model_Impl::isIsLeapYearDefaulted() const
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->isIsLeapYearDefaulted();
  }

  void Model_Impl::setCalendarYear(int calendarYear)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    m_cachedYearDescription->setCalendarYear(calendarYear);
  }

  void Model_Impl::resetCalendarYear()
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    m_cachedYearDescription->resetCalendarYear();
  }

  bool Model_Impl::setDayofWeekforStartDay(std::string dayofWeekforStartDay)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->setDayofWeekforStartDay(dayofWeekforStartDay);
  }

  void Model_Impl::resetDayofWeekforStartDay()
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    m_cachedYearDescription->resetDayofWeekforStartDay();
  }

  bool Model_Impl::setIsLeapYear(bool isLeapYear)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->setIsLeapYear(isLeapYear);
  }

  void Model_Impl::resetIsLeapYear()
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    m_cachedYearDescription->resetIsLeapYear();
  }

  int Model_Impl::assumedYear()
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->assumedYear();
  }

  openstudio::Date Model_Impl::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->makeDate(monthOfYear, dayOfMonth);
  }

  openstudio::Date Model_Impl::makeDate(unsigned monthOfYear, unsigned dayOfMonth)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->makeDate(monthOfYear, dayOfMonth);
  }

  openstudio::Date Model_Impl::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->makeDate(n, dayOfWeek, monthOfYear);
  }

  openstudio::Date Model_Impl::makeDate(unsigned dayOfYear)
  {
    if (!m_cachedYearDescription){
      m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
    }
    OS_ASSERT(m_cachedYearDescription);
    return m_cachedYearDescription->makeDate(dayOfYear);
  }

  boost::optional<WeatherFile> Model_Impl::weatherFile() const
  {
    if (m_cachedWeatherFile){
      return m_cachedWeatherFile;
    }

    boost::optional<WeatherFile> result = this->model().getOptionalUniqueModelObject<WeatherFile>();
    if (result){
      m_cachedWeatherFile = result;
      result->getImpl<WeatherFile_Impl>().get()->WeatherFile_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedWeatherFile>(const_cast<openstudio::model::detail::Model_Impl *>(this));
    }

    return m_cachedWeatherFile;
  }

  Schedule Model_Impl::alwaysOffDiscreteSchedule() const
  {
    std::string alwaysOffName = this->alwaysOffDiscreteScheduleName();

    std::vector<ScheduleConstant> schedules = model().getConcreteModelObjects<ScheduleConstant>();

    for( const auto & schedule : schedules )
    {
      if( boost::optional<std::string> name = schedule.name() )
      {
        if( istringEqual(name.get(),alwaysOffName) )
        {
          // DLM: if a schedule matches the name but not these other properties we should reclaim the name or at least warn

          if( equal<double>(schedule.value(),0.0) )
          {
            if( boost::optional<ScheduleTypeLimits> limits = schedule.scheduleTypeLimits() )
            {
              if( boost::optional<std::string> type = limits->numericType() )
              {
                if( istringEqual(type.get(),"Discrete") )
                {
                  return schedule;
                }
              }
            }
          }
        }
      }
    }

    ScheduleConstant schedule(model());

    schedule.setName(alwaysOffName);

    ScheduleTypeLimits limits(model());

    limits.setName("OnOff");

    limits.setNumericType("Discrete");

    limits.setUnitType("Availability");

    limits.setLowerLimitValue(0.0);

    limits.setUpperLimitValue(1.0);

    schedule.setScheduleTypeLimits(limits);

    schedule.setValue(0.0);

    return schedule;
  }

  std::string Model_Impl::alwaysOffDiscreteScheduleName() const
  {
    return "Always Off Discrete";
  }

  Schedule Model_Impl::alwaysOnDiscreteSchedule() const
  {
    std::string alwaysOnName = this->alwaysOffDiscreteScheduleName();

    std::vector<ScheduleConstant> schedules = model().getConcreteModelObjects<ScheduleConstant>();

    for( const auto & schedule : schedules )
    {
      if( boost::optional<std::string> name = schedule.name() )
      {
        if( istringEqual(name.get(),alwaysOnName) )
        {
          // DLM: if a schedule matches the name but not these other properties we should reclaim the name or at least warn

          if( equal<double>(schedule.value(),1.0) )
          {
            if( boost::optional<ScheduleTypeLimits> limits = schedule.scheduleTypeLimits() )
            {
              if( boost::optional<std::string> type = limits->numericType() )
              {
                if( istringEqual(type.get(),"Discrete") )
                {
                  return schedule;
                }
              }
            }
          }
        }
      }
    }

    ScheduleConstant schedule(model());

    schedule.setName(alwaysOnName);

    ScheduleTypeLimits limits(model());

    limits.setName("OnOff");

    limits.setNumericType("Discrete");

    limits.setUnitType("Availability");

    limits.setLowerLimitValue(0.0);

    limits.setUpperLimitValue(1.0);

    schedule.setScheduleTypeLimits(limits);

    schedule.setValue(1.0);

    return schedule;
  }

  std::string Model_Impl::alwaysOnDiscreteScheduleName() const
  {
    return "Always On Discrete";
  }

  Schedule Model_Impl::alwaysOnContinuousSchedule() const
  {
    std::string alwaysOnName = this->alwaysOnContinuousScheduleName();

    std::vector<ScheduleConstant> schedules = model().getConcreteModelObjects<ScheduleConstant>();

    for (const auto & schedule : schedules)
    {
      if (boost::optional<std::string> name = schedule.name())
      {
        if (istringEqual(name.get(), alwaysOnName))
        {
          // DLM: if a schedule matches the name but not these other properties we should reclaim the name or at least warn

          if (equal<double>(schedule.value(), 1.0))
          {
            if (boost::optional<ScheduleTypeLimits> limits = schedule.scheduleTypeLimits())
            {
              if (boost::optional<std::string> type = limits->numericType())
              {
                if (istringEqual(type.get(), "Continuous"))
                {
                  return schedule;
                }
              }
            }
          }
        }
      }
    }

    ScheduleConstant schedule(model());

    schedule.setName(alwaysOnName);

    ScheduleTypeLimits limits(model());

    limits.setName("Fractional");

    limits.setNumericType("Continuous");

    limits.setUnitType("");

    limits.setLowerLimitValue(0.0);

    limits.setUpperLimitValue(1.0);

    schedule.setScheduleTypeLimits(limits);

    schedule.setValue(1.0);

    return schedule;
  }

  std::string Model_Impl::alwaysOnContinuousScheduleName() const
  {
    return "Always On Continuous";
  }

  SpaceType Model_Impl::plenumSpaceType() const
  {
    std::string plenumSpaceTypeName = this->plenumSpaceTypeName();

    std::vector<SpaceType> spaceTypes = model().getConcreteModelObjects<SpaceType>();

    for( const auto & spaceType : spaceTypes )
    {
      if( boost::optional<std::string> name = spaceType.name() )
      {
        if( istringEqual(name.get(),plenumSpaceTypeName) )
        {
          return spaceType;
        }
      }
    }

    SpaceType spaceType(model());

    spaceType.setName(plenumSpaceTypeName);

    return spaceType;
  }

  std::string Model_Impl::plenumSpaceTypeName() const
  {
    return "Plenum Space Type";
  }

  WorkflowJSON Model_Impl::workflowJSON() const
  {
    return m_workflowJSON;
  }

  /// get the sql file
  boost::optional<openstudio::SqlFile> Model_Impl::sqlFile() const
  {
    if (m_sqlFile)
    {
      return boost::optional<openstudio::SqlFile>(*m_sqlFile);
    } else {
      return boost::optional<openstudio::SqlFile>();
    }
  }

  bool Model_Impl::setWorkflowJSON(const openstudio::WorkflowJSON& workflowJSON)
  {
    m_workflowJSON = workflowJSON;
    return true;
  }

  void Model_Impl::resetWorkflowJSON()
  {
    m_workflowJSON = WorkflowJSON();
  }

  /// set the sql file
  bool Model_Impl::setSqlFile(const openstudio::SqlFile& sqlFile)
  {
    bool result = true;
    m_sqlFile = std::shared_ptr<openstudio::SqlFile>(new openstudio::SqlFile(sqlFile));
    return result;
  }

  /// reset the sql file
  bool Model_Impl::resetSqlFile()
  {
    bool result = true;
    m_sqlFile.reset();
    return result;
  }

  boost::optional<ComponentData> Model_Impl::insertComponent(const Component& component) {
    ComponentData newComponentData = component.componentData();
    for (const ComponentWatcher& cw : m_componentWatchers) {
      ComponentData candidate = cw.componentData();
      if (candidate.uuid() == newComponentData.uuid())
      {
        if (candidate.versionUUID() != newComponentData.versionUUID()) {
          LOG(Warn,"Component '" << newComponentData.name() << "' to be inserted into Model is "
              << "already in Model as determined by UUID comparison, but has a different version. "
              << "Returning the version already in Model; if you would like to use both versions, "
              << "please create a new UUID for one of the Components.");
        }
        return candidate;
      }
    }
    WorkspaceObjectVector resultingObjects = model().addObjects(component.objects());
    if (resultingObjects.empty()) { return boost::none; }
    OS_ASSERT(resultingObjects.size() == component.numObjects());
    for (const WorkspaceObject& wo : resultingObjects) {
      OptionalComponentData ocd = wo.optionalCast<ComponentData>();
      if (ocd) {
        ComponentData componentDataObject = *ocd;
        mf_createComponentWatcher(componentDataObject);
        return componentDataObject;
      }
    }
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<openstudio::IdfObject> Model_Impl::purgeUnusedResourceObjects() {
    ResourceObjectVector resources = model().getModelObjects<ResourceObject>();
    IdfObjectVector removedObjects;
    for (ResourceObject& resource : resources) {
      // test for initialized first in case earlier .remove() got this one already
      if ((resource.initialized()) && (resource.nonResourceObjectUseCount(true) == 0)) {
        IdfObjectVector thisCallRemoved = resource.remove();
        removedObjects.insert(removedObjects.end(),thisCallRemoved.begin(),thisCallRemoved.end());
      }
    }
    return removedObjects;
  }

  std::vector<openstudio::IdfObject> Model_Impl::purgeUnusedResourceObjects(IddObjectType iddObjectType) {
    IdfObjectVector removedObjects;
    for (const WorkspaceObject& workspaceObject : getObjectsByType(iddObjectType)) {
      boost::optional<ResourceObject> resource = workspaceObject.optionalCast<ResourceObject>();
      if (resource){
        // test for initialized first in case earlier .remove() got this one already
        if ((resource->initialized()) && (resource->directUseCount(true) == 0)) {
          IdfObjectVector thisCallRemoved = resource->remove();
          removedObjects.insert(removedObjects.end(),thisCallRemoved.begin(),thisCallRemoved.end());
        }
      }

    }
    return removedObjects;
  }

  void Model_Impl::connect(const Model& m,
                           ModelObject sourceObject,
                           unsigned sourcePort,
                           ModelObject targetObject,
                           unsigned targetPort)
  {
    disconnect(sourceObject,sourcePort);
    disconnect(targetObject,targetPort);

    Connection c(m);
    c.setSourceObject(sourceObject);
    c.setSourceObjectPort(sourcePort);
    c.setTargetObject(targetObject);
    c.setTargetObjectPort(targetPort);

    sourceObject.setPointer(sourcePort,c.handle());
    targetObject.setPointer(targetPort,c.handle());
  }

  void Model_Impl::disconnect(ModelObject object,
                              unsigned port)
  {
    if( boost::optional<HVACComponent> hvacComponent = object.optionalCast<HVACComponent>() )
    {
      std::shared_ptr<HVACComponent_Impl> hvacComponentImpl;
      hvacComponentImpl = hvacComponent->getImpl<HVACComponent_Impl>();
      hvacComponentImpl->m_airLoopHVAC = boost::none;
      hvacComponentImpl->m_airLoopHVACOutdoorAirSystem = boost::none;
      hvacComponentImpl->m_plantLoop = boost::none;
    }

    if( boost::optional<WaterToWaterComponent> waterToWaterComponent = object.optionalCast<WaterToWaterComponent>() )
    {
      std::shared_ptr<WaterToWaterComponent_Impl> waterToWaterComponentImpl;
      waterToWaterComponentImpl = waterToWaterComponent->getImpl<WaterToWaterComponent_Impl>();
      waterToWaterComponentImpl->m_secondaryPlantLoop = boost::none;
      waterToWaterComponentImpl->m_tertiaryPlantLoop = boost::none;
    }

    Model m = model();
    OptionalModelObject optionalObject;
    OptionalUnsigned optionalPort;
    OptionalWorkspaceObject wo;

    // find a connection that exists at the given object and port
    if( boost::optional<Connection> connection = object.getModelObjectTarget<Connection>(port) )
    {
      boost::optional<ModelObject> sourceObject;
      boost::optional<ModelObject> targetObject;
      boost::optional<unsigned> sourcePort;
      boost::optional<unsigned> targetPort;

      sourceObject = connection->sourceObject();
      sourcePort = connection->sourceObjectPort();

      targetObject = connection->targetObject();
      targetPort = connection->targetObjectPort();

      // This resets the cache
      if( targetObject )
      {
        if( boost::optional<HVACComponent> hvacComponent = targetObject->optionalCast<HVACComponent>() )
        {
          std::shared_ptr<HVACComponent_Impl> hvacComponentImpl;
          hvacComponentImpl = hvacComponent->getImpl<HVACComponent_Impl>();
          hvacComponentImpl->m_airLoopHVAC = boost::none;
          hvacComponentImpl->m_airLoopHVACOutdoorAirSystem = boost::none;
          hvacComponentImpl->m_plantLoop = boost::none;
        }

        if( boost::optional<WaterToWaterComponent> waterToWaterComponent = targetObject->optionalCast<WaterToWaterComponent>() )
        {
          std::shared_ptr<WaterToWaterComponent_Impl> waterToWaterComponentImpl;
          waterToWaterComponentImpl = waterToWaterComponent->getImpl<WaterToWaterComponent_Impl>();
          waterToWaterComponentImpl->m_secondaryPlantLoop = boost::none;
          waterToWaterComponentImpl->m_tertiaryPlantLoop = boost::none;
        }
      }

      if( targetObject && targetPort )
      {
        if( boost::optional<PortList> portList = targetObject->optionalCast<PortList>() )
        {
          portList->getImpl<model::detail::PortList_Impl>()->removePort(targetPort.get());
        }
        else
        {
          targetObject->setString(targetPort.get(),"");
        }
      }

      // This resets the cache
      if( sourceObject )
      {
        if( boost::optional<HVACComponent> hvacComponent = sourceObject->optionalCast<HVACComponent>() )
        {
          std::shared_ptr<HVACComponent_Impl> hvacComponentImpl;
          hvacComponentImpl = hvacComponent->getImpl<HVACComponent_Impl>();
          hvacComponentImpl->m_airLoopHVAC = boost::none;
          hvacComponentImpl->m_airLoopHVACOutdoorAirSystem = boost::none;
          hvacComponentImpl->m_plantLoop = boost::none;
        }

        if( boost::optional<WaterToWaterComponent> waterToWaterComponent = sourceObject->optionalCast<WaterToWaterComponent>() )
        {
          std::shared_ptr<WaterToWaterComponent_Impl> waterToWaterComponentImpl;
          waterToWaterComponentImpl = waterToWaterComponent->getImpl<WaterToWaterComponent_Impl>();
          waterToWaterComponentImpl->m_secondaryPlantLoop = boost::none;
          waterToWaterComponentImpl->m_tertiaryPlantLoop = boost::none;
        }
      }

      if( sourceObject && sourcePort )
      {
        if( boost::optional<PortList> portList = sourceObject->optionalCast<PortList>() )
        {
          portList->getImpl<model::detail::PortList_Impl>()->removePort(sourcePort.get());
        }
        else
        {
          sourceObject->setString(sourcePort.get(),"");
        }
      }

      connection->remove();
    }
  }

  void Model_Impl::obsoleteComponentWatcher(const ComponentWatcher& watcher) {
    auto it = std::find(m_componentWatchers.begin(),
        m_componentWatchers.end(),watcher);
    OS_ASSERT(it != m_componentWatchers.end());
    m_componentWatchers.erase(it);
  }

  void Model_Impl::reportInitialModelObjects()
  {
    for (const WorkspaceObject& workspaceObject : this->objects()) {
      this->initialModelObject.nano_emit(workspaceObject.getImpl<detail::ModelObject_Impl>().get(), workspaceObject.iddObject().type(), workspaceObject.handle());
    }
    this->initialReportComplete.nano_emit();
  }

  void Model_Impl::mf_createComponentWatcher(ComponentData& componentData) {
    try {
      ComponentWatcher watcher(componentData);
      watcher.getImpl().get()->ComponentWatcher_Impl::obsolete.connect<Model_Impl, &Model_Impl::obsoleteComponentWatcher>(this); // #HASHTAG Problem?
      m_componentWatchers.push_back(watcher);
    }
    catch (...) {
      componentData.remove();
      return;
    }
  }

  void Model_Impl::clearCachedData()
  {
    Handle dummy;
    clearCachedBuilding(dummy);
    clearCachedLifeCycleCostParameters(dummy);
    clearCachedRunPeriod(dummy);
    clearCachedYearDescription(dummy);
    clearCachedWeatherFile(dummy);
  }

  void Model_Impl::clearCachedBuilding(const Handle &)
  {
    m_cachedBuilding.reset();
  }

  void Model_Impl::clearCachedLifeCycleCostParameters(const Handle &handle)
  {
    m_cachedLifeCycleCostParameters.reset();
  }

  void Model_Impl::clearCachedRunPeriod(const Handle& handle)
  {
    m_cachedRunPeriod.reset();
  }

  void Model_Impl::clearCachedYearDescription(const Handle& handle)
  {
    m_cachedYearDescription.reset();
  }

  void Model_Impl::clearCachedWeatherFile(const Handle& handle)
  {
    m_cachedWeatherFile.reset();
  }
} // detail

Model::Model()
  : Workspace(std::shared_ptr<detail::Model_Impl>(new detail::Model_Impl()))
{
  this->addVersionObject();
}

Model::Model(const openstudio::IdfFile& idfFile)
  : Workspace(std::shared_ptr<detail::Model_Impl>(new detail::Model_Impl(idfFile)))
{
  // construct WorkspaceObject_ImplPtrs
  openstudio::detail::WorkspaceObject_ImplPtrVector objectImplPtrs;
  if (OptionalIdfObject vo = idfFile.versionObject()) {
    objectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(*vo,true));
  }
  for (const IdfObject& idfObject : idfFile.objects()) {
    objectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(idfObject,true));
    LOG(Trace,"idfObject: " << toString(idfObject.handle()));
    LOG(Trace,"objectImplPtr: " << toString(objectImplPtrs.back()->handle()));
  }
  // add Object_ImplPtrs to Workspace_Impl
  getImpl<detail::Model_Impl>()->addObjects(objectImplPtrs);
  // watch loaded components
  getImpl<detail::Model_Impl>()->createComponentWatchers();
}

Model::Model(const openstudio::Workspace& workspace)
  : Workspace(std::shared_ptr<detail::Model_Impl>(new
    detail::Model_Impl(*(workspace.getImpl<openstudio::detail::Workspace_Impl>()),true)))
{
  // construct WorkspaceObject_ImplPtrs
  openstudio::detail::WorkspaceObject_ImplPtrVector newObjectImplPtrs;
  HandleMap oldNewHandleMap;
  if (OptionalWorkspaceObject vo = workspace.versionObject()) {
    newObjectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(
        vo->getImpl<openstudio::detail::WorkspaceObject_Impl>(),true));
  }
  for (const WorkspaceObject& object :
                workspace.getImpl<openstudio::detail::Workspace_Impl>()->objects())
  {
    newObjectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(
        object.getImpl<openstudio::detail::WorkspaceObject_Impl>(),true));
  }
  // add Object_ImplPtrs to clone's Workspace_Impl
  getImpl<detail::Model_Impl>()->addClones(newObjectImplPtrs,oldNewHandleMap,true);
  // watch loaded components
  getImpl<detail::Model_Impl>()->createComponentWatchers();
}

boost::optional<Model> Model::load(const path& p) {
  OptionalModel result;
  OptionalIdfFile oIdfFile = IdfFile::load(p,IddFileType::OpenStudio);
  if (oIdfFile) {
    try {
      result = Model(*oIdfFile);
    }
    catch (...) {}
  }

  if (result){
    path workflowJSONPath = p.parent_path() / p.stem() / toPath("workflow.osw");
    if (exists(workflowJSONPath)){
      boost::optional<WorkflowJSON> workflowJSON = WorkflowJSON::load(workflowJSONPath);
      if (workflowJSON){
        result->setWorkflowJSON(*workflowJSON);
      }
    }
  }

  return result;
}

boost::optional<Model> Model::load(const path& osmPath, const path& workflowJSONPath)
{
  OptionalModel result = load(osmPath);
  if (result){
    boost::optional<WorkflowJSON> workflowJSON = WorkflowJSON::load(workflowJSONPath);
    if (workflowJSON){
      result->setWorkflowJSON(*workflowJSON);
    } else{
      result.reset();
    }
  }
  return result;
}


Model::Model(std::shared_ptr<detail::Model_Impl> p)
  : Workspace(std::move(p))
{}

boost::optional<Building> Model::building() const
{
  return getImpl<detail::Model_Impl>()->building();
}

boost::optional<LifeCycleCostParameters> Model::lifeCycleCostParameters() const
{
  return getImpl<detail::Model_Impl>()->lifeCycleCostParameters();
}

boost::optional<RunPeriod> Model::runPeriod() const
{
  return getImpl<detail::Model_Impl>()->runPeriod();
}

boost::optional<YearDescription> Model::yearDescription() const
{
  return getImpl<detail::Model_Impl>()->yearDescription();
}

boost::optional<int> Model::calendarYear() const
{
  return getImpl<detail::Model_Impl>()->calendarYear();
}

std::string Model::dayofWeekforStartDay() const
{
  return getImpl<detail::Model_Impl>()->dayofWeekforStartDay();
}

bool Model::isDayofWeekforStartDayDefaulted() const
{
  return getImpl<detail::Model_Impl>()->isDayofWeekforStartDayDefaulted();
}

bool Model::isLeapYear() const
{
  return getImpl<detail::Model_Impl>()->isLeapYear();
}

bool Model::isIsLeapYearDefaulted() const
{
  return getImpl<detail::Model_Impl>()->isIsLeapYearDefaulted();
}

void Model::setCalendarYear(int calendarYear)
{
  getImpl<detail::Model_Impl>()->setCalendarYear(calendarYear);
}

void Model::resetCalendarYear()
{
  getImpl<detail::Model_Impl>()->resetCalendarYear();
}

bool Model::setDayofWeekforStartDay(std::string dayofWeekforStartDay)
{
  return getImpl<detail::Model_Impl>()->setDayofWeekforStartDay(dayofWeekforStartDay);
}

void Model::resetDayofWeekforStartDay()
{
  getImpl<detail::Model_Impl>()->resetDayofWeekforStartDay();
}

bool Model::setIsLeapYear(bool isLeapYear)
{
  return getImpl<detail::Model_Impl>()->setIsLeapYear(isLeapYear);
}

void Model::resetIsLeapYear()
{
  getImpl<detail::Model_Impl>()->resetIsLeapYear();
}

int Model::assumedYear()
{
  return getImpl<detail::Model_Impl>()->assumedYear();
}

openstudio::Date Model::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth)
{
  return getImpl<detail::Model_Impl>()->makeDate(monthOfYear, dayOfMonth);
}

openstudio::Date Model::makeDate(unsigned monthOfYear, unsigned dayOfMonth)
{
  return getImpl<detail::Model_Impl>()->makeDate(monthOfYear, dayOfMonth);
}

openstudio::Date Model::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear)
{
  return getImpl<detail::Model_Impl>()->makeDate(n, dayOfWeek, monthOfYear);
}

openstudio::Date Model::makeDate(unsigned dayOfYear)
{
  return getImpl<detail::Model_Impl>()->makeDate(dayOfYear);
}

boost::optional<WeatherFile> Model::weatherFile() const
{
  return getImpl<detail::Model_Impl>()->weatherFile();
}

Schedule Model::alwaysOffDiscreteSchedule() const
{
  return getImpl<detail::Model_Impl>()->alwaysOffDiscreteSchedule();
}

std::string Model::alwaysOffDiscreteScheduleName() const
{
  return getImpl<detail::Model_Impl>()->alwaysOffDiscreteScheduleName();
}

Schedule Model::alwaysOnDiscreteSchedule() const
{
  return getImpl<detail::Model_Impl>()->alwaysOnDiscreteSchedule();
}

std::string Model::alwaysOnDiscreteScheduleName() const
{
  return getImpl<detail::Model_Impl>()->alwaysOnDiscreteScheduleName();
}

Schedule Model::alwaysOnContinuousSchedule() const
{
  return getImpl<detail::Model_Impl>()->alwaysOnContinuousSchedule();
}

std::string Model::alwaysOnContinuousScheduleName() const
{
  return getImpl<detail::Model_Impl>()->alwaysOnContinuousScheduleName();
}

SpaceType Model::plenumSpaceType() const
{
  return getImpl<detail::Model_Impl>()->plenumSpaceType();
}

std::string Model::plenumSpaceTypeName() const
{
  return getImpl<detail::Model_Impl>()->plenumSpaceTypeName();
}
openstudio::WorkflowJSON Model::workflowJSON() const
{
  return getImpl<detail::Model_Impl>()->workflowJSON();
}

bool Model::setWorkflowJSON(const openstudio::WorkflowJSON& setWorkflowJSON)
{
  return getImpl<detail::Model_Impl>()->setWorkflowJSON(setWorkflowJSON);
}

void Model::resetWorkflowJSON()
{
  return getImpl<detail::Model_Impl>()->resetWorkflowJSON();
}

openstudio::OptionalSqlFile Model::sqlFile() const
{
  return getImpl<detail::Model_Impl>()->sqlFile();
}

bool Model::setSqlFile(const openstudio::SqlFile& sqlFile)
{
  return getImpl<detail::Model_Impl>()->setSqlFile(sqlFile);
}

bool Model::resetSqlFile()
{
  return getImpl<detail::Model_Impl>()->resetSqlFile();
}

bool Model::operator==(const Model& other) const
{
  return (getImpl<detail::Model_Impl>() == other.getImpl<detail::Model_Impl>());
}

std::vector<ModelObject> Model::modelObjects(bool sorted) const
{
  // can't use resize because ModelObject has no default ctor
  std::vector<ModelObject> result;
  for (WorkspaceObject object : this->objects(sorted)){
    result.push_back(object.cast<ModelObject>());
  }
  return result;
}

boost::optional<ComponentData> Model::insertComponent(const Component& component) {
  return getImpl<detail::Model_Impl>()->insertComponent(component);
}

std::vector<openstudio::IdfObject> Model::purgeUnusedResourceObjects() {
  return getImpl<detail::Model_Impl>()->purgeUnusedResourceObjects();
}

std::vector<openstudio::IdfObject> Model::purgeUnusedResourceObjects(IddObjectType iddObjectType) {
  return getImpl<detail::Model_Impl>()->purgeUnusedResourceObjects(iddObjectType);
}

void Model::addVersionObject() {
  getUniqueModelObject<Version>();
}

void Model::connect(ModelObject sourceObject,
                    unsigned sourcePort,
                    ModelObject targetObject,
                    unsigned targetPort) const
{
  getImpl<detail::Model_Impl>()->connect(*this,sourceObject,sourcePort,targetObject,targetPort);
}

void Model::disconnect(ModelObject object, unsigned port)
{
  getImpl<detail::Model_Impl>()->disconnect(object,port);
}

detail::Model_Impl* Model::rawImpl() const
{
  return getImpl<detail::Model_Impl>().get();
}

bool compareInputAndOutput(const ModelObject& object,
                           const std::string& attributeName,
                           double inputResult,
                           double outputResult,
                           double tol,
                           LogLevel logLevel)
{
  if (!openstudio::equal(inputResult,outputResult,tol)) {
    LOG_FREE(logLevel,"openstudio.model.Model","The " << attributeName << " values determined for "
        << object.briefDescription() << " using input and output data differ by a (relative) error "
        << "greater than " << tol << ". The value calculated from input data was " << inputResult
        << ", whereas the value calculated from output data was " << outputResult << ".");
    return false;
  }
  return true;
}

Model exampleModel()
{
  Model model;
  addExampleModelObjects(model);
  return model;
}

void addExampleSchedules(Model& model);

void addExampleConstructions(Model& model);

void addExampleModelObjects(Model& model)
{
  std::vector<Surface> searchResults;

  // add Version
  Version version = model.getUniqueModelObject<Version>();

  // add SimulationControl
  SimulationControl simulationControl = model.getUniqueModelObject<SimulationControl>();
  simulationControl.setDoZoneSizingCalculation(true);
  simulationControl.setDoSystemSizingCalculation(true);
  simulationControl.setRunSimulationforSizingPeriods(false);
  simulationControl.setRunSimulationforWeatherFileRunPeriods(true);

  // add Timestep
  Timestep timestep = model.getUniqueModelObject<Timestep>();

  // add RunPeriod
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  runPeriod.setBeginMonth(1);
  runPeriod.setBeginDayOfMonth(1);
  runPeriod.setEndMonth(12);
  runPeriod.setEndDayOfMonth(31);
  runPeriod.setUseWeatherFileHolidays(false);
  runPeriod.setUseWeatherFileDaylightSavings(false);
  runPeriod.setApplyWeekendHolidayRule(false);
  runPeriod.setUseWeatherFileRainInd(true);
  runPeriod.setUseWeatherFileSnowInd(true);
  runPeriod.setNumTimePeriodRepeats(1);

  // add SurfaceConvectionAlgorithmInside
  InsideSurfaceConvectionAlgorithm insideSurfaceConvectionAlgorithm = model.getUniqueModelObject<InsideSurfaceConvectionAlgorithm>();

  // add SurfaceConvectionAlgorithmOutside
  OutsideSurfaceConvectionAlgorithm outsideSurfaceConvectionAlgorithm = model.getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();

  // add HeatBalanceAlgorithm
  HeatBalanceAlgorithm heatBalanceAlgorithm = model.getUniqueModelObject<HeatBalanceAlgorithm>();

  // add ZoneAirHeatBalanceAlgorithm
  ZoneAirHeatBalanceAlgorithm zoneAirHeatBalanceAlgorithm = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  // add ConvergenceLimits
  ConvergenceLimits convergenceLimits = model.getUniqueModelObject<ConvergenceLimits>();

  // add ShadowCalculation
  ShadowCalculation shadowCalculation = model.getUniqueModelObject<ShadowCalculation>();

  // add Site
  Site site = model.getUniqueModelObject<Site>();
  site.setLatitude(41.77);
  site.setLongitude(-87.75);
  site.setTimeZone(-6.0);
  site.setElevation(190.0);

  // add SiteGroundTemperatureBuildingSurface
  SiteGroundTemperatureBuildingSurface groundTemp = model.getUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
  groundTemp.setJanuaryGroundTemperature(19.527);
  groundTemp.setFebruaryGroundTemperature(19.502);
  groundTemp.setMarchGroundTemperature(19.536);
  groundTemp.setAprilGroundTemperature(19.598);
  groundTemp.setMayGroundTemperature(20.002);
  groundTemp.setJuneGroundTemperature(21.640);
  groundTemp.setJulyGroundTemperature(22.225);
  groundTemp.setAugustGroundTemperature(22.375);
  groundTemp.setSeptemberGroundTemperature(21.449);
  groundTemp.setOctoberGroundTemperature(20.121);
  groundTemp.setNovemberGroundTemperature(19.802);
  groundTemp.setDecemberGroundTemperature(19.633);

  // add SiteGroundTemperatureDeep
  SiteGroundTemperatureDeep groundTempDeep = model.getUniqueModelObject<SiteGroundTemperatureDeep>();
  groundTempDeep.setJanuaryDeepGroundTemperature(19.527);
  groundTempDeep.setFebruaryDeepGroundTemperature(19.502);
  groundTempDeep.setMarchDeepGroundTemperature(19.536);
  groundTempDeep.setAprilDeepGroundTemperature(19.598);
  groundTempDeep.setMayDeepGroundTemperature(20.002);
  groundTempDeep.setJuneDeepGroundTemperature(21.640);
  groundTempDeep.setJulyDeepGroundTemperature(22.225);
  groundTempDeep.setAugustDeepGroundTemperature(22.375);
  groundTempDeep.setSeptemberDeepGroundTemperature(21.449);
  groundTempDeep.setOctoberDeepGroundTemperature(20.121);
  groundTempDeep.setNovemberDeepGroundTemperature(19.802);
  groundTempDeep.setDecemberDeepGroundTemperature(19.633);

  // add SiteWaterMainsTemperature
  SiteWaterMainsTemperature waterTemp = model.getUniqueModelObject<SiteWaterMainsTemperature>();
  waterTemp.setAnnualAverageOutdoorAirTemperature(9.69);
  waterTemp.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(28.10);

  // add DesignDay
  DesignDay designDay1(model);
  designDay1.setMaximumDryBulbTemperature(-20.6);
  designDay1.setDailyDryBulbTemperatureRange(0.0);
  designDay1.setHumidityIndicatingConditionsAtMaximumDryBulb(-20.6);
  designDay1.setBarometricPressure(99063.0);
  designDay1.setWindSpeed(4.9);
  designDay1.setWindDirection(270);
  designDay1.setSkyClearness(0.0);
  designDay1.setRainIndicator(false);
  designDay1.setSnowIndicator(false);
  designDay1.setDayOfMonth(21);
  designDay1.setMonth(1);
  designDay1.setDayType("WinterDesignDay");
  designDay1.setDaylightSavingTimeIndicator(false);
  designDay1.setHumidityIndicatingType("WetBulb");

  DesignDay designDay2(model);
  designDay2.setMaximumDryBulbTemperature(33.2);
  designDay2.setDailyDryBulbTemperatureRange(10.7);
  designDay2.setHumidityIndicatingConditionsAtMaximumDryBulb(23.8);
  designDay2.setBarometricPressure(99063.0);
  designDay2.setWindSpeed(5.3);
  designDay2.setWindDirection(230);
  designDay2.setSkyClearness(1.0);
  designDay2.setRainIndicator(false);
  designDay2.setSnowIndicator(false);
  designDay2.setDayOfMonth(21);
  designDay2.setMonth(7);
  designDay2.setDayType("SummerDesignDay");
  designDay2.setDaylightSavingTimeIndicator(false);
  designDay2.setHumidityIndicatingType("WetBulb");

  // add schedules
  addExampleSchedules(model);
  OS_ASSERT(model.getConcreteModelObjects<DefaultScheduleSet>().size() >= 1);
  DefaultScheduleSet defaultScheduleSet = model.getConcreteModelObjects<DefaultScheduleSet>()[0];

  // add constructions
  addExampleConstructions(model);
  OS_ASSERT(model.getConcreteModelObjects<DefaultConstructionSet>().size() >= 1);
  DefaultConstructionSet defaultConstructionSet = model.getConcreteModelObjects<DefaultConstructionSet>()[0];

  // add a space type
  SpaceType spaceType(model);

  // add some lights to the space type
  LightsDefinition lightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(10.0);
  Lights lights(lightsDefinition);
  lights.setSpaceType(spaceType);

  // add some electric equipment to the space type
  ElectricEquipmentDefinition electricEquipmentDefinition(model);
  electricEquipmentDefinition.setWattsperSpaceFloorArea(5.0);
  ElectricEquipment electricEquipment(electricEquipmentDefinition);
  electricEquipment.setSpaceType(spaceType);

  // add some people to the space type
  PeopleDefinition peopleDefinition(model);
  peopleDefinition.setPeopleperSpaceFloorArea(0.05);
  People people(peopleDefinition);
  people.setSpaceType(spaceType);

  // create the facility
  Facility facility = model.getUniqueModelObject<Facility>();

  // create the building
  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);
  building.setDefaultConstructionSet(defaultConstructionSet);
  building.setDefaultScheduleSet(defaultScheduleSet);

  // create a thermal zone
  ThermalZone thermalZone(model);

  ThermostatSetpointDualSetpoint thermostat(model);
  thermalZone.setThermostatSetpointDualSetpoint(thermostat);

  Schedule heatingSchedule = model.getModelObjectByName<Schedule>("Medium Office Heating Setpoint Schedule").get();
  Schedule coolingSchedule = model.getModelObjectByName<Schedule>("Medium Office Cooling Setpoint Schedule").get();

  thermostat.setHeatingSchedule(heatingSchedule);
  thermostat.setCoolingSchedule(coolingSchedule);

  // create a building story
  BuildingStory buildingStory(model);
  buildingStory.setNominalZCoordinate(0);
  buildingStory.setNominalFloortoFloorHeight(3);

  // create spaces from floor print
  std::vector<Point3d> floorPrint;
  floorPrint.push_back(Point3d(0,0,0));
  floorPrint.push_back(Point3d(0,10,0));
  floorPrint.push_back(Point3d(10,10,0));
  floorPrint.push_back(Point3d(10,0,0));

  // make spaces
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space1);
  space1->setThermalZone(thermalZone);
  space1->setBuildingStory(buildingStory);

  ModelObject clone = space1->clone(model);
  Space space2 = clone.cast<Space>();
  space2.setXOrigin(10);

  clone = space1->clone(model);
  Space space3 = clone.cast<Space>();
  space3.setYOrigin(10);

  clone = space1->clone(model);
  Space space4 = clone.cast<Space>();
  space4.setXOrigin(10);
  space4.setYOrigin(10);

  // add a door to south wall of space1
  std::vector<Point3d> doorPoints;
  doorPoints.push_back(Point3d(2,0,2));
  doorPoints.push_back(Point3d(2,0,0));
  doorPoints.push_back(Point3d(4,0,0));
  doorPoints.push_back(Point3d(4,0,2));

  // find south wall
  searchResults = space1->findSurfaces(180.0,180.0,90.0,90.0);
  OS_ASSERT(searchResults.size() >= 1);

  // add door
  SubSurface door(doorPoints, model);
  door.setSurface(searchResults[0]);

  // add a window to east wall of space2
  std::vector<Point3d> windowPoints;
  windowPoints.push_back(Point3d(10,2,2));
  windowPoints.push_back(Point3d(10,2,1));
  windowPoints.push_back(Point3d(10,8,1));
  windowPoints.push_back(Point3d(10,8,2));

  // find east wall
  searchResults = space2.findSurfaces(90.0,90.0,90.0,90.0);
  OS_ASSERT(searchResults.size() >= 1);

  // add window
  SubSurface window(windowPoints, model);
  window.setSurface(searchResults[0]);

  // add overhang to the window
  bool test = window.addOverhangByProjectionFactor(0.5, 0.1);
  OS_ASSERT(test);

  // add daylighting control point to center of space2
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space2);
  daylightingControl.setPosition(Point3d(5, 5, 1.1));

  // hook daylighting control up to zone
  test = thermalZone.setPrimaryDaylightingControl(daylightingControl);
  OS_ASSERT(test);
  thermalZone.setFractionofZoneControlledbyPrimaryDaylightingControl(0.25);

  // add illuminance map to space2
  IlluminanceMap illuminanceMap(model);
  illuminanceMap.setSpace(space2);
  illuminanceMap.setOriginXCoordinate(1);
  illuminanceMap.setXLength(8);
  illuminanceMap.setOriginYCoordinate(1);
  illuminanceMap.setYLength(8);
  illuminanceMap.setOriginZCoordinate(1.1);

  // hook illuminanceMap up to zone
  test = thermalZone.setIlluminanceMap(illuminanceMap);
  OS_ASSERT(test);


  // add a glare sensor to center of space2
  GlareSensor glareSensor(model);
  glareSensor.setSpace(space2);
  glareSensor.setPosition(Point3d(5, 5, 1.1));

  // add a desk to space 2
  InteriorPartitionSurfaceGroup deskGroup(model);
  deskGroup.setSpace(space2);

  std::vector<Point3d> deskPoints;
  deskPoints.push_back(Point3d(5,8,1));
  deskPoints.push_back(Point3d(5,6,1));
  deskPoints.push_back(Point3d(8,6,1));
  deskPoints.push_back(Point3d(8,8,1));
  InteriorPartitionSurface desk(deskPoints, model);
  desk.setInteriorPartitionSurfaceGroup(deskGroup);

  // add a printer to space4
  ElectricEquipmentDefinition printerDefinition(model);
  printerDefinition.setName("Printer Definition");
  printerDefinition.setDesignLevel(200.0);
  ElectricEquipment printer(printerDefinition);
  printer.setName("Printer");
  printer.setSpace(space4);

  // add a building shading device
  ShadingSurfaceGroup canopyGroup(model);
  canopyGroup.setShadingSurfaceType("Building");

  std::vector<Point3d> canopyPoints;
  canopyPoints.push_back(Point3d(2,0,2));
  canopyPoints.push_back(Point3d(2,-1,2));
  canopyPoints.push_back(Point3d(4,-1,2));
  canopyPoints.push_back(Point3d(4,0,2));
  ShadingSurface canopy(canopyPoints, model);
  canopy.setShadingSurfaceGroup(canopyGroup);

  // add a neighboring building
  ShadingSurfaceGroup neighboringBuildingGroup(model);
  neighboringBuildingGroup.setShadingSurfaceType("Site");

  std::vector<Point3d> neighboringBuildingPoints;
  neighboringBuildingPoints.push_back(Point3d(-30,0,20));
  neighboringBuildingPoints.push_back(Point3d(-30,0,0));
  neighboringBuildingPoints.push_back(Point3d(-30,20,0));
  neighboringBuildingPoints.push_back(Point3d(-30,20,20));
  ShadingSurface neighboringBuilding(neighboringBuildingPoints, model);
  neighboringBuilding.setShadingSurfaceGroup(neighboringBuildingGroup);

  // match surfaces
  std::vector<Space> spaces =  model.getConcreteModelObjects<Space>();
  matchSurfaces(spaces);

  // Add an air loop
  ScheduleCompact alwaysOnSchedule(model);
  alwaysOnSchedule.setName("ALWAYS_ON");
  alwaysOnSchedule.setString(3,"Through: 12/31");
  alwaysOnSchedule.setString(4,"For: AllDays");
  alwaysOnSchedule.setString(5,"Until: 24:00");
  alwaysOnSchedule.setString(6,"1");

  FanConstantVolume fan(model,alwaysOnSchedule);
  fan.setName("Standard Fan");
  CoilHeatingGas coilHeatingGas(model,alwaysOnSchedule);

  CurveBiquadratic coolingCurveFofTemp(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);
  coolingCurveFofTemp.setMinimumCurveOutput(-1000);
  coolingCurveFofTemp.setMaximumCurveOutput(1000);

  CurveQuadratic coolingCurveFofFlow(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);
  coolingCurveFofFlow.setMinimumCurveOutput(-1000);
  coolingCurveFofFlow.setMaximumCurveOutput(1000);

  CurveBiquadratic energyInputRatioFofTemp(model);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMaximumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);
  energyInputRatioFofTemp.setMinimumCurveOutput(-1000);
  energyInputRatioFofTemp.setMaximumCurveOutput(1000);

  CurveQuadratic energyInputRatioFofFlow(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);
  energyInputRatioFofFlow.setMinimumCurveOutput(-1000);
  energyInputRatioFofFlow.setMaximumCurveOutput(1000);

  CurveQuadratic partLoadFraction(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);
  partLoadFraction.setMinimumCurveOutput(0.71);
  partLoadFraction.setMaximumCurveOutput(1.0);

  CoilCoolingDXSingleSpeed coilCooling( model,
                                        alwaysOnSchedule,
                                        coolingCurveFofTemp,
                                        coolingCurveFofFlow,
                                        energyInputRatioFofTemp,
                                        energyInputRatioFofFlow,
                                        partLoadFraction );
  EvaporativeCoolerDirectResearchSpecial evaporativeCoolerDirectResearchSpecial(model,alwaysOnSchedule);

  AirTerminalSingleDuctUncontrolled airTerminalSingleDuctUncontrolled(model,alwaysOnSchedule);

  ControllerOutdoorAir controller(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem(model,controller);

  AirLoopHVAC airLoopHVAC(model);

  airLoopHVAC.addBranchForZone(thermalZone,airTerminalSingleDuctUncontrolled);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();
  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  fan.addToNode(supplyInletNode);

  coilHeatingGas.addToNode(supplyInletNode);

  coilCooling.addToNode(supplyInletNode);

  outdoorAirSystem.addToNode(supplyInletNode);

  Node mixedAirNode = outdoorAirSystem.mixedAirModelObject()->cast<Node>();
  Node coolCoilOutletNode = coilCooling.outletModelObject()->cast<Node>();
  Node heatingCoilOutletNode = coilHeatingGas.outletModelObject()->cast<Node>();
  Node oaNode = outdoorAirSystem.outdoorAirModelObject()->cast<Node>();

  evaporativeCoolerDirectResearchSpecial.addToNode(oaNode);

  Node evapOutletNode = evaporativeCoolerDirectResearchSpecial.outletModelObject()->cast<Node>();

  SetpointManagerMixedAir setpointMMA1(model);
  SetpointManagerMixedAir setpointMMA2(model);
  SetpointManagerMixedAir setpointMMA3(model);
  SetpointManagerMixedAir setpointMMA4(model);
  SetpointManagerSingleZoneReheat setpointMSZR(model);

  setpointMSZR.addToNode(supplyOutletNode);
  setpointMMA1.addToNode(mixedAirNode);
  setpointMMA2.addToNode(coolCoilOutletNode);
  setpointMMA3.addToNode(heatingCoilOutletNode);
  setpointMMA4.addToNode(evapOutletNode);

  // add some example variables
  int i = 1;
  for (const std::string& variableName : thermalZone.outputVariableNames()){
    OutputVariable(variableName, model);
    if (++i > 2){
      break;
    }
  }

  // add some example variables
  i = 1;
  for (const Surface& surface : model.getConcreteModelObjects<Surface>()){
    for (const std::string& variableName : surface.outputVariableNames()){
      OutputVariable(variableName, model);
      if (++i > 2){
        break;
      }
    }
    break;
  }

  // add some meters
  OutputMeter electricityMeter(model);
  electricityMeter.setFuelType(FuelType(FuelType::Electricity));
  electricityMeter.setReportingFrequency(ReportingFrequency(ReportingFrequency::Hourly).valueName());
  electricityMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));

  OutputMeter gasMeter(model);
  gasMeter.setFuelType(FuelType(FuelType::Gas));
  gasMeter.setReportingFrequency(ReportingFrequency(ReportingFrequency::Hourly).valueName());
  gasMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));

  OutputMeter propaneMeter(model);
  propaneMeter.setFuelType(FuelType(FuelType::Propane));
  propaneMeter.setReportingFrequency(ReportingFrequency(ReportingFrequency::Hourly).valueName());
  propaneMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));
}

void addExampleSchedules(Model& model) {

  // nice to pre-define Temperature limits because default limits do not provide bounds
  ScheduleTypeLimits temperatureLimits(model);
  temperatureLimits.setName("Temperature");
  temperatureLimits.setLowerLimitValue(-60.0);
  temperatureLimits.setUpperLimitValue(200.0);
  temperatureLimits.setNumericType("CONTINUOUS");
  temperatureLimits.setUnitType("Temperature");

  DefaultScheduleSet defaultScheduleSet(model);
  defaultScheduleSet.setName("Default Schedules");

  ScheduleRuleset numberOfPeopleSchedule(model);
  numberOfPeopleSchedule.setName("Medium Office Number of People Schedule");
  numberOfPeopleSchedule.defaultDaySchedule().setName("Medium Office Number of People All Other Days Schedule");
  ScheduleDay numberOfPeopleSummerDesignDay(model);
  numberOfPeopleSummerDesignDay.setName("Medium Office Number of People Summer Design Day Schedule");
  numberOfPeopleSummerDesignDay.clearValues();
  numberOfPeopleSummerDesignDay.addValue(Time(0,6,0,0),0.0);
  numberOfPeopleSummerDesignDay.addValue(Time(0,22,0,0),1.0);
  numberOfPeopleSummerDesignDay.addValue(Time(0,24,0,0),0.05);
  numberOfPeopleSchedule.setSummerDesignDaySchedule(numberOfPeopleSummerDesignDay);
  ScheduleRule numberOfPeopleWeekdaysRule(numberOfPeopleSchedule);
  numberOfPeopleWeekdaysRule.setName("Medium Office Number of People Schedule Weekdays Rule");
  numberOfPeopleWeekdaysRule.setApplyMonday(true);
  numberOfPeopleWeekdaysRule.setApplyTuesday(true);
  numberOfPeopleWeekdaysRule.setApplyWednesday(true);
  numberOfPeopleWeekdaysRule.setApplyThursday(true);
  numberOfPeopleWeekdaysRule.setApplyFriday(true);
  numberOfPeopleWeekdaysRule.daySchedule().setName("Medium Office Number of People Weekdays Schedule");
  numberOfPeopleWeekdaysRule.daySchedule().clearValues();
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,6,0,0),0.0);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,7,0,0),0.1);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.2);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,12,0,0),0.95);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,13,0,0),0.5);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.95);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.7);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.4);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.1);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.05);
  ScheduleRule numberOfPeopleSaturdayRule(numberOfPeopleSchedule);
  numberOfPeopleSaturdayRule.setName("Medium Office Number of People Schedule Saturday Rule");
  numberOfPeopleSaturdayRule.setApplySaturday(true);
  numberOfPeopleSaturdayRule.daySchedule().setName("Medium Office Number of People Saturday Schedule");
  numberOfPeopleSaturdayRule.daySchedule().clearValues();
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.0);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.1);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.1);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.0);

  defaultScheduleSet.setNumberofPeopleSchedule(numberOfPeopleSchedule);

  ScheduleRuleset peopleActivityLevelSchedule(model);
  peopleActivityLevelSchedule.setName("Medium Office People Activity Level Schedule");
  peopleActivityLevelSchedule.defaultDaySchedule().setName("Medium Office People Activity Level All Days Schedule");
  peopleActivityLevelSchedule.defaultDaySchedule().clearValues();
  peopleActivityLevelSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),120.0);

  defaultScheduleSet.setPeopleActivityLevelSchedule(peopleActivityLevelSchedule);

  ScheduleRuleset lightingSchedule(model);
  lightingSchedule.setName("Medium Office Lighting Schedule");
  lightingSchedule.defaultDaySchedule().setName("Medium Office Lighting All Other Days Schedule");
  lightingSchedule.defaultDaySchedule().clearValues();
  lightingSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.05);
  ScheduleDay lightingSummerDesignDay(model);
  lightingSummerDesignDay.setName("Medium Office Lighting Summer Design Day Schedule");
  lightingSummerDesignDay.clearValues();
  lightingSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  lightingSchedule.setSummerDesignDaySchedule(lightingSummerDesignDay);
  ScheduleDay lightingWinterDesignDay(model);
  lightingWinterDesignDay.setName("Medium Office Lighting Winter Design Day Schedule");
  lightingSchedule.setWinterDesignDaySchedule(lightingWinterDesignDay);
  ScheduleRule lightingWeekdaysRule(lightingSchedule);
  lightingWeekdaysRule.setName("Medium Office Lighting Schedule Weekdays Rule");
  lightingWeekdaysRule.setApplyMonday(true);
  lightingWeekdaysRule.setApplyTuesday(true);
  lightingWeekdaysRule.setApplyWednesday(true);
  lightingWeekdaysRule.setApplyThursday(true);
  lightingWeekdaysRule.setApplyFriday(true);
  lightingWeekdaysRule.daySchedule().setName("Medium Office Lighting Weekdays Schedule");
  lightingWeekdaysRule.daySchedule().clearValues();
  lightingWeekdaysRule.daySchedule().addValue(Time(0,5,0,0),0.05);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,7,0,0),0.1);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.3);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.9);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.7);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.5);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.3);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,23,0,0),0.1);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.05);
  ScheduleRule lightingSaturdayRule(lightingSchedule);
  lightingSaturdayRule.setName("Medium Office Lighting Schedule Saturday Rule");
  lightingSaturdayRule.setApplySaturday(true);
  lightingSaturdayRule.daySchedule().setName("Medium Office Lighting Saturday Schedule");
  lightingSaturdayRule.daySchedule().clearValues();
  lightingSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.05);
  lightingSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.1);
  lightingSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  lightingSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.15);
  lightingSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.05);

  defaultScheduleSet.setLightingSchedule(lightingSchedule);

  ScheduleRuleset electricEquipmentSchedule(model);
  electricEquipmentSchedule.setName("Medium Office Electric Equipment Schedule");
  electricEquipmentSchedule.defaultDaySchedule().setName("Medium Office Electric Equipment All Other Days Schedule");
  electricEquipmentSchedule.defaultDaySchedule().clearValues();
  electricEquipmentSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.3);
  ScheduleDay electricEquipmentSummerDesignDay(model);
  electricEquipmentSummerDesignDay.setName("Medium Office Electric Equipment Summer Design Day Schedule");
  electricEquipmentSummerDesignDay.clearValues();
  electricEquipmentSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  electricEquipmentSchedule.setSummerDesignDaySchedule(electricEquipmentSummerDesignDay);
  ScheduleDay electricEquipmentWinterDesignDay(model);
  electricEquipmentWinterDesignDay.setName("Medium Office Electric Equipment Winter Design Day Schedule");
  electricEquipmentSchedule.setWinterDesignDaySchedule(electricEquipmentWinterDesignDay);
  ScheduleRule electricEquipmentWeekdaysRule(electricEquipmentSchedule);
  electricEquipmentWeekdaysRule.setName("Medium Office Electric Equipment Schedule Weekdays Rule");
  electricEquipmentWeekdaysRule.setApplyMonday(true);
  electricEquipmentWeekdaysRule.setApplyTuesday(true);
  electricEquipmentWeekdaysRule.setApplyWednesday(true);
  electricEquipmentWeekdaysRule.setApplyThursday(true);
  electricEquipmentWeekdaysRule.setApplyFriday(true);
  electricEquipmentWeekdaysRule.daySchedule().setName("Medium Office Electric Equipment Weekdays Schedule");
  electricEquipmentWeekdaysRule.daySchedule().clearValues();
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,12,0,0),0.9);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,13,0,0),0.8);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.9);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.8);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.6);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.5);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.4);
  ScheduleRule electricEquipmentSaturdayRule(electricEquipmentSchedule);
  electricEquipmentSaturdayRule.setName("Medium Office Electric Equipment Schedule Saturday Rule");
  electricEquipmentSaturdayRule.setApplySaturday(true);
  electricEquipmentSaturdayRule.daySchedule().setName("Medium Office Electric Equipment Saturday Schedule");
  electricEquipmentSaturdayRule.daySchedule().clearValues();
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.3);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.35);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.3);

  defaultScheduleSet.setElectricEquipmentSchedule(electricEquipmentSchedule);

  ScheduleRuleset gasEquipmentSchedule(model);
  gasEquipmentSchedule.setName("Medium Office Gas Equipment Schedule");
  gasEquipmentSchedule.defaultDaySchedule().setName("Medium Office Gas Equipment All Other Days Schedule");
  gasEquipmentSchedule.defaultDaySchedule().clearValues();
  gasEquipmentSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.3);
  ScheduleDay gasEquipmentSummerDesignDay(model);
  gasEquipmentSummerDesignDay.setName("Medium Office Gas Equipment Summer Design Day Schedule");
  gasEquipmentSummerDesignDay.clearValues();
  gasEquipmentSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  gasEquipmentSchedule.setSummerDesignDaySchedule(gasEquipmentSummerDesignDay);
  ScheduleDay gasEquipmentWinterDesignDay(model);
  gasEquipmentWinterDesignDay.setName("Medium Office Gas Equipment Winter Design Day Schedule");
  gasEquipmentSchedule.setWinterDesignDaySchedule(gasEquipmentWinterDesignDay);
  ScheduleRule gasEquipmentWeekdaysRule(gasEquipmentSchedule);
  gasEquipmentWeekdaysRule.setName("Medium Office Gas Equipment Schedule Weekdays Rule");
  gasEquipmentWeekdaysRule.setApplyMonday(true);
  gasEquipmentWeekdaysRule.setApplyTuesday(true);
  gasEquipmentWeekdaysRule.setApplyWednesday(true);
  gasEquipmentWeekdaysRule.setApplyThursday(true);
  gasEquipmentWeekdaysRule.setApplyFriday(true);
  gasEquipmentWeekdaysRule.daySchedule().setName("Medium Office Gas Equipment Weekdays Schedule");
  gasEquipmentWeekdaysRule.daySchedule().clearValues();
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,12,0,0),0.9);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,13,0,0),0.8);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.9);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.8);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.6);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.5);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.4);
  ScheduleRule gasEquipmentSaturdayRule(gasEquipmentSchedule);
  gasEquipmentSaturdayRule.setName("Medium Office Gas Equipment Schedule Saturday Rule");
  gasEquipmentSaturdayRule.setApplySaturday(true);
  gasEquipmentSaturdayRule.daySchedule().setName("Medium Office Gas Equipment Saturday Schedule");
  gasEquipmentSaturdayRule.daySchedule().clearValues();
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.3);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.35);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.3);

  defaultScheduleSet.setGasEquipmentSchedule(gasEquipmentSchedule);

  ScheduleRuleset hotWaterEquipmentSchedule(model);
  hotWaterEquipmentSchedule.setName("Medium Office Hot Water Equipment Schedule");
  hotWaterEquipmentSchedule.defaultDaySchedule().setName("Medium Office Hot Water Equipment Default Schedule");
  hotWaterEquipmentSchedule.defaultDaySchedule().clearValues();
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,5,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,6,0,0),0.07);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,11,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,13,0,0),0.06);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,14,0,0),0.09);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,15,0,0),0.06);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,21,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,22,0,0),0.07);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.04);
  ScheduleDay hotWaterEquipmentSummerDesignDay(model);
  hotWaterEquipmentSummerDesignDay.setName("Medium Office Hot Water Equipment Summer Design Day Schedule");
  hotWaterEquipmentSummerDesignDay.clearValues();
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,5,0,0),0.05);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,6,0,0),0.08);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,7,0,0),0.07);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,8,0,0),0.19);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,9,0,0),0.35);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,10,0,0),0.38);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,11,0,0),0.39);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,12,0,0),0.47);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,13,0,0),0.57);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,14,0,0),0.54);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,15,0,0),0.34);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,16,0,0),0.33);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,17,0,0),0.44);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,18,0,0),0.26);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,19,0,0),0.21);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,20,0,0),0.15);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,21,0,0),0.17);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,22,0,0),0.08);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,24,0,0),0.05);
  hotWaterEquipmentSchedule.setSummerDesignDaySchedule(hotWaterEquipmentSummerDesignDay);
  ScheduleRule hotWaterEquipmentWeekdaysRule(hotWaterEquipmentSchedule,hotWaterEquipmentSummerDesignDay);
  hotWaterEquipmentWeekdaysRule.setName("Medium Office Hot Water Schedule Weekdays Rule");
  hotWaterEquipmentWeekdaysRule.setApplyMonday(true);
  hotWaterEquipmentWeekdaysRule.setApplyTuesday(true);
  hotWaterEquipmentWeekdaysRule.setApplyWednesday(true);
  hotWaterEquipmentWeekdaysRule.setApplyThursday(true);
  hotWaterEquipmentWeekdaysRule.setApplyFriday(true);
  hotWaterEquipmentWeekdaysRule.daySchedule().setName("Medium Office Hot Water Equipment Weekdays Schedule");
  ScheduleDay hotWaterEquipmentWinterDesignDay(model);
  hotWaterEquipmentWinterDesignDay.setName("Medium Office Hot Water Equipment Winter Design Day Schedule");
  hotWaterEquipmentWinterDesignDay.clearValues();
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,5,0,0),0.05);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,6,0,0),0.08);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,7,0,0),0.07);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,8,0,0),0.11);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,9,0,0),0.15);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,10,0,0),0.21);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,11,0,0),0.19);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,12,0,0),0.23);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,13,0,0),0.20);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,14,0,0),0.19);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,15,0,0),0.15);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,16,0,0),0.13);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,17,0,0),0.14);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,21,0,0),0.07);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,22,0,0),0.09);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,24,0,0),0.05);
  hotWaterEquipmentSchedule.setWinterDesignDaySchedule(hotWaterEquipmentWinterDesignDay);
  ScheduleRule hotWaterEquipmentSaturdayRule(hotWaterEquipmentSchedule,hotWaterEquipmentWinterDesignDay);
  hotWaterEquipmentSaturdayRule.setName("Medium Office Hot Water Schedule Saturday Rule");
  hotWaterEquipmentSaturdayRule.setApplySaturday(true);
  hotWaterEquipmentSaturdayRule.daySchedule().setName("Medium Office Hot Water Equipment Saturday Schedule");

  defaultScheduleSet.setHotWaterEquipmentSchedule(hotWaterEquipmentSchedule);

  ScheduleRuleset infiltrationSchedule(model);
  infiltrationSchedule.setName("Medium Office Infiltration Schedule");
  infiltrationSchedule.defaultDaySchedule().setName("Medium Office Infiltration Default Schedule");
  infiltrationSchedule.defaultDaySchedule().clearValues();
  infiltrationSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),1.0);
  ScheduleDay infiltrationSummerDesignDay(model);
  infiltrationSummerDesignDay.setName("Medium Office Infiltration Summer Design Day Schedule");
  infiltrationSummerDesignDay.clearValues();
  infiltrationSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  infiltrationSummerDesignDay.addValue(Time(0,22,0,0),0.25);
  infiltrationSummerDesignDay.addValue(Time(0,24,0,0),1.0);
  infiltrationSchedule.setSummerDesignDaySchedule(infiltrationSummerDesignDay);
  ScheduleRule infiltrationWeekdaysRule(infiltrationSchedule,infiltrationSummerDesignDay);
  infiltrationWeekdaysRule.setName("Medium Office Infiltration Schedule Weekdays Rule");
  infiltrationWeekdaysRule.setApplyMonday(true);
  infiltrationWeekdaysRule.setApplyTuesday(true);
  infiltrationWeekdaysRule.setApplyWednesday(true);
  infiltrationWeekdaysRule.setApplyThursday(true);
  infiltrationWeekdaysRule.setApplyFriday(true);
  infiltrationWeekdaysRule.daySchedule().setName("Medium Office Infiltration Weekdays Schedule");
  ScheduleDay infiltrationWinterDesignDay(model);
  infiltrationWinterDesignDay.setName("Medium Office Infiltration Winter Design Day Schedule");
  infiltrationWinterDesignDay.clearValues();
  infiltrationWinterDesignDay.addValue(Time(0,6,0,0),1.0);
  infiltrationWinterDesignDay.addValue(Time(0,18,0,0),0.25);
  infiltrationWinterDesignDay.addValue(Time(0,24,0,0),1.0);
  infiltrationSchedule.setWinterDesignDaySchedule(infiltrationWinterDesignDay);
  ScheduleRule infiltrationSaturdayRule(infiltrationSchedule,infiltrationWinterDesignDay);
  infiltrationSaturdayRule.setName("Medium Office Infiltration Schedule Saturday Rule");
  infiltrationSaturdayRule.setApplySaturday(true);
  infiltrationSaturdayRule.daySchedule().setName("Medium Office Infiltration Saturday Schedule");

  defaultScheduleSet.setInfiltrationSchedule(infiltrationSchedule);

  ScheduleRuleset coolingSetpointSchedule(model);
  coolingSetpointSchedule.setName("Medium Office Cooling Setpoint Schedule");
  coolingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
  coolingSetpointSchedule.defaultDaySchedule().setName("Medium Office Cooling Setpoint All Other Days Schedule");
  coolingSetpointSchedule.defaultDaySchedule().clearValues();
  coolingSetpointSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),26.7);
  ScheduleDay coolingSetpointSummerDesignDay(model);
  coolingSetpointSummerDesignDay.setName("Medium Office Cooling Setpoint Summer Design Day Schedule");
  coolingSetpointSummerDesignDay.setScheduleTypeLimits(temperatureLimits);
  coolingSetpointSummerDesignDay.clearValues();
  coolingSetpointSummerDesignDay.addValue(Time(0,6,0,0),26.7);
  coolingSetpointSummerDesignDay.addValue(Time(0,22,0,0),24.0);
  coolingSetpointSummerDesignDay.addValue(Time(0,24,0,0),26.7);
  coolingSetpointSchedule.setSummerDesignDaySchedule(coolingSetpointSummerDesignDay);
  ScheduleRule coolingSetpointWeekdaysRule(coolingSetpointSchedule,coolingSetpointSummerDesignDay);
  coolingSetpointWeekdaysRule.setName("Medium Office Cooling Setpoint Schedule Weekdays Rule");
  coolingSetpointWeekdaysRule.setApplyMonday(true);
  coolingSetpointWeekdaysRule.setApplyTuesday(true);
  coolingSetpointWeekdaysRule.setApplyWednesday(true);
  coolingSetpointWeekdaysRule.setApplyThursday(true);
  coolingSetpointWeekdaysRule.setApplyFriday(true);
  coolingSetpointWeekdaysRule.daySchedule().setName("Medium Office Cooling Setpoint Weekdays Schedule");
  ScheduleRule coolingSetpointSaturdayRule(coolingSetpointSchedule);
  coolingSetpointSaturdayRule.setName("Medium Office Cooling Setpoint Schedule Saturday Rule");
  coolingSetpointSaturdayRule.setApplySaturday(true);
  coolingSetpointSaturdayRule.daySchedule().setName("Medium Office Cooling Setpoint Saturday Schedule");
  coolingSetpointSaturdayRule.daySchedule().clearValues();
  coolingSetpointSaturdayRule.daySchedule().addValue(Time(0,6,0,0),26.7);
  coolingSetpointSaturdayRule.daySchedule().addValue(Time(0,18,0,0),24.0);
  coolingSetpointSaturdayRule.daySchedule().addValue(Time(0,24,0,0),26.7);

  ScheduleRuleset heatingSetpointSchedule(model);
  heatingSetpointSchedule.setName("Medium Office Heating Setpoint Schedule");
  heatingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
  heatingSetpointSchedule.defaultDaySchedule().setName("Medium Office Heating Setpoint All Other Days Schedule");
  heatingSetpointSchedule.defaultDaySchedule().clearValues();
  heatingSetpointSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),15.6);
  ScheduleDay heatingSetpointWinterDesignDay(model);
  heatingSetpointWinterDesignDay.setName("Medium Office Heating Setpoint Winter Design Day Schedule");
  heatingSetpointWinterDesignDay.setScheduleTypeLimits(temperatureLimits);
  heatingSetpointWinterDesignDay.clearValues();
  heatingSetpointWinterDesignDay.addValue(Time(0,24,0,0),21.0);
  heatingSetpointSchedule.setWinterDesignDaySchedule(heatingSetpointWinterDesignDay);
  ScheduleRule heatingSetpointWeekdaysRule(heatingSetpointSchedule);
  heatingSetpointWeekdaysRule.setName("Medium Office Heating Setpoint Schedule Weekdays Rule");
  heatingSetpointWeekdaysRule.setApplyMonday(true);
  heatingSetpointWeekdaysRule.setApplyTuesday(true);
  heatingSetpointWeekdaysRule.setApplyWednesday(true);
  heatingSetpointWeekdaysRule.setApplyThursday(true);
  heatingSetpointWeekdaysRule.setApplyFriday(true);
  heatingSetpointWeekdaysRule.daySchedule().setName("Medium Office Heating Setpoint Weekdays Schedule");
  heatingSetpointWeekdaysRule.daySchedule().clearValues();
  heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0,6,0,0),15.6);
  heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),21.0);
  heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),15.6);
  ScheduleRule heatingSetpointSaturdayRule(heatingSetpointSchedule);
  heatingSetpointSaturdayRule.setName("Medium Office Heating Setpoint Schedule Saturday Rule");
  heatingSetpointSaturdayRule.setApplySaturday(true);
  heatingSetpointSaturdayRule.daySchedule().setName("Medium Office Heating Setpoint Saturday Schedule");
  heatingSetpointSaturdayRule.daySchedule().clearValues();
  heatingSetpointSaturdayRule.daySchedule().addValue(Time(0,6,0,0),15.6);
  heatingSetpointSaturdayRule.daySchedule().addValue(Time(0,18,0,0),21.0);
  heatingSetpointSaturdayRule.daySchedule().addValue(Time(0,24,0,0),15.6);
}

void addExampleConstructions(Model& model) {

  DefaultConstructionSet defaultConstructions(model);
  defaultConstructions.setName("Default Constructions");
  DefaultSurfaceConstructions exteriorSurfaceConstructions(model);
  exteriorSurfaceConstructions.setName("Exterior Surface Constructions");
  defaultConstructions.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructions);
  DefaultSurfaceConstructions interiorSurfaceConstructions(model);
  interiorSurfaceConstructions.setName("Interior Surface Constructions");
  defaultConstructions.setDefaultInteriorSurfaceConstructions(interiorSurfaceConstructions);
  DefaultSurfaceConstructions groundContactSurfaceConstructions(model);
  groundContactSurfaceConstructions.setName("Ground Contact Surface Constructions");
  defaultConstructions.setDefaultGroundContactSurfaceConstructions(groundContactSurfaceConstructions);
  DefaultSubSurfaceConstructions exteriorSubSurfaceConstructions(model);
  exteriorSubSurfaceConstructions.setName("Exterior SubSurface Constructions");
  defaultConstructions.setDefaultExteriorSubSurfaceConstructions(exteriorSubSurfaceConstructions);
  DefaultSubSurfaceConstructions interiorSubSurfaceConstructions(model);
  interiorSubSurfaceConstructions.setName("Interior SubSurface Constructions");
  defaultConstructions.setDefaultInteriorSubSurfaceConstructions(interiorSubSurfaceConstructions);

  OpaqueMaterialVector opaqueMaterials;

  // Exterior Wall

  StandardOpaqueMaterial m01_100mm_brick(model);
  m01_100mm_brick.setName("M01 100mm brick");
  m01_100mm_brick.setRoughness("MediumRough");
  m01_100mm_brick.setThickness(0.1016);
  m01_100mm_brick.setThermalConductivity(0.89);
  m01_100mm_brick.setDensity(1920.0);
  m01_100mm_brick.setSpecificHeat(790.0);

  opaqueMaterials.push_back(m01_100mm_brick);

  StandardOpaqueMaterial m15_200mm_heavyweight_concrete(model);
  m15_200mm_heavyweight_concrete.setName("M15 200mm heavyweight concrete");
  m15_200mm_heavyweight_concrete.setRoughness("MediumRough");
  m15_200mm_heavyweight_concrete.setThickness(0.2032);
  m15_200mm_heavyweight_concrete.setThermalConductivity(1.95);
  m15_200mm_heavyweight_concrete.setDensity(2240.0);
  m15_200mm_heavyweight_concrete.setSpecificHeat(900.0);

  opaqueMaterials.push_back(m15_200mm_heavyweight_concrete);

  StandardOpaqueMaterial i02_50mm_insulation_board(model);
  i02_50mm_insulation_board.setName("I02 50mm insulation board");
  i02_50mm_insulation_board.setRoughness("MediumRough");
  i02_50mm_insulation_board.setThickness(0.0508);
  i02_50mm_insulation_board.setThermalConductivity(0.03);
  i02_50mm_insulation_board.setDensity(43.0);
  i02_50mm_insulation_board.setSpecificHeat(1210.0);

  opaqueMaterials.push_back(i02_50mm_insulation_board);

  AirGap f04_wall_air_space_resistance(model);
  f04_wall_air_space_resistance.setName("F04 Wall air space resistance");
  f04_wall_air_space_resistance.setThermalResistance(0.15);

  opaqueMaterials.push_back(f04_wall_air_space_resistance);

  StandardOpaqueMaterial g01a_19mm_gypsum_board(model);
  g01a_19mm_gypsum_board.setName("G01a 19mm gypsum board");
  g01a_19mm_gypsum_board.setRoughness("MediumSmooth");
  g01a_19mm_gypsum_board.setThickness(0.019);
  g01a_19mm_gypsum_board.setThermalConductivity(0.16);
  g01a_19mm_gypsum_board.setDensity(800.0);
  g01a_19mm_gypsum_board.setSpecificHeat(1090.0);

  opaqueMaterials.push_back(g01a_19mm_gypsum_board);

  Construction exteriorWall(opaqueMaterials);
  exteriorWall.setName("Exterior Wall");
  exteriorWall.setInsulation(i02_50mm_insulation_board);
  exteriorSurfaceConstructions.setWallConstruction(exteriorWall);
  opaqueMaterials.clear();

  // Exterior Roof

  StandardOpaqueMaterial m11_100mm_lightweight_concrete(model);
  m11_100mm_lightweight_concrete.setName("M11 100mm lightweight concrete");
  m11_100mm_lightweight_concrete.setRoughness("MediumRough");
  m11_100mm_lightweight_concrete.setThickness(0.1016);
  m11_100mm_lightweight_concrete.setThermalConductivity(0.53);
  m11_100mm_lightweight_concrete.setDensity(1280.0);
  m11_100mm_lightweight_concrete.setSpecificHeat(840.0);

  opaqueMaterials.push_back(m11_100mm_lightweight_concrete);

  AirGap f05_ceiling_air_space_resistance(model);
  f05_ceiling_air_space_resistance.setName("F05 Ceiling air space resistance");
  f05_ceiling_air_space_resistance.setThermalResistance(0.18);

  opaqueMaterials.push_back(f05_ceiling_air_space_resistance);

  StandardOpaqueMaterial f16_acoustic_tile(model);
  f16_acoustic_tile.setName("F16 Acoustic tile");
  f16_acoustic_tile.setRoughness("MediumSmooth");
  f16_acoustic_tile.setThickness(0.0191);
  f16_acoustic_tile.setThermalConductivity(0.06);
  f16_acoustic_tile.setDensity(368.0);
  f16_acoustic_tile.setSpecificHeat(590.0);

  opaqueMaterials.push_back(f16_acoustic_tile);

  Construction exteriorRoof(opaqueMaterials);
  exteriorRoof.setName("Exterior Roof");
  exteriorSurfaceConstructions.setRoofCeilingConstruction(exteriorRoof);
  opaqueMaterials.clear();

  // Interior Floor

  opaqueMaterials.push_back(f16_acoustic_tile);
  opaqueMaterials.push_back(f05_ceiling_air_space_resistance);
  opaqueMaterials.push_back(m11_100mm_lightweight_concrete);

  Construction interiorFloor(opaqueMaterials);
  interiorFloor.setName("Interior Floor");
  interiorSurfaceConstructions.setFloorConstruction(interiorFloor);
  opaqueMaterials.clear();

  // Air Wall

  AirWallMaterial airWallMaterial(model);
  airWallMaterial.setName("Air Wall Material");

  Construction airWall(airWallMaterial);
  airWall.setName("Air Wall");
  interiorSurfaceConstructions.setWallConstruction(airWall);

  // Interior Ceiling

  opaqueMaterials.push_back(m11_100mm_lightweight_concrete);
  opaqueMaterials.push_back(f05_ceiling_air_space_resistance);
  opaqueMaterials.push_back(f16_acoustic_tile);

  Construction interiorCeiling(opaqueMaterials);
  interiorCeiling.setName("Interior Ceiling");
  interiorSurfaceConstructions.setRoofCeilingConstruction(interiorCeiling);
  opaqueMaterials.clear();

  // Slab

  StandardOpaqueMaterial mat_cc05_8_hw_concrete(model);
  mat_cc05_8_hw_concrete.setName("MAT-CC05 8 HW CONCRETE");
  mat_cc05_8_hw_concrete.setRoughness("Rough");
  mat_cc05_8_hw_concrete.setThickness(0.2032);
  mat_cc05_8_hw_concrete.setThermalConductivity(1.3110);
  mat_cc05_8_hw_concrete.setDensity(2240.0);
  mat_cc05_8_hw_concrete.setSpecificHeat(836.8);
  mat_cc05_8_hw_concrete.setThermalAbsorptance(0.9);
  mat_cc05_8_hw_concrete.setSolarAbsorptance(0.7);
  mat_cc05_8_hw_concrete.setVisibleAbsorptance(0.7);

  opaqueMaterials.push_back(mat_cc05_8_hw_concrete);

  MasslessOpaqueMaterial cp02_carpet_pad(model);
  cp02_carpet_pad.setName("CP02 CARPET PAD");
  cp02_carpet_pad.setRoughness("VeryRough");
  cp02_carpet_pad.setThermalResistance(0.2165);
  cp02_carpet_pad.setThermalAbsorptance(0.9);
  cp02_carpet_pad.setSolarAbsorptance(0.7);
  cp02_carpet_pad.setVisibleAbsorptance(0.8);

  opaqueMaterials.push_back(cp02_carpet_pad);

  Construction slab(opaqueMaterials);
  slab.setName("Slab");
  groundContactSurfaceConstructions.setFloorConstruction(slab);
  opaqueMaterials.clear();

  FenestrationMaterialVector fenestrationMaterials;

  // Exterior Window

  SimpleGlazing simple_glazing(model);
  simple_glazing.setName("Simple Glazing");
  simple_glazing.setUFactor(3.23646);
  simple_glazing.setSolarHeatGainCoefficient(0.39);
  simple_glazing.setVisibleTransmittance(0.6);

  StandardGlazing clear_3mm(model);
  clear_3mm.setName("Clear 3mm");
  clear_3mm.setOpticalDataType("SpectralAverage");
  clear_3mm.setThickness(0.003);
  clear_3mm.setSolarTransmittance(0.837);
  clear_3mm.setFrontSideSolarReflectanceatNormalIncidence(0.075);
  clear_3mm.setBackSideSolarReflectanceatNormalIncidence(0.075);
  clear_3mm.setVisibleTransmittance(0.898);
  clear_3mm.setFrontSideVisibleReflectanceatNormalIncidence(0.081);
  clear_3mm.setBackSideVisibleReflectanceatNormalIncidence(0.081);
  clear_3mm.setInfraredTransmittance(0.0);
  clear_3mm.setFrontSideInfraredHemisphericalEmissivity(0.084);
  clear_3mm.setBackSideInfraredHemisphericalEmissivity(0.084);
  clear_3mm.setThermalConductivity(0.9);

  Gas air_13mm(model);
  air_13mm.setName("Air 13mm");
  air_13mm.setGasType("Air");
  air_13mm.setThickness(0.0127);

  //fenestrationMaterials.push_back(clear_3mm);
  //fenestrationMaterials.push_back(air_13mm);
  //fenestrationMaterials.push_back(clear_3mm);

  // DLM: use simple glazing so we can know window properties without requiring E+ run
  fenestrationMaterials.push_back(simple_glazing);

  Construction exteriorWindow(fenestrationMaterials);
  exteriorWindow.setName("Exterior Window");
  exteriorSubSurfaceConstructions.setFixedWindowConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setOperableWindowConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setGlassDoorConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setSkylightConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setTubularDaylightDomeConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(exteriorWindow);
  fenestrationMaterials.clear();

  // Exterior Door

  StandardOpaqueMaterial f08_metal_surface(model);
  f08_metal_surface.setName("F08 Metal surface");
  f08_metal_surface.setRoughness("Smooth");
  f08_metal_surface.setThickness(0.0008);
  f08_metal_surface.setThermalConductivity(45.28);
  f08_metal_surface.setDensity(7824.0);
  f08_metal_surface.setSpecificHeat(500.0);

  opaqueMaterials.push_back(f08_metal_surface);

  StandardOpaqueMaterial i01_25mm_insulation_board(model);
  i01_25mm_insulation_board.setName("I02 25mm insulation board");
  i01_25mm_insulation_board.setRoughness("MediumRough");
  i01_25mm_insulation_board.setThickness(0.0254);
  i01_25mm_insulation_board.setThermalConductivity(0.03);
  i01_25mm_insulation_board.setDensity(43.0);
  i01_25mm_insulation_board.setSpecificHeat(1210.0);

  opaqueMaterials.push_back(i01_25mm_insulation_board);

  Construction exteriorDoor(opaqueMaterials);
  exteriorDoor.setName("Exterior Door");
  exteriorDoor.setInsulation(i01_25mm_insulation_board);
  exteriorSubSurfaceConstructions.setDoorConstruction(exteriorDoor);
  exteriorSubSurfaceConstructions.setOverheadDoorConstruction(exteriorDoor);
  opaqueMaterials.clear();

  // Interior Window

  //fenestrationMaterials.push_back(clear_3mm);

  // DLM: use simple glazing so we can know window properties without requiring E+ run
  fenestrationMaterials.push_back(simple_glazing);

  Construction interiorWindow(fenestrationMaterials);
  interiorWindow.setName("Interior Window");
  interiorSubSurfaceConstructions.setFixedWindowConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setOperableWindowConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setGlassDoorConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setSkylightConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setTubularDaylightDomeConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(interiorWindow);
  fenestrationMaterials.clear();

  // Interior Door

  StandardOpaqueMaterial g05_25mm_wood(model);
  g05_25mm_wood.setName("G05 25mm wood");
  g05_25mm_wood.setRoughness("MediumSmooth");
  g05_25mm_wood.setThickness(0.0254);
  g05_25mm_wood.setThermalConductivity(0.15);
  g05_25mm_wood.setDensity(608.0);
  g05_25mm_wood.setSpecificHeat(1630.0);

  opaqueMaterials.push_back(g05_25mm_wood);

  Construction interiorDoor(opaqueMaterials);
  interiorDoor.setName("Interior Door");
  interiorSubSurfaceConstructions.setDoorConstruction(interiorDoor);
  interiorSubSurfaceConstructions.setOverheadDoorConstruction(interiorDoor);
  opaqueMaterials.clear();

  // Interior Partition

  opaqueMaterials.push_back(g05_25mm_wood);

  Construction interiorPartition(opaqueMaterials);
  interiorPartition.setName("Interior Partition");
  defaultConstructions.setInteriorPartitionConstruction(interiorPartition);
  opaqueMaterials.clear();
}

} // model
} // openstudio
