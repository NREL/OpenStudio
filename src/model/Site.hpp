/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITE_HPP
#define MODEL_SITE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  // forward declarations
  class ClimateZones;
  class WeatherFile;
  class SiteGroundReflectance;
  class SiteGroundTemperatureBuildingSurface;
  class SiteGroundTemperatureDeep;
  class SiteGroundTemperatureShallow;
  class SiteGroundTemperatureFCfactorMethod;
  class SiteWaterMainsTemperature;
  class Facility;
  class ShadingSurfaceGroup;

  namespace detail {

    class Site_Impl;

  }  // namespace detail

  /** Site is a ParentObject that wraps the OpenStudio IDD object 'OS_Site'. */
  class MODEL_API Site : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Site() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Site(const Site& other) = default;
    Site(Site&& other) = default;
    Site& operator=(const Site&) = default;
    Site& operator=(Site&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validTerrainValues();

    //@}
    /** @name Getters */
    //@{

    double latitude() const;

    bool isLatitudeDefaulted() const;

    double longitude() const;

    bool isLongitudeDefaulted() const;

    double timeZone() const;

    bool isTimeZoneDefaulted() const;

    double elevation() const;

    bool isElevationDefaulted() const;

    std::string terrain() const;

    bool isTerrainDefaulted() const;

    bool keepSiteLocationInformation() const;

    bool isKeepSiteLocationInformationDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setLatitude(double latitude);

    void resetLatitude();

    bool setLongitude(double longitude);

    void resetLongitude();

    bool setTimeZone(double timeZone);

    void resetTimeZone();

    bool setElevation(double elevation);

    void resetElevation();

    bool setTerrain(const std::string& terrain);

    void resetTerrain();

    bool setKeepSiteLocationInformation(bool keepSiteLocationInformation);

    void resetKeepSiteLocationInformation();

    //@}

    /** Unique WeatherFile object parented by Site. */
    boost::optional<WeatherFile> weatherFile() const;

    /** Unique SiteGroundReflectance object parented by Site. */
    boost::optional<SiteGroundReflectance> siteGroundReflectance() const;

    /** Unique SiteGroundTemperatureBuildingSurface object parented by Site. */
    boost::optional<SiteGroundTemperatureBuildingSurface> siteGroundTemperatureBuildingSurface() const;

    /** Unique SiteGroundTemperatureDeep object parented by Site. */
    boost::optional<SiteGroundTemperatureDeep> siteGroundTemperatureDeep() const;

    /** Unique SiteGroundTemperatureShallow object parented by Site. */
    boost::optional<SiteGroundTemperatureShallow> siteGroundTemperatureShallow() const;

    /** Unique SiteGroundTemperatureFCfactorMethod object parented by Site. */
    boost::optional<SiteGroundTemperatureFCfactorMethod> siteGroundTemperatureFCfactorMethod() const;

    /** Unique SiteWaterMainsTemperature object parented by Site. */
    boost::optional<SiteWaterMainsTemperature> siteWaterMainsTemperature() const;

    /** Unique ClimateZones object parented by Site. */
    boost::optional<ClimateZones> climateZones() const;

    /** ShadingSurfaceGroup objects of type Site. */
    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

    /** The value of the active climate zone in the ClimateZones object. May be empty. */
    //std::string activeClimateZoneValue() const;

    /** Set the value of the active climate zone. */
    //bool setActiveClimateZoneValue(const std::string& value);

    /** The institution name of the active climate zone in the ClimateZones object. May be empty. */
    //std::string activeClimateZoneInstitution() const;

    /** Set the active climate zone institution to institution. */
    //bool setActiveClimateZoneInstitution(const std::string& institution);

   protected:
    /// @cond
    using ImplType = detail::Site_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit Site(std::shared_ptr<detail::Site_Impl> impl);

    explicit Site(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Site");
  };

  /** \relates Site*/
  using OptionalSite = boost::optional<Site>;

  /** \relates Site*/
  using SiteVector = std::vector<Site>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITE_HPP
