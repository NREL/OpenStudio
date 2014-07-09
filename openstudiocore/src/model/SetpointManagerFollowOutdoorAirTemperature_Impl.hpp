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

#ifndef MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP
#define MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP

#include "SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class MODEL_API SetpointManagerFollowOutdoorAirTemperature_Impl : public SetpointManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerFollowOutdoorAirTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SetpointManagerFollowOutdoorAirTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                         Model_Impl* model, 
                                         bool keepHandle);

    SetpointManagerFollowOutdoorAirTemperature_Impl(const SetpointManagerFollowOutdoorAirTemperature_Impl& other, 
                                         Model_Impl* model,
                                         bool keepHandles);

    virtual ~SetpointManagerFollowOutdoorAirTemperature_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters and Setters */
    //@{

    std::string referenceTemperatureType() const;

    void setReferenceTemperatureType(const std::string & value);

    double offsetTemperatureDifference() const;

    void setOffsetTemperatureDifference(double value);

    double maximumSetpointTemperature() const;

    void setMaximumSetpointTemperature(double value);

    double minimumSetpointTemperature() const;

    void setMinimumSetpointTemperature(double value);

    virtual boost::optional<Node> setpointNode() const;

    virtual std::string controlVariable() const;

    virtual bool setControlVariable(const std::string & value);

    //@}
   private:

    virtual bool setSetpointNode(const Node & node );

    virtual void resetSetpointNode();
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP

