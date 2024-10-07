/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREDEEP_HPP
#define MODEL_SITEGROUNDTEMPERATUREDEEP_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureDeep_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureDeep is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Deep'. */
  class MODEL_API SiteGroundTemperatureDeep : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureDeep(Model& model);

    virtual ~SiteGroundTemperatureDeep() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureDeep(const SiteGroundTemperatureDeep& other) = default;
    SiteGroundTemperatureDeep(SiteGroundTemperatureDeep&& other) = default;
    SiteGroundTemperatureDeep& operator=(const SiteGroundTemperatureDeep&) = default;
    SiteGroundTemperatureDeep& operator=(SiteGroundTemperatureDeep&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double januaryDeepGroundTemperature() const;

    bool isJanuaryDeepGroundTemperatureDefaulted() const;

    double februaryDeepGroundTemperature() const;

    bool isFebruaryDeepGroundTemperatureDefaulted() const;

    double marchDeepGroundTemperature() const;

    bool isMarchDeepGroundTemperatureDefaulted() const;

    double aprilDeepGroundTemperature() const;

    bool isAprilDeepGroundTemperatureDefaulted() const;

    double mayDeepGroundTemperature() const;

    bool isMayDeepGroundTemperatureDefaulted() const;

    double juneDeepGroundTemperature() const;

    bool isJuneDeepGroundTemperatureDefaulted() const;

    double julyDeepGroundTemperature() const;

    bool isJulyDeepGroundTemperatureDefaulted() const;

    double augustDeepGroundTemperature() const;

    bool isAugustDeepGroundTemperatureDefaulted() const;

    double septemberDeepGroundTemperature() const;

    bool isSeptemberDeepGroundTemperatureDefaulted() const;

    double octoberDeepGroundTemperature() const;

    bool isOctoberDeepGroundTemperatureDefaulted() const;

    double novemberDeepGroundTemperature() const;

    bool isNovemberDeepGroundTemperatureDefaulted() const;

    double decemberDeepGroundTemperature() const;

    bool isDecemberDeepGroundTemperatureDefaulted() const;

    double getTemperatureByMonth(int month) const;

    double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

    bool isMonthDefaulted(int month) const;

    bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

    std::vector<double> getAllMonthlyTemperatures() const;

    //@}
    /** @name Setters */
    //@{

    bool setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature);

    void resetJanuaryDeepGroundTemperature();

    bool setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature);

    void resetFebruaryDeepGroundTemperature();

    bool setMarchDeepGroundTemperature(double marchDeepGroundTemperature);

    void resetMarchDeepGroundTemperature();

    bool setAprilDeepGroundTemperature(double aprilDeepGroundTemperature);

    void resetAprilDeepGroundTemperature();

    bool setMayDeepGroundTemperature(double mayDeepGroundTemperature);

    void resetMayDeepGroundTemperature();

    bool setJuneDeepGroundTemperature(double juneDeepGroundTemperature);

    void resetJuneDeepGroundTemperature();

    bool setJulyDeepGroundTemperature(double julyDeepGroundTemperature);

    void resetJulyDeepGroundTemperature();

    bool setAugustDeepGroundTemperature(double augustDeepGroundTemperature);

    void resetAugustDeepGroundTemperature();

    bool setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature);

    void resetSeptemberDeepGroundTemperature();

    bool setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature);

    void resetOctoberDeepGroundTemperature();

    bool setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature);

    void resetNovemberDeepGroundTemperature();

    bool setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature);

    void resetDecemberDeepGroundTemperature();

    bool setTemperatureByMonth(int month, double temperature);

    bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

    void resetTemperatureByMonth(int month);

    void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

    void resetAllMonths();

    bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureDeep_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureDeep_Impl;

    explicit SiteGroundTemperatureDeep(std::shared_ptr<detail::SiteGroundTemperatureDeep_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureDeep");
  };

  /** \relates SiteGroundTemperatureDeep*/
  using OptionalSiteGroundTemperatureDeep = boost::optional<SiteGroundTemperatureDeep>;

  /** \relates SiteGroundTemperatureDeep*/
  using SiteGroundTemperatureDeepVector = std::vector<SiteGroundTemperatureDeep>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREDeep_HPP
