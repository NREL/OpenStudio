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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP

#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

class ZoneHVACEquipmentConnections;

namespace detail {

  class MODEL_API SetpointManagerSingleZoneReheat_Impl : public SetpointManager_Impl {
    Q_OBJECT;
    Q_PROPERTY(double minimumSupplyAirTemperature READ minimumSupplyAirTemperature WRITE setMinimumSupplyAirTemperature);
    Q_PROPERTY(double maximumSupplyAirTemperature READ maximumSupplyAirTemperature WRITE setMaximumSupplyAirTemperature);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> controlZone READ controlZoneAsModelObject WRITE setControlZoneAsModelObject);
   public:

    SetpointManagerSingleZoneReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SetpointManagerSingleZoneReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                         Model_Impl* model, 
                                         bool keepHandle);

    SetpointManagerSingleZoneReheat_Impl(const SetpointManagerSingleZoneReheat_Impl& other, 
                                         Model_Impl* model,
                                         bool keepHandles);

    virtual ~SetpointManagerSingleZoneReheat_Impl();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;

    virtual bool addToNode(Node & node);
    
    double minimumSupplyAirTemperature();
    
    void setMinimumSupplyAirTemperature( double value );
    
    double maximumSupplyAirTemperature();
    
    void setMaximumSupplyAirTemperature( double value );
    
    virtual boost::optional<Node> setpointNode() const;

    virtual std::string controlVariable() const;

    virtual bool setControlVariable( const std::string& controlVariable );

    boost::optional<ThermalZone> controlZone();

    void setControlZone(ThermalZone& thermalZone);

    void resetControlZone();

   private:
    virtual bool setSetpointNode( const Node & node );

    virtual void resetSetpointNode();

    boost::optional<ModelObject> controlZoneAsModelObject();

    bool setControlZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP

