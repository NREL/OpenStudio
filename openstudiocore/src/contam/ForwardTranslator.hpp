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

#ifndef CONTAM_FORWARDTRANSLATOR_HPP
#define CONTAM_FORWARDTRANSLATOR_HPP

#include "ContamAPI.hpp"

#include <contam/PrjModel.hpp>

#include <model/Model.hpp>
#include <utilities/idf/Handle.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/StringStreamLogSink.hpp>
#include <utilities/data/TimeSeries.hpp>
#include <utilities/time/Date.hpp>
#include <utilities/filetypes/EpwFile.hpp>

namespace openstudio{
class ProgressBar;

namespace contam{

class CONTAM_API CvFile
{
public:
  CvFile();
  void addTimeSeries(std::string name, TimeSeries series);
  bool write(openstudio::path filepath);
  Date start() const {return m_start;}
  void setStart(Date date){m_start=date;}
  Date end() const {return m_end;}
  void setEnd(Date date){m_end=date;}
  bool isEmpty() {return m_names.size() == 0;}

private:
  std::vector<std::string> m_names;
  std::vector<TimeSeries> m_series;

  Date m_start;
  Date m_end;
};

class CONTAM_API ForwardTranslator : public CxModel
{
public:
  ForwardTranslator();

  //void translate(const openstudio::model::Model& model, bool translateHVAC, ProgressBar* progressBar);

  /*ForwardTranslator(const openstudio::model::Model& model, bool translateHVAC=true, ProgressBar* progressBar=NULL);
  ForwardTranslator(const openstudio::model::Model& model, std::string leakageDescriptor, bool translateHVAC=true,
    ProgressBar* progressBar=NULL);
  ForwardTranslator(const openstudio::model::Model& model, double flow, double n, double deltaP,
    bool translateHVAC=true, ProgressBar* progressBar=NULL);
  ForwardTranslator(const openstudio::model::Model& model, double returnSupplyRatio, bool translateHVAC=true, 
    ProgressBar* progressBar=NULL);
  ForwardTranslator(const openstudio::model::Model& model, double returnSupplyRatio, std::string leakageDescriptor,
    bool translateHVAC=true, ProgressBar* progressBar=NULL);
  ForwardTranslator(const openstudio::model::Model& model, double returnSupplyRatio, double flow, double n, 
    double deltaP, bool translateHVAC=true, ProgressBar* progressBar=NULL);*/

  bool toPrj(const openstudio::path& path);
  
  // Static translation function
  static bool modelToPrj(const openstudio::model::Model& model, const openstudio::path& path,
    bool translateHVAC=true, std::string leakageDescriptor="Average", ProgressBar* progressBar=NULL);

  // Secondary translation functions - need to add more of these by chopping out parts of the main translation function
  boost::optional<EpwFile> translateEpw(openstudio::path epwpath, openstudio::path outpath);

  bool ready() const {return m_ready && valid();}
  std::map <Handle, int> surfaceMap() const {return m_surfaceMap;}
  std::map <Handle, int> zoneMap() const {return m_zoneMap;}

  // Getters and setters - the setters modify how translation is done or modify the model after translation (eventually). 
  // Setters that could fail return a boolean
  boost::optional<std::string> airtightnessLevel() const;
  bool setAirtightnessLevel(std::string level);
  double exteriorFlowRate() const;
  double exteriorExponent() const;
  double exteriorDeltaP() const;
  bool setExteriorFlowRate(double flow,double n=0.65,double deltaP=75.0);

  // Getters and setters
  double returnSupplyRatio() const
  {
    return m_returnSupplyRatio;
  }
  void setReturnSupplyRatio(double returnSupplyRatio)
  {
    m_returnSupplyRatio = fabs(returnSupplyRatio);
  }

  bool ratioOverride() const
  {
    return m_ratioOverride;
  }
  /*
  void setRatioOverride(bool ratioOverride)
  {
    m_ratioOverride = ratioOverride;
  }
  */

  //boost::optional<EpwFile> epwFile() const
  //{
  //  return m_epwFile;
  //}

  boost::optional<DateTime> startDateTime() const
  {
    return m_startDateTime;
  }

  boost::optional<DateTime> endDateTime() const
  {
    return m_endDateTime;
  }
  // Postprocessing Functions
  //boost::optional<std::vector<TimeSeries> > zoneInfiltration(openstudio::path simPath);

  // We may need more functions like this that modify the CONTAM model
  bool setSteadyWeather(double windSpeed, double windDirection);
  int addNewAirflowElement(std::string name,double flow,double n=0.65,double deltaP=75.0);

  // Write control files
  bool writeCvFile(openstudio::path filepath);

  // Get warning messages generated by the last translation.
  std::vector<LogMessage> warnings() const;

  // Get error messages generated by the last translation.
  std::vector<LogMessage> errors() const;

private:
  // Translators
  boost::optional<contam::CxModel> translate(model::Model model, bool translateHVAC=true);

  // Really need to look at these and determine if they are really needed
  int tableLookup(QMap<std::string,int> map, std::string str, const char *name);
  int tableLookup(QMap<Handle,int> map, Handle handle, const char *name);
  int tableLookup(std::map<Handle,int> map, Handle handle, const char *name);
  std::string reverseLookup(QMap<std::string,int> map, int nr, const char *name);
  Handle reverseLookup(QMap<Handle,int> map, int nr, const char *name);

  void clear();

  // Maps - will be populated after a call of translateToPrj
  // All map to the CONTAM index (1,2,...,nElement)
  std::map<std::string,int> m_afeMap;  // Map from descriptor ("exterior", "floor", etc.) to CONTAM airflow element index
  QMap <Handle, int> m_levelMap;      // Building story to level map by handle
  std::map <Handle, int> m_zoneMap;       // Thermal zone to airflow zone map by handle
  //QMap <std::string, int> volumeMap; // Map of AHS volumes - may not be needed
  QMap <std::string, int> m_pathMap;  // AHS paths stored by name
  std::map <Handle, int> m_surfaceMap;    // Surface paths stored by handle
  QMap <Handle, int> m_ahsMap;        // Airloop to AHS map by handle

  //openstudio::model::Model m_model;
  CvFile m_cvf;
  //boost::optional<EpwFile> m_epwFile;
  boost::optional<DateTime> m_startDateTime;
  boost::optional<DateTime> m_endDateTime;
  bool m_ready;
  boost::optional<std::string> m_leakageDescriptor;
  boost::optional<double> m_flow;
  boost::optional<double> m_n;
  boost::optional<double> m_deltaP;
  double m_returnSupplyRatio;
  bool m_ratioOverride;

  ProgressBar* m_progressBar;

  StringStreamLogSink m_logSink;

  REGISTER_LOGGER("openstudio.contam.ForwardTranslator");
};

} // contam
} // openstudio

#endif // CONTAM_FORWARDTRANSLATOR_HPP
