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

#ifndef MODEL_SPACEITEM_HPP
#define MODEL_SPACEITEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Space;

namespace detail{
  class SpaceItem_Impl;
}

/** SpaceItem is an abstract class derived from ModelObject.
 */
class MODEL_API SpaceItem : public ModelObject {
 public:
  virtual ~SpaceItem() {}

  /// Returns the parent Space.
  boost::optional<Space> space() const;

  /// Sets the parent Space.
  bool setSpace(const Space& space);

  void resetSpace();

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new SpaceItem object in the model.
  SpaceItem(IddObjectType type,const Model& model);


  //@}

  /// @cond

  typedef detail::SpaceItem_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SpaceItem_Impl;

  explicit SpaceItem(std::shared_ptr<detail::SpaceItem_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.SpaceItem");

  /// @endcond

};

/** \relates SpaceItem */
typedef boost::optional<SpaceItem> OptionalSpaceItem;

/** \relates SpaceItem */
typedef std::vector<SpaceItem> SpaceItemVector;

} // model
} // openstudio

#endif // MODEL_SPACEITEM_HPP
