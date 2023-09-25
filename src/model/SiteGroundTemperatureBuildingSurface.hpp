/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_HPP
#define MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureBuildingSurface_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureBuildingSurface is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_BuildingSurface'. */
  class MODEL_API SiteGroundTemperatureBuildingSurface : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureBuildingSurface(Model& model);

    virtual ~SiteGroundTemperatureBuildingSurface() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureBuildingSurface(const SiteGroundTemperatureBuildingSurface& other) = default;
    SiteGroundTemperatureBuildingSurface(SiteGroundTemperatureBuildingSurface&& other) = default;
    SiteGroundTemperatureBuildingSurface& operator=(const SiteGroundTemperatureBuildingSurface&) = default;
    SiteGroundTemperatureBuildingSurface& operator=(SiteGroundTemperatureBuildingSurface&&) = default;

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
    using ImplType = detail::SiteGroundTemperatureBuildingSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureBuildingSurface_Impl;

    explicit SiteGroundTemperatureBuildingSurface(std::shared_ptr<detail::SiteGroundTemperatureBuildingSurface_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureBuildingSurface");
  };

  /** \relates SiteGroundTemperatureBuildingSurface*/
  using OptionalSiteGroundTemperatureBuildingSurface = boost::optional<SiteGroundTemperatureBuildingSurface>;

  /** \relates SiteGroundTemperatureBuildingSurface*/
  using SiteGroundTemperatureBuildingSurfaceVector = std::vector<SiteGroundTemperatureBuildingSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_HPP
