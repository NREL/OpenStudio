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

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRRESET_IMPL_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRRESET_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Node;
class Schedule;

namespace detail {

  /** SetpointManagerOutdoorAirReset_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerOutdoorAirReset.*/
  class MODEL_API SetpointManagerOutdoorAirReset_Impl : public SetpointManager_Impl {
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
    
    

    
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerOutdoorAirReset_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    SetpointManagerOutdoorAirReset_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    SetpointManagerOutdoorAirReset_Impl(const SetpointManagerOutdoorAirReset_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~SetpointManagerOutdoorAirReset_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    virtual std::string controlVariable() const override;

    bool isControlVariableDefaulted() const;

    double setpointatOutdoorLowTemperature() const;

    Quantity getSetpointatOutdoorLowTemperature(bool returnIP=false) const;

    double outdoorLowTemperature() const;

    Quantity getOutdoorLowTemperature(bool returnIP=false) const;

    double setpointatOutdoorHighTemperature() const;

    Quantity getSetpointatOutdoorHighTemperature(bool returnIP=false) const;

    double outdoorHighTemperature() const;

    Quantity getOutdoorHighTemperature(bool returnIP=false) const;

    virtual boost::optional<Node> setpointNode() const override;

    boost::optional<Schedule> schedule() const;

    boost::optional<double> setpointatOutdoorLowTemperature2() const;

    OSOptionalQuantity getSetpointatOutdoorLowTemperature2(bool returnIP=false) const;

    boost::optional<double> outdoorLowTemperature2() const;

    OSOptionalQuantity getOutdoorLowTemperature2(bool returnIP=false) const;

    boost::optional<double> setpointatOutdoorHighTemperature2() const;

    OSOptionalQuantity getSetpointatOutdoorHighTemperature2(bool returnIP=false) const;

    boost::optional<double> outdoorHighTemperature2() const;

    OSOptionalQuantity getOutdoorHighTemperature2(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    virtual bool setControlVariable(const std::string& controlVariable) override;

    void resetControlVariable();

    void setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

    bool setSetpointatOutdoorLowTemperature(const Quantity& setpointatOutdoorLowTemperature);

    void setOutdoorLowTemperature(double outdoorLowTemperature);

    bool setOutdoorLowTemperature(const Quantity& outdoorLowTemperature);

    void setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

    bool setSetpointatOutdoorHighTemperature(const Quantity& setpointatOutdoorHighTemperature);

    void setOutdoorHighTemperature(double outdoorHighTemperature);

    bool setOutdoorHighTemperature(const Quantity& outdoorHighTemperature);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    void setSetpointatOutdoorLowTemperature2(boost::optional<double> setpointatOutdoorLowTemperature2);

    bool setSetpointatOutdoorLowTemperature2(const OSOptionalQuantity& setpointatOutdoorLowTemperature2);

    void resetSetpointatOutdoorLowTemperature2();

    void setOutdoorLowTemperature2(boost::optional<double> outdoorLowTemperature2);

    bool setOutdoorLowTemperature2(const OSOptionalQuantity& outdoorLowTemperature2);

    void resetOutdoorLowTemperature2();

    void setSetpointatOutdoorHighTemperature2(boost::optional<double> setpointatOutdoorHighTemperature2);

    bool setSetpointatOutdoorHighTemperature2(const OSOptionalQuantity& setpointatOutdoorHighTemperature2);

    void resetSetpointatOutdoorHighTemperature2();

    void setOutdoorHighTemperature2(boost::optional<double> outdoorHighTemperature2);

    bool setOutdoorHighTemperature2(const OSOptionalQuantity& outdoorHighTemperature2);

    void resetOutdoorHighTemperature2();

    //@}
    /** @name Other */
    //@{

    // bool addToNode(Node & node);

    //@}
   protected:
   private:
    virtual bool setSetpointNode(const Node& node) override;

    virtual void resetSetpointNode() override;

    REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirReset");

    std::vector<std::string> controlVariableValues() const;
    openstudio::Quantity setpointatOutdoorLowTemperature_SI() const;
    openstudio::Quantity setpointatOutdoorLowTemperature_IP() const;
    openstudio::Quantity outdoorLowTemperature_SI() const;
    openstudio::Quantity outdoorLowTemperature_IP() const;
    openstudio::Quantity setpointatOutdoorHighTemperature_SI() const;
    openstudio::Quantity setpointatOutdoorHighTemperature_IP() const;
    openstudio::Quantity outdoorHighTemperature_SI() const;
    openstudio::Quantity outdoorHighTemperature_IP() const;
    openstudio::OSOptionalQuantity setpointatOutdoorLowTemperature2_SI() const;
    openstudio::OSOptionalQuantity setpointatOutdoorLowTemperature2_IP() const;
    openstudio::OSOptionalQuantity outdoorLowTemperature2_SI() const;
    openstudio::OSOptionalQuantity outdoorLowTemperature2_IP() const;
    openstudio::OSOptionalQuantity setpointatOutdoorHighTemperature2_SI() const;
    openstudio::OSOptionalQuantity setpointatOutdoorHighTemperature2_IP() const;
    openstudio::OSOptionalQuantity outdoorHighTemperature2_SI() const;
    openstudio::OSOptionalQuantity outdoorHighTemperature2_IP() const;

    boost::optional<ModelObject> setpointNodeAsModelObject() const;
    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setSetpointNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGEROUTDOORAIRRESET_IMPL_HPP

