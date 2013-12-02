/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/HVACComponent_Impl.hpp>

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class MODEL_API SetpointManagerFollowOutdoorAirTemperature_Impl : public HVACComponent_Impl {
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

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model);

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ModelObject> children() const;

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

    boost::optional<Node> setpointNode() const;

    std::string controlVariable() const;

    void setControlVariable(const std::string & value);

    //@}
   private:

    void setSetpointNode( Node & node );
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP

