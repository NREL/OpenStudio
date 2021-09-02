/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ReverseTranslator.hpp"
#include "Helpers.hpp"

#include "../model/Model.hpp"
#include "../model/Component.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Node.hpp"
#include "../model/Node_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../model/AvailabilityManagerOptimumStart.hpp"
#include "../model/AvailabilityManagerOptimumStart_Impl.hpp"
#include "../model/AvailabilityManagerNightCycle.hpp"
#include "../model/AvailabilityManagerNightCycle_Impl.hpp"
#include "../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/FanZoneExhaust.hpp"
#include "../model/FanZoneExhaust_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/Timestep.hpp"
#include "../model/Timestep_Impl.hpp"
#include "../model/OutputVariable.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/SimulationControl_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"
#include "../model/OutputControlReportingTolerances.hpp"
#include "../model/OutputControlReportingTolerances_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ChillerElectricEIR.hpp"
#include "../model/ChillerElectricEIR_Impl.hpp"
#include "../model/CoolingTowerSingleSpeed.hpp"
#include "../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../model/BoilerHotWater.hpp"
#include "../model/BoilerHotWater_Impl.hpp"
#include "../model/SizingParameters.hpp"
#include "../model/SizingParameters_Impl.hpp"
#include "../model/SiteWaterMainsTemperature.hpp"
#include "../model/SiteWaterMainsTemperature_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/Splitter.hpp"
#include "../model/Splitter_Impl.hpp"
#include "../model/Mixer.hpp"
#include "../model/Mixer_Impl.hpp"
#include "../model/OutputMeter.hpp"
#include "../model/OutputMeter_Impl.hpp"
#include "../model/WaterToWaterComponent.hpp"
#include "../model/WaterToWaterComponent_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"
#include "../model/PortList.hpp"
#include "../model/PortList_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../model/ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../model/ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../model/ZoneHVACBaseboardConvectiveElectric_Impl.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater_Impl.hpp"
#include "../energyplus/ReverseTranslator.hpp"
#include "../osversion/VersionTranslator.hpp"
#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/CFMUnit.hpp"
#include "../utilities/units/FahrenheitUnit.hpp"
#include "../utilities/units/MPHUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/Unit.hpp"

#include <pugixml.hpp>

namespace openstudio {
namespace sdd {

  std::ostream& operator<<(std::ostream& os, const pugi::xml_node& element) {
    pugi::xml_document doc;
    doc.append_copy(element);
    doc.save(os, "  ");
    return os;
  }

  ReverseTranslator::ReverseTranslator(bool masterAutosize) : m_isInputXML(false), m_autosize(true), m_masterAutosize(masterAutosize) {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.sdd\\.ReverseTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  ReverseTranslator::~ReverseTranslator() {}

  boost::optional<openstudio::model::Model> ReverseTranslator::loadModel(const openstudio::path& path, ProgressBar* progressBar) {

    m_path = path;

    m_progressBar = progressBar;

    m_logSink.setThreadId(std::this_thread::get_id());

    m_logSink.resetStringStream();

    boost::optional<openstudio::model::Model> result;

    if (openstudio::filesystem::exists(path)) {
      openstudio::filesystem::ifstream file(path, std::ios_base::binary);
      if (file.is_open()) {
        pugi::xml_document doc;
        pugi::xml_parse_result success = doc.load(file);
        file.close();
        if (success) {
          result = this->convert(doc.document_element());
        } else {
          LOG(Error, "Could not open file '" << toString(path) << "'");
        }
      } else {
        LOG(Error, "Could not open file '" << toString(path) << "'");
      }
    } else {
      LOG(Error, "File '" << toString(path) << "' does not exist");
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::warnings() const {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::errors() const {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::string ReverseTranslator::escapeName(std::string name) {
    std::replace(name.begin(), name.end(), ',', '-');
    std::replace(name.begin(), name.end(), ';', '-');
    return name;
  }

  boost::optional<model::Model> ReverseTranslator::convert(const pugi::xml_node& root) {
    return translateSDD(root);
  }

  boost::optional<model::Model> ReverseTranslator::translateSDD(const pugi::xml_node& root) {
    boost::optional<model::Model> result;

    // get project, assume one project per file
    pugi::xml_node projectElement = root.child("Proj");
    if (!projectElement) {
      LOG(Error, "Could not find required element 'Proj'");
      return boost::none;
    }

    // check if this is a simulation xml or input xml
    pugi::xml_node simFlagElement = projectElement.child("SimFlag");
    if (!simFlagElement) {
      m_isInputXML = true;
      LOG(Error, "Import of Input SDD XML type is not currently supported");
      return boost::none;
    } else {
      m_isInputXML = false;
    }

    result = openstudio::model::Model();
    result->setFastNaming(true);

    // do runperiod
    boost::optional<model::ModelObject> runPeriod = translateRunPeriod(projectElement, *result);
    //if (!runPeriod) {
    //  LOG(Error, "Could not translate RunPeriod");
    //}

    // do design days
    std::vector<WorkspaceObject> designDays = translateDesignDays(projectElement, *result);
    //if (designDays.empty()) {
    //  LOG(Error, "Empty DesignDays");
    //}

    // do weather file
    boost::optional<model::ModelObject> weatherFile = translateWeatherFile(projectElement, *result);
    //if (!weatherFile) {
    //  LOG(Error, "Could not translate WeatherFile");
    //}

    // do site after design days and weather file
    boost::optional<model::ModelObject> site = translateSite(projectElement, *result);
    if (!site) {
      LOG(Error, "Could not find site information in SDD");
    }

    // Shading Model
    std::string solDistribution = projectElement.child("SolDistribution").text().as_string();
    if (istringEqual("FullExterior", solDistribution)) {
      model::SimulationControl simulationControl = result->getUniqueModelObject<model::SimulationControl>();
      simulationControl.setSolarDistribution("FullExterior");
    } else if (istringEqual("MinimalShadowing", solDistribution)) {
      model::SimulationControl simulationControl = result->getUniqueModelObject<model::SimulationControl>();
      simulationControl.setSolarDistribution("MinimalShadowing");
    }

    // HVACAutoSizing
    pugi::xml_node hvacAutoSizingElement = projectElement.child("HVACAutoSizing");
    if (hvacAutoSizingElement.text().as_int() == 0) {
      m_autosize = false;
    }

    model::SizingParameters sp = result->getUniqueModelObject<model::SizingParameters>();
    sp.setHeatingSizingFactor(1.0);
    sp.setCoolingSizingFactor(1.0);

    // do materials before constructions
    std::vector<pugi::xml_node> materialElements = makeVectorOfChildren(projectElement, "Mat");

    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(materialElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < materialElements.size(); ++i) {
      boost::optional<model::ModelObject> material = translateMaterial(materialElements[i], *result);
      if (!material) {
        LOG(Error, "Failed to translate 'Mat' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do constructions before geometry

    // layered constructions
    std::vector<pugi::xml_node> constructionElements = makeVectorOfChildren(projectElement, "ConsAssm");

    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(constructionElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < constructionElements.size(); ++i) {
      boost::optional<model::ModelObject> construction = translateConstructAssembly(constructionElements[i], *result);
      if (!construction) {
        LOG(Error, "Failed to translate 'ConsAssm' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // door constructions
    std::vector<pugi::xml_node> doorConstructionElements = makeVectorOfChildren(projectElement, "DrCons");

    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Door Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(doorConstructionElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < doorConstructionElements.size(); ++i) {
      boost::optional<model::ModelObject> doorConstruction = translateDoorConstruction(doorConstructionElements[i], *result);
      if (!doorConstruction) {
        LOG(Error, "Failed to translate 'DrCons' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // fenestration constructions
    std::vector<pugi::xml_node> fenestrationConstructionElements = makeVectorOfChildren(projectElement, "FenCons");

    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Fenestration Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(fenestrationConstructionElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < fenestrationConstructionElements.size(); ++i) {
      boost::optional<model::ModelObject> fenestrationConstruction = translateFenestrationConstruction(fenestrationConstructionElements[i], *result);
      if (!fenestrationConstruction) {
        LOG(Error, "Failed to translate 'FenCons' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    int i = 0;
    for (const pugi::xml_node& crvDblQuadElement : projectElement.children("CrvDblQuad")) {
      boost::optional<model::ModelObject> curve = translateCrvDblQuad(crvDblQuadElement, *result);
      if (!curve) {
        LOG(Error, "Failed to translate 'CrvDblQuad' element " << i);
      }
    }

    i = 0;
    for (const pugi::xml_node& crvCubicElement : projectElement.children("CrvCubic")) {
      boost::optional<model::ModelObject> curve = translateCrvCubic(crvCubicElement, *result);
      if (!curve) {
        LOG(Error, "Failed to translate 'CrvCubic' element " << i);
      }
    }

    i = 0;
    for (const pugi::xml_node& crvQuadElement : projectElement.children("CrvQuad")) {
      boost::optional<model::ModelObject> curve = translateCrvQuad(crvQuadElement, *result);
      if (!curve) {
        LOG(Error, "Failed to translate 'CrvQuad' element " << i << " named '" << crvQuadElement.child("Name").text().as_string() << "'.");
      }
    }

    i = 0;
    for (const pugi::xml_node& crvLinElement : projectElement.children("CrvLin")) {
      boost::optional<model::ModelObject> curve = translateCrvLin(crvLinElement, *result);
      if (!curve) {
        LOG(Error, "Failed to translate 'CrvLin' element " << i);
      }
    }

    i = 0;
    for (const pugi::xml_node& crvMapSglVarElement : projectElement.children("CrvMapSglVar")) {
      boost::optional<model::ModelObject> curve = translateCrvMapSglVar(crvMapSglVarElement, *result);
      if (!curve) {
        LOG(Error, "Failed to translate 'CrvMapSglVar' element " << i);
      }
    }

    i = 0;
    for (const pugi::xml_node& crvMapDblVarElement : projectElement.children("CrvMapDblVar")) {
      boost::optional<model::ModelObject> curve = translateCrvMapDblVar(crvMapDblVarElement, *result);
      if (!curve) {
        LOG(Error, "Failed to translate 'CrvMapDblVar' element " << i);
      }
    }

    // do schedules before loads
    std::vector<pugi::xml_node> scheduleDayElements = makeVectorOfChildren(projectElement, "SchDay");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Day Schedules"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(scheduleDayElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < scheduleDayElements.size(); ++i) {
      boost::optional<model::ModelObject> scheduleDay = translateScheduleDay(scheduleDayElements[i], *result);
      if (!scheduleDay) {
        LOG(Error, "Failed to translate 'SchDay' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    std::vector<pugi::xml_node> scheduleWeekElements = makeVectorOfChildren(projectElement, "SchWeek");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Week Schedules"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(scheduleWeekElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < scheduleWeekElements.size(); ++i) {
      boost::optional<model::ModelObject> scheduleWeek = translateScheduleWeek(scheduleWeekElements[i], *result);
      if (!scheduleWeek) {
        LOG(Error, "Failed to translate 'SchWeek' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    std::vector<pugi::xml_node> scheduleElements = makeVectorOfChildren(projectElement, "Sch");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Year Schedules"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(scheduleElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < scheduleElements.size(); ++i) {
      boost::optional<model::ModelObject> schedule = translateSchedule(scheduleElements[i], *result);
      if (!schedule) {
        LOG(Error, "Failed to translate 'Sch' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    std::vector<pugi::xml_node> holidayElements = makeVectorOfChildren(projectElement, "Hol");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Holidays"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(holidayElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < holidayElements.size(); ++i) {
      boost::optional<model::ModelObject> holiday = translateHoliday(holidayElements[i], *result);
      if (!holiday) {
        LOG(Error, "Failed to translate 'Hol' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do water mains temperatures, do after schedules
    boost::optional<model::ModelObject> waterMainsTemperature = translateWaterMainsTemperature(projectElement, *result);
    //if (!waterMainsTemperature){
    //  LOG(Error, "Could not translate WaterMainsTemperature");
    //}

    // translate shadingSurfaces
    std::vector<pugi::xml_node> exteriorShadingElements = makeVectorOfChildren(projectElement, "ExtShdgObj");
    if (exteriorShadingElements.size() > 0) {
      model::ShadingSurfaceGroup shadingSurfaceGroup(*result);
      shadingSurfaceGroup.setName("Site ShadingGroup");
      shadingSurfaceGroup.setShadingSurfaceType("Site");
      for (std::vector<pugi::xml_node>::size_type i = 0; i < exteriorShadingElements.size(); ++i) {
        if (exteriorShadingElements[i].parent() == projectElement) {
          boost::optional<model::ModelObject> exteriorShading = translateShadingSurface(exteriorShadingElements[i], shadingSurfaceGroup);
          if (!exteriorShading) {
            LOG(Error, "Failed to translate 'ExtShdgObj' element " << i);
          }
        }
      }
    }

    openstudio::model::Facility facility = result->getUniqueModelObject<openstudio::model::Facility>();

    // translate the building
    pugi::xml_node buildingElement = projectElement.child("Bldg");
    if (!buildingElement) {
      LOG(Error, "Required 'Bldg' element is Null");
    } else {
      boost::optional<model::ModelObject> building = translateBuilding(buildingElement, *result);
      if (!building) {
        LOG(Error, "Failed to translate 'Bldg'");
      }
    }

    result->setFastNaming(false);

    // FluidSys
    std::vector<pugi::xml_node> fluidSysElements = makeVectorOfChildren(projectElement, "FluidSys");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Fluid Systems"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(fluidSysElements.size());
      m_progressBar->setValue(0);
    }

    // Translate condenser systems
    for (std::vector<pugi::xml_node>::size_type i = 0; i < fluidSysElements.size(); ++i) {
      if (!fluidSysElements[i].child("Name")) {
        continue;
      }
      if (!openstudio::istringEqual(fluidSysElements[i].child("Type").text().as_string(), "condenserwater")) {
        continue;
      }
      boost::optional<model::ModelObject> plantLoop = translateFluidSys(fluidSysElements[i], *result);
      OS_ASSERT(plantLoop);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // Translate hot water systems
    for (std::vector<pugi::xml_node>::size_type i = 0; i < fluidSysElements.size(); ++i) {
      if (!fluidSysElements[i].child("Name")) {
        continue;
      }
      if (!openstudio::istringEqual(fluidSysElements[i].child("Type").text().as_string(), "hotwater")) {
        continue;
      }

      boost::optional<model::ModelObject> plantLoop = translateFluidSys(fluidSysElements[i], *result);
      OS_ASSERT(plantLoop);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // Translate chilled water systems
    for (std::vector<pugi::xml_node>::size_type i = 0; i < fluidSysElements.size(); ++i) {
      if (!fluidSysElements[i].child("Name")) {
        continue;
      }
      if (!openstudio::istringEqual(fluidSysElements[i].child("Type").text().as_string(), "chilledwater")) {
        continue;
      }

      boost::optional<model::ModelObject> plantLoop = translateFluidSys(fluidSysElements[i], *result);
      OS_ASSERT(plantLoop);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // We're left with 'servicehotwater' type

    // AirSystem
    std::vector<pugi::xml_node> airSystemElements = makeVectorOfChildren(buildingElement, "AirSys");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Air Systems"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(airSystemElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < airSystemElements.size(); ++i) {
      if (!airSystemElements[i].child("Name")) {
        continue;
      }
      // TODO: No check on whether that worked or not?
      translateAirSystem(airSystemElements[i], *result);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // VRFSys
    std::vector<pugi::xml_node> vrfSystemElements = makeVectorOfChildren(buildingElement, "VRFSys");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating VRF Systems"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(vrfSystemElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < vrfSystemElements.size(); ++i) {
      if (!vrfSystemElements[i].child("Name")) {
        continue;
      }
      // TODO: No check on whether that worked or not?
      translateVRFSys(vrfSystemElements[i], *result);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // ThermalZone
    std::vector<pugi::xml_node> thermalZoneElements = makeVectorOfChildren(buildingElement, "ThrmlZn");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(airSystemElements.size());
      m_progressBar->setValue(0);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < thermalZoneElements.size(); ++i) {
      if (!thermalZoneElements[i].child("Name")) {
        continue;
      }

      boost::optional<model::ModelObject> thermalZone = translateThermalZone(thermalZoneElements[i], *result);
      if (!thermalZone) {
        LOG(Error, "Failed to translate 'ThrmlZn' element " << i);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // Give the nodes better names
    // We do this here because the loops need to be completely assembled
    // with their supply AND demand sides.  ie. After zones are attached.
    std::vector<model::PlantLoop> plantLoops = result->getModelObjects<model::PlantLoop>();

    for (auto plantLoop = plantLoops.begin(); plantLoop != plantLoops.end(); ++plantLoop) {
      std::string plantName = plantLoop->name().get();
      plantLoop->supplyInletNode().setName(plantName + " Supply Inlet Node");
      plantLoop->demandInletNode().setName(plantName + " Demand Inlet Node");
      plantLoop->demandSplitter().setName(plantName + " Demand Splitter");
      plantLoop->demandMixer().setName(plantName + " Demand Mixer");
      plantLoop->supplySplitter().setName(plantName + " Supply Splitter");
      plantLoop->supplyMixer().setName(plantName + " Supply Mixer");

      std::vector<model::ModelObject> comps = plantLoop->components();
      for (auto it = comps.begin(); it != comps.end(); ++it) {
        if (!it->optionalCast<model::Node>()) {
          if (boost::optional<model::StraightComponent> comp = it->optionalCast<model::StraightComponent>()) {
            if (boost::optional<model::ModelObject> mo = comp->outletModelObject()) {
              mo->setName(comp->name().get() + " Outlet Node");
            }
          } else if (boost::optional<model::WaterToAirComponent> comp = it->optionalCast<model::WaterToAirComponent>()) {
            if (boost::optional<model::ModelObject> mo = comp->waterOutletModelObject()) {
              mo->setName(comp->name().get() + " Water Outlet Node");
            }
          } else if (boost::optional<model::WaterToWaterComponent> comp = it->optionalCast<model::WaterToWaterComponent>()) {
            if (boost::optional<model::ModelObject> mo = comp->supplyOutletModelObject()) {
              mo->setName(comp->name().get() + " Supply Outlet Node");
            }
            if (boost::optional<model::ModelObject> mo = comp->demandOutletModelObject()) {
              mo->setName(comp->name().get() + " Demand Outlet Node");
            }
            if (boost::optional<model::ModelObject> mo = comp->tertiaryOutletModelObject()) {
              mo->setName(comp->name().get() + " Tertiary Demand Outlet Node");
            }
          } else if (boost::optional<model::Splitter> comp = it->optionalCast<model::Splitter>()) {
            int branchI = 1;
            std::vector<model::ModelObject> splitterOutletObjects = comp->outletModelObjects();
            for (auto it = splitterOutletObjects.begin(); it != splitterOutletObjects.end(); ++it) {
              if (it->optionalCast<model::Node>()) {
                std::string branchOutuletName = comp->name().get() + " Outlet Node " + boost::lexical_cast<std::string>(branchI);
                it->setName(branchOutuletName);
                ++branchI;
              }
            }
          } else if (boost::optional<model::Mixer> comp = it->optionalCast<model::Mixer>()) {
            if (boost::optional<model::ModelObject> mixerOutlet = comp->outletModelObject()) {
              mixerOutlet->setName(comp->name().get() + " Outlet Node");
            }
          }
        }
      }
    }

    std::vector<model::AirLoopHVAC> airSystems = result->getModelObjects<model::AirLoopHVAC>();

    for (auto airSystem = airSystems.begin(); airSystem != airSystems.end(); ++airSystem) {
      std::string systemName = airSystem->name().get();
      airSystem->supplyInletNode().setName(systemName + " Supply Side (Return Air) Inlet Node");
      airSystem->demandInletNode().setName(systemName + " Demand Side (Supply Air) Inlet Node");
      airSystem->demandSplitter().setName(systemName + " Zone Splitter");
      airSystem->demandMixer().setName(systemName + " Zone Mixer");

      std::vector<model::ModelObject> comps = airSystem->components();
      for (auto it = comps.begin(); it != comps.end(); ++it) {
        if (!it->optionalCast<model::Node>()) {
          if (boost::optional<model::StraightComponent> comp = it->optionalCast<model::StraightComponent>()) {
            if (boost::optional<model::ModelObject> mo = comp->outletModelObject()) {
              mo->setName(comp->name().get() + " Outlet Node");
            }
          } else if (boost::optional<model::WaterToAirComponent> comp = it->optionalCast<model::WaterToAirComponent>()) {
            if (boost::optional<model::ModelObject> mo = comp->airOutletModelObject()) {
              mo->setName(comp->name().get() + " Air Outlet Node");
            }
          } else if (boost::optional<model::Splitter> comp = it->optionalCast<model::Splitter>()) {
            int branchI = 1;
            std::vector<model::ModelObject> splitterOutletObjects = comp->outletModelObjects();
            for (auto it = splitterOutletObjects.begin(); it != splitterOutletObjects.end(); ++it) {
              if (it->optionalCast<model::Node>()) {
                std::string branchOutuletName = comp->name().get() + " Outlet Node " + boost::lexical_cast<std::string>(branchI);
                it->setName(branchOutuletName);
                ++branchI;
              }
            }
          } else if (boost::optional<model::ThermalZone> comp = it->optionalCast<model::ThermalZone>()) {
            if (boost::optional<model::ModelObject> returnAir = comp->returnAirModelObject()) {
              returnAir->setName(comp->name().get() + " Return Air Node");
            }
          } else if (boost::optional<model::AirLoopHVACOutdoorAirSystem> comp = it->optionalCast<model::AirLoopHVACOutdoorAirSystem>()) {
            if (boost::optional<model::ModelObject> mixedAir = comp->mixedAirModelObject()) {
              mixedAir->setName(comp->name().get() + " Mixed Air Node");
            }

            auto oaNode = comp->outboardOANode();
            auto reliefNode = comp->outboardReliefNode();
            if (oaNode) {
              oaNode->setName(comp->name().get() + " OA Node");
            }
            if (reliefNode) {
              reliefNode->setName(comp->name().get() + " Relief Node");
            }

            {
              auto oaNodes = subsetCastVector<model::Node>(comp->oaComponents());
              for (auto& node : oaNodes) {
                if (oaNode && (node != oaNode.get())) {
                  if (const auto& outletComp = node.inletModelObject()) {
                    node.setName(outletComp->nameString() + " OA Stream Outlet Node");
                  }
                }
              }
            }

            {
              auto reliefNodes = subsetCastVector<model::Node>(comp->reliefComponents());
              for (auto& node : reliefNodes) {
                if (reliefNode && (node != reliefNode.get())) {
                  if (const auto& outletComp = node.inletModelObject()) {
                    node.setName(outletComp->nameString() + " Relief Stream Outlet Node");
                  }
                }
              }
            }
          } else if (boost::optional<model::Mixer> comp = it->optionalCast<model::Mixer>()) {
            if (boost::optional<model::ModelObject> mixerOutlet = comp->outletModelObject()) {
              mixerOutlet->setName(comp->name().get() + " Outlet Node");
            }
          }
        }
      }
    }

    std::vector<model::ZoneHVACComponent> zoneEquipment = result->getModelObjects<model::ZoneHVACComponent>();
    for (auto zoneComp = zoneEquipment.begin(); zoneComp != zoneEquipment.end(); ++zoneComp) {
      if (boost::optional<model::Node> inlet = zoneComp->inletNode()) {
        inlet->setName(zoneComp->name().get() + " Inlet Node");
      }
      if (boost::optional<model::Node> outlet = zoneComp->outletNode()) {
        outlet->setName(zoneComp->name().get() + " Outlet Node");
      }
    }

    // timestep
    pugi::xml_node numTimeStepsPerHrElement = projectElement.child("NumTimeStepsPerHr");
    boost::optional<int> _numTimeStepsPerHr = lexicalCastToInt(numTimeStepsPerHrElement);
    if (_numTimeStepsPerHr) {
      model::Timestep timestep = result->getUniqueModelObject<model::Timestep>();
      timestep.setNumberOfTimestepsPerHour(_numTimeStepsPerHr.get());
    } else {
      LOG(Warn, "Cannot cast NumTimeStepsPerHr to an integer, NumTimeStepsPerHr: [" << numTimeStepsPerHrElement.text().as_string()
                                                                                    << "]. Defaulting to 4/hr.");
      model::Timestep timestep = result->getUniqueModelObject<model::Timestep>();
      timestep.setNumberOfTimestepsPerHour(4);
    }

    // request output meters for TDV calculations
    std::set<int> fuelTypes = FuelType::getValues();
    for (int fuelType : fuelTypes) {

      if (fuelType == FuelType::Gasoline || fuelType == FuelType::Diesel || fuelType == FuelType::FuelOil_1 || fuelType == FuelType::FuelOil_2
          || fuelType == FuelType::Propane || fuelType == FuelType::Water || fuelType == FuelType::Steam || fuelType == FuelType::OtherFuel_1
          || fuelType == FuelType::OtherFuel_2 || fuelType == FuelType::EnergyTransfer) {
        // TODO: why are we ignoring these?! Aside from EnergyTransfer at least
        // skip these to avoid E+ warning, EnergyTransfer is internal to the simulation
        continue;
      }

      // overall meter for this fuel type
      model::OutputMeter meter(*result);
      meter.setFuelType(FuelType(fuelType));
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      std::set<int> endUseTypes = EndUseType::getValues();
      for (int endUseType : endUseTypes) {

        if (endUseType == EndUseType::HeatingCoils || endUseType == EndUseType::CoolingCoils || endUseType == EndUseType::Boilers
            || endUseType == EndUseType::Baseboard || endUseType == EndUseType::HeatRecoveryForCooling
            || endUseType == EndUseType::HeatRecoveryForHeating) {
          // ignore energy transfer meters
          continue;
        }

        // meter for this fuel type and end use
        // DLM: many of these will not be applicable and will cause E+ warnings
        model::OutputMeter meter(*result);
        meter.setFuelType(FuelType(fuelType));
        meter.setEndUseType(EndUseType(endUseType));
        meter.setInstallLocationType(InstallLocationType::Facility);
        meter.setReportingFrequency("Hourly");
      }
    }

    // request specific meters
    // ElectricEquipment - Receptacle, Process, Refrig
    model::OutputMeter meter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::InteriorEquipment);
    meter.setSpecificEndUse("Receptacle");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::InteriorEquipment);
    meter.setSpecificEndUse("Process");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::InteriorEquipment);
    meter.setSpecificEndUse("Refrig");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::InteriorEquipment);
    meter.setSpecificEndUse("Internal Transport");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::ExteriorEquipment);
    meter.setSpecificEndUse("Receptacle");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::ExteriorEquipment);
    meter.setSpecificEndUse("Process");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::ExteriorEquipment);
    meter.setSpecificEndUse("Refrig");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    // GasEquipment - Receptacle, Process
    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Gas);
    meter.setEndUseType(EndUseType::InteriorEquipment);
    meter.setSpecificEndUse("Receptacle");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Gas);
    meter.setEndUseType(EndUseType::InteriorEquipment);
    meter.setSpecificEndUse("Process");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Gas);
    meter.setEndUseType(EndUseType::ExteriorEquipment);
    meter.setSpecificEndUse("Receptacle");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Gas);
    meter.setEndUseType(EndUseType::ExteriorEquipment);
    meter.setSpecificEndUse("Process");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    // Lights - ComplianceLtg, NonComplianceLtg
    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::InteriorLights);
    meter.setSpecificEndUse("ComplianceLtg");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::InteriorLights);
    meter.setSpecificEndUse("NonComplianceLtg");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    // Exterior Lights - Reg Ltg, NonReg Ltg
    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::ExteriorLights);
    meter.setSpecificEndUse("Reg Ltg");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    meter = model::OutputMeter(*result);
    meter.setFuelType(FuelType::Electricity);
    meter.setEndUseType(EndUseType::ExteriorLights);
    meter.setSpecificEndUse("NonReg Ltg");
    meter.setInstallLocationType(InstallLocationType::Facility);
    meter.setReportingFrequency("Hourly");

    {
      auto fanZoneExhausts = result->getModelObjects<model::FanZoneExhaust>();
      std::vector<std::string> subCategories(fanZoneExhausts.size());
      std::transform(fanZoneExhausts.begin(), fanZoneExhausts.end(), subCategories.begin(),
                     [](const model::FanZoneExhaust& fan) { return fan.endUseSubcategory(); });
      std::sort(subCategories.begin(), subCategories.end());
      auto last = std::unique(subCategories.begin(), subCategories.end());
      subCategories.erase(last, subCategories.end());

      for (const auto& subcat : subCategories) {
        meter = model::OutputMeter(*result);
        meter.setFuelType(FuelType::Electricity);
        meter.setEndUseType(EndUseType::Fans);
        meter.setSpecificEndUse(subcat);
        meter.setInstallLocationType(InstallLocationType::Facility);
        meter.setReportingFrequency("Hourly");
      }
    }

    // Output Variables

    pugi::xml_node simVarsIntervalElement = projectElement.child("SimVarsInterval");

    std::string interval(simVarsIntervalElement.text().as_string());

    // SimVarsSite

    pugi::xml_node simVarsSiteElement = projectElement.child("SimVarsSite");

    // Here as_int() is fine, if the key wasn't present or not an int, it'd convert to zero
    if (simVarsSiteElement.text().as_int() == 1) {
      model::OutputVariable var("Site Outdoor Air Drybulb Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Site Outdoor Air Wetbulb Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Site Direct Solar Radiation Rate per Area", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Site Diffuse Solar Radiation Rate per Area", *result);
      var.setReportingFrequency(interval);
    }

    // SimVarsDayltg
    pugi::xml_node simVarsDayltgElement = projectElement.child("SimVarsDayltg");
    if (simVarsDayltgElement.text().as_int() == 1) {
      model::OutputVariable var("Zone Lights Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Daylighting Reference Point 1 Illuminance", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Daylighting Reference Point 2 Illuminance", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Daylighting Lighting Power Multiplier", *result);
      var.setReportingFrequency(interval);
    }

    // SimVarsFen
    pugi::xml_node simVarsFenElement = projectElement.child("SimVarsFen");
    if ((simVarsFenElement.text().as_int() == 1)) {
      for (const auto& subSurface : result->getConcreteModelObjects<model::SubSurface>()) {
        std::string subSurfaceType = subSurface.subSurfaceType();

        if (istringEqual(subSurfaceType, "FixedWindow") || istringEqual(subSurfaceType, "OperableWindow") || istringEqual(subSurfaceType, "GlassDoor")
            || istringEqual(subSurfaceType, "Skylight") || istringEqual(subSurfaceType, "TubularDaylightDome")
            || istringEqual(subSurfaceType, "TubularDaylightDiffuser")) {
          model::OutputVariable var("Surface Window Heat Gain Rate", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);

          var = model::OutputVariable("Surface Window Heat Loss Rate", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);

          var = model::OutputVariable("Surface Inside Face Adjacent Air Temperature", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);

          var = model::OutputVariable("Surface Outside Face Outdoor Air Drybulb Temperature", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);
        }
      }
    }

    // SimVarsDr

    pugi::xml_node simVarsDrElement = projectElement.child("SimVarsDr");
    if ((simVarsDrElement.text().as_int() == 1)) {
      for (const auto& subSurface : result->getConcreteModelObjects<model::SubSurface>()) {
        std::string subSurfaceType = subSurface.subSurfaceType();

        if (istringEqual(subSurfaceType, "Door") || istringEqual(subSurfaceType, "OverheadDoor")) {
          model::OutputVariable var("Surface Average Face Conduction Heat Gain Rate", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);

          var = model::OutputVariable("Surface Average Face Conduction Heat Loss Rate", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);

          var = model::OutputVariable("Surface Inside Face Adjacent Air Temperature", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);

          var = model::OutputVariable("Surface Outside Face Outdoor Air Drybulb Temperature", *result);
          var.setKeyValue(subSurface.name().get());
          var.setReportingFrequency(interval);
        }
      }
    }

    // SimVarsThrmlZn

    pugi::xml_node simVarsThrmlZnElement = projectElement.child("SimVarsThrmlZn");
    if (simVarsThrmlZnElement.text().as_int() == 1) {
      model::OutputVariable var("Zone Air Temperature", *result);
      var.setReportingFrequency(interval);

      std::vector<model::ThermalZone> zones = result->getModelObjects<model::ThermalZone>();
      for (auto it = zones.begin(); it != zones.end(); ++it) {
        if (boost::optional<model::ModelObject> returnAirNode = it->returnAirModelObject()) {
          var = model::OutputVariable("System Node Temperature", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(returnAirNode->name().get());

          var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(returnAirNode->name().get());
        }

        std::vector<model::ModelObject> objects = it->inletPortList().modelObjects();
        for (auto inletIt = objects.begin(); inletIt != objects.end(); ++inletIt) {
          var = model::OutputVariable("System Node Temperature", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(inletIt->name().get());

          var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(inletIt->name().get());
        }

        objects = it->exhaustPortList().modelObjects();
        for (auto exhIt = objects.begin(); exhIt != objects.end(); ++exhIt) {
          var = model::OutputVariable("System Node Temperature", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(exhIt->name().get());

          var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(exhIt->name().get());
        }
      }
    }

    // SimVarsVRFSys
    pugi::xml_node simVarsVRFSys = projectElement.child("SimVarsVRFSys");
    if (simVarsVRFSys.text().as_int() == 1) {
      model::OutputVariable var("VRF Heat Pump Total Cooling Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Total Heating Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Cooling Electricity Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Heating Electricity Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Cooling COP", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Heating COP", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump COP", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Defrost Electricity Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Part Load Ratio", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Runtime Fraction", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Cycling Ratio", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Operating Mode", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Condenser Inlet Temperature", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Maximum Capacity Cooling Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Maximum Capacity Heating Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Crankcase Heater Electricity Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Terminal Unit Cooling Load Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Terminal Unit Heating Load Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("VRF Heat Pump Heat Recovery Status Change Multiplier", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Cooling Electricity Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Total Cooling Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Sensible Cooling Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Latent Cooling Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Heating Electricity Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Total Heating Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Sensible Heating Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Latent Heating Rate", *result);
      var.setReportingFrequency(interval);
      var = model::OutputVariable("Zone VRF Air Terminal Fan Availability Status", *result);
      var.setReportingFrequency(interval);
    }

    // SimVarsHVACZn

    pugi::xml_node simVarsHVACZnElement = projectElement.child("SimVarsHVACZn");
    if (simVarsHVACZnElement.text().as_int() == 1) {
      model::OutputVariable var("Zone Air Terminal VAV Damper Position", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Zone Air Terminal Outdoor Air Volume Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Zone Packaged Terminal Heat Pump Total Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Zone Packaged Terminal Heat Pump Total Cooling Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Zone Packaged Terminal Air Conditioner Total Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Zone Packaged Terminal Air Conditioner Total Cooling Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Baseboard Total Heating Rate", *result);
      var.setReportingFrequency(interval);

      auto createOutputForNode = [&](const std::string& nodename) {
        auto var = model::OutputVariable("System Node Temperature", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(nodename);
        var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(nodename);
      };

      auto createOutputForZoneHVAC = [&](const model::ZoneHVACComponent& comp) {
        auto name = comp.name().get();
        if (auto mo = comp.inletNode()) {
          createOutputForNode(mo->name().get());
        }
        if (auto mo = comp.outletNode()) {
          createOutputForNode(mo->name().get());
        }
        auto mixedAirNodeName = name + " Mixed Air Node";
        createOutputForNode(mixedAirNodeName);
        auto reliefAirNodeName = name + " Relief Air Node";
        createOutputForNode(reliefAirNodeName);
        auto oaNodeName = name + " OA Node";
        createOutputForNode(oaNodeName);
      };

      // Really need some abstraction so this sillyness isn't required
      {
        auto zonehvac = result->getModelObjects<model::ZoneHVACPackagedTerminalAirConditioner>();
        for (const auto& comp : zonehvac) {
          createOutputForZoneHVAC(comp);
        }
      }
      {
        auto zonehvac = result->getModelObjects<model::ZoneHVACPackagedTerminalHeatPump>();
        for (const auto& comp : zonehvac) {
          createOutputForZoneHVAC(comp);
        }
      }
      {
        auto zonehvac = result->getModelObjects<model::ZoneHVACWaterToAirHeatPump>();
        for (const auto& comp : zonehvac) {
          createOutputForZoneHVAC(comp);

          var = model::OutputVariable("Zone Water to Air Heat Pump Total Heating Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());

          var = model::OutputVariable("Zone Water to Air Heat Pump Total Cooling Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());

          var = model::OutputVariable("Zone Water to Air Heat Pump Total Cooling Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());

          var = model::OutputVariable("Zone Water to Air Heat Pump Sensible Cooling Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());

          var = model::OutputVariable("Zone Water to Air Heat Pump Electricity Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());

          var = model::OutputVariable("Zone Water to Air Heat Pump Compressor Part Load Ratio", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());
        }
      }
      {
        auto zonehvac = result->getModelObjects<model::ZoneHVACFourPipeFanCoil>();
        for (const auto& comp : zonehvac) {
          createOutputForZoneHVAC(comp);
        }
      }
      {
        auto zonehvac = result->getModelObjects<model::ZoneHVACBaseboardConvectiveElectric>();
        for (const auto& comp : zonehvac) {
          var = model::OutputVariable("Baseboard Electricity Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(comp.nameString());
        }
      }
    }

    // SimVarsHVACSec

    pugi::xml_node simVarsHVACSecElement = projectElement.child("SimVarsHVACSec");

    if (simVarsHVACSecElement.text().as_int() == 1) {
      model::OutputVariable var("Heating Coil Air Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Heating Coil Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Coil Total Cooling Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Coil Sensible Cooling Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Coil Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Coil Part Load Ratio", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Coil Runtime Fraction", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Fan Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Heating Coil NaturalGas Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Heating Coil Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Heating Coil Runtime Fraction", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Heating Coil Part Load Ratio", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Evaporative Cooler Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Heating Coil Total Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Air System Outdoor Air Heat Recovery Bypass Status", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Air System Outdoor Air Economizer Status", *result);
      var.setReportingFrequency(interval);

      auto hxs = result->getModelObjects<model::HeatExchangerAirToAirSensibleAndLatent>();
      for (auto& hx : hxs) {
        var = model::OutputVariable("Heat Exchanger Sensible Heating Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Latent Gain Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Total Heating Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Sensible Cooling Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Latent Cooling Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Total Cooling Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Electricity Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Sensible Effectiveness", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Latent Effectiveness", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Supply Air Bypass Mass Flow Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Exhaust Air Bypass Mass Flow Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());
        var = model::OutputVariable("Heat Exchanger Defrost Time Fraction", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(hx.nameString());

        if (auto node = hx.primaryAirOutletModelObject()) {
          var = model::OutputVariable("System Node Temperature", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(node->name().get());

          var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(node->name().get());
        }

        if (auto node = hx.secondaryAirOutletModelObject()) {
          var = model::OutputVariable("System Node Temperature", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(node->name().get());

          var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(node->name().get());
        }
      }

      std::vector<model::AirLoopHVAC> airloops = result->getModelObjects<model::AirLoopHVAC>();
      for (auto& airloop : airloops) {
        var = model::OutputVariable("System Node Temperature", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(airloop.supplyInletNode().name().get());

        var = model::OutputVariable("System Node Temperature", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(airloop.supplyOutletNode().name().get());

        var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(airloop.supplyOutletNode().name().get());

        var = model::OutputVariable("System Node Temperature", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(airloop.demandInletNode().name().get());

        var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(airloop.demandInletNode().name().get());

        if (boost::optional<model::Node> node = airloop.mixedAirNode()) {
          var = model::OutputVariable("System Node Temperature", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(node->name().get());

          var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
          var.setReportingFrequency(interval);
          var.setKeyValue(node->name().get());
        }

        if (boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airloop.airLoopHVACOutdoorAirSystem()) {
          if (boost::optional<model::ModelObject> node = oaSystem->reliefAirModelObject()) {
            var = model::OutputVariable("System Node Temperature", *result);
            var.setReportingFrequency(interval);
            var.setKeyValue(node->name().get());

            var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
            var.setReportingFrequency(interval);
            var.setKeyValue(node->name().get());
          }

          if (boost::optional<model::Node> node = oaSystem->outboardOANode()) {
            var = model::OutputVariable("System Node Temperature", *result);
            var.setReportingFrequency(interval);
            var.setKeyValue(node->name().get());

            var = model::OutputVariable("System Node Standard Density Volume Flow Rate", *result);
            var.setReportingFrequency(interval);
            var.setKeyValue(node->name().get());
          }
        }
      }
    }

    // SimVarsHVACPri

    pugi::xml_node simVarsHVACPriElement = projectElement.child("SimVarsHVACPri");

    if ((simVarsHVACPriElement.text().as_int() == 1) || autosize()) {
      model::OutputVariable var("Chiller Evaporator Cooling Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Tower Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Boiler Heating Rate", *result);
      var.setReportingFrequency(interval);
    }

    if (simVarsHVACPriElement.text().as_int() == 1) {
      model::OutputVariable var("Pump Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Pump Mass Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Pump Outlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Debug Plant Loop Bypass Fraction", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Debug Plant Loop Bypass Fraction", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chiller Condenser Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chiller Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Tower Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Cooling Tower Fan Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Boiler NaturalGas Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Boiler Ancillary Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Plant Supply Side Cooling Demand Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Plant Supply Side Heating Demand Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chiller Evaporator Cooling Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Boiler Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Plant Supply Side Inlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Plant Supply Side Outlet Temperature", *result);
      var.setReportingFrequency(interval);

      std::vector<model::PlantLoop> plants = result->getModelObjects<model::PlantLoop>();
      for (auto& plant : plants) {
        var = model::OutputVariable("System Node Mass Flow Rate", *result);
        var.setReportingFrequency(interval);
        var.setKeyValue(plant.demandOutletNode().name().get());
      }
    }

    // SimVarsWtrHtrSys
    pugi::xml_node simVarsWtrHtrSys = projectElement.child("SimVarsWtrHtr");
    if (simVarsWtrHtrSys.text().as_int() == 1) {
      model::OutputVariable var("Water Heater Tank Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Electricity Energy", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Electricity Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Heating Energy", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Heating Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Mains Water Volume", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Use Side Inlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Use Side Outlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Use Side Mass Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Use Side Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Source Side Inlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Source Side Outlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Source Side Mass Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Source Side Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Water Volume Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Heat Pump Control Tank Final Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater Heat Pump Control Tank Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater NaturalGas Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Water Heater NaturalGas Energy", *result);
      var.setReportingFrequency(interval);
    }

    // SimVarsThrmlEngyStor
    pugi::xml_node simVarsThrmlEngyStorElement = projectElement.child("SimVarsThrmlEngyStor");

    if (simVarsThrmlEngyStorElement.text().as_int() == 1) {
      model::OutputVariable var("Chilled Water Thermal Storage Tank Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Use Side Mass Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Use Side Inlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Use Side Outlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Use Side Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Use Side Heat Transfer Energy", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Source Side Mass Flow Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Source Side Inlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Source Side Outlet Temperature", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Source Side Heat Transfer Rate", *result);
      var.setReportingFrequency(interval);

      var = model::OutputVariable("Chilled Water Thermal Storage Source Side Heat Transfer Energy", *result);
      var.setReportingFrequency(interval);
    }

    model::OutputControlReportingTolerances rt = result->getUniqueModelObject<model::OutputControlReportingTolerances>();
    rt.setToleranceforTimeCoolingSetpointNotMet(0.56);
    rt.setToleranceforTimeHeatingSetpointNotMet(0.56);

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateRunPeriod(const pugi::xml_node& element,
                                                                                        openstudio::model::Model& model) {
    //<HVACAutoSizing>1</HVACAutoSizing>
    //<SimDsgnDays>0</SimDsgnDays>
    //<RunPeriodBeginMonth>1</RunPeriodBeginMonth>
    //<RunPeriodBeginDay>1</RunPeriodBeginDay>
    //<RunPeriodEndMonth>12</RunPeriodEndMonth>
    //<RunPeriodEndDay>31</RunPeriodEndDay>
    //<RunPeriodYear>1991</RunPeriodYear>

    boost::optional<openstudio::model::ModelObject> result;

    pugi::xml_node hvacAutoSizingElement = element.child("HVACAutoSizing");
    pugi::xml_node runDesignDaysElement = element.child("SimDsgnDays");
    pugi::xml_node beginMonthElement = element.child("RunPeriodBeginMonth");
    pugi::xml_node beginDayElement = element.child("RunPeriodBeginDay");
    pugi::xml_node endMonthElement = element.child("RunPeriodEndMonth");
    pugi::xml_node endDayElement = element.child("RunPeriodEndDay");
    pugi::xml_node yearElement = element.child("RunPeriodYear");

    bool test = true;

    if (!hvacAutoSizingElement) {
      test = false;
      LOG(Error, "HVACAutoSizing not specified");
    }
    if (!runDesignDaysElement) {
      test = false;
      LOG(Error, "SimDsgnDays not specified");
    }
    if (!beginMonthElement) {
      test = false;
      LOG(Error, "RunPeriodBeginMonth not specified");
    }
    if (!beginDayElement) {
      test = false;
      LOG(Error, "RunPeriodBeginDay not specified");
    }
    if (!endMonthElement) {
      test = false;
      LOG(Error, "RunPeriodEndMonth not specified");
    }
    if (!endDayElement) {
      test = false;
      LOG(Error, "RunPeriodEndDay not specified");
    }
    if (!yearElement) {
      test = false;
      LOG(Error, "RunPeriodYear not specified");
    }

    if (!test) {
      return boost::none;
    }

    model::SimulationControl simulationControl = model.getUniqueModelObject<model::SimulationControl>();

    simulationControl.setMaximumNumberofWarmupDays(50);

    if ((runDesignDaysElement.text().as_int() == 1) || (hvacAutoSizingElement.text().as_int() == 1) || m_masterAutosize) {
      simulationControl.setRunSimulationforSizingPeriods(true);
    } else {
      simulationControl.setRunSimulationforSizingPeriods(false);
    }

    if (beginMonthElement.text().as_int(-1) == 0) {
      simulationControl.setRunSimulationforWeatherFileRunPeriods(false);

      // remove any existing run periods
      boost::optional<model::RunPeriod> runPeriod = model.getOptionalUniqueModelObject<model::RunPeriod>();
      if (runPeriod) {
        runPeriod->remove();
      }

    } else {
      simulationControl.setRunSimulationforWeatherFileRunPeriods(true);

      model::YearDescription yearDescription = model.getUniqueModelObject<model::YearDescription>();
      yearDescription.setCalendarYear(yearElement.text().as_int());

      std::string runPeriodName = "Run Period";
      pugi::xml_node annualWeatherFileElement = element.child("AnnualWeatherFile");
      if (annualWeatherFileElement) {
        runPeriodName = openstudio::toString(openstudio::toPath(annualWeatherFileElement.text().as_string()).stem());
      }

      model::RunPeriod runPeriod = model.getUniqueModelObject<model::RunPeriod>();
      runPeriod.setName(runPeriodName);
      runPeriod.setBeginMonth(beginMonthElement.text().as_int());
      runPeriod.setBeginDayOfMonth(beginDayElement.text().as_int());
      runPeriod.setEndMonth(endMonthElement.text().as_int());
      runPeriod.setEndDayOfMonth(endDayElement.text().as_int());
      runPeriod.setUseWeatherFileHolidays(false);
      runPeriod.setUseWeatherFileDaylightSavings(false);
      runPeriod.setApplyWeekendHolidayRule(true);
      result = runPeriod;
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSite(const pugi::xml_node& element, openstudio::model::Model& model) {
    //<CliZn>ClimateZone12</CliZn>
    //<Lat>38.58</Lat>
    //<Long>-121.49</Long>
    //<Elevation>839.9</Elevation>

    pugi::xml_node climateZoneElement = element.child("CliZn");
    pugi::xml_node latElement = element.child("Lat");
    pugi::xml_node longElement = element.child("Long");
    pugi::xml_node elevationElement = element.child("Elevation");

    bool test = true;

    if (!climateZoneElement) {
      //test = false;
      LOG(Error, "No climate zone specified");
    }
    if (!latElement) {
      test = false;
      LOG(Error, "No lattitude specified");
    }
    if (!longElement) {
      test = false;
      LOG(Error, "No longitude specified");
    }
    if (!elevationElement) {
      test = false;
      LOG(Error, "No elevation specified");
    }

    if (!test) {
      return boost::none;
    }

    // set lat, lon, elev
    model::Site site = model.getUniqueModelObject<model::Site>();

    // DLM: what about time zone and terrain?

    double latitude = latElement.text().as_double();
    double longitude = longElement.text().as_double();
    double elevationIP = elevationElement.text().as_double();
    double elevationSI = 0.3048 * elevationIP;

    site.setLatitude(latitude);
    site.setLongitude(longitude);
    site.setElevation(elevationSI);

    // DLM: always assume California?
    site.setTimeZone(-8.0);

    return site;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWaterMainsTemperature(const pugi::xml_node& element,
                                                                                                    openstudio::model::Model& model) {
    //<WtrMnTempSchRef>WaterMainCZ12</WtrMnTempSchRef>

    pugi::xml_node wtrMnTempSchRefElement = element.child("WtrMnTempSchRef");
    if (wtrMnTempSchRefElement) {
      boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(wtrMnTempSchRefElement.text().as_string());
      if (schedule) {
        model::SiteWaterMainsTemperature waterMains = model.getUniqueModelObject<model::SiteWaterMainsTemperature>();
        waterMains.setTemperatureSchedule(*schedule);
        return waterMains;
      }
    }

    return boost::none;
  }

  std::vector<WorkspaceObject> ReverseTranslator::translateDesignDays(const pugi::xml_node& element, openstudio::model::Model& model) {
    std::vector<WorkspaceObject> result;

    //<DDWeatherFile>F:/360Local/AEC/CBECCn/CBECC Source Files/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.ddy</DDWeatherFile>

    pugi::xml_node ddWeatherFileElement = element.child("DDWeatherFile");

    if (!ddWeatherFileElement) {
      LOG(Error, "No design day file specified");
      return result;
    }

    openstudio::path ddyFilePath = toPath(ddWeatherFileElement.text().as_string());
    if (!ddyFilePath.is_complete()) {
      ddyFilePath = complete(ddyFilePath, m_path.parent_path());
    }

    if (!exists(ddyFilePath)) {
      LOG(Error, "Design day file '" << toString(ddyFilePath) << "' does not exist");
      return result;
    }

    boost::optional<Workspace> ddyFile = Workspace::load(ddyFilePath);
    if (!ddyFile) {
      LOG(Error, "Could not open design day file '" << toString(ddyFilePath) << "'");
      return result;
    }

    // convert ddy file to osm and add objects
    energyplus::ReverseTranslator rt;
    model::Model ddyModel = rt.translateWorkspace(*ddyFile);
    result = model.addObjects(ddyModel.objects());
    if (result.empty()) {
      LOG(Error, "Failed to add design days to model");
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWeatherFile(const pugi::xml_node& element,
                                                                                          openstudio::model::Model& model) {
    //<AnnualWeatherFile>F:/360Local/AEC/CBECCn/CBECC Source Files/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.epw</AnnualWeatherFile>

    pugi::xml_node annualWeatherFileElement = element.child("AnnualWeatherFile");

    if (!annualWeatherFileElement) {
      LOG(Error, "No annual weather file specified");
      return boost::none;
    }

    openstudio::path epwFilePath = toPath(annualWeatherFileElement.text().as_string());
    if (!epwFilePath.is_complete()) {
      epwFilePath = complete(epwFilePath, m_path.parent_path());
    }

    if (!exists(epwFilePath)) {
      LOG(Error, "Annual weather file '" << toString(epwFilePath) << "' does not exist");
      return boost::none;
    }

    boost::optional<EpwFile> epwFile;
    try {
      epwFile = EpwFile(epwFilePath);
    } catch (std::exception&) {
      LOG(Error, "Could not open epw file '" << toString(epwFilePath) << "'");
    }

    if (!epwFile) {
      return boost::none;
    }

    // set weatherfile
    boost::optional<model::WeatherFile> weatherFile = model::WeatherFile::setWeatherFile(model, *epwFile);
    if (!weatherFile) {
      LOG(Error, "Failed to set weather file for model");
      return boost::none;
    }

    return weatherFile.get();
  }

  pugi::xml_node ReverseTranslator::supplySegment(const pugi::xml_node& fluidSegInRefElement) {

    pugi::xml_node projectElement = getProjectElement(fluidSegInRefElement);
    std::string fluidSegmentName = fluidSegInRefElement.text().as_string();

    if (fluidSegmentName.empty()) {
      LOG(Error, "supplySegment called with an empty fluidSegmentName");
      OS_ASSERT(false);
    }

    for (auto& fluidSysElement : projectElement.children("FluidSys")) {
      for (auto& fluidSegmentElement : fluidSysElement.children("FluidSeg")) {
        auto nameElement = fluidSegmentElement.child("Name");
        auto typeElement = fluidSegmentElement.child("Type");

        if ((istringEqual(typeElement.text().as_string(), "SECONDARYSUPPLY") || istringEqual(typeElement.text().as_string(), "PRIMARYSUPPLY"))
            && istringEqual(nameElement.text().as_string(), fluidSegmentName)) {
          return fluidSegmentElement;
        }
      }
    }

    return pugi::xml_node();
  }

  boost::optional<model::PlantLoop> ReverseTranslator::loopForSupplySegment(const pugi::xml_node& fluidSegInRefElement,
                                                                            openstudio::model::Model& model) {
    auto fluidSegmentElement = supplySegment(fluidSegInRefElement);
    auto fluidSysElement = fluidSegmentElement.parent();
    auto fluidSysNameElement = fluidSysElement.child("Name");

    return model.getModelObjectByName<model::PlantLoop>(fluidSysNameElement.text().as_string());
  }

  boost::optional<model::PlantLoop> ReverseTranslator::serviceHotWaterLoopForSupplySegment(const pugi::xml_node& fluidSegInRefElement,
                                                                                           openstudio::model::Model& model) {

    pugi::xml_node projectElement = getProjectElement(fluidSegInRefElement);
    std::string fluidSegmentName = fluidSegInRefElement.text().as_string();
    if (fluidSegmentName.empty()) {
      LOG(Error, "supplySegment called with an empty fluidSegmentName");
      OS_ASSERT(false);
    }

    boost::optional<model::PlantLoop> result;

    for (auto& fluidSysElement : projectElement.children("FluidSys")) {

      auto fluidSysNameElement = fluidSysElement.child("Name");

      auto fluidSysTypeElement = fluidSysElement.child("Type");

      if (openstudio::istringEqual(fluidSysTypeElement.text().as_string(), "SERVICEHOTWATER")) {
        for (auto& fluidSegmentElement : fluidSysElement.children("FluidSeg")) {

          auto nameElement = fluidSegmentElement.child("Name");
          auto typeElement = fluidSegmentElement.child("Type");

          if ((openstudio::istringEqual(typeElement.text().as_string(), "SECONDARYSUPPLY")
               || openstudio::istringEqual(typeElement.text().as_string(), "PRIMARYSUPPLY"))
              && openstudio::istringEqual(nameElement.text().as_string(), fluidSegmentName)) {
            if (boost::optional<model::PlantLoop> loop = model.getModelObjectByName<model::PlantLoop>(fluidSysNameElement.text().as_string())) {
              return loop;
            } else {
              if (boost::optional<model::ModelObject> mo = translateFluidSys(fluidSysElement, model)) {
                return mo->optionalCast<model::PlantLoop>();
              }
            }
          }
        }
      }
    }

    return result;
  }

  //TODO probably should be in OS proper
  //helper method to do unit conversions;
  boost::optional<double> ReverseTranslator::unitToUnit(double val, const std::string& fstUnitString, const std::string& secUnitString) {
    //create the units from the strings
    boost::optional<Unit> fstUnit = UnitFactory::instance().createUnit(fstUnitString);
    boost::optional<Unit> secUnit = UnitFactory::instance().createUnit(secUnitString);

    //make sure both unit strings were valid
    if (fstUnit && secUnit) {

      //make the IP quantity
      Quantity ipQuant = Quantity(val, *fstUnit);

      //convert the IP quantity from IP to SI
      boost::optional<Quantity> siQuant = QuantityConverter::instance().convert(ipQuant, *secUnit);

      //if the conversion
      if (siQuant) {
        return siQuant->value();
      }
    }

    return boost::none;
  }

  bool ReverseTranslator::autosize() const {
    if (m_masterAutosize) {
      return true;
    }

    return m_autosize;
  }

}  // namespace sdd
}  // namespace openstudio
