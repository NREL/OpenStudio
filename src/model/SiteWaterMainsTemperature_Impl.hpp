/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEWATERMAINSTEMPERATURE_IMPL_HPP
#define MODEL_SITEWATERMAINSTEMPERATURE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** SiteWaterMainsTemperature_Impl is a ModelObject_Impl that is the implementation class for SiteWaterMainsTemperature.*/
    class MODEL_API SiteWaterMainsTemperature_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteWaterMainsTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteWaterMainsTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteWaterMainsTemperature_Impl(const SiteWaterMainsTemperature_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SiteWaterMainsTemperature_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      std::string calculationMethod() const;

      boost::optional<Schedule> temperatureSchedule() const;

      boost::optional<double> annualAverageOutdoorAirTemperature() const;

      boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const;

      //@}
      /** @name Setters */
      //@{

      bool setCalculationMethod(const std::string& calculationMethod);

      bool setTemperatureSchedule(Schedule& schedule);

      void resetTemperatureSchedule();

      bool setAnnualAverageOutdoorAirTemperature(boost::optional<double> annualAverageOutdoorAirTemperature);

      void resetAnnualAverageOutdoorAirTemperature();

      bool
        setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

      void resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SiteWaterMainsTemperature");

      std::vector<std::string> calculationMethodValues() const;
      boost::optional<ModelObject> temperatureScheduleAsModelObject() const;

      bool setTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& temperatureSchedule);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEWATERMAINSTEMPERATURE_IMPL_HPP
