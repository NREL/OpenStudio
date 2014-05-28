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

#ifndef MODEL_TIMESTEP_HPP
#define MODEL_TIMESTEP_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class Timestep_Impl;
} // detail

/** Timestep derives from ModelObject and is an interface to the OpenStudio IDD object named "Timestep".
 *
 *  Timestep allows the user to set the zone timestep for EnergyPlus simulations.
 *  Timestep does not have a public constructor because it is a unique ModelObject.  
 *  To get the Timestep object for a Model or create one if it does not yet exist use model.getUniqueObject<Timestep>().  
 *  To get the Timestep object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<Timestep>().
 */
class MODEL_API Timestep : public ModelObject {
 public:

  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  virtual ~Timestep() {}

  //@}
  /** @name Getters */
  //@{

  int numberOfTimestepsPerHour() const;

  bool isNumberOfTimestepsPerHourDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour);

  void resetNumberOfTimestepsPerHour();

  //@}

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new Timestep object in the model.
  explicit Timestep(const Model& model);

  //@}

  /// @cond
  typedef detail::Timestep_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  // constructor
  explicit Timestep(std::shared_ptr<detail::Timestep_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.Timestep");

  /// @endcond

};

/** \relates Timestep */
typedef boost::optional<Timestep> OptionalTimestep;

} // model

} // openstudio

#endif // MODEL_TIMESTEP_HPP
