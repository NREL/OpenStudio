/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include <openstudio_lib/BuildingComponentController.hpp>

namespace openstudio {

BuildingComponentController::BuildingComponentController()
  : QObject()
{
  std::vector<QString> siteTidVector;
  siteTidVector.push_back(QString("tid1"));
  siteTidVector.push_back(QString("tid2"));
  siteTidVector.push_back(QString("tid3"));
  m_tabTidbMap[OSDocument::SITE] = siteTidVector;

  std::vector<QString> schedulesTidVector;
  m_tabTidbMap[OSDocument::SCHEDULES] = schedulesTidVector;

  std::vector<QString> constructionsTidVector;
  m_tabTidbMap[OSDocument::CONSTRUCTIONS] = constructionsTidVector;

  std::vector<QString> loadsTidVector;
  m_tabTidbMap[OSDocument::LOADS] = loadsTidVector;

  std::vector<QString> spaceTypesTidVector;
  m_tabTidbMap[OSDocument::SPACE_TYPES] = spaceTypesTidVector;

  std::vector<QString> buildingStoriesTidVector;
  m_tabTidbMap[OSDocument::BUILDING_STORIES] = buildingStoriesTidVector;

  std::vector<QString> facilityTidVector;
  m_tabTidbMap[OSDocument::FACILITY] = facilityTidVector;

  std::vector<QString> thermalZonesTidVector;
  m_tabTidbMap[OSDocument::THERMAL_ZONES] = thermalZonesTidVector;

  std::vector<QString> hvacSystemsTidVector;
  m_tabTidbMap[OSDocument::HVAC_SYSTEMS] = hvacSystemsTidVector;

  std::vector<QString> buildingSummaryTidVector;
  m_tabTidbMap[OSDocument::BUILDING_SUMMARY] = buildingSummaryTidVector;

  std::vector<QString> outputVariablesTidVector;
  m_tabTidbMap[OSDocument::OUTPUT_VARIABLES] = outputVariablesTidVector;

  std::vector<QString> simulationSettingsTidVector;
  m_tabTidbMap[OSDocument::SIMULATION_SETTINGS] = simulationSettingsTidVector;

  std::vector<QString> rubyScriptsTidVector;
  m_tabTidbMap[OSDocument::RUBY_SCRIPTS] = rubyScriptsTidVector;

  std::vector<QString> runSimulationsTidVector;
  m_tabTidbMap[OSDocument::RUN_SIMULATION] = runSimulationsTidVector;

  std::vector<QString> resultsSummaryTidVector;
  m_tabTidbMap[OSDocument::RESULTS_SUMMARY] = resultsSummaryTidVector;
}

} // openstudio

