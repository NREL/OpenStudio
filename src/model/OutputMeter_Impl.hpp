/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API OutputMeter_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      OutputMeter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputMeter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputMeter_Impl(const OutputMeter_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputMeter_Impl() override = default;

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

      bool setReportingFrequency(const std::string& reportingFrequency);

      void resetReportingFrequency();

      bool setMeterFileOnly(bool meterFileOnly);

      void resetMeterFileOnly();

      bool setCumulative(bool cumulative);

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
      boost::optional<openstudio::TimeSeries> getData(const std::string& envPeriod,
                                                      const boost::optional<std::string>& specificInstallLocation) const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputMeter");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTMETER_IMPL_HPP
