/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~Site_Impl() = default;

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
