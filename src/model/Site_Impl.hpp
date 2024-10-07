/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITE_IMPL_HPP
#define MODEL_SITE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

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
  class Site;

  namespace detail {

    /** Site_Impl is a ParentObject_Impl that is the implementation class for Site.*/
    class MODEL_API Site_Impl : public ParentObject_Impl
    {
      //
      //
     public:
      /** @name Constructors and Destructors */
      //@{

      Site_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Site_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Site_Impl(const Site_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Site_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

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
     private:
      REGISTER_LOGGER("openstudio.model.Site");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITE_IMPL_HPP
