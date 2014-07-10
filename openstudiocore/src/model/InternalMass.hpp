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

#ifndef MODEL_INTERNALMASS_HPP
#define MODEL_INTERNALMASS_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class InternalMassDefinition;

namespace detail {

  class InternalMass_Impl;

} // detail

/** InternalMass is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS_InternalMass'.
 *  Its fields are derived from the EnergyPlus IDD object 'InternalMass'. 
 *  \sa InternalMassDefinition
 */
class MODEL_API InternalMass : public SpaceLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit InternalMass(const InternalMassDefinition& internalMassDefinition);

  virtual ~InternalMass() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Gets the InternalMassDefinition object. */
  InternalMassDefinition internalMassDefinition() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the InternalMassDefinition object. */
  bool setInternalMassDefinition(const InternalMassDefinition& definition);

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  //@}
  /** @name Other */
  //@{

  /** Returns the surfaceArea if possible based on the underlying data of 
   *  internalMassDefinition(). */
  boost::optional<double> surfaceArea() const;

  /** Returns the surfaceAreaPerFloorArea if possible based on the underlying data of 
   *  internalMassDefinition(). */
  boost::optional<double> surfaceAreaPerFloorArea() const;

  /** Returns the surfaceAreaPerPerson if possible based on the underlying data of 
   *  internalMassDefinition(). */
  boost::optional<double> surfaceAreaPerPerson() const;

  /** Returns the construction surface area represented by this instance, assuming floorArea (m^2)
   *  and numPeople. */
  double getSurfaceArea(double floorArea, double numPeople) const;

  /** Returns the m^2/m^2 of construction surface area per floor area represented by this 
   *  instance, assuming floorArea (m^2) and numPeople. */
  double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

  /** Returns the m^2/person of construction surface area represented by this instance, assuming 
   *  floorArea (m^2) and numPeople. */
  double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

  //@}
 protected:
  /// @cond
  typedef detail::InternalMass_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit InternalMass(std::shared_ptr<detail::InternalMass_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.InternalMass");
};

/** \relates InternalMass*/
typedef boost::optional<InternalMass> OptionalInternalMass;

/** \relates InternalMass*/
typedef std::vector<InternalMass> InternalMassVector;

} // model
} // openstudio

#endif // MODEL_INTERNALMASS_HPP

