/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_OUTPUTMETER_IMPL_HPP
#define MODEL_OUTPUTMETER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "OutputMeter.hpp"

#include "../utilities/core/StaticInitializer.hpp"

namespace openstudio {
  
class EndUseType;
class FuelType;
class InstallLocationType;
class TimeSeries;

namespace model {

class OutputMeter;

namespace detail {

  /** OutputMeter_Impl is a ModelObject_Impl that is the implementation class for OutputMeter.*/
  class MODEL_API OutputMeter_Impl : public ModelObject_Impl {
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

    OutputMeter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    OutputMeter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    OutputMeter_Impl(const OutputMeter_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~OutputMeter_Impl() {}

    //@}

    virtual boost::optional<ParentObject> parent() const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<openstudio::IdfObject> remove() override;

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
    REGISTER_LOGGER("openstudio.model.OutputMeter");
  };

  struct OutputMeterRegexInitializer : StaticInitializer<OutputMeterRegexInitializer>
  {
    static void initialize()
    {
      openstudio::model::OutputMeter::meterRegex();
    }
  };

  struct MakeSureOutputMeterRegexInitializerIsInitialized
  {
    MakeSureOutputMeterRegexInitializerIsInitialized()
    {}

    OutputMeterRegexInitializer m_i;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_OUTPUTMETER_IMPL_HPP

