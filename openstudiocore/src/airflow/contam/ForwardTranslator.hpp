/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef AIRFLOW_CONTAM_FORWARDTRANSLATOR_HPP
#define AIRFLOW_CONTAM_FORWARDTRANSLATOR_HPP

#include "../AirflowAPI.hpp"

#include "PrjModel.hpp"
#include "../SurfaceNetworkBuilder.hpp"

//#include "../model/Model.hpp"
#include "../utilities/idf/Handle.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/filetypes/EpwFile.hpp"

namespace openstudio{
class ProgressBar;
namespace model {
class Model;
class Space;
class ThermalZone;
class SubSurface;
class Surface;
}

namespace contam{

/** CvFile is a container for data that is to be written to a CONTAM CVF.
 *
 *  CvFile contains time series data that is to be written to a CONTAM
 *  Continuous Values File, which is documented here:
 *
 *  www.bfrl.nist.gov/IAQanalysis/CONTAM/manual/Content/html/IDH_UsingControls_CVF.htm
 *
 *  Data is input as TimeSeries that should cover the entire time period to
 *  be simulated. TimeSeries with Celcius units will be converted to Kelvin
 *  on output. All other TimeSeries should be in the units CONTAM expects.
 *
 */
class AIRFLOW_API CvFile
{
public:
  /** Construct a new, empty object. */
  CvFile();
  /** Add a time series to the data contained in the object */
  void addTimeSeries(std::string name, TimeSeries series);
  /** Write out CVF-formatted data to the specified path. */
  bool write(openstudio::path filepath);
  /** Returns the start date of the data that will be written. */
  Date start() const {return m_start;}
  /** Set the start date of the data that will be written. */
  void setStart(Date date){m_start=date;}
  /** Returns the end date of the data that will be written. */
  Date end() const {return m_end;}
  /** Set the end date of the data that will be written. */
  void setEnd(Date date){m_end=date;}
  /** Returns true if the object has no time series. */
  bool isEmpty() {return m_names.size() == 0;}
  /** Clear the object and make it empty. */
  void clear();

private:
  std::vector<std::string> m_names;
  std::vector<TimeSeries> m_series;

  Date m_start;
  Date m_end;
};

/** ForwardTranslator translates an OpenStudio model into a CONTAM model.
 *
 *  ForwardTranslator translates an OpenStudio energy model into a CONTAM
 *  airflow model using a streamlined approach. Each wall is assigned an
 *  overall leakage rate and individual components are not directly represented.
 *
 */
class AIRFLOW_API ForwardTranslator : public airflow::SurfaceNetworkBuilder
{
public:
  enum class TranslationType {SurfacesOnly, SubSurfacesOnly, AllSurfaces};
  /** @name Constructors and Destructors */
  //@{
  /** Construct a new, empty translator. */
  ForwardTranslator();
  /** Destroy the translator.*/
  ~ForwardTranslator(){}

  //@}
  /** @name Translation Functions */
  //@{

  /** Translates an OpenStudio thermal model into a CONTAM airflow model. */
  boost::optional<contam::IndexModel> translateModel(model::Model model);

  /** Static translation function to translate an OpenStudio into a CONTAM airflow model and write it to a path. */
  static bool modelToPrj(const openstudio::model::Model& model, const openstudio::path& path,
    bool translateHVAC=true, std::string leakageDescriptor="Average", ProgressBar* progressBar=nullptr);

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns a map from the OpenStudio surface handles to the CONTAM airflow path index (which runs from 1 to the number of surfaces). */
  std::map <Handle, int> surfaceMap() const {return m_surfaceMap;}
  /** Returns a map from the OpenStudio thermal zone handles to the CONTAM airflow zone index (which runs from 1 to the number of airflow zones). */
  std::map <Handle, int> zoneMap() const {return m_zoneMap;}

  // Getters and setters - the setters modify how translation is done
  // Setters that could fail return a boolean
  /** Returns the airtightness level as a string if it has been set. */
  boost::optional<std::string> airtightnessLevel() const;
  /** Set the airtightness level. Three levels are supported: Leaky, Average, and Tight. */
  void setAirtightnessLevel(std::string level);
  /** Returns the test data flow rate of envelope flow paths if it has been set. */
  boost::optional<double> exteriorFlowRate() const;
  /** Returns the test data flow exponent of envelope flow paths if it has been set. */
  boost::optional<double> exteriorExponent() const;
  /** Returns the test data pressure difference of envelope flow paths if it has been set. */
  boost::optional<double> exteriorDeltaP() const;
  /** Set the test data point for envelope flow paths. */
  bool setExteriorFlowRate(double flow, double n, double deltaP);
  /** Returns the return-to-supply ratio. */
  double returnSupplyRatio() const;
  /** Sets the return-to-supply ratio. */
  void setReturnSupplyRatio(double returnSupplyRatio);
  /** Returns true if the return/supply ratio will be used to set return flows, false otherwise. */
  bool ratioOverride() const;
  /** Set the behavior of the translator in setting return flows. */
  void setRatioOverride(bool ratioOverride);
  /** Returns true if HVAC will be translated, false otherwise. */
  bool translateHVAC() const;
  /** Set the translator's HVAC translation behavior. */
  void setTranslateHVAC(bool translateHVAC);
  /** Returns the simulation start time of the translated model. */
  boost::optional<openstudio::DateTime> startDateTime() const;
  /** Returns the simulation end time of the translated model. */
  boost::optional<openstudio::DateTime> endDateTime() const;

  //@}
  /** @name Miscellaneous Functions */
  //@{

  /** Write out any continuous value file information to the specified path */
  bool writeCvFile(openstudio::path filepath);

  /** Returns warning messages generated by the last translation. */
  std::vector<LogMessage> warnings() const;

  /** Returns error messages generated by the last translation. */
  std::vector<LogMessage> errors() const;

  //@}

protected:
  virtual bool linkExteriorSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface) override;
  virtual bool linkExteriorSubSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface, openstudio::model::SubSurface subSurface) override;
  virtual bool linkInteriorSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface,
    openstudio::model::Surface adjacentSurface, openstudio::model::Space adjacentSpace, openstudio::model::ThermalZone adjacentZone) override;
  virtual bool linkInteriorSubSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface, openstudio::model::SubSurface subSurface,
    openstudio::model::SubSurface adjacentSubSurface, openstudio::model::Surface adjacentSurface, openstudio::model::Space adjacentSpace, openstudio::model::ThermalZone adjacentZone) override;


private:
  // Do the work to set up the leakage paths
  bool applyExteriorFlowRate(contam::IndexModel prjModel);
  bool applyAirtightnessLevel(contam::IndexModel prjModel);

  // Convenience function to add a new one-point test airflow element to the model
  int addNewAirflowElement(contam::IndexModel model,std::string name,double flow,double n=0.65,double deltaP=75.0);

  // Clear out the translator and reset to the defaults
  void clear() override;

  // Really need to look at these and determine if they are really needed
  int tableLookup(QMap<std::string,int> map, std::string str, const char *name);
  int tableLookup(QMap<Handle,int> map, Handle handle, const char *name);
  int tableLookup(std::map<Handle,int> map, Handle handle, const char *name);
  std::string reverseLookup(QMap<std::string,int> map, int nr, const char *name);
  Handle reverseLookup(QMap<Handle,int> map, int nr, const char *name);

  contam::IndexModel m_prjModel;

  // Maps - will be populated after a call of translateModel
  // All map to the CONTAM index (1,2,...,nElement)
  std::map<std::string,int> m_afeMap;  // Map from descriptor ("exterior", "floor", etc.) to CONTAM airflow element index
  QMap <Handle, int> m_levelMap;      // Building story to level map by handle
  std::map <Handle, int> m_zoneMap;       // Thermal zone to airflow zone map by handle
  //QMap <std::string, int> volumeMap; // Map of AHS volumes - may not be needed
  QMap <std::string, int> m_pathMap;  // AHS paths stored by name
  std::map <Handle, int> m_surfaceMap;    // Surface paths stored by handle
  QMap <Handle, int> m_ahsMap;        // Airloop to AHS map by handle

  CvFile m_cvf;
  boost::optional<openstudio::DateTime> m_startDateTime;
  boost::optional<openstudio::DateTime> m_endDateTime;
  boost::optional<std::string> m_leakageDescriptor;
  boost::optional<double> m_flow;
  boost::optional<double> m_n;
  boost::optional<double> m_deltaP;
  double m_returnSupplyRatio;
  bool m_ratioOverride;
  bool m_translateHVAC;

  StringStreamLogSink m_logSink;

  REGISTER_LOGGER("openstudio.contam.ForwardTranslator");
};

} // contam
} // openstudio

#endif // AIRFLOW_CONTAM_FORWARDTRANSLATOR_HPP
