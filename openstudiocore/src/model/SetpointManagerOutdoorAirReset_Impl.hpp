/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

    bool setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

    bool setSetpointatOutdoorLowTemperature(const Quantity& setpointatOutdoorLowTemperature);

    bool setOutdoorLowTemperature(double outdoorLowTemperature);

    bool setOutdoorLowTemperature(const Quantity& outdoorLowTemperature);

    bool setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

    bool setSetpointatOutdoorHighTemperature(const Quantity& setpointatOutdoorHighTemperature);

    bool setOutdoorHighTemperature(double outdoorHighTemperature);

    bool setOutdoorHighTemperature(const Quantity& outdoorHighTemperature);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    bool setSetpointatOutdoorLowTemperature2(boost::optional<double> setpointatOutdoorLowTemperature2);

    bool setSetpointatOutdoorLowTemperature2(const OSOptionalQuantity& setpointatOutdoorLowTemperature2);

    void resetSetpointatOutdoorLowTemperature2();

    bool setOutdoorLowTemperature2(boost::optional<double> outdoorLowTemperature2);

    bool setOutdoorLowTemperature2(const OSOptionalQuantity& outdoorLowTemperature2);

    void resetOutdoorLowTemperature2();

    bool setSetpointatOutdoorHighTemperature2(boost::optional<double> setpointatOutdoorHighTemperature2);

    bool setSetpointatOutdoorHighTemperature2(const OSOptionalQuantity& setpointatOutdoorHighTemperature2);

    void resetSetpointatOutdoorHighTemperature2();

    bool setOutdoorHighTemperature2(boost::optional<double> outdoorHighTemperature2);

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
