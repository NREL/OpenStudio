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

#include "ForwardTranslator.hpp"
#include "../WindPressure.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/BuildingStory_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/RunPeriod.hpp"

#include "../../utilities/time/Date.hpp"

#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/plot/ProgressBar.hpp"

#include <boost/math/constants/constants.hpp>

#include <airflow/embedded_files.hxx>

#include <thread>

#include <algorithm>

namespace openstudio {
namespace contam {

CvFile::CvFile()
{
  clear();
}

void CvFile::clear()
{
  m_start = Date(MonthOfYear::Jan,1);
  m_end = Date(MonthOfYear::Dec,31);
  m_names = std::vector<std::string>();
  m_series = std::vector<TimeSeries>();
}

void CvFile::addTimeSeries(std::string name, TimeSeries series)
{
  m_names.push_back(name);
  m_series.push_back(series);
}

static std::string convertDateTime(const DateTime &datetime)
{
  char buffer[256];
  sprintf(buffer, "%02d/%02d\t%02d:%02d:%02d", month(datetime.date().monthOfYear()), datetime.date().dayOfMonth(),
    datetime.time().hours(), datetime.time().minutes(), datetime.time().seconds());
  return std::string(buffer);
}

bool CvFile::write(openstudio::path filepath)
{
  openstudio::filesystem::ofstream file{filepath};
  if(file.good())
  {
    file << "ContinuousValuesFile ContamW 2.1\n";
    file << "CVF file from E+ results\n";
    file << month(m_start.monthOfYear()) << '/' << m_start.dayOfMonth() << '\t'
      <<  month(m_end.monthOfYear())  << '/' <<  m_end.dayOfMonth()  << '\n';
    file << m_names.size() << '\n';
    for(unsigned int i=0;i<m_names.size();i++)
    {
      file << m_names[i] << '\n';
    }
    Time delta(0,1,0,0); // Hard code hourly data for now
    DateTime current(m_start);
    DateTime last = current;
    // Another hard code for 8760 hours, really not sure what to do with 1/1 00:00:00
    file << convertDateTime(current);
    for(unsigned int i=0;i<m_names.size();i++)
    {
      double value = m_series[i].value(current+delta); // This works now...
      if(m_series[i].units() == "C")
      {
        value += 273.15;
      }
      file << '\t' << value;
    }
    file << '\n';
    // And the big hardcoded loop
    for(int j=0;j<8760;j++)
    {
      current += delta;
      // Mess with the time a little bit to put 24:00:00 in for 00:00:00
      if(current.time().hours() == 0 && current.time().minutes() == 0 && current.time().seconds() == 0)
      {
        char buffer[256];
        sprintf(buffer, "%02d/%02d\t24:00:00",month(last.date().monthOfYear()), last.date().dayOfMonth());
        file << buffer;
      }
      else
      {
        file << convertDateTime(current);
      }
      last = current;
      // Write out the data. May need more conversion here in the future.
      for(unsigned int i=0;i<m_names.size();i++)
      {
        double value = m_series[i].value(current);
        if(m_series[i].units() == "C")
        {
          value += 273.15;
        }
        file << '\t' << value;
      }
      file << '\n';
    }
    file.close();
    return true;
  }
  return false;
}

ForwardTranslator::ForwardTranslator()
{
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.contam\\.ForwardTranslator"));
  m_logSink.setThreadId(std::this_thread::get_id());
  clear();
}

void ForwardTranslator::clear()
{
  m_afeMap = std::map<std::string,int>();
  m_levelMap = std::map <Handle, int>();
  m_zoneMap = std::map <Handle, int>();
  m_pathMap = std::map <std::string, int>();
  m_surfaceMap = std::map <Handle, int>();
  m_ahsMap = std::map <Handle, int>();
  m_leakageDescriptor = boost::optional<std::string>("Average");
  m_returnSupplyRatio = 1.0;
  m_ratioOverride = false;
  m_startDateTime = boost::optional<DateTime>();
  m_endDateTime = boost::optional<DateTime>();
  m_translateHVAC = true;
}

int ForwardTranslator::tableLookup(std::map<std::string,int> map, std::string str, const char *name)
{
  const auto& it = map.find(str);
  if(it == map.end())
  {
    LOG(Warn, "Unable to look up '" << str << "' in " << name);
  }
  return it->second;
}

int ForwardTranslator::tableLookup(std::map<Handle,int> map, Handle handle, const char *name)
{
  const auto& it = map.find(handle);
  if (it == map.end())
  {
    LOG(Warn, "Unable to look up '" << handle << "' in " << name);
  }
  return it->second;
}

std::string ForwardTranslator::reverseLookup(std::map<std::string,int> map, int nr, const char *name)
{
  if(nr > 0)
  {
    std::vector<std::string> keys;
    for (const auto& pair : map) {
      if (pair.second == nr) {
        keys.push_back(pair.first);
      }
    }
    if(keys.size()>0)
    {
      if(keys.size()>1)
      {
        LOG(Warn, "Lookup table " << name << " contains multiple " << nr << " values");
      }
      return keys[0];
    }
  }
  LOG(Warn, "Unable to reverse look up " << nr << " in " << name);
  return std::string();
}

Handle ForwardTranslator::reverseLookup(std::map<Handle,int> map, int nr, const char *name)
{
  if(nr > 0)
  {
    std::vector<Handle> keys;
    for (const auto& pair : map) {
      if (pair.second == nr) {
        keys.push_back(pair.first);
      }
    }
    if(keys.size()>0)
    {
      if(keys.size()>1)
      {
        LOG(Warn, "Lookup table " << name << " contains multiple " << nr << " values");
      }
      return keys[0];
    }
  }
  LOG(Warn, "Unable to reverse look up " << nr << " in " << name);
  return Handle();
}

// Getters and setters
double ForwardTranslator::returnSupplyRatio() const
{
  return m_returnSupplyRatio;
}
void ForwardTranslator::setReturnSupplyRatio(double returnSupplyRatio)
{
  m_returnSupplyRatio = fabs(returnSupplyRatio);
}

bool ForwardTranslator::ratioOverride() const
{
  return m_ratioOverride;
}
void ForwardTranslator::setRatioOverride(bool ratioOverride)
{
  m_ratioOverride = ratioOverride;
}

bool ForwardTranslator::translateHVAC() const
{
  return m_translateHVAC;
}
void ForwardTranslator::setTranslateHVAC(bool translateHVAC)
{
  m_translateHVAC = translateHVAC;
}

boost::optional<DateTime> ForwardTranslator::startDateTime() const
{
  return m_startDateTime;
}

boost::optional<DateTime> ForwardTranslator::endDateTime() const
{
  return m_endDateTime;
}

boost::optional<std::string> ForwardTranslator::airtightnessLevel() const
{
  return m_leakageDescriptor;
}

void ForwardTranslator::setAirtightnessLevel(std::string level)
{
  m_leakageDescriptor = boost::optional<std::string>(level);
  m_flow = boost::optional<double>();
  m_n = boost::optional<double>();
  m_deltaP = boost::optional<double>();
}

bool ForwardTranslator::applyAirtightnessLevel(contam::IndexModel model)
{
  // For this to work, the "standard" names must be in the PRJ data
  if(!m_leakageDescriptor)
  {
    return false;
  }
  std::map<std::string,int> afeMap;
  std::vector<std::string> grade({ "Leaky", "Average", "Tight" });
  std::vector<std::string> wallExt({ "ExtWallLeaky", "ExtWallAvg", "ExtWallTight" });
  std::vector<std::string> wallInt({ "IntWallLeaky", "IntWallAvg", "IntWallTight" });
  std::vector<std::string> floor({ "FloorLeaky", "FloorAvg", "FloorTight" });
  std::vector<std::string> roof({ "RoofLeaky", "RoofAvg", "RoofTight" });
  int index = -1;
  for (std::vector<std::string>::size_type i=0; i < grade.size(); ++i) {
    if (grade[i] == m_leakageDescriptor.get()) {
      index = i;
    }
  }
  if(index == -1) // Default to average, unknown leakage level
  {
    LOG(Warn, "Unknown airtightness level '" << m_leakageDescriptor.get() << "', defaulting to 'Average'");
    index = 1;
  }
  int nr;
  // Exterior walls
  nr = model.airflowElementNrByName(wallExt[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["exterior"] = nr;
  // Interior walls
  nr = model.airflowElementNrByName(wallInt[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["interior"] =  nr;
  // Floors
  nr = model.airflowElementNrByName(floor[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["floor"] = nr;
  // Roof
  nr = model.airflowElementNrByName(roof[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["roof"] = nr;
  m_afeMap = afeMap;
  return true;
}

boost::optional<double> ForwardTranslator::exteriorFlowRate() const
{
  return m_flow;
}

boost::optional<double> ForwardTranslator::exteriorExponent() const
{
  return m_n;
}

boost::optional<double> ForwardTranslator::exteriorDeltaP() const
{
  return m_deltaP;
}

bool ForwardTranslator::setExteriorFlowRate(double flow, double n, double deltaP)
{
  if(flow > 0 && n > 0 && deltaP > 0)
  {
    m_flow = boost::optional<double>(flow);
    m_n = boost::optional<double>(n);
    m_deltaP = boost::optional<double>(deltaP);
    m_leakageDescriptor = boost::optional<std::string>();
    return true;
  }
  return false;
}

bool ForwardTranslator::applyExteriorFlowRate(contam::IndexModel model)
{
  if(m_flow && m_n && m_deltaP)
  {
    std::map<std::string,int> afeMap;
    afeMap["exterior"] = addNewAirflowElement(model,"CustomExterior",m_flow.get(),m_n.get(),m_deltaP.get());
    afeMap["roof"] = addNewAirflowElement(model,"CustomRoof",m_flow.get(),m_n.get(),m_deltaP.get());
    afeMap["interior"] = addNewAirflowElement(model,"CustomInterior",2*m_flow.get(),m_n.get(),m_deltaP.get());
    afeMap["floor"] = addNewAirflowElement(model,"CustomFloor",2*m_flow.get(),m_n.get(),m_deltaP.get());
    m_afeMap = afeMap;
    m_leakageDescriptor = boost::optional<std::string>();
    return true;
  }
  return false;
}

bool ForwardTranslator::modelToPrj(const openstudio::model::Model& model, const openstudio::path& path,
  bool translateHVAC, std::string leakageDescriptor, ProgressBar* progressBar)
{
  ForwardTranslator translator;
  translator.setTranslateHVAC(translateHVAC);
  translator.setAirtightnessLevel(leakageDescriptor);

  boost::optional<contam::IndexModel> prjModel = translator.translateModel(model);
  if(prjModel)
  {
    boost::optional<std::string> output = prjModel->toString();
    if (!output)
    {
      return false;
    }
    openstudio::filesystem::ofstream file(path);
    if(file.good())
    {
      file << *output;
      return true;
    }
  }
  return false;
}

bool compareElevation(openstudio::model::BuildingStory a, openstudio::model::BuildingStory b)
{
  double a_z = a.nominalZCoordinate().get();
  double b_z = b.nominalZCoordinate().get();
  return a_z < b_z;
}

// This function is altogether too long and needs to be broken up into smaller functions.
// This is particularly true for the HVAC translation.
boost::optional<contam::IndexModel> ForwardTranslator::translateModel(model::Model model)
{
  m_logSink.setThreadId(std::this_thread::get_id());
  m_logSink.resetStringStream();

  {
    std::string s = ::openstudiocontam::embedded_files::getFileAsString(":/templates/template.prj");
    Reader r(s);
    m_prjModel.read(r);
    if (!m_prjModel.valid()) {
      LOG(Error, "Unable to load templates");
      return boost::none;
    }
  }

  // The template is a legal PRJ file, so it has one level. Not for long.
  m_prjModel.setLevels(std::vector<Level>());

  // Do some setup work
  if(m_leakageDescriptor) {
    if(!applyAirtightnessLevel(m_prjModel)) {
      LOG(Error,"Application of airtightness level failed.");
      return boost::optional<contam::IndexModel>();
    }
  } else {
    if(!applyExteriorFlowRate(m_prjModel)) {
      LOG(Error,"Application of exterior flow rate failed.");
      return boost::optional<contam::IndexModel>();
    }
  }

  std::string output;
  int nr;
  // Set top-level model info
  boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
  std::string modelDescr = std::string("Automatically generated OpenStudio model");
  if(building) {
    boost::optional<std::string> name = building->name();
    if (name) {
      modelDescr = std::string("Automatically generated from \"") + name.get() + std::string("\" OpenStudio model");
    }
  }
  m_prjModel.setDesc(modelDescr);
  // Set the simulation length to match the length of the E+ simulation
  boost::optional<openstudio::model::RunPeriod> rp = model.runPeriod();
  if(rp) {
    bool goodDates = true;
    std::string startString,endString;
    try {
      char buffer[256];

      openstudio::Date start(rp->getBeginMonth(),rp->getBeginDayOfMonth());
      sprintf(buffer, "%02d", start.dayOfMonth());
      startString = start.monthOfYear().valueName() + std::string(buffer);
      openstudio::Date end(rp->getEndMonth(),rp->getEndDayOfMonth());

      sprintf(buffer, "%02d", end.dayOfMonth());
      endString = end.monthOfYear().valueName() + std::string(buffer);
      m_startDateTime = boost::optional<DateTime>(DateTime(start,Time(0)));
      m_endDateTime = boost::optional<DateTime>(DateTime(end,Time(0,24)));
    } catch(...) {
      goodDates = false;
      LOG(Error,"Failed to get usable start of end date from run period object, defaulting to steady state")
    }
    if(goodDates) {
      // Turn on transient simulation
      m_prjModel.rc().setSim_af(1);
      // Set start date
      m_prjModel.rc().setDate_0(startString);
      m_prjModel.rc().setTime_0("00:00:00");
      // Set start date
      m_prjModel.rc().setDate_1(endString);
      m_prjModel.rc().setTime_1("24:00:00");
    }
  }
  // Get stories
  std::vector<openstudio::model::BuildingStory> stories = model.getConcreteModelObjects<openstudio::model::BuildingStory>();
  // We will need each story to have an elevation and a floor-to-floor height.
  for (const openstudio::model::BuildingStory& buildingStory : stories) {
    boost::optional<double> elevation = buildingStory.nominalZCoordinate();
    if(!elevation) {
      LOG(Error, "Story '" << buildingStory.name().get() << "' has no elevation, translation aborted");
      return boost::none;
    }
    boost::optional<double> optHt = buildingStory.nominalFloortoFloorHeight();
    if(!optHt) {
      LOG(Error, "Story '" << buildingStory.name().get() << "' has no nominal floor-to-floor height, translation aborted");
      return boost::none;
    }
  }
  // Sort the stories by elevation
  std::sort(stories.begin(),stories.end(),compareElevation);
  nr=1;
  double totalHeight = 0;
  for (const openstudio::model::BuildingStory& buildingStory : stories) {
    openstudio::contam::Level level;
    level.setName(std::string("<")+std::to_string(nr)+std::string(">"));
    m_levelMap[buildingStory.handle()] = nr;
    double ht = buildingStory.nominalFloortoFloorHeight().get();
    totalHeight += ht;
    double z = buildingStory.nominalZCoordinate().get();
    level.setNr(nr);
    level.setRefht(std::to_string(z));
    level.setDelht(std::to_string(ht));
    m_prjModel.addLevel(level);
    nr++;
    //if (m_progressBar) {
    //  m_progressBar->setValue(m_progressBar->value() + 1);
    //}
  }
  m_prjModel.setWind_H(std::to_string(totalHeight));
  // Check for levels - translation can't proceed without levels
  if(m_prjModel.levels().size() == 0) {
    LOG(Error, "Failed to find building stories in model, translation aborted");
    return boost::optional<contam::IndexModel>();
  }
  // Translate each thermal zone and generate a lookup table by name.
  std::vector<model::ThermalZone> thermalZones = model.getConcreteModelObjects<model::ThermalZone>();
  initProgress(thermalZones.size(), "Translating Zones");

  nr=0;
  for (model::ThermalZone thermalZone : thermalZones) {
    nr++;
    openstudio::contam::Zone zone;
    m_zoneMap[thermalZone.handle()] = nr;
    //volumeMap[thermalZone.name().get()] = nr;
    zone.setNr(nr);
    zone.setName(std::string("Zone_") + std::to_string(nr));
    boost::optional<double> volume = thermalZone.volume();
    std::string volString("0.0");
    if(volume) {
      volString = std::to_string(*volume);
    } else {
      // Since it seems this is a pretty common thing, no warning unless we can't get a value
      // LOG(Warn, "Zone '" << name.toStdString() << "' has zero volume, trying to sum space volumes");
      double vol=0.0;
      for (openstudio::model::Space space : thermalZone.spaces()) {
        vol += space.volume();
      }
      if(vol == 0.0) {
        LOG(Warn, "Failed to compute volume for Zone '" << thermalZone.name().get() << "'");
      } else {
        volString = std::to_string(vol);
      }
    }
    zone.setVol(volString);
    zone.setVariablePressure(true);
    zone.setVariableContaminants(true);
    // Set level - this is not great and will fail to create a legitimate model in cases
    // where a zone is on more than one level. There are ugly workarounds - will need to
    // think about
    int levelNr = 0;
    for (openstudio::model::Space space : thermalZone.spaces()) {
      boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
      if(story) {
        levelNr = tableLookup(m_levelMap,(*story).handle(),"levelMap");
        break;
      }
    }
    if(levelNr) {
      zone.setPl(levelNr);
    } else {
      LOG(Error, "Unable to set level for zone '" << thermalZone.name().get() << "', translation aborted");
      return boost::optional<contam::IndexModel>();
    }
    // set T0
    zone.setT0("293.15");
    m_prjModel.addZone(zone);
    progress();
  }

  // Create paths and generate a lookup table by name
  std::vector<openstudio::model::Surface> surfaces = model.getConcreteModelObjects<openstudio::model::Surface>();
  initProgress(surfaces.size(), "Translating Surfaces");

  nr = 0;
  // Loop over surfaces and generate paths
  build(model);

  if(m_translateHVAC) {
    // Generate air handling systems
    std::vector<openstudio::model::AirLoopHVAC> systems = model.getConcreteModelObjects<openstudio::model::AirLoopHVAC>();
    initProgress(systems.size(), "Translating AirLoops");

    nr = 0;
    for (openstudio::model::AirLoopHVAC airloop : systems) {
      // Skip loops with no zones attached
      if(!airloop.thermalZones().size()) {
        progress();
        continue;
      }
      openstudio::contam::Ahs ahs;
      ahs.setNr(++nr);
      m_ahsMap[airloop.handle()] = nr;
      ahs.setName(std::string("AHS_")+std::to_string(nr));
      // Create supply and return zones
      openstudio::contam::Zone rz;
      rz.setNr(m_prjModel.zones().size()+1);
      rz.setPl(1);
      rz.setT0("293.15");
      rz.setSystem(true);
      rz.setVariableContaminants(true);
      rz.setName(std::string("AHS_")+std::to_string(nr)+std::string("(Rec)"));
      //volumeMap[rz.name.toStdString()] = rz.nr;
      openstudio::contam::Zone sz;
      sz.setNr(rz.nr()+1);
      sz.setPl(1);
      sz.setT0("293.15");
      sz.setSystem(true);
      sz.setVariableContaminants(true);
      sz.setName(std::string("AHS_")+std::to_string(nr)+std::string("(Sup)"));
      //volumeMap[sz.name.toStdString()] = sz.nr;
      // Store the zone numbers in the ahs
      ahs.setZone_r(rz.nr());
      ahs.setZone_s(sz.nr());
      // Add them to the zone list
      m_prjModel.addZone(rz);
      m_prjModel.addZone(sz);
      // Now hook the served zones up to the supply and return zones
      for (openstudio::model::ThermalZone thermalZone : airloop.thermalZones()) {
        int zoneNr = tableLookup(m_zoneMap,thermalZone.handle(),"zoneMap");
        // Supply path
        openstudio::contam::AirflowPath sp;
        sp.setNr(m_prjModel.airflowPaths().size()+1);
        sp.setPld(1);
        sp.setPzn(ahs.zone_s());
        sp.setPzm(zoneNr);
        sp.setPa(ahs.nr());
        sp.setSystem(true);
        m_pathMap[(thermalZone.name().get()+" supply")] = sp.nr();
        // Return path
        openstudio::contam::AirflowPath rp;
        rp.setNr(sp.nr()+1);
        rp.setPld(1);
        rp.setPzn(zoneNr);
        rp.setPzm(ahs.zone_r());
        rp.setPa(ahs.nr());
        rp.setSystem(true);
        m_pathMap[(thermalZone.name().get()+" return")] = rp.nr();
        // Add the paths to the path list
        m_prjModel.addAirflowPath(sp);
        m_prjModel.addAirflowPath(rp);
      }
      m_prjModel.addAhs(ahs);
      progress();
    }

    initProgress(m_prjModel.ahs().size(),"Connecting AHS to zones");
    // Now loop back through the AHS list and connect the supply and return zones together
    for(unsigned int i=0;i<m_prjModel.ahs().size();i++)
    {
      std::string loopName = std::string("AHS_") + std::to_string(i+1);
      // Recirculation path
      openstudio::contam::AirflowPath recirc;
      recirc.setNr(m_prjModel.airflowPaths().size()+1);
      recirc.setPld(1);
      // Set the OA fraction schedule here
      //recirc.ps = ?
      recirc.setPzn(m_prjModel.ahs()[i].zone_r());
      recirc.setPzm(m_prjModel.ahs()[i].zone_s());
      recirc.setRecirculation(true);
      m_pathMap[loopName + " recirculation"] = recirc.nr();
      // Outside air path
      openstudio::contam::AirflowPath oa;
      oa.setNr(recirc.nr()+1);
      oa.setPld(1);
      oa.setPzn(-1);
      oa.setPzm(m_prjModel.ahs()[i].zone_s());
      oa.setOutsideAir(true);
      m_pathMap[loopName + " oa"] = oa.nr();
      // Exhaust path;
      openstudio::contam::AirflowPath exhaust;
      exhaust.setNr(oa.nr()+1);
      exhaust.setPld(1);
      exhaust.setPzn(m_prjModel.ahs()[i].zone_r());
      exhaust.setPzm(-1);
      exhaust.setExhaust(true);
      m_pathMap[loopName + " exhaust"] = exhaust.nr();
      // Add the paths to the path list
      m_prjModel.addAirflowPath(recirc);
      m_prjModel.addAirflowPath(oa);
      m_prjModel.addAirflowPath(exhaust);
      // Store the nrs in the ahs
      m_prjModel.ahs()[i].setPath_r(recirc.nr());
      m_prjModel.ahs()[i].setPath_s(oa.nr());
      m_prjModel.ahs()[i].setPath_x(exhaust.nr());
      progress();
    }

    // The rest of this isn't hooked into the progress bar yet, will need to do that at some point

    // Try to use E+ results to set temperatures and flow rates. The supply and return flow paths are in the path
    // lookup table under the names thermalZone.name + supply|return (see above)
    boost::optional<openstudio::SqlFile> sqlFile = model.sqlFile();
    if(sqlFile) {
      std::vector<std::string> available = sqlFile->availableTimeSeries();
      //for (std::string var : available)
      //{
      //  std::cout << '\t' << var << std::endl;
      //}
      std::string envPeriod;
      for (std::string t : sqlFile->availableEnvPeriods()) {
        envPeriod = t; // should only ever be one
        break;
      }
      // bool setTime=false;
      if(std::find(available.begin(), available.end(), "Zone Mean Air Temperature")!=available.end()) {
        // Loop through and get a time series for each zone we can find
        for (model::ThermalZone thermalZone : model.getConcreteModelObjects<model::ThermalZone>()) {
          boost::optional<std::string> name = thermalZone.name();
          if(!name) {
            LOG(Warn,"Zone " << openstudio::toString(thermalZone.handle()) << " has no name and will have constant temperature");
            continue;
          }
          std::string keyValue = name.get();
          keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
          boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly",
            "Zone Mean Air Temperature", keyValue);
          if(timeSeries) {
            int nr =  m_zoneMap[thermalZone.handle()];
            // std::cout << "Found time series for zone " << name.get() << ", CONTAM index " << nr << std::endl;
            // Create a control node
            std::string controlName = std::string("ctrl_z_")+std::to_string(nr);
            std::string valueName = std::string("temp_")+std::to_string(nr);
            m_cvf.addTimeSeries(valueName,*timeSeries);
            CvfDat ctrl;
            ctrl.setName(controlName);
            ctrl.setValuename(valueName);
            m_prjModel.addControlNode(ctrl);
            // Connect to the zone
            m_prjModel.zones()[nr - 1].setPc(ctrl.nr());
          } else {
            LOG(Warn,"Zone '" << name.get() << "' has no Zone Mean Air Temperature time series");
          }
        }
      } else {
        LOG(Warn, "Activate \"Zone Mean Air Temperature\" output to set zone temperature controls.");
      }
      if(std::find(available.begin(), available.end(), "System Node MassFlowRate")!=available.end()) {
        LOG(Warn, "Zone equipment not yet accounted for.");
        // get sizing results, get flow rate schedules for each zone's inlet, return, and exhaust nodes
        // This should be moved to inside the contam translator
        for (model::ThermalZone thermalZone : model.getConcreteModelObjects<model::ThermalZone>()) {
          // todo: this does not include OA from zone equipment (PTAC, PTHP, etc) or exhaust fans

          boost::optional<model::Node> supplyAirNode;
          boost::optional<model::ModelObject> supplyAirModelObject = thermalZone.inletPortList().airLoopHVACModelObject();
          if (supplyAirModelObject) {
            supplyAirNode = supplyAirModelObject->optionalCast<model::Node>();
          }
          if (supplyAirNode) {
            std::string keyValue = supplyAirNode->name().get();
            keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
            boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly",
              "System Node MassFlowRate", keyValue);
            if (timeSeries) {
              //std::cout << "Found time series for supply to zone " << thermalZone.name().get() << std::endl;
              nr = 0;
              const auto& it = m_pathMap.find(thermalZone.name().get() + " supply");
              if (it != m_pathMap.end()) {
                nr = it->second;
              }
              // There really should not be a case of missing number here, but it is better to be safe
              if(!nr) {
                LOG(Error,"Supply node for zone '" << thermalZone.name().get() << "' has no associated CONTAM path");
                continue;
              }
              // Create a control node
              std::string controlName = std::string("ctrl_p_")+std::to_string(nr);
              std::string valueName = std::string("supply_")+std::to_string(nr);
              m_cvf.addTimeSeries(valueName,*timeSeries);
              CvfDat ctrl;
              ctrl.setName(controlName);
              ctrl.setValuename(valueName);
              m_prjModel.addControlNode(ctrl);
              // Connect to the path
              m_prjModel.airflowPaths()[nr-1].setPc(ctrl.nr());
              if(m_ratioOverride) { // This assumes that there *is* a return, which could be wrong? maybe?
                // Create a new time series
                TimeSeries returnSeries = (*timeSeries)*m_returnSupplyRatio;
                nr = 0;
                const auto& it = m_pathMap.find(thermalZone.name().get() + " return");
                if (it != m_pathMap.end()) {
                  nr = it->second;
                }
                // There really should not be a case of missing number here, but it is better to be safe
                if(!nr) {
                  LOG(Error,"Failed to find return path for zone '" << thermalZone.name().get() << "'");
                  continue;
                }
                // Create a control node
                std::string controlName = std::string("ctrl_p_")+std::to_string(nr);
                std::string valueName = std::string("return_")+std::to_string(nr);
                m_cvf.addTimeSeries(valueName,*timeSeries);
                CvfDat ctrl;
                ctrl.setName(controlName);
                ctrl.setValuename(valueName);
                m_prjModel.addControlNode(ctrl);
                // Connect to the path
                m_prjModel.airflowPaths()[nr-1].setPc(ctrl.nr());
              }
            }
          }

          if(!m_ratioOverride) {
            boost::optional<model::Node> returnAirNode;
            boost::optional<model::ModelObject> returnAirModelObject = thermalZone.returnAirModelObject();
            if (returnAirModelObject) {
              returnAirNode = returnAirModelObject->optionalCast<model::Node>();
            }
            if (returnAirNode) {
              std::string keyValue = returnAirNode->name().get();
              keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
              boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly",
                "System Node MassFlowRate", keyValue);
              if (timeSeries) {
                //std::cout << "Found time series for return from zone " << thermalZone.name().get() << std::endl;
                nr = 0;
                const auto& it = m_pathMap.find(thermalZone.name().get() + " return");
                if (it != m_pathMap.end()) {
                  nr = it->second;
                }
                // There really should not be a case of missing number here, but it is better to be safe
                if(!nr) {
                  LOG(Error,"Return node for zone '" << thermalZone.name().get() << "' has no associated CONTAM path");
                  continue;
                }
                // Create a control node
                std::string controlName = std::string("ctrl_p_")+std::to_string(nr);
                std::string valueName = std::string("return_")+std::to_string(nr);
                m_cvf.addTimeSeries(valueName,*timeSeries);
                CvfDat ctrl;
                ctrl.setName(controlName);
                ctrl.setValuename(valueName);
                m_prjModel.addControlNode(ctrl);
                // Connect to the path
                m_prjModel.airflowPaths()[nr - 1].setPc(ctrl.nr());
              }
            }
          }
        }
      } else {
        LOG(Warn, "Activate \"System Node MassFlowRate\" output to set zone supply/return flows.");
      }
    } else {
      LOG(Warn, "Simulation results not available, using 1 scfm/ft^2 to set supply flows");
      // Use the 1 scfm/ft^2 approximation with 90% return
      for (openstudio::model::ThermalZone thermalZone : model.getConcreteModelObjects<openstudio::model::ThermalZone>()) {
        double area=0.0;
        for (openstudio::model::Space space : thermalZone.spaces()) {
          area += space.floorArea();
        }
        if(area == 0.0) {
          LOG(Warn, "Failed to compute floor area for Zone '" << thermalZone.name().get() << "'");
        } else {
          double flowRate = area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation
          std::string supplyName = thermalZone.name().get() + " supply";
          std::string returnName = thermalZone.name().get() + " return";
          int supplyNr = 0;
          const auto& its = m_pathMap.find(supplyName);
          if (its != m_pathMap.end()) {
            supplyNr = its->second;
          }
          if(supplyNr) {
            m_prjModel.airflowPaths()[supplyNr - 1].setFahs(std::to_string(flowRate));
          }

          int returnNr = 0;
          const auto& itr = m_pathMap.find(returnName);
          if (itr != m_pathMap.end()) {
            returnNr = itr->second;
          }
          if(returnNr) {
            m_prjModel.airflowPaths()[returnNr - 1].setFahs(std::to_string(m_returnSupplyRatio*flowRate));
          }
        }
      }
    }
  }

  return boost::optional<contam::IndexModel>(m_prjModel);

  // these are probably useful, will have to ask Kyle
  // Kyle, should these functions be const?
  //boost::optional<model::ModelObject> airInletModelObject = zone.airInletModelObject();
  //boost::optional<model::ModelObject> returnAirModelObject = zone.returnAirModelObject();
  //boost::optional<model::Node> exhaustAirNode = zone.exhaustAirNode();
  //boost::optional<model::Node> zoneAirNode = zone.zoneAirNode();

  //ZoneData *afz = zoneList.at(i);
  //double flowRate = afz->area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation

}

static double laminarCoefficient(double Ct, double x)
{
  // Ct  turbulent flow coefficient
  // x   exponent
  double Clam;                // laminar flow coefficient
  double F;                   // mass flow rate at L-T transition
  double dP=0;                // pressure drop at L-T transition
  double RHOAIR = 1.20410;    // density of standard air
  double SRHO = 1.097315;     // sqrt( RHOAIR )
  double SQRT2 = 1.414213562; // sqrt( 2.0 )
  double MUAIR = 1.81625e-5;  // viscosity of standard air
  double DPTMIN = 1.0e-10;    // minimum L-T transition dP
  double Re = 30.0;           // Reynolds number of L-T transition

  double A = Ct/(0.6*SQRT2);  // flow area
  double D = sqrt(A);         // hydraulic diameter

  // Re = rho * V * D / mu; F = rho * V * A; therefore
  F = MUAIR * Re * A / D;
  // F = Ct * sqrt(rho) * dP^x
  dP = std::pow( F / (Ct * SRHO), 1.0 / x );
  if( dP < DPTMIN )
  {
    dP = DPTMIN;   // 1998/07/17 - dPt > 0
  }
  // F = Clam * (rho / mu) * dP
  Clam = ( (MUAIR * F) / (RHOAIR * dP) );

  return Clam;

}

int ForwardTranslator::addNewAirflowElement(contam::IndexModel model,std::string name, double flow,double n,double deltaP)
{
  // flow - volume flow rate in m^3/h
  // deltaP - pressure difference in Pa
  // n - exponent

  double RHOAIR = 1.20410;    // density of standard air
  double SRHO = 1.097315;     // sqrt( RHOAIR )

  double F = RHOAIR*flow/3600.0;  // mass flow in kg/s

  double Ct = F/(SRHO*std::pow(deltaP,n));
  double Cl = laminarCoefficient(Ct,n);

  std::string lam = std::to_string(Cl);
  std::string turb = std::to_string(Ct);
  std::string expt = std::to_string(n);
  std::string dP = std::to_string(deltaP);
  std::string Flow = std::to_string(F);
  int u_P = 0;
  int u_F = 1; // Display units are m^3/h

  // Create a 1-point test element with display units of m^3/h
  PlrTest1 afe(0, 0, name, " ", lam, turb, expt, dP, Flow, u_P, u_F);

  model.addAirflowElement(afe);

  return afe.nr();
}

bool ForwardTranslator::writeCvFile(openstudio::path filepath)
{
  if(!m_cvf.isEmpty())
  {
    return m_cvf.write(filepath);
  }
  return false;
}

bool ForwardTranslator::linkExteriorSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface)
{
  contam::AirflowPath path;
  // Use the lookup table to get the zone info
  int zoneNr;
  if (!(zoneNr = tableLookup(m_zoneMap, zone.handle(), "zoneMap"))) {
    // Maybe this needs a warning?
    return false;
  }
  contam::Zone airflowZone = m_prjModel.zones()[zoneNr - 1];
  // Get the surface area - will need to do more work here later if large openings are present
  double area = surface.grossArea();
  std::string type = surface.surfaceType();
  double averageZ = 0;
  double numVertices = (double)surface.vertices().size();
  for(const Point3d& point : surface.vertices()) {
    averageZ += point.z();
  }
  // Now set the path info
  path.setRelHt(averageZ / numVertices - m_prjModel.levels()[airflowZone.pl() - 1].refht());
  path.setPld(airflowZone.pl());
  path.setMult(area);
  // Make an exterior flow path
  path.setPzn(airflowZone.nr());
  path.setPzm(-1);
  // Set the wind-related stuff here
  path.setWazm(openstudio::radToDeg(surface.azimuth()));
  path.setWindPressure(true);
  path.setWPmod(openstudio::wind::pressureModifier(openstudio::wind::Default, m_prjModel.wind_H()));
  path.setPw(4); // Assume standard template
  // Set flow element
  if (type == "RoofCeiling") {
    path.setPe(m_afeMap["roof"]);
    path.setPw(5); // Assume standard template
  }
  else {
    path.setPe(m_afeMap["exterior"]);
  }
  m_prjModel.addAirflowPath(path);
  m_surfaceMap[surface.handle()] = path.nr();
  return true;
}

bool ForwardTranslator::linkInteriorSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface,
  openstudio::model::Surface adjacentSurface, openstudio::model::Space adjacentSpace, openstudio::model::ThermalZone adjacentZone)
{
  contam::AirflowPath path;
  // Use the lookup table to get the zone info
  int zoneNr;
  if (!(zoneNr = tableLookup(m_zoneMap, zone.handle(), "zoneMap"))) {
    // Maybe this needs a warning?
    return false;
  }
  contam::Zone airflowZone = m_prjModel.zones()[zoneNr - 1];
  // Get the surface area - will need to do more work here later if large openings are present
  double area = 0.5*(surface.grossArea() + adjacentSurface.grossArea());
  std::string type = surface.surfaceType();
  double averageZ = 0;
  double numVertices = (double)surface.vertices().size();
  for(const Point3d& point : surface.vertices()) {
    averageZ += point.z();
  }
  // Now set the path info
  path.setRelHt(averageZ / numVertices - m_prjModel.levels()[airflowZone.pl() - 1].refht());
  path.setPld(airflowZone.pl());
  path.setMult(area);

  // Make an interior flow path
  path.setPzn(airflowZone.nr());
  path.setPzm(m_zoneMap[adjacentZone.handle()]);
  // Set flow element
  if (type == "Floor" || type == "RoofCeiling") {
    path.setPe(m_afeMap["floor"]);
  }
  else {
    path.setPe(m_afeMap["interior"]);
  }
  m_prjModel.addAirflowPath(path);
  m_surfaceMap[surface.handle()] = path.nr();

  return true;
}

bool ForwardTranslator::linkExteriorSubSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface,
  openstudio::model::SubSurface subSurface)
{
  // Not doing this yet
  return true;
}

bool ForwardTranslator::linkInteriorSubSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface,
  openstudio::model::SubSurface subSurface, openstudio::model::SubSurface adjacentSubSurface, openstudio::model::Surface adjacentSurface, openstudio::model::Space adjacentSpace,
  openstudio::model::ThermalZone adjacentZone)
{
  // Not doing this yet
  return true;
}

std::vector<LogMessage> ForwardTranslator::warnings() const
{
  std::vector<LogMessage> result;

  for (LogMessage logMessage : m_logSink.logMessages())
  {
    if (logMessage.logLevel() == Warn)
    {
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> ForwardTranslator::errors() const
{
  std::vector<LogMessage> result;

  for (LogMessage logMessage : m_logSink.logMessages())
  {
    if (logMessage.logLevel() > Warn)
    {
      result.push_back(logMessage);
    }
  }

  return result;
}

} // contam
} // openstudio
