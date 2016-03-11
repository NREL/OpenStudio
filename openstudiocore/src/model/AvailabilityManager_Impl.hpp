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

#ifndef MODEL_AVAILABILITYMANAGER_IMPL_HPP
#define MODEL_AVAILABILITYMANAGER_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Loop;

namespace detail {

  class MODEL_API AvailabilityManager_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AvailabilityManager_Impl(IddObjectType type, Model_Impl* model);

    AvailabilityManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    AvailabilityManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    AvailabilityManager_Impl(const AvailabilityManager_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~AvailabilityManager_Impl() {}

    boost::optional<Loop> loop() const;

   private:

    REGISTER_LOGGER("openstudio.model.AvailabilityManager");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGER_IMPL_HPP
