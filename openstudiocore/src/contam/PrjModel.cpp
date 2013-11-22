/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
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

CxModel::CxModel(openstudio::path path)
{
  read(path);
}

CxModel::CxModel(std::string filename)
{
  read(filename);
}

CxModel::CxModel(Reader &input)
{
  read(input);
}

bool CxModel::read(openstudio::path path)
{
  return read(openstudio::toString(path));
}

bool CxModel::read(std::string filename)
{
  QFile fp(QString().fromStdString(filename));

  d->valid = false;
  if (fp.open(QFile::ReadOnly))
  {
    QTextStream stream(&fp);
    Reader input(&stream);
    read(input);

  }
  return d->valid;
}

bool CxModel::read(Reader &input)
{
  d->valid = false;
  // Section 1: Project, Weather, Simulation, and Output Controls
  d->rc.read(input); // Read the run control section
  input.read999(FILELINE);
  // Section 2: Species and Contaminants
  d->contaminants = input.readIntVector(FILELINEC false);
  d->species = input.readSectionVector<Species>(FILELINEC "species");
  // Section 3: Level and Icon Data
  d->levels = input.readSectionVector<Level>(FILELINEC "level");
  // Section 4: Day Schedules
  d->daySchedules = input.readSectionVector<DaySchedule>(FILELINEC "day schedule");
  // Section 5: Week Schedules
  d->weekSchedules = input.readSectionVector<WeekSchedule>(FILELINEC "week schedule");
  // Section 6: Wind Pressure Profiles
  d->windPressureProfiles = input.readSectionVector<WindPressureProfile>(FILELINEC "wind pressure profiles");
  // Section 7: Kinetic Reactions
  std::string kinr = input.readSection(FILELINE); // Skip it
  d->unsupported["KineticReaction"] = kinr;
  // Section 8a: Filter Elements
  std::string flte = input.readSection(FILELINE); // Skip it
  d->unsupported["FilterElement"] = flte;
  // Section 8b: Filters
  std::string filt = input.readSection(FILELINE); // Skip it
  d->unsupported["Filter"] = filt;
  // Section 9: Source/Sink Elements
  std::string cse = input.readSection(FILELINE); // Skip it
  d->unsupported["SourceSink"] = cse;
  // Section 10: Airflow Elements
  d->airflowElements = input.readElementVector<AirflowElement>(FILELINEC "airflow element");
  // Section 11: Duct Elements
  std::string dfe = input.readSection(FILELINE); // Skip it
  d->unsupported["DuctElement"] = dfe;
  // Section 12a: Control Super Elements
  std::string selmt = input.readSection(FILELINE); // Skip it
  d->unsupported["ControlSuperElements"] = selmt;
  // Section 12b: Control Nodes
  //std::string ctrl = input.readSection(FILELINE); // Skip it
  //m_unsupported["ControlNode"] = ctrl;
  d->controlNodes = input.readElementVector<ControlNode>(FILELINEC "control node");
  // Section 13: Simple Air Handling System (AHS)
  d->ahs = input.readSectionVector<Ahs>(FILELINEC "ahs");
  // Section 14: Zones
  d->zones = input.readSectionVector<Zone>(FILELINEC "zone");
  // Section 15: Initial Zone Concentrations
  readZoneIc(input);
  // Section 16: Airflow Paths
  d->paths = input.readSectionVector<Path>(FILELINEC "path");
  // Section 17: Duct Junctions
  std::string jct = input.readSection(FILELINE); // Skip it
  d->unsupported["DuctJunction"] = jct;
  // Section 18: Initial Junction Concentrations
  std::string jctic = input.readSection(FILELINE); // Skip it
  d->unsupported["JunctionIC"] = jctic;
  // Section 19: Duct Segments
  std::string dct = input.readSection(FILELINE); // Skip it
  d->unsupported["DuctSegment"] = dct;
  // Section 20: Source/Sinks
  //m_sourceSinks = input.readSectionVector<SourceSink>(FILELINEC QString("source/sink"));
  std::string css = input.readSection(FILELINE); // Skip it
  d->unsupported["SourceSink"] = css;
  // Section 21: Occupancy Schedules
  std::string osch = input.readSection(FILELINE); // Skip it
  d->unsupported["OccupancySchedule"] = osch;
  // Section 22: Exposures
  std::string pexp = input.readSection(FILELINE); // Skip it
  d->unsupported["Exposure"] = pexp;
  // Section 23: Annotations
  std::string note = input.readSection(FILELINE); // Skip it
  d->unsupported["Annotation"] = note;
  input.readEnd(FILELINE);
  d->valid = true;
  return true;
}

std::string CxModel::toString()
{
  std::string output;
  if(!d->valid)
  {
    return output;
  }
  // Section 1: Project, Weather, Simulation, and Output Controls
  output += d->rc.write();
  output += "-999\n";
  // Section 2: Species and Contaminants
  output += writeArray(d->contaminants,"contaminants:");
  output += writeSectionVector(d->species,"species:");
  // Section 3: Level and Icon Data
  output += writeSectionVector(d->levels,"levels:");
  // Section 4: Day Schedules
  output += writeSectionVector(d->daySchedules,"day-schedules:");
  // Section 5: Week Schedules
  output += writeSectionVector(d->weekSchedules,"week-schedules:");
  // Section 6: Wind Pressure Profiles
  output += writeSectionVector(d->windPressureProfiles,"wind pressure profiles:");
  // Section 7: Kinetic Reactions
  output += d->unsupported["KineticReaction"];
  // Section 8a: Filter Elements
  output += d->unsupported["FilterElement"];
  // Section 8b: Filters
  output += d->unsupported["Filter"];
  // Section 9: Source/Sink Elements
  output += d->unsupported["SourceSink"];
  // Section 10: Airflow Elements
  output += writeSectionVector(d->airflowElements,"flow elements:");
  // Section 11: Duct Elements
  output += d->unsupported["DuctElement"];
  // Section 12a: Control Super Elements
  output += d->unsupported["ControlSuperElements"];
  // Section 12b: Control Nodes
  //output += m_unsupported["ControlNode"];
  output += writeSectionVector(d->controlNodes,"control nodes:");
  // Section 13: Simple Air Handling System (AHS)
  output += writeSectionVector(d->ahs,"simple AHS:");
  // Section 14: Zones
  output += writeSectionVector(d->zones,"zones:");
  // Section 15: Initial Zone Concentrations
  output += writeZoneIc();
  // Section 16: Airflow Paths
  output += writeSectionVector(d->paths,"flow paths:");
  // Section 17: Duct Junctions
  output += d->unsupported["DuctJunction"];
  // Section 18: Initial Junction Concentrations
  output += d->unsupported["JunctionIC"];
  // Section 19: Duct Segments
  output += d->unsupported["DuctSegment"];
  // Section 20: Source/Sinks
  output += d->unsupported["SourceSink"];
  // Section 21: Occupancy Schedules
  output += d->unsupported["OccupancySchedule"];
  // Section 22: Exposures
  output += d->unsupported["Exposure"];
  // Section 23: Annotations
  output += d->unsupported["Annotation"];
  // End of the PRJ file
  output += "* end project file.";
  return output;
}

std::vector<std::vector<int> > CxModel::zoneExteriorFlowPaths()
{
  std::vector<std::vector<int> > paths(d->zones.size());

  for(unsigned int i=0;i<d->paths.size();i++)
  {
    if(d->paths[i].pzn() == -1)
    {
      int nr = d->paths[i].pzm();
      if(nr > 0 && (unsigned int)nr<=d->zones.size())
      {
        paths[nr-1].push_back(-d->paths[i].nr()); // This flow path is negative for flow into zone
      }
    }
    else if(d->paths[i].pzm() == -1)
    {
      int nr = d->paths[i].pzn();
      if(nr > 0 && (unsigned int)nr<=d->zones.size())
      {
        paths[nr-1].push_back(d->paths[i].nr()); // This flow path is positive for flow into zone
      }
    }
  }
  return paths;
}

std::vector<TimeSeries> CxModel::zoneInfiltration(SimFile *sim)
{
  // This should probably include a lot more checks of things and is written in
  // somewhat strange way to avoid taking too much advantage of the specifics 
  // of the text form outputs.
  std::vector<TimeSeries> results;
  //std::vector<std::vector<double> > flow0 = sim->F0();
  std::vector<std::vector<int> > paths = zoneExteriorFlowPaths();
  unsigned int ntimes = sim->dateTimes().size();
  for(unsigned int i=0; i<d->zones.size(); i++)
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

void CxModel::rebuildContaminants()
{
  d->contaminants.clear();
  for(unsigned int i=1;i<=d->species.size();i++)
  {
    d->species[i].setNr(i);
    if(d->species[i].sflag())
    {
      d->contaminants.push_back(i);
    }
  }
}

void CxModel::readZoneIc(Reader &input)
{
  unsigned int nn = input.readUInt(FILELINE);
  if(nn != 0)
  {
    unsigned int nctm = d->contaminants.size();
    if(nn != nctm*d->zones.size())
    {
      QString mesg("Mismatch between number of zones, contaminants, and initial conditions");
#ifndef NOFILELINE
      mesg +=  QString(" (%1,%2)").arg(__FILE__).arg(__LINE__);
#endif
      LOG_FREE_AND_THROW("openstudio.contam.ForwardTranslator",mesg.toStdString());
    }
    for(unsigned int i=0;i<d->zones.size();i++)
    {
      unsigned int nr = input.readUInt(FILELINE);
      if(nr != i+1)
      {
        QString mesg = QString("Mismatch between zone IC number and zone number at line %1 ")
          .arg(input.lineNumber());
#ifndef NOFILELINE
        mesg +=  QString(" (%1,%2)").arg(__FILE__).arg(__LINE__);
#endif
        LOG_FREE_AND_THROW("openstudio.contam.ForwardTranslator",mesg.toStdString());
      }
      std::vector<RX> ic;
      for(unsigned int j=0;j<nctm;j++)
      {
        ic.push_back(input.readNumber<RX>(FILELINE));
      }
      d->zones[i].setIc(ic);
    }
  }
  input.read999("Failed to find zone IC section termination" CFILELINE);
}

std::string CxModel::writeZoneIc(int start)
{
  int offset = 1;
  if(start != 0)
  {
    offset = 1-start;
  }
  int nctm = d->contaminants.size()*(d->zones.size()-start);
  std::string string = openstudio::toString(nctm) + " ! initial zone concentrations:\n";
  if(nctm)
  {
    for(unsigned int i=start;i<d->zones.size();i++)
    {
      string += openstudio::toString(i+offset);
      for(unsigned int j=0;j<d->contaminants.size();j++)
      {
        string += ' ' + openstudio::toString(d->zones[i].ic(j));
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

int CxModel::airflowElementNrByName(std::string name) const
{
  for(int i=0;i<d->airflowElements.size();i++)
  {
    if(d->airflowElements[i]->name() == name)
    {
      return d->airflowElements[i]->nr();
    }
  }
  return 0;
}

} // contam
} // openstudio
