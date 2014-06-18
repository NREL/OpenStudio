/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_SITE_IMPL_HPP
#define MODEL_SITE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

// forward declarations
class ClimateZones;
class TimeDependentValuation;
class WeatherFile;
class SiteGroundReflectance;
class SiteGroundTemperatureBuildingSurface;
class SiteWaterMainsTemperature;
class Facility;
class ShadingSurfaceGroup;
class Site;

namespace detail {

  /** Site_Impl is a ParentObject_Impl that is the implementation class for Site.*/
  class MODEL_API Site_Impl : public ParentObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude RESET resetLatitude);
    Q_PROPERTY(bool isLatitudeDefaulted READ isLatitudeDefaulted);
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude RESET resetLongitude);
    Q_PROPERTY(bool isLongitudeDefaulted READ isLongitudeDefaulted);
    Q_PROPERTY(double timeZone READ timeZone WRITE setTimeZone RESET resetTimeZone);
    Q_PROPERTY(bool isTimeZoneDefaulted READ isTimeZoneDefaulted);
    Q_PROPERTY(double elevation READ elevation WRITE setElevation RESET resetElevation);
    Q_PROPERTY(bool isElevationDefaulted READ isElevationDefaulted);
    Q_PROPERTY(std::string terrain READ terrain WRITE setTerrain RESET resetTerrain);
    Q_PROPERTY(bool isTerrainDefaulted READ isTerrainDefaulted);
    //Q_PROPERTY(std::string activeClimateZoneValue READ activeClimateZoneValue WRITE setActiveClimateZoneValue);  
    //Q_PROPERTY(std::string activeClimateZoneInstitution READ activeClimateZoneInstitution WRITE setActiveClimateZoneInstitution);  
   public:
    /** @name Constructors and Destructors */
    //@{

    Site_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Site_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
              Model_Impl* model,
              bool keepHandle);

    Site_Impl(const Site_Impl& other,
              Model_Impl* model,
              bool keepHandle);

    virtual ~Site_Impl() {}

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

    bool setTerrain(std::string terrain);

    void resetTerrain();

    //@}

    /** Unique WeatherFile object parented by Site. */
    boost::optional<WeatherFile> weatherFile() const;

    /** Unique SiteGroundReflectance object parented by Site. */
    boost::optional<SiteGroundReflectance> siteGroundReflectance() const;

    /** Unique SiteGroundTemperatureBuildingSurface object parented by Site. */
    boost::optional<SiteGroundTemperatureBuildingSurface> siteGroundTemperatureBuildingSurface() const;

    /** Unique SiteWaterMainsTemperature object parented by Site. */
    boost::optional<SiteWaterMainsTemperature> siteWaterMainsTemperature() const; 

    /** Unique ClimateZones object parented by Site. */
    boost::optional<ClimateZones> climateZones() const;

    /** Unique TimeDependentValuation object parented by Site. */
    boost::optional<TimeDependentValuation> timeDependentValuation() const;

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

} // detail

} // model
} // openstudio

#endif // MODEL_SITE_IMPL_HPP

