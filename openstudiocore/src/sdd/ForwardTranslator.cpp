/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/ClimateZones.hpp"
#include "../model/ClimateZones_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/CoilHeatingGasMultiStageStageData.hpp"
#include "../model/Connection.hpp"
#include "../model/CoolingTowerPerformanceCoolTools.hpp"
#include "../model/StandardsInformationMaterial.hpp"
#include "../model/StandardsInformationConstruction.hpp"
#include "../model/PortList.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ConvergenceLimits.hpp"
#include "../model/CoolingTowerPerformanceYorkCalc.hpp"
#include "../model/CurveBicubic.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/CurveCubic.hpp"
#include "../model/CurveDoubleExponentialDecay.hpp"
#include "../model/CurveExponent.hpp"
#include "../model/CurveExponentialDecay.hpp"
#include "../model/CurveExponentialSkewNormal.hpp"
#include "../model/CurveFanPressureRise.hpp"
#include "../model/CurveFunctionalPressureDrop.hpp"
#include "../model/CurveLinear.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/CurveQuadraticLinear.hpp"
#include "../model/CurveQuartic.hpp"
#include "../model/CurveRectangularHyperbola1.hpp"
#include "../model/CurveRectangularHyperbola2.hpp"
#include "../model/CurveSigmoid.hpp"
#include "../model/CurveTriquadratic.hpp"
#include "../model/TableMultiVariableLookup.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultSurfaceConstructions.hpp"
#include "../model/DefaultSubSurfaceConstructions.hpp"
#include "../model/DesignSpecificationZoneAirDistribution.hpp"
#include "../model/HeatBalanceAlgorithm.hpp"
#include "../model/InsideSurfaceConvectionAlgorithm.hpp"
#include "../model/LifeCycleCost.hpp"
#include "../model/LifeCycleCostParameters.hpp"
#include "../model/LightingDesignDay.hpp"
#include "../model/LightingSimulationControl.hpp"
#include "../model/OutputMeter.hpp"
#include "../model/ModelObjectList.hpp"
#include "../model/OutputControlReportingTolerances.hpp"
#include "../model/OutputVariable.hpp"
#include "../model/OutsideSurfaceConvectionAlgorithm.hpp"
#include "../model/CoilCoolingDXMultiSpeedStageData.hpp"
#include "../model/ComponentCostAdjustments.hpp"
#include "../model/CurrencyType.hpp"
#include "../model/ControllerWaterCoil.hpp"
#include "../model/SetpointManagerMixedAir.hpp"
#include "../model/BoilerSteam.hpp"
#include "../model/Node.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/ScheduleCompact.hpp"
#include "../model/ScheduleConstant.hpp"
#include "../model/ScheduleFixedInterval.hpp"
#include "../model/ScheduleVariableInterval.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleYear.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleWeek.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/DesignDay.hpp"
#include "../model/WeatherFileConditionType.hpp"
#include "../model/WeatherFileDays.hpp"
#include "../model/UtilityCost_Charge_Block.hpp"
#include "../model/UtilityCost_Charge_Simple.hpp"
#include "../model/UtilityCost_Computation.hpp"
#include "../model/UtilityCost_Qualify.hpp"
#include "../model/UtilityCost_Ratchet.hpp"
#include "../model/UtilityCost_Tariff.hpp"
#include "../model/UtilityCost_Variable.hpp"
#include "../model/ProgramControl.hpp"
#include "../model/RadianceParameters.hpp"
#include "../model/ShadowCalculation.hpp"
#include "../model/SiteGroundReflectance.hpp"
#include "../model/SiteGroundTemperatureBuildingSurface.hpp"
#include "../model/SiteWaterMainsTemperature.hpp"
#include "../model/SizingParameters.hpp"
#include "../model/SkyTemperature.hpp"
#include "../model/IlluminanceMap.hpp"
#include "../model/InternalMass.hpp"
#include "../model/Timestep.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/Version.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/YearDescription.hpp"
#include "../model/ZoneAirContaminantBalance.hpp"
#include "../model/ZoneAirHeatBalanceAlgorithm.hpp"
#include "../model/ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../model/ZoneHVACEquipmentList.hpp"

#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QThread>

#include <algorithm>

namespace openstudio {
namespace sdd {

  ForwardTranslator::ForwardTranslator()
    : m_autoHardSize(false),
      m_autoEfficiency(false)
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.sdd\\.ForwardTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  ForwardTranslator::~ForwardTranslator()
  {
  }

  bool ForwardTranslator::modelToSDD(const openstudio::model::Model& model, const openstudio::path& path, ProgressBar* progressBar)
  {
    m_progressBar = progressBar;
    m_translatedObjects.clear();
    m_ignoreTypes.clear();
    m_ignoreObjects.clear();

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    model::Model modelCopy = model.clone().cast<model::Model>();

    // remove unused resource objects
    modelCopy.purgeUnusedResourceObjects();

    boost::optional<QDomDocument> doc = this->translateModel(modelCopy);
    logUntranslatedObjects(modelCopy);
    if (!doc){
      return false;
    }

    if (exists(path)){
      remove(path);
    }

    if (!exists(path.parent_path())){
      create_directory(path.parent_path());
    }

    QFile file(toQString(path));
    if (file.open(QFile::WriteOnly)){
      QTextStream textStream(&file);
      textStream.setCodec("UTF-8");
      textStream << doc->toString(2);
      file.close();
      return true;
    }

    return false;
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  QString ForwardTranslator::escapeName(const std::string& name)
  {
    return toQString(name);
  }

  boost::optional<QDomDocument> ForwardTranslator::translateModel(const openstudio::model::Model& model)
  {
    QDomDocument doc;
    doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    QDomElement sddElement = doc.createElement("SDDXML");
    sddElement.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    sddElement.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    doc.appendChild(sddElement);

    // set ruleset, where should this data come from?
    QDomElement rulesetFilenameElement = doc.createElement("RulesetFilename");
    sddElement.appendChild(rulesetFilenameElement);
    rulesetFilenameElement.setAttribute("file", "CEC 2013 NonRes.bin"); // DLM: only allow one value for now

    // set project, where should this data come from?
    QDomElement projectElement = doc.createElement("Proj");
    sddElement.appendChild(projectElement);

    // DLM: what name to use here?
    QDomElement projectNameElement = doc.createElement("Name");
    projectElement.appendChild(projectNameElement);
    projectNameElement.appendChild(doc.createTextNode("unknown"));

    // site data
    boost::optional<model::ClimateZones> climateZones = model.getOptionalUniqueModelObject<model::ClimateZones>();
    if (climateZones){
      // todo: check document year
      std::vector<model::ClimateZone> zones = climateZones->getClimateZones("CEC");
      if (zones.size() > 0 && !zones[0].value().empty()){

        bool isNumber;
        QString value = toQString(zones[0].value());
        value.toInt(&isNumber);
        if (isNumber){
          value = QString("ClimateZone") + value;
        }

        QDomElement projectClimateZoneElement = doc.createElement("CliZn");
        projectElement.appendChild(projectClimateZoneElement);
        projectClimateZoneElement.appendChild(doc.createTextNode(value));

        m_translatedObjects[climateZones->handle()] = projectClimateZoneElement;
      }
    }

    // set lat, lon, elev
    // DLM: do not translate forward,  Issue 242: Forward Translator - Remove Proj:Lat/Lon/Elevation translation
    /*
    boost::optional<model::Site> site = model.getOptionalUniqueModelObject<model::Site>();
    if (site){
      double latitude = site->latitude();
      QDomElement latElement = doc.createElement("Lat");
      projectElement.appendChild(latElement);
      latElement.appendChild( doc.createTextNode(QString::number(latitude)));

      double longitude = site->longitude();
      QDomElement longElement = doc.createElement("Long");
      projectElement.appendChild(longElement);
      longElement.appendChild( doc.createTextNode(QString::number(longitude)));

      double elevationSI = site->elevation();
      double elevationIP = elevationSI/0.3048;
      QDomElement elevationElement = doc.createElement("Elevation");
      projectElement.appendChild(elevationElement);
      elevationElement.appendChild( doc.createTextNode(QString::number(elevationIP)));

      m_translatedObjects[site.handle()] = latElement;
    }
    */

    // todo: write out epw file path
    // todo: write out ddy file and set path
    //<DDWeatherFile>C:/svn-CBECC-Com-130521/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.ddy</DDWeatherFile>
    //<AnnualWeatherFile>C:/svn-CBECC-Com-130521/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.epw</AnnualWeatherFile>
    //<HVACAutoSizing>1</HVACAutoSizing>
    //<SimDsgnDays>1</SimDsgnDays>
    //<RunPeriodBeginMonth>0</RunPeriodBeginMonth>
    //<RunPeriodBeginDay>0</RunPeriodBeginDay>
    //<RunPeriodEndMonth>0</RunPeriodEndMonth>
    //<RunPeriodEndDay>0</RunPeriodEndDay>
    //<RunPeriodYear>0</RunPeriodYear>

    // do materials before constructions 
    std::vector<model::Material> materials = model.getModelObjects<model::Material>();
    std::sort(materials.begin(), materials.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)materials.size());
      m_progressBar->setValue(0);
    }

    for (const model::Material& material : materials){

      boost::optional<QDomElement> materialElement = translateMaterial(material, doc);
      if (materialElement){
        projectElement.appendChild(*materialElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }
  
    // do constructions before geometry

    std::vector<model::ConstructionBase> constructions = model.getModelObjects<model::ConstructionBase>();
    std::sort(constructions.begin(), constructions.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(3 * ((int)constructions.size())); // three loops below
      m_progressBar->setValue(0);
    }

    std::set<Handle> surfaceConstructions;
    for (const model::Surface& surface : model.getConcreteModelObjects<model::Surface>()){
      boost::optional<model::ConstructionBase> construction = surface.construction();
      if (construction){
        surfaceConstructions.insert(construction->handle());
      }
    }

    std::set<Handle> doorConstructions;
    std::set<Handle> fenestrationConstructions;
    for (const model::SubSurface& subSurface : model.getConcreteModelObjects<model::SubSurface>()){
      boost::optional<model::ConstructionBase> construction = subSurface.construction();
      if (construction){
        std::string subSurfaceType = subSurface.subSurfaceType();
        if (istringEqual("Door", subSurfaceType) || istringEqual("OverheadDoor", subSurfaceType)){
          doorConstructions.insert(construction->handle());
        }else{
          fenestrationConstructions.insert(construction->handle());
        }
      }
    }

    // translate surface constructions
    for (const model::ConstructionBase& constructionBase : constructions){
      if (surfaceConstructions.find(constructionBase.handle()) == surfaceConstructions.end()){
        continue;
      }

      boost::optional<QDomElement> constructionElement = translateConstructionBase(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }
            
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate door constructions
    for (const model::ConstructionBase& constructionBase : constructions){
      if (doorConstructions.find(constructionBase.handle()) == doorConstructions.end()){
        continue;
      }

      boost::optional<QDomElement> constructionElement = translateDoorConstruction(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }
            
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate fenestration constructions
    for (const model::ConstructionBase& constructionBase : constructions){
      if (fenestrationConstructions.find(constructionBase.handle()) == fenestrationConstructions.end()){
        continue;
      }

      boost::optional<QDomElement> constructionElement = translateFenestrationConstruction(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate site shading
    std::vector<model::ShadingSurfaceGroup> shadingSurfaceGroups = model.getConcreteModelObjects<model::ShadingSurfaceGroup>();
    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Site Shading"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)shadingSurfaceGroups.size()); 
      m_progressBar->setValue(0);
    }

    for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
      if (istringEqual(shadingSurfaceGroup.shadingSurfaceType(), "Site")){

        Transformation transformation = shadingSurfaceGroup.siteTransformation();

        for (const model::ShadingSurface& shadingSurface : shadingSurfaceGroup.shadingSurfaces()){
          boost::optional<QDomElement> shadingSurfaceElement = translateShadingSurface(shadingSurface, transformation, doc);
          if (shadingSurfaceElement){
            projectElement.appendChild(*shadingSurfaceElement);
          }
        }
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate the building
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
    if (building){
      boost::optional<QDomElement> buildingElement = translateBuilding(*building, doc);
      if (buildingElement){
        projectElement.appendChild(*buildingElement);
      }
    }

    m_ignoreTypes.push_back(model::BoilerSteam::iddObjectType());
    m_ignoreTypes.push_back(model::ClimateZones::iddObjectType()); // might not be translated but it is checked
    m_ignoreTypes.push_back(model::CoilCoolingDXMultiSpeedStageData::iddObjectType());
    m_ignoreTypes.push_back(model::CoilHeatingGasMultiStageStageData::iddObjectType());
    m_ignoreTypes.push_back(model::ComponentCostAdjustments::iddObjectType());
    m_ignoreTypes.push_back(model::ComponentData::iddObjectType());
    m_ignoreTypes.push_back(model::Connection::iddObjectType());
    m_ignoreTypes.push_back(model::ControllerWaterCoil::iddObjectType());
    m_ignoreTypes.push_back(model::ConvergenceLimits::iddObjectType());
    m_ignoreTypes.push_back(model::CoolingTowerPerformanceCoolTools::iddObjectType());
    m_ignoreTypes.push_back(model::CoolingTowerPerformanceYorkCalc::iddObjectType());
    m_ignoreTypes.push_back(model::CurrencyType::iddObjectType());
    m_ignoreTypes.push_back(model::CurveBicubic::iddObjectType());
    m_ignoreTypes.push_back(model::CurveBiquadratic::iddObjectType());
    m_ignoreTypes.push_back(model::CurveCubic::iddObjectType());
    m_ignoreTypes.push_back(model::CurveDoubleExponentialDecay::iddObjectType());
    m_ignoreTypes.push_back(model::CurveExponent::iddObjectType());
    m_ignoreTypes.push_back(model::CurveExponentialDecay::iddObjectType());
    m_ignoreTypes.push_back(model::CurveExponentialSkewNormal::iddObjectType());
    m_ignoreTypes.push_back(model::CurveFanPressureRise::iddObjectType());
    m_ignoreTypes.push_back(model::CurveFunctionalPressureDrop::iddObjectType());
    m_ignoreTypes.push_back(model::CurveLinear::iddObjectType());
    m_ignoreTypes.push_back(model::CurveQuadratic::iddObjectType());
    m_ignoreTypes.push_back(model::CurveQuadraticLinear::iddObjectType());
    m_ignoreTypes.push_back(model::CurveQuartic::iddObjectType());
    m_ignoreTypes.push_back(model::CurveRectangularHyperbola1::iddObjectType());
    m_ignoreTypes.push_back(model::CurveRectangularHyperbola2::iddObjectType());
    m_ignoreTypes.push_back(model::CurveSigmoid::iddObjectType());
    m_ignoreTypes.push_back(model::CurveTriquadratic::iddObjectType());
    m_ignoreTypes.push_back(model::DefaultConstructionSet::iddObjectType());
    m_ignoreTypes.push_back(model::DesignDay::iddObjectType());
    m_ignoreTypes.push_back(model::DefaultScheduleSet::iddObjectType());
    m_ignoreTypes.push_back(model::DesignSpecificationZoneAirDistribution::iddObjectType());
    m_ignoreTypes.push_back(model::DefaultSurfaceConstructions::iddObjectType());
    m_ignoreTypes.push_back(model::DefaultSubSurfaceConstructions::iddObjectType());
    m_ignoreTypes.push_back(model::Facility::iddObjectType());
    m_ignoreTypes.push_back(model::HeatBalanceAlgorithm::iddObjectType());
    m_ignoreTypes.push_back(model::IlluminanceMap::iddObjectType());
    m_ignoreTypes.push_back(model::InsideSurfaceConvectionAlgorithm::iddObjectType());
    m_ignoreTypes.push_back(model::InternalMass::iddObjectType());
    m_ignoreTypes.push_back(model::LifeCycleCost::iddObjectType());
    m_ignoreTypes.push_back(model::LifeCycleCostParameters::iddObjectType());
    m_ignoreTypes.push_back(model::LightingDesignDay::iddObjectType());
    m_ignoreTypes.push_back(model::LightingSimulationControl::iddObjectType());
    m_ignoreTypes.push_back(model::ModelObjectList::iddObjectType());
    m_ignoreTypes.push_back(model::Node::iddObjectType());
    m_ignoreTypes.push_back(model::OutputControlReportingTolerances::iddObjectType());
    m_ignoreTypes.push_back(model::OutputMeter::iddObjectType());
    m_ignoreTypes.push_back(model::OutputVariable::iddObjectType());
    m_ignoreTypes.push_back(model::OutsideSurfaceConvectionAlgorithm::iddObjectType());
    m_ignoreTypes.push_back(model::PortList::iddObjectType());
    m_ignoreTypes.push_back(model::ProgramControl::iddObjectType());
    m_ignoreTypes.push_back(model::RadianceParameters::iddObjectType());
    m_ignoreTypes.push_back(model::RenderingColor::iddObjectType());
    m_ignoreTypes.push_back(model::RunPeriod::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleCompact::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleConstant::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleDay::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleFixedInterval::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleRule::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleRuleset::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleTypeLimits::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleVariableInterval::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleWeek::iddObjectType());
    m_ignoreTypes.push_back(model::ScheduleYear::iddObjectType());
    m_ignoreTypes.push_back(model::SetpointManagerMixedAir::iddObjectType());
    m_ignoreTypes.push_back(model::ShadowCalculation::iddObjectType());
    m_ignoreTypes.push_back(model::SimulationControl::iddObjectType());
    m_ignoreTypes.push_back(model::Site::iddObjectType()); // might not be translated but it is checked
    m_ignoreTypes.push_back(model::SiteGroundReflectance::iddObjectType());
    m_ignoreTypes.push_back(model::SiteGroundTemperatureBuildingSurface::iddObjectType());
    m_ignoreTypes.push_back(model::SiteWaterMainsTemperature::iddObjectType());
    m_ignoreTypes.push_back(model::SizingParameters::iddObjectType());
    m_ignoreTypes.push_back(model::SkyTemperature::iddObjectType());
    m_ignoreTypes.push_back(model::StandardsInformationConstruction::iddObjectType());
    m_ignoreTypes.push_back(model::StandardsInformationMaterial::iddObjectType());
    m_ignoreTypes.push_back(model::TableMultiVariableLookup::iddObjectType());
    m_ignoreTypes.push_back(model::Timestep::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityBill::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Charge_Block::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Charge_Simple::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Computation::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Qualify::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Ratchet::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Tariff::iddObjectType());
    m_ignoreTypes.push_back(model::UtilityCost_Variable::iddObjectType());
    m_ignoreTypes.push_back(model::Version::iddObjectType());
    m_ignoreTypes.push_back(model::WeatherFile::iddObjectType());
    m_ignoreTypes.push_back(model::WeatherFileConditionType::iddObjectType());
    m_ignoreTypes.push_back(model::WeatherFileDays::iddObjectType());
    m_ignoreTypes.push_back(model::YearDescription::iddObjectType());
    m_ignoreTypes.push_back(model::ZoneAirContaminantBalance::iddObjectType());
    m_ignoreTypes.push_back(model::ZoneAirHeatBalanceAlgorithm::iddObjectType());
    m_ignoreTypes.push_back(model::ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
    m_ignoreTypes.push_back(model::ZoneHVACEquipmentList::iddObjectType());

    return doc;
  }

  void ForwardTranslator::logUntranslatedObjects(const model::Model& model)
  {
    auto allModelObjects = model.modelObjects();
    for(const auto & mo : allModelObjects) {
      // If mo is not in m_translatedObjects
      if(m_translatedObjects.find(mo.handle()) == m_translatedObjects.end()) {
        // If mo.iddObjectType is not int the m_ignoreTypes list
        if(std::find(m_ignoreTypes.begin(),m_ignoreTypes.end(),mo.iddObjectType()) == m_ignoreTypes.end()) {
          // If mo is not in the m_ignoreObjects list
          if(std::find(m_ignoreObjects.begin(),m_ignoreObjects.end(),mo.handle()) == m_ignoreObjects.end()) {
            LOG(Error,mo.briefDescription() << " was not translated.");
          } 
        }
      }
    }
  }

} // sdd
} // openstudio
