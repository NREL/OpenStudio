/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
#define MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureShallow_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureShallow is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Shallow'. */
  class MODEL_API SiteGroundTemperatureShallow : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureShallow(Model& model);

    virtual ~SiteGroundTemperatureShallow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureShallow(const SiteGroundTemperatureShallow& other) = default;
    SiteGroundTemperatureShallow(SiteGroundTemperatureShallow&& other) = default;
    SiteGroundTemperatureShallow& operator=(const SiteGroundTemperatureShallow&) = default;
    SiteGroundTemperatureShallow& operator=(SiteGroundTemperatureShallow&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double januarySurfaceGroundTemperature() const;

    bool isJanuarySurfaceGroundTemperatureDefaulted() const;

    double februarySurfaceGroundTemperature() const;

    bool isFebruarySurfaceGroundTemperatureDefaulted() const;

    double marchSurfaceGroundTemperature() const;

    bool isMarchSurfaceGroundTemperatureDefaulted() const;

    double aprilSurfaceGroundTemperature() const;

    bool isAprilSurfaceGroundTemperatureDefaulted() const;

    double maySurfaceGroundTemperature() const;

    bool isMaySurfaceGroundTemperatureDefaulted() const;

    double juneSurfaceGroundTemperature() const;

    bool isJuneSurfaceGroundTemperatureDefaulted() const;

    double julySurfaceGroundTemperature() const;

    bool isJulySurfaceGroundTemperatureDefaulted() const;

    double augustSurfaceGroundTemperature() const;

    bool isAugustSurfaceGroundTemperatureDefaulted() const;

    double septemberSurfaceGroundTemperature() const;

    bool isSeptemberSurfaceGroundTemperatureDefaulted() const;

    double octoberSurfaceGroundTemperature() const;

    bool isOctoberSurfaceGroundTemperatureDefaulted() const;

    double novemberSurfaceGroundTemperature() const;

    bool isNovemberSurfaceGroundTemperatureDefaulted() const;

    double decemberSurfaceGroundTemperature() const;

    bool isDecemberSurfaceGroundTemperatureDefaulted() const;

    double getTemperatureByMonth(int month) const;

    double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

    bool isMonthDefaulted(int month) const;

    bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

    std::vector<double> getAllMonthlyTemperatures() const;

    //@}
    /** @name Setters */
    //@{

    bool setJanuarySurfaceGroundTemperature(double januarySurfaceGroundTemperature);

    void resetJanuarySurfaceGroundTemperature();

    bool setFebruarySurfaceGroundTemperature(double februarySurfaceGroundTemperature);

    void resetFebruarySurfaceGroundTemperature();

    bool setMarchSurfaceGroundTemperature(double marchSurfaceGroundTemperature);

    void resetMarchSurfaceGroundTemperature();

    bool setAprilSurfaceGroundTemperature(double aprilSurfaceGroundTemperature);

    void resetAprilSurfaceGroundTemperature();

    bool setMaySurfaceGroundTemperature(double maySurfaceGroundTemperature);

    void resetMaySurfaceGroundTemperature();

    bool setJuneSurfaceGroundTemperature(double juneSurfaceGroundTemperature);

    void resetJuneSurfaceGroundTemperature();

    bool setJulySurfaceGroundTemperature(double julySurfaceGroundTemperature);

    void resetJulySurfaceGroundTemperature();

    bool setAugustSurfaceGroundTemperature(double augustSurfaceGroundTemperature);

    void resetAugustSurfaceGroundTemperature();

    bool setSeptemberSurfaceGroundTemperature(double septemberSurfaceGroundTemperature);

    void resetSeptemberSurfaceGroundTemperature();

    bool setOctoberSurfaceGroundTemperature(double octoberSurfaceGroundTemperature);

    void resetOctoberSurfaceGroundTemperature();

    bool setNovemberSurfaceGroundTemperature(double novemberSurfaceGroundTemperature);

    void resetNovemberSurfaceGroundTemperature();

    bool setDecemberSurfaceGroundTemperature(double decemberSurfaceGroundTemperature);

    void resetDecemberSurfaceGroundTemperature();

    bool setTemperatureByMonth(int month, double temperature);

    bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

    void resetTemperatureByMonth(int month);

    void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

    void resetAllMonths();

    bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureShallow_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureShallow_Impl;

    explicit SiteGroundTemperatureShallow(std::shared_ptr<detail::SiteGroundTemperatureShallow_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureShallow");
  };

  /** \relates SiteGroundTemperatureShallow*/
  using OptionalSiteGroundTemperatureShallow = boost::optional<SiteGroundTemperatureShallow>;

  /** \relates SiteGroundTemperatureShallow*/
  using SiteGroundTemperatureShallowVector = std::vector<SiteGroundTemperatureShallow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
