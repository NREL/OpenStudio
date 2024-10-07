/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEWATERMAINSTEMPERATURE_HPP
#define MODEL_SITEWATERMAINSTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class SiteWaterMainsTemperature_Impl;

  }  // namespace detail

  /** SiteWaterMainsTemperature is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:Site:WaterMainsTemperature'. */
  class MODEL_API SiteWaterMainsTemperature : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SiteWaterMainsTemperature() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteWaterMainsTemperature(const SiteWaterMainsTemperature& other) = default;
    SiteWaterMainsTemperature(SiteWaterMainsTemperature&& other) = default;
    SiteWaterMainsTemperature& operator=(const SiteWaterMainsTemperature&) = default;
    SiteWaterMainsTemperature& operator=(SiteWaterMainsTemperature&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> calculationMethodValues();

    /** \deprecated */
    static std::vector<std::string> validCalculationMethodValues();

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

    /** Also sets calculation method to 'Schedule'. */
    bool setTemperatureSchedule(Schedule& schedule);

    void resetTemperatureSchedule();

    /** Also sets calculation method to 'Correlation'. */
    bool setAnnualAverageOutdoorAirTemperature(double annualAverageOutdoorAirTemperature);

    /** Also sets calculation method to 'Correlation'. */
    void resetAnnualAverageOutdoorAirTemperature();

    /** Also sets calculation method to 'Correlation'. */
    bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(double maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

    /** Also sets calculation method to 'Correlation'. */
    void resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteWaterMainsTemperature_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteWaterMainsTemperature_Impl;

    explicit SiteWaterMainsTemperature(std::shared_ptr<detail::SiteWaterMainsTemperature_Impl> impl);

    explicit SiteWaterMainsTemperature(const Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteWaterMainsTemperature");
  };

  /** \relates SiteWaterMainsTemperature*/
  using OptionalSiteWaterMainsTemperature = boost::optional<SiteWaterMainsTemperature>;

  /** \relates SiteWaterMainsTemperature*/
  using SiteWaterMainsTemperatureVector = std::vector<SiteWaterMainsTemperature>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEWATERMAINSTEMPERATURE_HPP
