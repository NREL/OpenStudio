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

#ifndef MODEL_ELECTRICEQUIPMENTDEFINITION_HPP
#define MODEL_ELECTRICEQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ElectricEquipmentDefinition_Impl;

} // detail

/** ElectricEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 
 *  'OS_ElectricEquipment_Definition'. Its fields are derived from the EnergyPlus IDD object 
 *  'ElectricEquipment'. \sa ElectricEquipment 
 */
class MODEL_API ElectricEquipmentDefinition : public SpaceLoadDefinition {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricEquipmentDefinition(const Model& model);

  virtual ~ElectricEquipmentDefinition() {}

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

  /** Also sets designLevelCalculationMethod accordingly. */
  bool setDesignLevel(double designLevel);

  /** Also sets designLevelCalculationMethod accordingly. */
  bool setWattsperSpaceFloorArea(double wattsperSpaceFloorArea);

  /** Also sets designLevelCalculationMethod accordingly. */
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
  typedef detail::ElectricEquipmentDefinition_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit ElectricEquipmentDefinition(std::shared_ptr<detail::ElectricEquipmentDefinition_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ElectricEquipmentDefinition");
};

/** \relates ElectricEquipmentDefinition*/
typedef boost::optional<ElectricEquipmentDefinition> OptionalElectricEquipmentDefinition;

/** \relates ElectricEquipmentDefinition*/
typedef std::vector<ElectricEquipmentDefinition> ElectricEquipmentDefinitionVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICEQUIPMENTDEFINITION_HPP

