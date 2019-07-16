/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "EnergyPlusAPI.hpp"
#include "ReverseTranslator.hpp"
#include "GeometryTranslator.hpp"

#include "../model/ModelObject.hpp"

#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/ValidityReport.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <QThread>
#include <boost/serialization/version.hpp>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

ReverseTranslator::ReverseTranslator()
{
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ReverseTranslator"));
  m_logSink.setThreadId(QThread::currentThread());
}

boost::optional<model::Model> ReverseTranslator::loadModel(const openstudio::path& path, ProgressBar* progressBar)
{
  m_model = Model();
  m_model.setFastNaming(true);

  m_workspace = Workspace(StrictnessLevel(StrictnessLevel::None),
                          IddFileType(IddFileType::EnergyPlus));

  m_workspaceToModelMap.clear();

  m_untranslatedIdfObjects.clear();

  m_logSink.resetStringStream();

  m_logSink.setThreadId(QThread::currentThread());

  m_logSink.setChannelRegex(boost::regex("openstudio\\.IdfFile"));

  // load idf
  boost::optional<openstudio::IdfFile> idfFile = IdfFile::load(path, IddFileType::EnergyPlus, progressBar);

  // change channel after loading file
  // DLM: is this right?  we miss messages from loading idf
  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ReverseTranslator"));

  // energyplus idfs may not be draft level strictness, eventually need a fixer
  if (!idfFile){

    LOG(Error, "Could not read idf file at path ='" << toString(path) << "'");

  }else{

    if (!idfFile->isValid(StrictnessLevel::Draft)){

      LOG(Error, "Idf file at path ='" << toString(path) << "' is not valid to draft strictness.");
      LOG(Error, "Check that IDF is of correct version and that all fields are valid against Energy+.idd.");
      LOG(Error, idfFile->validityReport(StrictnessLevel::Draft));
      return boost::none;
    }

    VersionString expectedVersion(IddFileAndFactoryWrapper(IddFileType::EnergyPlus).version());
    boost::optional<IdfObject> versionObject = idfFile->versionObject();
    if (!versionObject) {
      LOG(Warn, "Idf file missing Version object, use IDFVersionUpdater to ensure that Idf file is at expected version = '" << expectedVersion.str() << "'");
    } else {
      boost::optional<std::string> vs = versionObject->getString(versionObject->numFields() - 1);
      if (!vs){
        LOG(Warn, "Idf file contains empty Version object, use IDFVersionUpdater to ensure that Idf file is at expected version = '" << expectedVersion.str() << "'");
      } else{
        VersionString fileVersion(*vs);
        if ((expectedVersion.major() != fileVersion.major()) || (expectedVersion.minor() != fileVersion.minor())){
          LOG(Warn, "Idf file Version = '" << fileVersion.str() << "' does not match expected version = '" << expectedVersion.str() << "'");
        }
      }
    }

    if (progressBar){
      progressBar->setWindowTitle(toString("Creating EnergyPlus Workspace"));
    }

    Workspace workspace(StrictnessLevel::None,
                        IddFileType(IddFileType::EnergyPlus));

    if (progressBar){
      workspace.connectProgressBar(*progressBar);
    }

    workspace.addObjects(idfFile->objects());

    if (progressBar){
      workspace.disconnectProgressBar(*progressBar);
    }

    return this->translateWorkspace(workspace, progressBar, false);

  }

  return boost::none;
}

Model ReverseTranslator::translateWorkspace(const Workspace & workspace, ProgressBar* progressBar, bool clearLogSink )
{
  if (clearLogSink){
    m_logSink.resetStringStream();
  }

  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ReverseTranslator"));

  // check input
  if (workspace.iddFileType() != IddFileType::EnergyPlus){
    LOG(Error, "Cannot translate Workspace with IddFileType = '" << workspace.iddFileType().valueName() << "'");
    return Model();
  }

  m_model = Model();
  m_model.setFastNaming(true);

  m_workspace = workspace.clone();

  m_workspaceToModelMap.clear();

  m_untranslatedIdfObjects.clear();

  // if multiple runperiod objects in idf, remove them all
  vector<WorkspaceObject> runPeriods = m_workspace.getObjectsByType(IddObjectType::RunPeriod);
  if (runPeriods.size() > 1){
    LOG(Warn, "Multiple RunPeriod objects detected, removing all RunPeriod objects.");
    for(auto & runPeriod : runPeriods)
    {
      runPeriod.remove();
    }
  }

  // first thing to do is convert geometry system
  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.GeometryTranslator"));

  m_progressBar = progressBar;
  if (m_progressBar){
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(workspace.numObjects());
  }

  LOG(Trace,"Calling geometry translator.");
  GeometryTranslator geometryTranslator(m_workspace);
  geometryTranslator.convert(CoordinateSystem::Relative, CoordinateSystem::Relative);

  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ReverseTranslator"));

  // look for site object in workspace and translate if found
  LOG(Trace,"Translating Site:Location object.");
  vector<WorkspaceObject> site = m_workspace.getObjectsByType(IddObjectType::Site_Location);
  for(auto & elem : site)
  {
    translateAndMapWorkspaceObject(elem);
  }

  // look for simulation control object in workspace and translate if found
  LOG(Trace,"Translating SimulationControl object.");
  vector<WorkspaceObject> simulationControl = m_workspace.getObjectsByType(IddObjectType::SimulationControl);
  for(auto & elem : simulationControl)
  {
    translateAndMapWorkspaceObject(elem);
  }

  // loop over all of the air loops
  LOG(Trace,"Translating AirLoops.");
  vector<WorkspaceObject> airLoops = m_workspace.getObjectsByType(IddObjectType::AirLoopHVAC);
  for(auto it = airLoops.begin(),iend=airLoops.end();it!=iend;++it)
  {
    LOG(Trace,"Translating AirLoop '" << it->name().get() << "'.");
    translateAndMapWorkspaceObject( *it );
  }

  // Now loop over all objects to make sure nothing as missed.
  // In the future this might be removed.
  LOG(Trace,"Translating remaining objects.");
  vector<WorkspaceObject> all = m_workspace.objects();
  for(auto & elem : all)
  {
    translateAndMapWorkspaceObject( elem );
  }

  LOG(Trace,"Translation nominally complete.");
  m_model.setFastNaming(false);
  return m_model;
}

std::vector<LogMessage> ReverseTranslator::warnings() const
{
  std::vector<LogMessage> result;

  for (LogMessage logMessage : m_logSink.logMessages()){
    if (logMessage.logLevel() == Warn){
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> ReverseTranslator::errors() const
{
  std::vector<LogMessage> result;

  for (LogMessage logMessage : m_logSink.logMessages()){
    if (logMessage.logLevel() > Warn){
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<IdfObject> ReverseTranslator::untranslatedIdfObjects() const
{
  return m_untranslatedIdfObjects;
}

struct IdfObjectEqual {
  explicit IdfObjectEqual(const IdfObject& target)
    : m_target(target)
  {}

  bool operator()(const IdfObject& other) const
  {
    return (m_target == other);
  }

  IdfObject m_target;
};

boost::optional<ModelObject> ReverseTranslator::translateAndMapWorkspaceObject(const WorkspaceObject & workspaceObject)
{
  auto i = m_workspaceToModelMap.find(workspaceObject.handle());

  boost::optional<ModelObject> modelObject;

  if( i !=  m_workspaceToModelMap.end())
  {
    return boost::optional<ModelObject>(i->second);
  }

  LOG(Trace,"Translating " << workspaceObject.briefDescription() << ".");

  // DLM: the scope of this translator is being changed, we now only import objects from idf
  // in the geometry, loads, resources, and general simulation control portions of the model.
  // Users can add idf objects to their model using idf measures.  Only objects viewable in the
  // current GUIs should be imported, I am making an exception for curves.

  bool addToUntranslated = true;

  switch(workspaceObject.iddObject().type().value())
  {
  case openstudio::IddObjectType::AirLoopHVAC :
    {
      //modelObject = translateAirLoopHVAC(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::AirLoopHVAC_ControllerList :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::AirLoopHVAC_OutdoorAirSystem :
    {
      //modelObject = translateAirLoopHVACOutdoorAirSystem(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::AirLoopHVAC_OutdoorAirSystem_EquipmentList :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::AirLoopHVAC_ReturnPath :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::CoilSystem_Cooling_DX :
    {
      //modelObject = translateCoilSystemCoolingDX(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::AirLoopHVAC_ZoneSplitter :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_Reheat :
    {
      modelObject = translateAirTerminalSingleDuctConstantVolumeReheat(workspaceObject );
      break;
    }

  case openstudio::IddObjectType::AirTerminal_SingleDuct_Uncontrolled :
    {
      // We map this to ATU:CV:NoReheat which is the new name
      modelObject = translateAirTerminalSingleDuctConstantVolumeNoReheat(workspaceObject );
      break;
    }

  case openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_NoReheat :
    {
      modelObject = translateAirTerminalSingleDuctConstantVolumeNoReheat(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::AirTerminal_SingleDuct_VAV_NoReheat :
    {
      modelObject = translateAirTerminalSingleDuctVAVNoReheat(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::AirTerminal_SingleDuct_VAV_Reheat :
    {
      //modelObject = translateAirTerminalSingleDuctVAVReheat(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::AvailabilityManagerAssignmentList :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Branch :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::BranchList :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::BuildingSurface_Detailed :
    {
      modelObject = translateBuildingSurfaceDetailed(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Building :
    {
      modelObject = translateBuilding(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Coil_Heating_Fuel :
    {
      //modelObject = translateCoilHeatingGas(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Coil_Cooling_DX_SingleSpeed :
    {
      //modelObject = translateCoilCoolingDXSingleSpeed(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::CommentOnly :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::ComponentCost_LineItem :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Connector_Mixer :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Connector_Splitter :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::ConnectorList :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Construction :
    {
      modelObject = translateConstruction(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Controller_OutdoorAir :
    {
      //modelObject = translateControllerOutdoorAir(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ConvergenceLimits :
    {
      modelObject = translateConvergenceLimits(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Bicubic :
    {
      modelObject = translateCurveBicubic(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Biquadratic :
    {
      modelObject = translateCurveBiquadratic(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Cubic :
    {
      modelObject = translateCurveCubic(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_DoubleExponentialDecay :
    {
      modelObject = translateCurveDoubleExponentialDecay(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_ExponentialSkewNormal :
    {
      modelObject = translateCurveExponentialSkewNormal(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_FanPressureRise :
    {
      modelObject = translateCurveFanPressureRise(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Functional_PressureDrop :
    {
      modelObject = translateCurveFunctionalPressureDrop(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Linear :
    {
      modelObject = translateCurveLinear(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Quadratic :
    {
      modelObject = translateCurveQuadratic(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_QuadraticLinear :
    {
      modelObject = translateCurveQuadraticLinear(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Quartic :
    {
      modelObject = translateCurveQuartic(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_RectangularHyperbola1 :
    {
      modelObject = translateCurveRectangularHyperbola1(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_RectangularHyperbola2 :
    {
      modelObject = translateCurveRectangularHyperbola2(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Sigmoid :
    {
      modelObject = translateCurveSigmoid(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Curve_Triquadratic :
    {
      modelObject = translateCurveTriquadratic(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Daylighting_Controls :
    {
      modelObject = translateDaylightingControls(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::DesignSpecification_OutdoorAir :
    {
      // Call this directly because we don't want to translate all of them
      // only those that are connected to the SizingZone object
      //modelObject = translateDesignSpecificationOutdoorAir(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ElectricEquipment :
    {
      modelObject = translateElectricEquipment(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Exterior_Lights :
    {
      modelObject = translateExteriorLights(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Exterior_FuelEquipment :
    {
      modelObject = translateExteriorFuelEquipment(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Exterior_WaterEquipment :
    {
      modelObject = translateExteriorWaterEquipment(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ElectricLoadCenter_Storage_Simple :
    {
      modelObject = translateElectricLoadCenterStorageSimple(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ElectricLoadCenter_Storage_Converter :
    {
      modelObject = translateElectricLoadCenterStorageConverter(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::EnergyManagementSystem_Actuator:
  {
    modelObject = translateEnergyManagementSystemActuator(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_ConstructionIndexVariable:
  {
    modelObject = translateEnergyManagementSystemConstructionIndexVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable:
  {
    modelObject = translateEnergyManagementSystemCurveOrTableIndexVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_GlobalVariable:
  {
    modelObject = translateEnergyManagementSystemGlobalVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_InternalVariable:
  {
    modelObject = translateEnergyManagementSystemInternalVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_MeteredOutputVariable:
  {
    modelObject = translateEnergyManagementSystemMeteredOutputVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_OutputVariable:
  {
    modelObject = translateEnergyManagementSystemOutputVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_Program:
  {
    modelObject = translateEnergyManagementSystemProgram(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_ProgramCallingManager:
  {
    modelObject = translateEnergyManagementSystemProgramCallingManager(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_Sensor:
  {
    modelObject = translateEnergyManagementSystemSensor(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_Subroutine:
  {
    modelObject = translateEnergyManagementSystemSubroutine(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EnergyManagementSystem_TrendVariable:
  {
    modelObject = translateEnergyManagementSystemTrendVariable(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::EvaporativeCooler_Direct_ResearchSpecial :
    {
      //modelObject = translateEvaporativeCoolerDirectResearchSpecial(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::EvaporativeFluidCooler_SingleSpeed :
    {
      modelObject = translateEvaporativeFluidCoolerSingleSpeed(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Fan_ConstantVolume :
    {
      //modelObject = translateFanConstantVolume(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::FenestrationSurface_Detailed :
    {
      modelObject = translateFenestrationSurfaceDetailed(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Generator_MicroTurbine :
    {
      modelObject = translateGeneratorMicroTurbine(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::GlobalGeometryRules :
    {
      // added by geometry translator, do not add to untranslated objects
      addToUntranslated = false;
      break; // no-op
    }
  case openstudio::IddObjectType::GasEquipment :
    {
      modelObject = translateGasEquipment(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::GroundHeatExchanger_System :
    {
      //modelObject = translateGroundHeatExchangerVertical(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::HeatBalanceAlgorithm :
    {
      modelObject = translateHeatBalanceAlgorithm(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::HotWaterEquipment :
    {
      modelObject = translateHotWaterEquipment(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::HVACTemplate_Thermostat :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::InternalMass :
    {
      modelObject = translateInternalMass(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Lights :
    {
      modelObject = translateLights(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Material :
    {
      modelObject = translateMaterial(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Material_AirGap :
    {
      modelObject = translateMaterialAirGap(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Material_NoMass :
    {
      modelObject = translateMaterialNoMass(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Meter_Custom :
    {
      modelObject = translateMeterCustom(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Meter_CustomDecrement :
    {
      modelObject = translateMeterCustomDecrement(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::OtherEquipment :
    {
      modelObject = translateOtherEquipment(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::OutdoorAir_Mixer :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::OutdoorAir_Node :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::OutdoorAir_NodeList :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Output_EnergyManagementSystem:
  {
    modelObject = translateOutputEnergyManagementSystem(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::Output_IlluminanceMap :
    {
      modelObject = translateOutputIlluminanceMap(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Output_Meter :
    {
      modelObject = translateOutputMeter(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Output_Meter_Cumulative :
    {
      modelObject = translateOutputMeterCumulative(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Output_Meter_Cumulative_MeterFileOnly :
    {
      modelObject = translateOutputMeterCumulativeMeterFileOnly(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Output_Meter_MeterFileOnly :
    {
      modelObject = translateOutputMeterMeterFileOnly(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Output_SQLite :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Output_Table_Monthly :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Output_Table_SummaryReports :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::Output_Variable :
    {
      modelObject = translateOutputVariable(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::OutputControl_Table_Style :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::People :
    {
      modelObject = translatePeople(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Refrigeration_Case :
    {
      // modelObject = translateRefrigerationCase(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Refrigeration_Compressor :
    {
      // modelObject = translateRefrigerationCompressor(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::RunPeriod :
    {
      modelObject = translateRunPeriod(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::RunPeriodControl_DaylightSavingTime :
    {
      modelObject = translateRunPeriodControlDaylightSavingTime(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::RunPeriodControl_SpecialDays :
    {
      //modelObject = translateRunPeriodControlSpecialDays(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Schedule_Compact :
    {
      modelObject = translateScheduleCompact(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Schedule_Constant :
    {
      modelObject = translateScheduleConstant(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Schedule_Day_Hourly :
    {
      modelObject = translateScheduleDayHourly(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Schedule_File:
    {
    modelObject = translateScheduleFile(workspaceObject);
    break;
    }
  case openstudio::IddObjectType::Schedule_Day_Interval :
    {
      modelObject = translateScheduleDayInterval(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ScheduleTypeLimits :
    {
      modelObject = translateScheduleTypeLimits(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Schedule_Week_Daily :
    {
      modelObject = translateScheduleWeekDaily(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Schedule_Year :
    {
      modelObject = translateScheduleYear(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::SetpointManager_MixedAir :
    {
      //modelObject = translateSetpointManagerMixedAir(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::SetpointManager_Scheduled :
    {
      //modelObject = translateSetpointManagerScheduled(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::SetpointManager_SingleZone_Reheat :
    {
      //modelObject = translateSetpointManagerSingleZoneReheat(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Shading_Building_Detailed :
    {
      modelObject = translateShadingBuildingDetailed(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Shading_Site_Detailed :
    {
      modelObject = translateShadingSiteDetailed(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Shading_Zone_Detailed :
    {
      modelObject = translateShadingZoneDetailed(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ShadowCalculation :
    {
      modelObject = translateShadowCalculation(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::SimulationControl :
    {
      modelObject = translateSimulationControl(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Site_Location :
    {
      modelObject = translateSiteLocation(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Site_GroundReflectance :
    {
      //modelObject = translateSiteGroundReflectance(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Site_GroundTemperature_BuildingSurface :
    {
      //modelObject = translateSiteGroundTemperatureBuildingSurface(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Site_WaterMainsTemperature :
    {
      //modelObject = translateSiteWaterMainsTemperature(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Sizing_Parameters :
    {
      modelObject = translateSizingParameters(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::SizingPeriod_DesignDay :
    {
      modelObject = translateSizingPeriodDesignDay(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Sizing_System :
    {
      //modelObject = translateSizingSystem(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::Sizing_Zone :
    {
      modelObject = translateSizingZone(workspaceObject );
      break;
    }
  case openstudio::IddObjectType::SteamEquipment :
    {
      modelObject = translateSteamEquipment(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::SurfaceConvectionAlgorithm_Inside :
    {
      //modelObject = translateSurfaceConvectionAlgorithmInside(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::SurfaceConvectionAlgorithm_Outside :
    {
      //modelObject = translateSurfaceConvectionAlgorithmOutside(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ThermostatSetpoint_DualSetpoint :
    {
      modelObject = translateThermostatSetpointDualSetpoint(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::Timestep :
    {
      modelObject = translateTimestep(workspaceObject);
      break;
    }

   // TODO: once UtilityCost objects are wrapped (and ReverseTranslated)
  //case openstudio::IddObjectType::OS_UtilityCost_Charge_Block:
    //{
      //break; // no-op
    //}
  //case openstudio::IddObjectType::UtilityCost_Charge_Simple :
    //{
      //break; // no-op
    //}
  //case openstudio::IddObjectType::UtilityCost_Computation :
    //{
      //break; // no-op
    //}
  //case openstudio::IddObjectType::UtilityCost_Qualify :
    //{
      //break; // no-op
    //}
  //case openstudio::IddObjectType::UtilityCost_Ratchet :
    //{
      //break; // no-op
    //}
  //case openstudio::IddObjectType::UtilityCost_Tariff :
    //{
      //break; // no-op
    //}
  //case openstudio::IddObjectType::UtilityCost_Variable :
    //{
      //break; // no-op
    //}

  case openstudio::IddObjectType::Version :
   {
     modelObject = translateVersion(workspaceObject );
     break;
   }
  case openstudio::IddObjectType::WindowMaterial_Gas:
    {
      modelObject = translateWindowMaterialGas(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::WindowMaterial_Glazing:
    {
      modelObject = translateWindowMaterialGlazing(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::WindowMaterial_SimpleGlazingSystem:
    {
      modelObject = translateWindowMaterialSimpleGlazingSystem(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::WindowProperty_FrameAndDivider:
  {
    modelObject = translateWindowPropertyFrameAndDivider(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::Zone:
    {
      modelObject = translateZone(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ZoneAirHeatBalanceAlgorithm:
    {
      // DLM: why is this commented out?
      //modelObject = translateZoneAirHeatBalanceAlgorithm(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ZoneAirMassFlowConservation:
  {
    modelObject = translateZoneAirMassFlowConservation(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::ZoneControl_Thermostat :
    {
      break; // no-op
    }
  case openstudio::IddObjectType::ZoneCrossMixing:
  {
    modelObject = translateZoneCrossMixing(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::ZoneHVAC_EquipmentList :
    {
      //modelObject = translateZoneHVACEquipmentList(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ZoneHVAC_IdealLoadsAirSystem :
    {
      //modelObject = translateZoneHVACIdealLoadsAirSystem(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ZoneInfiltration_DesignFlowRate :
    {
      modelObject = translateZoneInfiltrationDesignFlowRate(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ZoneInfiltration_EffectiveLeakageArea :
  {
      modelObject = translateZoneInfiltrationEffectiveLeakageArea(workspaceObject);
      break;
  }
  case openstudio::IddObjectType::ZoneList:
    {
      modelObject = translateZone(workspaceObject);
      break;
    }
  case openstudio::IddObjectType::ZoneMixing:
  {
    modelObject = translateZoneMixing(workspaceObject);
    break;
  }
  case openstudio::IddObjectType::ZoneVentilation_DesignFlowRate :
    {
      modelObject = translateZoneVentilationDesignFlowRate(workspaceObject);
      break;
    }
  default:
    {
      break; // no-op
    }
  }

  if( modelObject )
  {
    LOG(Trace,"Adding " << modelObject.get().briefDescription() << " to map.");
    m_workspaceToModelMap.insert(make_pair(workspaceObject.handle(), modelObject.get()));
  }else{
    if (addToUntranslated){
      if (std::find_if(m_untranslatedIdfObjects.begin(), m_untranslatedIdfObjects.end(), IdfObjectEqual(workspaceObject.idfObject())) == m_untranslatedIdfObjects.end()){
        LOG(Trace,"Ignoring " << workspaceObject.briefDescription() << ".");
        m_untranslatedIdfObjects.push_back(workspaceObject.idfObject());
      }
    }
  }

  if (m_progressBar){
    m_progressBar->setValue(m_untranslatedIdfObjects.size() + m_workspaceToModelMap.size());
  }

  return modelObject;
}

boost::optional<openstudio::model::Model> loadAndTranslateIdf(const openstudio::path& path)
{
  boost::optional<openstudio::model::Model> result;
  boost::optional<openstudio::Workspace> workspace = openstudio::Workspace::load(path);
  if (workspace){
    ReverseTranslator rt;
    result = rt.translateWorkspace(*workspace);
  }
  return result;
}


} // energyplus

} // openstudio

