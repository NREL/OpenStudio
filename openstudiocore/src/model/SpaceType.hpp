/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_SPACETYPE_HPP
#define MODEL_SPACETYPE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

// forward declarations
class Schedule;
class Space;
class InternalMass;
class People;
class Lights;
class Luminaire;
class ElectricEquipment;
class GasEquipment;
class HotWaterEquipment;
class SteamEquipment;
class OtherEquipment;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class DefaultConstructionSet;
class DefaultScheduleSet;
class DefaultScheduleType;
class RenderingColor;
class DesignSpecificationOutdoorAir;

namespace detail {

  class SpaceType_Impl;

} // detail

/** SpaceType is a ResourceObject that wraps the OpenStudio IDD object 'OS_SpaceType'. */
class MODEL_API SpaceType : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SpaceType(const Model& model);

  virtual ~SpaceType() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /// Returns the default construction set.
  boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

  /// Returns the default schedule set.
  boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

  /// Returns the rendering color.
  boost::optional<RenderingColor> renderingColor() const;

  /// Returns the standards building type. This is a freeform field used to identify the building type for standards.
  /// Standards applied to this model will use this field to determine correct levels for lighting, occupancy, etc.
  /// More information can be found at https://github.com/NREL/openstudio-standards.
  boost::optional<std::string> standardsBuildingType() const;

  /// If standardsBuildingType is empty, returns a list of suggestions.  If standardsBuildingType is not empty,
  /// returns standardsBuildingType.
  std::vector<std::string> suggestedStandardsBuildingTypes() const;

  /// Returns the standards space type. This is a freeform field used to identify the space type for standards.
  /// Standards applied to this model will use this field to determine correct levels for lighting, occupancy, etc.
  /// More information can be found at https://github.com/NREL/openstudio-standards.
  boost::optional<std::string> standardsSpaceType() const;

  /// If standardsSpaceType is empty, returns a list of suggestions based on standardsBuildingType.  If standardsSpaceType is not empty,
  /// returns standardsSpaceType.
  std::vector<std::string> suggestedStandardsSpaceTypes() const;

  //@}
  /** @name Setters */
  //@{

  /// Sets the default construction set.
  bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);
  
  /// Resets the default construction set.
  void resetDefaultConstructionSet();

  /// Sets the default schedule set.
  bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

  /// Resets the default schedule set.
  void resetDefaultScheduleSet();

  /// Sets the rendering color.
  bool setRenderingColor(const RenderingColor& renderingColor);

  /// Resets the rendering color.
  void resetRenderingColor();

  /// Sets the standards building type. This is a freeform field used to identify the building type for standards.
  /// Standards applied to this model will use this field to determine correct levels for lighting, occupancy, etc.
  /// More information can be found at https://github.com/NREL/openstudio-standards.
  bool setStandardsBuildingType(const std::string& standardsBuildingType);
  void resetStandardsBuildingType();

  /// Sets the standards space type. This is a freeform field used to identify the space type for standards.
  /// Standards applied to this model will use this field to determine correct levels for lighting, occupancy, etc.
  /// More information can be found at https://github.com/NREL/openstudio-standards.
  bool setStandardsSpaceType(const std::string& standardsSpaceType);
  void resetStandardsSpaceType();

  //@}
  /** @name Other */
  //@{

  /// Get all spaces of this space type, includes spaces that inherit this space type
  std::vector<Space> spaces() const;

  /// Returns the default schedule set for the specified type if available by searching (in order):
  /// This space types's default schedule set
  /// The building's default schedule set
  /// The building's space type's default schedule set
  boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

  /// Returns all InternalMass in this space type.
  std::vector<InternalMass> internalMass() const;

  /// Returns all People in this space type.
  std::vector<People> people() const;

  /// Returns all Lights in this space type.
  std::vector<Lights> lights() const;

  /// Returns all \link Luminaire Luminaires \endlink in this space type.
  std::vector<Luminaire> luminaires() const;

  /// Returns all ElectricEquipment in this space type.
  std::vector<ElectricEquipment> electricEquipment() const;

  /// Returns all GasEquipment in this space type.
  std::vector<GasEquipment> gasEquipment() const;

  /// Returns all HotWaterEquipment in this space type.
  std::vector<HotWaterEquipment> hotWaterEquipment() const;

  /// Returns all SteamEquipment in this space type.
  std::vector<SteamEquipment> steamEquipment() const;

  /// Returns all OtherEquipment in this space type.
  std::vector<OtherEquipment> otherEquipment() const;

  /// Returns all SpaceInfiltrationDesignFlowRate objects in this space type.
  std::vector<SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates() const;

  /// Returns all SpaceInfiltrationEffectiveLeakageArea objects in this space type.
  std::vector<SpaceInfiltrationEffectiveLeakageArea> spaceInfiltrationEffectiveLeakageAreas() const;

  /// Returns DesignSpecificationOutdoorAir for this space type.
  boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir() const;

  /// Returns true if DesignSpecificationOutdoorAir is not directly specified for this space type.
  bool isDesignSpecificationOutdoorAirDefaulted() const;

  /// Sets DesignSpecificationOutdoorAir for this space type.
  bool setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) ;

  /// Resets DesignSpecificationOutdoorAir for this space type.
  void resetDesignSpecificationOutdoorAir();

  /** Set all schedules for child space loads to their default value if there is one. */
  void hardApplySpaceLoadSchedules();

  /** Returns the total people per space floor area in this space type, if it can be calculated 
   *  directly from the underlying people() data (without knowing floorArea). */
  boost::optional<double> peoplePerFloorArea() const;

  /** Sets the peoplePerFloorArea, using people()[0], if it exists, as a template for the 
   *  remaining People and PeopleDefinition parameters. All other people() in this SpaceType will
   *  be removed. */
  bool setPeoplePerFloorArea(double peoplePerFloorArea);

  /** Sets the peoplePerFloorArea, using templatePeople as a template for the remaining 
   *  People and PeopleDefinition parameters. All people() in this SpaceType (except for 
   *  templatePeople, if applicable), will be removed. */
  bool setPeoplePerFloorArea(double peoplePerFloorArea, const People& templatePeople);

  /** Returns the floor area per person for this space type, if it can be calculated directly 
   *  from the underlying people() data (without knowing floorArea). */
  boost::optional<double> spaceFloorAreaPerPerson() const;

  /** Sets the spaceFloorAreaPerPerson, using people()[0], if it exists, as a template for the 
   *  remaining People and PeopleDefinition parameters. All other people() in this SpaceType will
   *  be removed. */
  bool setSpaceFloorAreaPerPerson(double spaceFloorAreaPerPerson);

  /** Sets the spaceFloorAreaPerPerson, using templatePeople as a template for the remaining 
   *  People and PeopleDefinition parameters. All people() in this SpaceType (except for 
   *  templatePeople, if applicable), will be removed. */
  bool setSpaceFloorAreaPerPerson(double spaceFloorAreaPerPerson, const People& templatePeople);

  double getNumberOfPeople(double floorArea) const;

  double getPeoplePerFloorArea(double floorArea) const;

  double getFloorAreaPerPerson(double floorArea) const;

  /** Returns the total lighting power per space floor area, if possible. Only works if there are
   *  no luminaires in the space type, and if all lights are defined on a per space floor area 
   *  basis. */
  boost::optional<double> lightingPowerPerFloorArea() const;

  /** Sets the lightingPowerPerFloorArea, using lights()[0], if it exists, as a template for
   *  the remaining Lights and LightsDefinition parameters. All other lights() and luminaires() 
   *  in this SpaceType will be removed. */
  bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea);

  /** Sets the lightingPowerPerFloorArea, using templateLights as a template for the remaining
   *  Lights and LightsDefinition parameters. All lights() and luminaires() in this SpaceType 
   *  (except for templateLights, if applicable), will be removed. */
  bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, 
                                         const Lights& templateLights);

  /** Returns the total lighting power per person, if possible. Only works if there are no 
   *  luminaires in the space type, and if all lights are defined on a per person basis. */
  boost::optional<double> lightingPowerPerPerson() const;

  /** Sets the lightingPowerPerPerson, using lights()[0], if it exists, as a template for
   *  the remaining Lights and LightsDefinition parameters. All other lights() and luminaires() 
   *  in this SpaceType will be removed. */
  bool setLightingPowerPerPerson(double lightingPowerPerPerson);

  /** Sets the lightingPowerPerPerson, using templateLights as a template for the remaining
   *  Lights and LightsDefinition parameters. All lights() and luminaires() in this SpaceType 
   *  (except for templateLights, if applicable), will be removed. */
  bool setLightingPowerPerPerson(double lightingPowerPerPerson,const Lights& templateLights);

  /** Returns total lighting power for this space type, including lights and luminaires, assuming
   *  floorArea (m^2) and numPeople. */
  double getLightingPower(double floorArea, double numPeople) const;

  /** Returns the total lighting watts per space floor area for this space type, including lights
   *  and luminaires, assuming floorArea (m^2) and numPeople. */
  double getLightingPowerPerFloorArea(double floorArea, double numPeople) const;

  /** Returns the total lighting watts per person for this space type, including lights and 
   *  luminaires, assuming floorArea (m^2) and numPeople. */
  double getLightingPowerPerPerson(double floorArea, double numPeople) const;

  /** Returns the total electric equipment power per space floor area, if it can be calculated 
   *  directly from the underlying electricEquipment() data (without knowing floorArea and 
   *  numPeople). */
  boost::optional<double> electricEquipmentPowerPerFloorArea() const;

  /** Sets the electricEquipmentPowerPerFloorArea, using electricEquipment()[0], if it exists,
   *  as a template for the remaining ElectricEquipment and ElectricEquipmentDefinition parameters.
   *  All other electricEquipment() in this SpaceType will be removed. */
  bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea);

  /** Sets the electricEquipmentPowerPerFloorArea, using templateElectricEquipment as a template
   *  for the remaining ElectricEquipment and ElectricEquipmentDefinition parameters. All other 
   *  electricEquipment() in this SpaceType will be removed. */
  bool setElectricEquipmentPowerPerFloorArea(
      double electricEquipmentPowerPerFloorArea,
      const ElectricEquipment& templateElectricEquipment);

  /** Returns the total electric equipment power per person, if it can be calculated 
   *  directly from the underlying electricEquipment() data (without knowing floorArea and 
   *  numPeople). */
  boost::optional<double> electricEquipmentPowerPerPerson() const;

  /** Sets the electricEquipmentPowerPerPerson, using electricEquipment()[0], if it exists,
   *  as a template for the remaining ElectricEquipment and ElectricEquipmentDefinition parameters.
   *  All other electricEquipment() in this SpaceType will be removed. */
  bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson);

  /** Sets the electricEquipmentPowerPerPerson, using templateElectricEquipment as a template
   *  for the remaining ElectricEquipment and ElectricEquipmentDefinition parameters. All other 
   *  electricEquipment() in this SpaceType will be removed. */
  bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson,
                                          const ElectricEquipment& templateElectricEquipment);

  double getElectricEquipmentDesignLevel(double floorArea, double numPeople) const;

  double getElectricEquipmentPowerPerFloorArea(double floorArea, double numPeople) const;

  double getElectricEquipmentPowerPerPerson(double floorArea, double numPeople) const;

  /** Returns the total gas equipment power per space floor area, if it can be calculated 
   *  directly from the underlying gasEquipment() data (without knowing floorArea and 
   *  numPeople). */
  boost::optional<double> gasEquipmentPowerPerFloorArea() const;

  /** Sets the gasEquipmentPowerPerFloorArea, using gasEquipment()[0], if it exists,
   *  as a template for the remaining GasEquipment and GasEquipmentDefinition parameters.
   *  All other gasEquipment() in this SpaceType will be removed. */
  bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea);

  /** Sets the gasEquipmentPowerPerFloorArea, using templateGasEquipment as a template
   *  for the remaining GasEquipment and GasEquipmentDefinition parameters. All other 
   *  gasEquipment() in this SpaceType will be removed. */
  bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea,
                                             const GasEquipment& templateGasEquipment);

  /** Returns the total gas equipment power per person, if it can be calculated directly from 
   *  the underlying gasEquipment() data (without knowing floorArea and numPeople). */
  boost::optional<double> gasEquipmentPowerPerPerson() const;

  /** Sets the gasEquipmentPowerPerPerson, using gasEquipment()[0], if it exists,
   *  as a template for the remaining GasEquipment and GasEquipmentDefinition parameters.
   *  All other gasEquipment() in this SpaceType will be removed. */
  bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson);

  /** Sets the gasEquipmentPowerPerPerson, using templateGasEquipment as a template
   *  for the remaining GasEquipment and GasEquipmentDefinition parameters. All other 
   *  gasEquipment() in this SpaceType will be removed. */
  bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson,
                                     const GasEquipment& templateGasEquipment);

  double getGasEquipmentDesignLevel(double floorArea, double numPeople) const;

  double getGasEquipmentPowerPerFloorArea(double floorArea, double numPeople) const;

  double getGasEquipmentPowerPerPerson(double floorArea, double numPeople) const;

  // Returns the floor area in the building using this SpaceType.  Includes Space multipliers.
  double floorArea() const;

  // Will not allow user to set name to protected values such as "Plenum Space Type"
  boost::optional<std::string> setNameProtected(const std::string& newName);

  //@}
 protected:
  /// @cond
  typedef detail::SpaceType_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::SpaceType_Impl;

  explicit SpaceType(std::shared_ptr<detail::SpaceType_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SpaceType");
};

/** \relates SpaceType*/
typedef boost::optional<SpaceType> OptionalSpaceType;

/** \relates SpaceType*/
typedef std::vector<SpaceType> SpaceTypeVector;

} // model
} // openstudio

#endif // MODEL_SPACETYPE_HPP

