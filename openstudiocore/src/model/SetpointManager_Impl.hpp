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

#ifndef MODEL_SETPOINTMANAGER_IMPL_HPP
#define MODEL_SETPOINTMANAGER_IMPL_HPP

#include <model/HVACComponent_Impl.hpp>
#include <boost/optional.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API SetpointManager_Impl : public HVACComponent_Impl {
   public:

    SetpointManager_Impl(IddObjectType type, Model_Impl* model);

    SetpointManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SetpointManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                 Model_Impl* model, 
                                 bool keepHandle);

    SetpointManager_Impl(const SetpointManager_Impl& other, Model_Impl* model,bool keepHandles);

    virtual ~SetpointManager_Impl();

    virtual bool addToNode(Node & node);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual boost::optional<ParentObject> parent() const;

    virtual std::vector<ModelObject> children() const;

    virtual std::string controlVariable() const = 0;

    virtual bool setControlVariable( const std::string & value ) = 0;

    virtual boost::optional<Node> setpointNode() const = 0;

    virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

    virtual boost::optional<PlantLoop> plantLoop() const;

    virtual boost::optional<Loop> loop() const;

    virtual boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

   private:

    /** This is used to set the field SetpointNodeName.
      * Use addToNode to add a setpoint manager to a node. */
    virtual bool setSetpointNode( const Node & node ) = 0;

    /** This is used to reset the field SetpointNodeName. */
    virtual void resetSetpointNode() = 0;

    REGISTER_LOGGER("openstudio.model.SetpointManager");
  };

} // detail

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGER_IMPL_HPP

