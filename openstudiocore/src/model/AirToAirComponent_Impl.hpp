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

#ifndef MODEL_AIRTOAIRCOMPONENT_IMPL_HPP
#define MODEL_AIRTOAIRCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class AirLoopHVAC;

namespace detail {

  class MODEL_API AirToAirComponent_Impl : public HVACComponent_Impl {
   public:

    AirToAirComponent_Impl(IddObjectType type, Model_Impl* model);

    AirToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    AirToAirComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    AirToAirComponent_Impl(const AirToAirComponent_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~AirToAirComponent_Impl() {}

    virtual unsigned primaryAirInletPort() = 0;

    virtual unsigned primaryAirOutletPort() = 0;

    virtual unsigned secondaryAirInletPort() = 0;

    virtual unsigned secondaryAirOutletPort() = 0;

    boost::optional<ModelObject> primaryAirInletModelObject();

    boost::optional<ModelObject> primaryAirOutletModelObject();

    boost::optional<ModelObject> secondaryAirInletModelObject();

    boost::optional<ModelObject> secondaryAirOutletModelObject();

    bool addToNode(Node & node);

    std::vector<openstudio::IdfObject> remove();

    ModelObject clone(Model model) const;

   private:

    REGISTER_LOGGER("openstudio.model.AirToAirComponent");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTOAIRCOMPONENT_IMPL_HPP
