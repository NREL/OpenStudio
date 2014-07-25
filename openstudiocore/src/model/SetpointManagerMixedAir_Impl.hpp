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

#ifndef MODEL_SETPOINTMANAGERMIXEDAIR_IMPL_HPP
#define MODEL_SETPOINTMANAGERMIXEDAIR_IMPL_HPP

#include "SetpointManager_Impl.hpp"
#include <boost/optional.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API SetpointManagerMixedAir_Impl : public SetpointManager_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string controlVariable READ controlVariable WRITE setControlVariable);
   public:

    SetpointManagerMixedAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SetpointManagerMixedAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                 Model_Impl* model, 
                                 bool keepHandle);

    SetpointManagerMixedAir_Impl(const SetpointManagerMixedAir_Impl& other, Model_Impl* model,bool keepHandles);

    virtual ~SetpointManagerMixedAir_Impl();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;

    virtual bool addToNode(Node & node);

    virtual std::string controlVariable() const;

    virtual bool setControlVariable( const std::string& controlVariable );

    boost::optional<Node> referenceSetpointNode();

    void setReferenceSetpointNode(Node& node );

    void resetReferenceSetpointNode();

    boost::optional<Node> fanInletNode();

    void setFanInletNode(Node& node );

    void resetFanInletNode();

    boost::optional<Node> fanOutletNode();

    void setFanOutletNode(Node& node );

    void resetFanOutletNode();

    virtual boost::optional<Node> setpointNode() const;

   private:
    virtual bool setSetpointNode( const Node & node );

    virtual void resetSetpointNode();

    REGISTER_LOGGER("openstudio.model.SetpointManagerMixedAir");
  };

} // detail

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGERMIXEDAIR_IMPL_HPP

