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
    Q_OBJECT;

    Q_PROPERTY(std::string controlVariable READ controlVariable WRITE setControlVariable RESET resetControlVariable);
    Q_PROPERTY(bool isControlVariableDefaulted READ isControlVariableDefaulted);
    Q_PROPERTY(std::vector<std::string> controlVariableValues READ controlVariableValues);

    Q_PROPERTY(double setpointatOutdoorLowTemperature READ setpointatOutdoorLowTemperature WRITE setSetpointatOutdoorLowTemperature);
    Q_PROPERTY(openstudio::Quantity setpointatOutdoorLowTemperature_SI READ setpointatOutdoorLowTemperature_SI WRITE setSetpointatOutdoorLowTemperature);
    Q_PROPERTY(openstudio::Quantity setpointatOutdoorLowTemperature_IP READ setpointatOutdoorLowTemperature_IP WRITE setSetpointatOutdoorLowTemperature);

    Q_PROPERTY(double outdoorLowTemperature READ outdoorLowTemperature WRITE setOutdoorLowTemperature);
    Q_PROPERTY(openstudio::Quantity outdoorLowTemperature_SI READ outdoorLowTemperature_SI WRITE setOutdoorLowTemperature);
    Q_PROPERTY(openstudio::Quantity outdoorLowTemperature_IP READ outdoorLowTemperature_IP WRITE setOutdoorLowTemperature);

    Q_PROPERTY(double setpointatOutdoorHighTemperature READ setpointatOutdoorHighTemperature WRITE setSetpointatOutdoorHighTemperature);
    Q_PROPERTY(openstudio::Quantity setpointatOutdoorHighTemperature_SI READ setpointatOutdoorHighTemperature_SI WRITE setSetpointatOutdoorHighTemperature);
    Q_PROPERTY(openstudio::Quantity setpointatOutdoorHighTemperature_IP READ setpointatOutdoorHighTemperature_IP WRITE setSetpointatOutdoorHighTemperature);

    Q_PROPERTY(double outdoorHighTemperature READ outdoorHighTemperature WRITE setOutdoorHighTemperature);
    Q_PROPERTY(openstudio::Quantity outdoorHighTemperature_SI READ outdoorHighTemperature_SI WRITE setOutdoorHighTemperature);
    Q_PROPERTY(openstudio::Quantity outdoorHighTemperature_IP READ outdoorHighTemperature_IP WRITE setOutdoorHighTemperature);

    Q_PROPERTY(boost::optional<double> setpointatOutdoorLowTemperature2 READ setpointatOutdoorLowTemperature2 WRITE setSetpointatOutdoorLowTemperature2 RESET resetSetpointatOutdoorLowTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity setpointatOutdoorLowTemperature2_SI READ setpointatOutdoorLowTemperature2_SI WRITE setSetpointatOutdoorLowTemperature2 RESET resetSetpointatOutdoorLowTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity setpointatOutdoorLowTemperature2_IP READ setpointatOutdoorLowTemperature2_IP WRITE setSetpointatOutdoorLowTemperature2 RESET resetSetpointatOutdoorLowTemperature2);

    Q_PROPERTY(boost::optional<double> outdoorLowTemperature2 READ outdoorLowTemperature2 WRITE setOutdoorLowTemperature2 RESET resetOutdoorLowTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity outdoorLowTemperature2_SI READ outdoorLowTemperature2_SI WRITE setOutdoorLowTemperature2 RESET resetOutdoorLowTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity outdoorLowTemperature2_IP READ outdoorLowTemperature2_IP WRITE setOutdoorLowTemperature2 RESET resetOutdoorLowTemperature2);

    Q_PROPERTY(boost::optional<double> setpointatOutdoorHighTemperature2 READ setpointatOutdoorHighTemperature2 WRITE setSetpointatOutdoorHighTemperature2 RESET resetSetpointatOutdoorHighTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity setpointatOutdoorHighTemperature2_SI READ setpointatOutdoorHighTemperature2_SI WRITE setSetpointatOutdoorHighTemperature2 RESET resetSetpointatOutdoorHighTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity setpointatOutdoorHighTemperature2_IP READ setpointatOutdoorHighTemperature2_IP WRITE setSetpointatOutdoorHighTemperature2 RESET resetSetpointatOutdoorHighTemperature2);

    Q_PROPERTY(boost::optional<double> outdoorHighTemperature2 READ outdoorHighTemperature2 WRITE setOutdoorHighTemperature2 RESET resetOutdoorHighTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity outdoorHighTemperature2_SI READ outdoorHighTemperature2_SI WRITE setOutdoorHighTemperature2 RESET resetOutdoorHighTemperature2);
    Q_PROPERTY(openstudio::OSOptionalQuantity outdoorHighTemperature2_IP READ outdoorHighTemperature2_IP WRITE setOutdoorHighTemperature2 RESET resetOutdoorHighTemperature2);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject RESET resetSchedule);
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters */
    //@{

    virtual std::string controlVariable() const;

    bool isControlVariableDefaulted() const;

    double setpointatOutdoorLowTemperature() const;

    Quantity getSetpointatOutdoorLowTemperature(bool returnIP=false) const;

    double outdoorLowTemperature() const;

    Quantity getOutdoorLowTemperature(bool returnIP=false) const;

    double setpointatOutdoorHighTemperature() const;

    Quantity getSetpointatOutdoorHighTemperature(bool returnIP=false) const;

    double outdoorHighTemperature() const;

    Quantity getOutdoorHighTemperature(bool returnIP=false) const;

    virtual boost::optional<Node> setpointNode() const;

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

    virtual bool setControlVariable(const std::string& controlVariable);

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
    virtual bool setSetpointNode(const Node& node);

    virtual void resetSetpointNode();

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

