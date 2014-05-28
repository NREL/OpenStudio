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

#ifndef MODEL_HOTWATEREQUIPMENTDEFINITION_HPP
#define MODEL_HOTWATEREQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class HotWaterEquipmentDefinition_Impl;

} // detail

/** HotWaterEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 
 *  'OS:HotWaterEquipment:Definition'. Its fields are derived from the EnergyPlus IDD object 
 *  'HotWaterEquipment'. Note that this object is not tied into any plant loop or water system,
 *  so it should not be used in models for which water use is an important output. 
 *  \sa HotWaterEquipment */
class MODEL_API HotWaterEquipmentDefinition : public SpaceLoadDefinition {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HotWaterEquipmentDefinition(const Model& model);

  virtual ~HotWaterEquipmentDefinition() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validDesignLevelCalculationMethodValues();

  //@}
  /** @name Getters */
  //@{

  std::string designLevelCalculationMethod() const;

  boost::optional<double> designLevel() const;

  boost::optional<double> wattsperSpaceFloorArea() const;

  boost::optional<double> wattsperPerson() const;

  double fractionLatent() const;

  bool isFractionLatentDefaulted() const;

  double fractionRadiant() const;

  bool isFractionRadiantDefaulted() const;

  double fractionLost() const;

  bool isFractionLostDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /// Sets designLevelCalculationMethod to 'DesignLevel', clears wattsperSpaceFloorArea
  /// and wattsperPerson, and sets designLevel.
  bool setDesignLevel(double designLevel);

  /// Sets designLevelCalculationMethod to 'Watts/Area', clears designLevel
  /// and wattsperPerson, and sets wattsperSpaceFloorArea.
  bool setWattsperSpaceFloorArea(double wattsperSpaceFloorArea);

  /// Sets designLevelCalculationMethod to 'Watts/Person', clears designLevel
  /// and wattsperSpaceFloorArea, and sets wattsperPerson.
  bool setWattsperPerson(double wattsperPerson);

  bool setFractionLatent(double fractionLatent);

  void resetFractionLatent();

  bool setFractionRadiant(double fractionRadiant);

  void resetFractionRadiant();

  bool setFractionLost(double fractionLost);

  void resetFractionLost();

  //@}
  /** @name Other */
  //@{

  /** Returns the design level represented by this definition, assuming floorArea (m^2) and 
   *  numPeople. */
  double getDesignLevel(double floorArea, double numPeople) const;

  /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and 
   *  numPeople. */
  double getPowerPerFloorArea(double floorArea, double numPeople) const;

  /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and 
   *  numPeople. */
  double getPowerPerPerson(double floorArea, double numPeople) const;
 
  /** If method is a \link validDesignLevelCalculationMethodValues valid design level 
   *  calculation method \endlink, changes this definition to an equivalent power level, under 
   *  the assumptions of floorArea (m^2) and numPeople. */
  bool setDesignLevelCalculationMethod(const std::string& method, 
                                       double floorArea, 
                                       double numPeople);

  //@}
 protected:
  /// @cond
  typedef detail::HotWaterEquipmentDefinition_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit HotWaterEquipmentDefinition(std::shared_ptr<detail::HotWaterEquipmentDefinition_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.HotWaterEquipmentDefinition");
};

/** \relates HotWaterEquipmentDefinition*/
typedef boost::optional<HotWaterEquipmentDefinition> OptionalHotWaterEquipmentDefinition;

/** \relates HotWaterEquipmentDefinition*/
typedef std::vector<HotWaterEquipmentDefinition> HotWaterEquipmentDefinitionVector;

} // model
} // openstudio

#endif // MODEL_HOTWATEREQUIPMENTDEFINITION_HPP

