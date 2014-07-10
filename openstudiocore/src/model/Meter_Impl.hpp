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

#ifndef MODEL_METER_IMPL_HPP
#define MODEL_METER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "Meter.hpp"

#include "../utilities/core/StaticInitializer.hpp"

namespace openstudio {
  
class EndUseType;
class FuelType;
class InstallLocationType;
class TimeSeries;

namespace model {

class Meter;

namespace detail {

  /** Meter_Impl is a ModelObject_Impl that is the implementation class for Meter.*/
  class MODEL_API Meter_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string name READ name WRITE setName);
    Q_PROPERTY(std::string reportingFrequency READ reportingFrequency WRITE setReportingFrequency RESET resetReportingFrequency);
    Q_PROPERTY(bool isReportingFrequencyDefaulted READ isReportingFrequencyDefaulted);
    Q_PROPERTY(bool meterFileOnly READ meterFileOnly WRITE setMeterFileOnly RESET resetMeterFileOnly);
    Q_PROPERTY(bool isMeterFileOnlyDefaulted READ isMeterFileOnlyDefaulted);
    Q_PROPERTY(bool cumulative READ cumulative WRITE setCumulative RESET resetCumulative);
    Q_PROPERTY(bool isCumulativeDefaulted READ isCumulativeDefaulted);
   public:
    /** @name Constructors and Destructors */
    //@{

    Meter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Meter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    Meter_Impl(const Meter_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    virtual ~Meter_Impl() {}

    //@}

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    std::string name() const;

    std::string reportingFrequency() const;

    bool isReportingFrequencyDefaulted() const;

    bool meterFileOnly() const;

    bool isMeterFileOnlyDefaulted() const;

    bool cumulative() const;

    bool isCumulativeDefaulted() const;

    /// get user provided specific end use category to meter
    boost::optional<std::string> specificEndUse() const;

    /// get end use type
    boost::optional<EndUseType> endUseType() const;

    /// get fuel type
    boost::optional<FuelType> fuelType() const;

    /// get the location type the meter is installed at
    boost::optional<InstallLocationType> installLocationType() const;

    /// get the specific location the meter is installed at
    boost::optional<std::string> specificInstallLocation() const;

    //@}
    /** @name Setters */
    //@{

    bool setName(std::string name);

    bool setReportingFrequency(const std::string& reportingFrequency);

    void resetReportingFrequency();

    void setMeterFileOnly(bool meterFileOnly);

    void resetMeterFileOnly();

    void setCumulative(bool cumulative);

    void resetCumulative();

    /// set user provided specific end use category to meter, empty string will unset the value
    bool setSpecificEndUse(const std::string& endUse);
    bool resetSpecificEndUse();

    /// set end use type
    bool setEndUseType(EndUseType type);
    bool resetEndUseType();

    /// set fuel type
    bool setFuelType(FuelType type);
    bool resetFuelType();

    /// set the location type the meter is installed at
    bool setInstallLocationType(InstallLocationType type);
    bool resetInstallLocationType();

    /// set the specific location the meter is installed at, empty string will unset the value
    bool setSpecificInstallLocation(const std::string& locationName);
    bool resetSpecificInstallLocation();

    //@}

    /// get data for the meter
    boost::optional<openstudio::TimeSeries> getData(const std::string& envPeriod) const;

    /// get data for the meter, using given specificInstallLocation instead of this->specificInstallLocation (e.g. '*')
    boost::optional<openstudio::TimeSeries> getData(const std::string& envPeriod, const boost::optional<std::string>& specificInstallLocation) const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.Meter");
  };

  struct MeterRegexInitializer : StaticInitializer<MeterRegexInitializer>
  {
    static void initialize()
    {
      openstudio::model::Meter::meterRegex();
    }
  };

  struct MakeSureMeterRegexInitializerIsInitialized
  {
    MakeSureMeterRegexInitializerIsInitialized()
    {}

    MeterRegexInitializer m_i;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_METER_IMPL_HPP

