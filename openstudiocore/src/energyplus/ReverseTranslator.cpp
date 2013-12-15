/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "EnergyPlusAPI.hpp"
#include <energyplus/ReverseTranslator.hpp>
#include <energyplus/GeometryTranslator.hpp>

#include <model/ModelObject.hpp>

#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/WorkspaceObject.hpp>
#include <utilities/idf/ValidityReport.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFieldEnums.hxx>

#include <utilities/core/Assert.hpp>
#include <utilities/plot/ProgressBar.hpp>

#include <QThread>

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

  //load idf and convert to a workspace
  boost::optional<openstudio::IdfFile> idfFile = IdfFile::load(path, IddFileType::EnergyPlus, progressBar);

  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ReverseTranslator"));

  // energyplus idfs may not be draft level strictness, eventually need a fixer
  if (!idfFile){

    LOG(Error, "Could not read idf file at path ='" << toString(path) << "'");

  }else{

    if (!idfFile->isValid(StrictnessLevel::Draft)){

      LOG(Error, "Idf file at path ='" << toString(path) << "' is not valid to draft strictness");
      LOG(Error, idfFile->validityReport(StrictnessLevel::Draft));
      return boost::none;

    }

    if (progressBar){
      progressBar->setWindowTitle(toString("Creating EnergyPlus Workspace"));
    }

    Workspace workspace(StrictnessLevel::None,
                        IddFileType(IddFileType::EnergyPlus));

    if (progressBar){
      workspace.connectProgressBar(progressBar);
    }

    workspace.addObjects(idfFile->objects());

    if (progressBar){
      workspace.disconnectProgressBar(progressBar);
    }

    return this->translateWorkspace(workspace, progressBar);

  }

  return boost::none;
}

Model ReverseTranslator::translateWorkspace(const Workspace & workspace, ProgressBar* progressBar )
{
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

  m_logSink.resetStringStream();

  // if multiple runperiod objects in idf, remove them all
  vector<WorkspaceObject> runPeriods = m_workspace.getObjectsByType(IddObjectType::RunPeriod);
  if (runPeriods.size() > 1){
    for(vector<WorkspaceObject>::iterator i = runPeriods.begin(),iend=runPeriods.end();i!=iend;++i)
    {
      i->remove();
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
  for(vector<WorkspaceObject>::iterator i = site.begin(),iend=site.end();i!=iend;++i)
  {
    translateAndMapWorkspaceObject(*i);
  }

  // look for simulation control object in workspace and translate if found
  LOG(Trace,"Translating SimulationControl object.");
  vector<WorkspaceObject> simulationControl = m_workspace.getObjectsByType(IddObjectType::SimulationControl);
  for(vector<WorkspaceObject>::iterator i = simulationControl.begin(),iend=simulationControl.end();i!=iend;++i)
  {
    translateAndMapWorkspaceObject(*i);
  }

  // loop over all of the air loops
  LOG(Trace,"Translating AirLoops.");
  vector<WorkspaceObject> airLoops = m_workspace.getObjectsByType(IddObjectType::AirLoopHVAC);
  for(vector<WorkspaceObject>::iterator it = airLoops.begin(),iend=airLoops.end();it!=iend;++it)
  {
    LOG(Trace,"Translating AirLoop '" << it->name().get() << "'.");
    translateAndMapWorkspaceObject( *it );
  }

  // Now loop over all objects to make sure nothing as missed.
  // In the future this might be removed.
  LOG(Trace,"Translating remaining objects.");
  vector<WorkspaceObject> all = m_workspace.objects();
  for(vector<WorkspaceObject>::iterator it = all.begin(),iend=all.end();it!=iend;++it)
  {
    translateAndMapWorkspaceObject( *it );
  }

  LOG(Trace,"Translation nominally complete.");
  m_model.setFastNaming(false);
  return m_model;
}

std::vector<LogMessage> ReverseTranslator::warnings() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
    if (logMessage.logLevel() == Warn){
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> ReverseTranslator::errors() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
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
  map<Handle,ModelObject>::iterator i = m_workspaceToModelMap.find(workspaceObject.handle());

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

  if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirLoopHVAC) )
    {
      //modelObject = translateAirLoopHVAC(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirLoopHVAC_ControllerList) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirLoopHVAC_OutdoorAirSystem) )
    {
      //modelObject = translateAirLoopHVACOutdoorAirSystem(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirLoopHVAC_OutdoorAirSystem_EquipmentList) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirLoopHVAC_ReturnPath) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::CoilSystem_Cooling_DX) )
    {
      //modelObject = translateCoilSystemCoolingDX(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirLoopHVAC_ZoneSplitter) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirTerminal_SingleDuct_ConstantVolume_Reheat) )
    {
      modelObject = translateAirTerminalSingleDuctConstantVolumeReheat(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirTerminal_SingleDuct_Uncontrolled) )
    {
      //modelObject = translateAirTerminalSingleDuctUncontrolled(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirTerminal_SingleDuct_VAV_NoReheat) )
    {
      modelObject = translateAirTerminalSingleDuctVAVNoReheat(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AirTerminal_SingleDuct_VAV_Reheat) )
    {
      //modelObject = translateAirTerminalSingleDuctVAVReheat(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::AvailabilityManagerAssignmentList) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Branch) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::BranchList) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::BuildingSurface_Detailed) )
    {
      modelObject = translateBuildingSurfaceDetailed(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Building) )
    {
      modelObject = translateBuilding(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Coil_Heating_Gas) )
    {
      //modelObject = translateCoilHeatingGas(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Coil_Cooling_DX_SingleSpeed) )
    {
      //modelObject = translateCoilCoolingDXSingleSpeed(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::CommentOnly) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ComponentCost_LineItem) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Connector_Mixer) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Connector_Splitter) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ConnectorList) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Construction) )
    {
      modelObject = translateConstruction(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Controller_OutdoorAir) )
    {
      //modelObject = translateControllerOutdoorAir(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ConvergenceLimits) )
    {
      modelObject = translateConvergenceLimits(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Bicubic) )
    {
      modelObject = translateCurveBicubic(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Biquadratic) )
    {
      modelObject = translateCurveBiquadratic(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Cubic) )
    {
      modelObject = translateCurveCubic(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_DoubleExponentialDecay) )
    {
      modelObject = translateCurveDoubleExponentialDecay(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_ExponentialSkewNormal) )
    {
      modelObject = translateCurveExponentialSkewNormal(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_FanPressureRise) )
    {
      modelObject = translateCurveFanPressureRise(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Functional_PressureDrop) )
    {
      modelObject = translateCurveFunctionalPressureDrop(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Linear) )
    {
      modelObject = translateCurveLinear(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Quadratic) )
    {
      modelObject = translateCurveQuadratic(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_QuadraticLinear) )
    {
      modelObject = translateCurveQuadraticLinear(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Quartic) )
    {
      modelObject = translateCurveQuartic(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_RectangularHyperbola1) )
    {
      modelObject = translateCurveRectangularHyperbola1(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_RectangularHyperbola2) )
    {
      modelObject = translateCurveRectangularHyperbola2(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Sigmoid) )
    {
      modelObject = translateCurveSigmoid(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Curve_Triquadratic) )
    {
      modelObject = translateCurveTriquadratic(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Daylighting_Controls) )
    {
      modelObject = translateDaylightingControls(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::DesignSpecification_OutdoorAir) )
    {
      modelObject = translateDesignSpecificationOutdoorAir(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ElectricEquipment) )
    {
      modelObject = translateElectricEquipment(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Exterior_Lights) )
    {
      //modelObject = translateExteriorLights(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::EvaporativeCooler_Direct_ResearchSpecial) )
    {
      //modelObject = translateEvaporativeCoolerDirectResearchSpecial(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::EvaporativeFluidCooler_SingleSpeed) )
    {
      modelObject = translateEvaporativeFluidCoolerSingleSpeed(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Fan_ConstantVolume) )
    {
      //modelObject = translateFanConstantVolume(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::FenestrationSurface_Detailed) )
    {
      modelObject = translateFenestrationSurfaceDetailed(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::GlobalGeometryRules) )
    {
      // added by geometry translator, do not add to untranslated objects
      addToUntranslated = false;
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::GasEquipment) )
    {
      modelObject = translateGasEquipment(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::GroundHeatExchanger_Vertical) )
    {
      //modelObject = translateGroundHeatExchangerVertical(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::HeatBalanceAlgorithm) )
    {
      modelObject = translateHeatBalanceAlgorithm(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::HotWaterEquipment) )
    {
      modelObject = translateHotWaterEquipment(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::HVACTemplate_Thermostat) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::InternalMass) )
    {
      modelObject = translateInternalMass(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Lights) )
    {
      modelObject = translateLights(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Material) )
    {
      modelObject = translateMaterial(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Material_AirGap) )
    {
      modelObject = translateMaterialAirGap(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Material_NoMass) )
    {
      modelObject = translateMaterialNoMass(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Meter_Custom) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Meter_CustomDecrement) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::OtherEquipment) )
    {
      modelObject = translateOtherEquipment(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::OutdoorAir_Mixer) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::OutdoorAir_Node) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::OutdoorAir_NodeList) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_IlluminanceMap) )
    {
      modelObject = translateOutputIlluminanceMap(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Meter) )
    {
      modelObject = translateOutputMeter(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Meter_Cumulative) )
    {
      modelObject = translateOutputMeterCumulative(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Meter_Cumulative_MeterFileOnly) )
    {
      modelObject = translateOutputMeterCumulativeMeterFileOnly(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Meter_MeterFileOnly) )
    {
      modelObject = translateOutputMeterMeterFileOnly(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_SQLite) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Table_Monthly) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Table_SummaryReports) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Output_Variable) )
    {
      modelObject = translateOutputVariable(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::OutputControl_Table_Style) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::People) )
    {
      modelObject = translatePeople(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::RunPeriod) )
    {
      modelObject = translateRunPeriod(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::RunPeriodControl_DaylightSavingTime) )
    {
      //modelObject = translateRunPeriodControlDaylightSavingTime(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::RunPeriodControl_SpecialDays) )
    {
      //modelObject = translateRunPeriodControlSpecialDays(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Schedule_Compact) )
    {
      modelObject = translateScheduleCompact(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Schedule_Constant) )
    {
      modelObject = translateScheduleConstant(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Schedule_Day_Hourly) )
    {
      modelObject = translateScheduleDayHourly(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Schedule_Day_Interval) )
    {
      modelObject = translateScheduleDayInterval(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ScheduleTypeLimits) )
    {
      modelObject = translateScheduleTypeLimits(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Schedule_Week_Daily) )
    {
      modelObject = translateScheduleWeekDaily(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Schedule_Year) )
    {
      modelObject = translateScheduleYear(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SetpointManager_MixedAir) )
    {
      //modelObject = translateSetpointManagerMixedAir(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SetpointManager_Scheduled) )
    {
      //modelObject = translateSetpointManagerScheduled(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SetpointManager_SingleZone_Reheat) )
    {
      //modelObject = translateSetpointManagerSingleZoneReheat(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Shading_Building_Detailed) )
    {
      modelObject = translateShadingBuildingDetailed(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Shading_Site_Detailed) )
    {
      modelObject = translateShadingSiteDetailed(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Shading_Zone_Detailed) )
    {
      modelObject = translateShadingZoneDetailed(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ShadowCalculation) )
    {
      modelObject = translateShadowCalculation(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SimulationControl) )
    {
      modelObject = translateSimulationControl(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Site_Location) )
    {
      modelObject = translateSiteLocation(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Site_GroundReflectance) )
    {
      //modelObject = translateSiteGroundReflectance(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Site_GroundTemperature_BuildingSurface) )
    {
      //modelObject = translateSiteGroundTemperatureBuildingSurface(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Site_WaterMainsTemperature) )
    {
      //modelObject = translateSiteWaterMainsTemperature(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Sizing_Parameters) )
    {
      modelObject = translateSizingParameters(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SizingPeriod_DesignDay) )
    {
      modelObject = translateSizingPeriodDesignDay(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Sizing_System) )
    {
      //modelObject = translateSizingSystem(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Sizing_Zone) )
    {
      //modelObject = translateSizingZone(workspaceObject );
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SteamEquipment) )
    {
      modelObject = translateSteamEquipment(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SurfaceConvectionAlgorithm_Inside) )
    {
      //modelObject = translateSurfaceConvectionAlgorithmInside(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::SurfaceConvectionAlgorithm_Outside) )
    {
      //modelObject = translateSurfaceConvectionAlgorithmOutside(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ThermostatSetpoint_DualSetpoint) )
    {
      modelObject = translateThermostatSetpointDualSetpoint(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Timestep) )
    {
      modelObject = translateTimestep(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::UtilityCost_Charge_Simple) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::UtilityCost_Qualify) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Version) )
   {
     modelObject = translateVersion(workspaceObject );
   }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::WindowMaterial_Gas) )
    {
      modelObject = translateWindowMaterialGas(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::WindowMaterial_Glazing) )
    {
      modelObject = translateWindowMaterialGlazing(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::WindowMaterial_SimpleGlazingSystem) )
    {
      modelObject = translateWindowMaterialSimpleGlazingSystem(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::Zone) )
    {
      modelObject = translateZone(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneAirHeatBalanceAlgorithm) )
    {
      //modelObject = translateZoneAirHeatBalanceAlgorithm(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneControl_Thermostat) )
    {
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneHVAC_EquipmentList) )
    {
      //modelObject = translateZoneHVACEquipmentList(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneHVAC_IdealLoadsAirSystem) )
    {
      //modelObject = translateZoneHVACIdealLoadsAirSystem(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneInfiltration_DesignFlowRate) )
    {
      modelObject = translateZoneInfiltrationDesignFlowRate(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneInfiltration_EffectiveLeakageArea) )
  {
      modelObject = translateZoneInfiltrationEffectiveLeakageArea(workspaceObject);
  }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneList) )
    {
      modelObject = translateZone(workspaceObject);
    }
  else if( workspaceObject.iddObject().type() == IddObjectType(iddobjectname::ZoneVentilation_DesignFlowRate) )
    {
      modelObject = translateZoneVentilationDesignFlowRate(workspaceObject);
    }
  else
    {
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

