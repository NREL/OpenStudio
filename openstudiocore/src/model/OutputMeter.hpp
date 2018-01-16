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

#ifndef MODEL_OUTPUTMETER_HPP
#define MODEL_OUTPUTMETER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class EndUseType;
class FuelType;
class InstallLocationType;
class TimeSeries;

namespace model {

namespace detail {

  class OutputMeter_Impl;

} // detail

/** OutputMeter is a ModelObject that wraps the OpenStudio IDD object 'OS_Output_Meter'.
 *
 *  A OutputMeter is a virtual instrument measuring energy use of some fuel type at a particular location.
 *  OutputMeter objects may be associated with various classes in the building model hierarchy (e.g. Facility, Building, Zone, Plant, etc).
 *  A OutputMeter can be used to request output data as well as access it from the EnergyPlus SQLite output.
 *  If the user wants to access measured data from a particular OutputMeter, they must ensure that input objects are present to
 *  trigger EnergyPlus to output the desired data.  This is similar to needing to instrument a real
 *  building during field measurements.
 *
 */
class MODEL_API OutputMeter : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit OutputMeter(const Model& model);

  virtual ~OutputMeter() {}

  virtual std::vector<openstudio::IdfObject> remove();

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  /// Returns the regular expression used to parse the name field.
  /// matches[1], specific end use type
  /// matches[2], end use type
  /// matches[3], fuel type
  /// matches[4], install location type
  /// matches[5], install location name
  static boost::regex meterRegex();

  /// Returns the Meter name for a set of inputs.
  static std::string getName(const boost::optional<std::string>& specificEndUseType,
                             const boost::optional<EndUseType>& endUseType,
                             const boost::optional<FuelType>& fuelType,
                             const boost::optional<InstallLocationType>& installLocationType,
                             const boost::optional<std::string>& specificInstallLocation);

  //@}
  /** @name Getters */
  //@{

  //DLM@20110823: this method is overriding name from IdfObject
  std::string name() const;

  std::string reportingFrequency() const;

  bool isReportingFrequencyDefaulted() const;

  bool meterFileOnly() const;

  bool isMeterFileOnlyDefaulted() const;

  bool cumulative() const;

  bool isCumulativeDefaulted() const;

  /// Returns the specific end use category to meter from the name field.
  /// For example, to meter all Lights with endUseSubcategory set to "General" the specific end use would be "General"
  /// and the Meter name would be "General:InteriorLights:Electricity".
  boost::optional<std::string> specificEndUse() const;

  /// Returns the end use type to meter from the name field.
  /// For example, to meter all Lights the end use type would be "InteriorLights"
  /// and the Meter name would be "InteriorLights:Electricity:Facility".
  boost::optional<EndUseType> endUseType() const;

  /// Returns the fuel type to meter from the name field.
  /// For example, to meter all electricity use the fuel type would be "Electricity"
  /// and the Meter name would be "Electricity:Facility".
  boost::optional<FuelType> fuelType() const;

  /// Returns the location the meter is installed at from the name field.
  /// For example, to meter all electricity at the Facility level the install location
  /// would be "Facility" and the Meter name would be "Electricity:Facility".
  boost::optional<InstallLocationType> installLocationType() const;

  /// Returns the specific location the meter is installed at from the name field.
  /// For example, to meter all electricity in a Zone named "Zone1" the specific install location
  /// would be "Zone1" and the Meter name would be "Electricity:Zone:Zone1".
  /// The value '*' indicates reporting for all locations.
  boost::optional<std::string> specificInstallLocation() const;

  //@}
  /** @name Setters */
  //@{

  bool setName(std::string name);

  bool setReportingFrequency(const std::string& reportingFrequency);

  void resetReportingFrequency();

  bool setMeterFileOnly(bool meterFileOnly);

  void resetMeterFileOnly();

  bool setCumulative(bool cumulative);

  void resetCumulative();

  /// Sets the specific end use category to meter in the name field.
  /// For example, to meter all Lights with endUseSubcategory set to "General" the specific end use would be "General"
  /// and the Meter name would be "General:InteriorLights:Electricity:Facility".  Passing an empty string will unset the value.
  bool setSpecificEndUse(const std::string& specifcEndUse) ;
  bool resetSpecificEndUse();

  /// Sets the end use type to meter in the name field.
  /// For example, to meter all Lights the end use type would be "InteriorLights"
  /// and the Meter name would be "InteriorLights:Electricity:Facility".
  bool setEndUseType(EndUseType type);
  bool resetEndUseType();

  /// Sets the fuel type to meter in the name field.
  /// For example, to meter all electricity use the fuel type would be "Electricity"
  /// and the Meter name would be "Electricity:Facility".
  bool setFuelType(FuelType type);
  bool resetFuelType();

  /// Sets the location the meter is installed at from the name field.
  /// For example, to meter all electricity at the Facility level the install location
  /// would be "Facility" and the Meter name would be "Electricity:Facility".
  bool setInstallLocationType(InstallLocationType type);
  bool resetInstallLocationType();

  /// Sets the specific location the meter is installed at from the name field.
  /// For example, to meter all electricity in a Zone named "Zone1" the specific install location
  /// would be "Zone1" and the Meter name would be "Electricity:Zone:Zone1".
  /// Passing an empty string will unset the value, '*' indicates reporting for all locations.
  bool setSpecificInstallLocation(const std::string& locationName);
  bool resetSpecificInstallLocation();

  //@}

  /// Returns TimeSeries data for this Meter.  Requires EnergyPlus simulation data.
  boost::optional<openstudio::TimeSeries> getData(const std::string& envPeriod) const;

  /// Returns TimeSeries data for this Meter, using given specificInstallLocation instead of this->specificInstallLocation.
  /// Useful if this->specificInstallLocation is '*'. Requires EnergyPlus simulation data.
  boost::optional<openstudio::TimeSeries> getData(const std::string& envPeriod, const boost::optional<std::string>& specificInstallLocation) const;

 protected:
  /// @cond
  typedef detail::OutputMeter_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit OutputMeter(std::shared_ptr<detail::OutputMeter_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.OutputMeter");
};

/** \relates OutputMeter*/
typedef boost::optional<OutputMeter> OptionalOutputMeter;

/** \relates OutputMeter*/
typedef std::vector<OutputMeter> OutputMeterVector;

/** Predicate for finding \link OutputMeter OutputMeters \endlink with fuelType() ft. \relates OutputMeter */
MODEL_API bool MeterFuelTypeEquals(const OutputMeter& meter, const FuelType& ft);

} // model
} // openstudio

#endif // MODEL_OUTPUTMETER_HPP
