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

#ifndef MODEL_SPACELOADINSTANCE_HPP
#define MODEL_SPACELOADINSTANCE_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

class SpaceLoadDefinition;

namespace detail{
  class SpaceLoadInstance_Impl;
}

/** SpaceLoadInstance is an abstract class derived from SpaceLoad. \link SpaceLoadInstance
 *  SpaceLoadInstances \endlink instantiate a given SpaceLoadDefinition in a Space or SpaceType 
 *  by defining a multiplier and a schedule. 
 */
class MODEL_API SpaceLoadInstance : public SpaceLoad {
 public:
  virtual ~SpaceLoadInstance() {}

  /** Returns the definition of this instance. **/
  SpaceLoadDefinition definition() const;

  /** Sets the definition of this instance. **/
  bool setDefinition(const SpaceLoadDefinition& definition);

  /** Ensures that no other instances share this instances definition. **/
  void makeUnique();

  /** Gets the multiplier for this space instance, defaults to 1. **/
  double multiplier() const;

  /** Returns true if the multiplier is defaulted. **/
  bool isMultiplierDefaulted() const;

  /** Returns the floor area associated with this space load instance.  
  If this space load is associated with a single space then that space's floor area is returned.
  If this space load is associated with a space type then the sum of all space floor area in that space type is returned.
  Space and SpaceLoadInstance multipliers are included in the result **/
  double floorArea() const;

  /** Returns the number of instances this space load instance represents.  
  Space and SpaceLoadInstance multipliers are included in the result **/
  int quantity() const;

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new SpaceLoadInstance object in the model.
  SpaceLoadInstance(IddObjectType type, const SpaceLoadDefinition& definition);


  //@}

  /// @cond

  typedef detail::SpaceLoadInstance_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SpaceLoadInstance_Impl;

  explicit SpaceLoadInstance(std::shared_ptr<detail::SpaceLoadInstance_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.SpaceLoadInstance");

  /// @endcond

};

/** \relates SpaceLoadInstance */
typedef boost::optional<SpaceLoadInstance> OptionalSpaceLoadInstance;

/** \relates SpaceLoadInstance */
typedef std::vector<SpaceLoadInstance> SpaceLoadInstanceVector;

} // model
} // openstudio

#endif // MODEL_SPACELOADINSTANCE_HPP
