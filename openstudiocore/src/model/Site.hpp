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

#ifndef MODEL_SITE_HPP
#define MODEL_SITE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

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

namespace detail {

  class Site_Impl;

} // detail

/** Site is a ParentObject that wraps the OpenStudio IDD object 'OS_Site'. */
class MODEL_API Site : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Site() {}

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
  /// @cond
  typedef detail::Site_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit Site(std::shared_ptr<detail::Site_Impl> impl);

  explicit Site(Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Site");
};

/** \relates Site*/
typedef boost::optional<Site> OptionalSite;

/** \relates Site*/
typedef std::vector<Site> SiteVector;

} // model
} // openstudio

#endif // MODEL_SITE_HPP

