/**********************************************************************
*  Copyright (c) 2013, The Pennsylvania State University.
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
#include "PrjModel.hpp"
#include "PrjReader.hpp"
#include "SimFile.hpp"
#include <QFile>

namespace openstudio {
namespace contam {

Model::Model(STRING filename)
{
  read(filename);
}

Model::Model(Reader &input)
{
  read(input);
}

bool Model::read(STRING filename)
{
#ifdef STD_STRING
  QFile fp(QString().fromStdString(filename));
#else
  QFile fp(filename);
#endif

  m_valid = false;
  if (fp.open(QFile::ReadOnly))
  {
    QTextStream stream(&fp);
    Reader input(&stream);
    read(input);

  }
  return m_valid;
}

bool Model::read(Reader &input)
{
  m_valid = false;
  // Section 1: Project, Weather, Simulation, and Output Controls
  m_rc.read(input); // Read the run control section
  input.read999(FILELINE);
  // Section 2: Species and Contaminants
  m_contaminants = input.readIntVector(FILELINEC false);
  m_species = input.readSectionVector<Species>(FILELINEC "species");
  // Section 3: Level and Icon Data
  m_levels = input.readSectionVector<Level>(FILELINEC "level");
  // Section 4: Day Schedules
  m_daySchedules = input.readSectionVector<DaySchedule>(FILELINEC "day schedule");
  // Section 5: Week Schedules
  m_weekSchedules = input.readSectionVector<WeekSchedule>(FILELINEC "week schedule");
  // Section 6: Wind Pressure Profiles
  m_windPressureProfiles = input.readSectionVector<WindPressureProfile>(FILELINEC "wind pressure profiles");
  // Section 7: Kinetic Reactions
  STRING kinr = input.readSection(FILELINE); // Skip it
  m_unsupported["KineticReaction"] = kinr;
  // Section 8a: Filter Elements
  STRING flte = input.readSection(FILELINE); // Skip it
  m_unsupported["FilterElement"] = flte;
  // Section 8b: Filters
  STRING filt = input.readSection(FILELINE); // Skip it
  m_unsupported["Filter"] = filt;
  // Section 9: Source/Sink Elements
  STRING cse = input.readSection(FILELINE); // Skip it
  m_unsupported["SourceSink"] = cse;
  // Section 10: Airflow Elements
  m_airflowElements = input.readElementVector<AirflowElement>(FILELINEC "airflow element");
  // Section 11: Duct Elements
  STRING dfe = input.readSection(FILELINE); // Skip it
  m_unsupported["DuctElement"] = dfe;
  // Section 12a: Control Super Elements
  STRING selmt = input.readSection(FILELINE); // Skip it
  m_unsupported["ControlSuperElements"] = selmt;
  // Section 12b: Control Nodes
  //STRING ctrl = input.readSection(FILELINE); // Skip it
  //m_unsupported["ControlNode"] = ctrl;
  m_controlNodes = input.readElementVector<ControlNode>(FILELINEC "control node");
  // Section 13: Simple Air Handling System (AHS)
  m_ahs = input.readSectionVector<Ahs>(FILELINEC "ahs");
  // Section 14: Zones
  m_zones = input.readSectionVector<Zone>(FILELINEC "zone");
  // Section 15: Initial Zone Concentrations
  readZoneIc(input);
  // Section 16: Airflow Paths
  m_paths = input.readSectionVector<Path>(FILELINEC "path");
  // Section 17: Duct Junctions
  STRING jct = input.readSection(FILELINE); // Skip it
  m_unsupported["DuctJunction"] = jct;
  // Section 18: Initial Junction Concentrations
  STRING jctic = input.readSection(FILELINE); // Skip it
  m_unsupported["JunctionIC"] = jctic;
  // Section 19: Duct Segments
  STRING dct = input.readSection(FILELINE); // Skip it
  m_unsupported["DuctSegment"] = dct;
  // Section 20: Source/Sinks
  //m_sourceSinks = input.readSectionVector<SourceSink>(FILELINEC QString("source/sink"));
  STRING css = input.readSection(FILELINE); // Skip it
  m_unsupported["SourceSink"] = css;
  // Section 21: Occupancy Schedules
  STRING osch = input.readSection(FILELINE); // Skip it
  m_unsupported["OccupancySchedule"] = osch;
  // Section 22: Exposures
  STRING pexp = input.readSection(FILELINE); // Skip it
  m_unsupported["Exposure"] = pexp;
  // Section 23: Annotations
  STRING note = input.readSection(FILELINE); // Skip it
  m_unsupported["Annotation"] = note;
  input.readEnd(FILELINE);
  m_valid = true;
  return true;
}

STRING Model::toString()
{
  STRING output;
  if(!m_valid)
  {
    return output;
  }
  // Section 1: Project, Weather, Simulation, and Output Controls
  output += m_rc.write();
  output += "-999\n";
  // Section 2: Species and Contaminants
  output += writeArray(m_contaminants,"contaminants:");
  output += writeSectionVector(m_species,"species:");
  // Section 3: Level and Icon Data
  output += writeSectionVector(m_levels,"levels:");
  // Section 4: Day Schedules
  output += writeSectionVector(m_daySchedules,"day-schedules:");
  // Section 5: Week Schedules
  output += writeSectionVector(m_weekSchedules,"week-schedules:");
  // Section 6: Wind Pressure Profiles
  output += writeSectionVector(m_windPressureProfiles,"wind pressure profiles:");
  // Section 7: Kinetic Reactions
  output += m_unsupported["KineticReaction"];
  // Section 8a: Filter Elements
  output += m_unsupported["FilterElement"];
  // Section 8b: Filters
  output += m_unsupported["Filter"];
  // Section 9: Source/Sink Elements
  output += m_unsupported["SourceSink"];
  // Section 10: Airflow Elements
  output += writeSectionVector(m_airflowElements,"flow elements:");
  // Section 11: Duct Elements
  output += m_unsupported["DuctElement"];
  // Section 12a: Control Super Elements
  output += m_unsupported["ControlSuperElements"];
  // Section 12b: Control Nodes
  //output += m_unsupported["ControlNode"];
  output += writeSectionVector(m_controlNodes,"control nodes:");
  // Section 13: Simple Air Handling System (AHS)
  output += writeSectionVector(m_ahs,"simple AHS:");
  // Section 14: Zones
  output += writeSectionVector(m_zones,"zones:");
  // Section 15: Initial Zone Concentrations
  output += writeZoneIc();
  // Section 16: Airflow Paths
  output += writeSectionVector(m_paths,"flow paths:");
  // Section 17: Duct Junctions
  output += m_unsupported["DuctJunction"];
  // Section 18: Initial Junction Concentrations
  output += m_unsupported["JunctionIC"];
  // Section 19: Duct Segments
  output += m_unsupported["DuctSegment"];
  // Section 20: Source/Sinks
  output += m_unsupported["SourceSink"];
  // Section 21: Occupancy Schedules
  output += m_unsupported["OccupancySchedule"];
  // Section 22: Exposures
  output += m_unsupported["Exposure"];
  // Section 23: Annotations
  output += m_unsupported["Annotation"];
  // End of the PRJ file
  output += "* end project file.";
  return output;
}

std::vector<std::vector<int> > Model::zoneExteriorFlowPaths()
{
  std::vector<std::vector<int> > paths(m_zones.size());

  for(unsigned int i=0;i<m_paths.size();i++)
  {
    if(m_paths[i].pzn() == -1)
    {
      int nr = m_paths[i].pzm();
      if(nr > 0 && (unsigned int)nr<=m_zones.size())
      {
        paths[nr-1].push_back(-m_paths[i].nr()); // This flow path is negative for flow into zone
      }
    }
    else if(m_paths[i].pzm() == -1)
    {
      int nr = m_paths[i].pzn();
      if(nr > 0 && (unsigned int)nr<=m_zones.size())
      {
        paths[nr-1].push_back(m_paths[i].nr()); // This flow path is positive for flow into zone
      }
    }
  }
  return paths;
}

std::vector<TimeSeries> Model::zoneInfiltration(SimFile *sim)
{
  // This should probably include a lot more checks of things and is written in
  // somewhat strange way to avoid taking too much advantage of the specifics 
  // of the text form outputs.
  std::vector<TimeSeries> results;
  //std::vector<std::vector<double> > flow0 = sim->F0();
  std::vector<std::vector<int> > paths = zoneExteriorFlowPaths();
  unsigned int ntimes = sim->dateTimes().size();
  for(unsigned int i=0; i<m_zones.size(); i++)
  {
    // This is lame, but I can't tell for sure if the values of a Vector are actually zero.
    Vector inf = createVector(std::vector<double>(ntimes,0));
    for(unsigned int j=0; j<paths[i].size(); j++)
    {
      if(paths[i][j] > 0) // Positive values are infiltration
      {
        boost::optional<openstudio::TimeSeries> optFlow = sim->pathFlow(paths[i][j]);
        if(optFlow)
        {
          Vector flow = optFlow.get().values();
          for(unsigned int k=0; k<ntimes; k++)
          {
            if(flow[k] > 0)
            {
              inf[k] += flow[k];
            }
          }
        }
        else
        {
          // Perhaps a warning? This shouldn't really happen unless someone has excluded a path from the 
          // results file for some reason - which unlikely to be accidental. So there must be a good reason
          // for getting here, and for now we won't issue a warning.
        }
      }
      else // Negative values are infiltration
      {
        boost::optional<openstudio::TimeSeries> optFlow = sim->pathFlow(paths[i][j]);
        if(optFlow)
        {
          Vector flow = optFlow.get().values();
          for(unsigned int k=0; k<ntimes; k++)
          {
            if(flow[k] < 0)
            {
              inf[k] -= flow[k];
            }
          }
        }
        else
        {
          // See above comment
        }
      }
    }
    /*
    // Run through the times and compute the infiltration
    for(unsigned int j=0; j<ntimes; j++)
    {
      for(unsigned int k=0; k<extPaths.size(); k++)
      {
        int nr = paths[i][k];
        if(nr > 0) // Positive value is infiltration
        {
          nr -= 1;
          if(flow0[nr][j] > 0)
          {
            inf[j] += flow0[nr][j];
          }
        }
        else // Negative value is infiltration
        {
          nr = -nr - 1;
          if(flow0[nr][j] < 0)
          {
            inf[j] -= flow0[nr][j];
          }
        }
      }
    }*/
    results.push_back(openstudio::TimeSeries(sim->dateTimes(),inf,"kg/s"));
  }
  return results;
}

void Model::rebuildContaminants()
{
  m_contaminants.clear();
  for(unsigned int i=1;i<=m_species.size();i++)
  {
    m_species[i].setNr(i);
    if(m_species[i].sflag())
    {
      m_contaminants.push_back(i);
    }
  }
}

void Model::readZoneIc(Reader &input)
{
  unsigned int nn = input.readUInt(FILELINE);
  if(nn != 0)
  {
    unsigned int nctm = m_contaminants.size();
    if(nn != nctm*m_zones.size())
    {
      QString mesg("Mismatch between number of zones, contaminants, and initial conditions");
#ifndef NOFILELINE
      mesg +=  QString(" (%1,%2)").arg(__FILE__).arg(__LINE__);
#endif
      ERROR(mesg.toStdString());
    }
    for(unsigned int i=0;i<m_zones.size();i++)
    {
      unsigned int nr = input.readUInt(FILELINE);
      if(nr != i+1)
      {
        QString mesg = QString("Mismatch between zone IC number and zone number at line %1 ")
          .arg(input.lineNumber());
#ifndef NOFILELINE
        mesg +=  QString(" (%1,%2)").arg(__FILE__).arg(__LINE__);
#endif
        ERROR(mesg.toStdString());
      }
      VECTOR_TYPE<RX> ic;
      for(unsigned int j=0;j<nctm;j++)
      {
        ic.push_back(input.readNumber<RX>(FILELINE));
      }
      m_zones[i].setIc(ic);
    }
  }
  input.read999("Failed to find zone IC section termination" CFILELINE);
}

STRING Model::writeZoneIc(int start)
{
  int offset = 1;
  if(start != 0)
  {
    offset = 1-start;
  }
  int nctm = m_contaminants.size()*(m_zones.size()-start);
  STRING string = TO_STRING(nctm) + " ! initial zone concentrations:\n";
  if(nctm)
  {
    for(unsigned int i=start;i<m_zones.size();i++)
    {
      string += TO_STRING(i+offset);
      for(unsigned int j=0;j<m_contaminants.size();j++)
      {
        string += ' ' + TO_STRING(m_zones[i].ic(j));
      }
      string += '\n';
    }
  }
  return string  + "-999\n";
}

//template <class T> void Model::addAirflowElement(T element)
//{
//    T *copy = new T;
//    *copy = element;
//    copy->setNr(m_airflowElements.size()+1);
//    m_airflowElements.push_back(QSharedPointer<AirflowElement>((AirflowElement*)copy));
//}

//template void Model::addAirflowElement(PlrTest1);

int Model::airflowElementNrByName(STRING name) const
{
  for(int i=0;i<m_airflowElements.size();i++)
  {
    if(m_airflowElements[i]->name() == name)
    {
      return m_airflowElements[i]->nr();
    }
  }
  return 0;
}

} // contam
} // openstudio
