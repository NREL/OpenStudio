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

#ifndef MODEL_SPACELOAD_HPP
#define MODEL_SPACELOAD_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {
namespace model {

class SpaceType;

namespace detail{
  class SpaceLoad_Impl;
}

/** SpaceLoad is an abstract class derived from SpaceItem.
 */
class MODEL_API SpaceLoad : public SpaceItem {
 public:
  virtual ~SpaceLoad() {}

  /// Returns the parent SpaceType.
  boost::optional<SpaceType> spaceType() const;

  /// Sets the parent SpaceType.
  bool setSpaceType(const SpaceType& spaceType);

  void resetSpaceType();

  /// Converts space load levels to abosolute values rather than by per area, etc.
  /// Also hard sizes any costs associated with this load.
  /// Only possible if this space load is parented by a space, returns false if 
  /// parented by a space type.
  bool hardSize();

  /// Directly applies default schedules to this space load if found.
  bool hardApplySchedules();

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new SpaceLoad object in the model.
  SpaceLoad(IddObjectType type,const Model& model);

  //@}

  /// @cond

  typedef detail::SpaceLoad_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SpaceLoad_Impl;

  explicit SpaceLoad(std::shared_ptr<detail::SpaceLoad_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.SpaceLoad");

  /// @endcond

};

/** \relates SpaceLoad */
typedef boost::optional<SpaceLoad> OptionalSpaceLoad;

/** \relates SpaceLoad */
typedef std::vector<SpaceLoad> SpaceLoadVector;

} // model
} // openstudio

#endif // MODEL_SPACELOAD_HPP
