/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREFCFACTORMETHOD_HPP
#define MODEL_SITEGROUNDTEMPERATUREFCFACTORMETHOD_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureFCfactorMethod_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureFCfactorMethod is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_FCfactorMethod'. */
  class MODEL_API SiteGroundTemperatureFCfactorMethod : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureFCfactorMethod(Model& model);

    virtual ~SiteGroundTemperatureFCfactorMethod() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureFCfactorMethod(const SiteGroundTemperatureFCfactorMethod& other) = default;
    SiteGroundTemperatureFCfactorMethod(SiteGroundTemperatureFCfactorMethod&& other) = default;
    SiteGroundTemperatureFCfactorMethod& operator=(const SiteGroundTemperatureFCfactorMethod&) = default;
    SiteGroundTemperatureFCfactorMethod& operator=(SiteGroundTemperatureFCfactorMethod&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double januaryGroundTemperature() const;

    bool isJanuaryGroundTemperatureDefaulted() const;

    double februaryGroundTemperature() const;

    bool isFebruaryGroundTemperatureDefaulted() const;

    double marchGroundTemperature() const;

    bool isMarchGroundTemperatureDefaulted() const;

    double aprilGroundTemperature() const;

    bool isAprilGroundTemperatureDefaulted() const;

    double mayGroundTemperature() const;

    bool isMayGroundTemperatureDefaulted() const;

    double juneGroundTemperature() const;

    bool isJuneGroundTemperatureDefaulted() const;

    double julyGroundTemperature() const;

    bool isJulyGroundTemperatureDefaulted() const;

    double augustGroundTemperature() const;

    bool isAugustGroundTemperatureDefaulted() const;

    double septemberGroundTemperature() const;

    bool isSeptemberGroundTemperatureDefaulted() const;

    double octoberGroundTemperature() const;

    bool isOctoberGroundTemperatureDefaulted() const;

    double novemberGroundTemperature() const;

    bool isNovemberGroundTemperatureDefaulted() const;

    double decemberGroundTemperature() const;

    bool isDecemberGroundTemperatureDefaulted() const;

    double getTemperatureByMonth(int month) const;

    double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

    bool isMonthDefaulted(int month) const;

    bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

    std::vector<double> getAllMonthlyTemperatures() const;

    //@}
    /** @name Setters */
    //@{

    bool setJanuaryGroundTemperature(double januaryGroundTemperature);

    void resetJanuaryGroundTemperature();

    bool setFebruaryGroundTemperature(double februaryGroundTemperature);

    void resetFebruaryGroundTemperature();

    bool setMarchGroundTemperature(double marchGroundTemperature);

    void resetMarchGroundTemperature();

    bool setAprilGroundTemperature(double aprilGroundTemperature);

    void resetAprilGroundTemperature();

    bool setMayGroundTemperature(double mayGroundTemperature);

    void resetMayGroundTemperature();

    bool setJuneGroundTemperature(double juneGroundTemperature);

    void resetJuneGroundTemperature();

    bool setJulyGroundTemperature(double julyGroundTemperature);

    void resetJulyGroundTemperature();

    bool setAugustGroundTemperature(double augustGroundTemperature);

    void resetAugustGroundTemperature();

    bool setSeptemberGroundTemperature(double septemberGroundTemperature);

    void resetSeptemberGroundTemperature();

    bool setOctoberGroundTemperature(double octoberGroundTemperature);

    void resetOctoberGroundTemperature();

    bool setNovemberGroundTemperature(double novemberGroundTemperature);

    void resetNovemberGroundTemperature();

    bool setDecemberGroundTemperature(double decemberGroundTemperature);

    void resetDecemberGroundTemperature();

    bool setTemperatureByMonth(int month, double temperature);

    bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

    void resetTemperatureByMonth(int month);

    void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

    void resetAllMonths();

    bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureFCfactorMethod_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureFCfactorMethod_Impl;

    explicit SiteGroundTemperatureFCfactorMethod(std::shared_ptr<detail::SiteGroundTemperatureFCfactorMethod_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureFCfactorMethod");
  };

  /** \relates SiteGroundTemperatureFCfactorMethod*/
  using OptionalSiteGroundTemperatureFCfactorMethod = boost::optional<SiteGroundTemperatureFCfactorMethod>;

  /** \relates SiteGroundTemperatureFCfactorMethod*/
  using SiteGroundTemperatureFCfactorMethodVector = std::vector<SiteGroundTemperatureFCfactorMethod>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREFCFACTORMETHOD_HPP
