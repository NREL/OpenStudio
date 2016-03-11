/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_AVAILABILITYMANAGER_HPP
#define MODEL_AVAILABILITYMANAGER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Loop;

namespace detail{
  class AvailabilityManager_Impl;
}

class MODEL_API AvailabilityManager : public ModelObject {

  public:

  AvailabilityManager(IddObjectType type,const Model& model);

  virtual ~AvailabilityManager() {}

  friend class openstudio::IdfObject;

  boost::optional<Loop> loop() const;

  /// @cond 

  typedef detail::AvailabilityManager_Impl ImplType;

  explicit AvailabilityManager(std::shared_ptr<detail::AvailabilityManager_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.AvailabilityManager");

  /// @endcond 

};

typedef boost::optional<AvailabilityManager> OptionalAvailabilityManager;

typedef std::vector<AvailabilityManager> AvailabilityManagerVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGER_HPP

