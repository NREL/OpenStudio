/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

//#include <utilities/units/Quantity.hpp>
//#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoilHeatingLowTempRadiantVarFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingLowTempRadiantVarFlow.*/
  class MODEL_API CoilHeatingLowTempRadiantVarFlow_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingLowTempRadiantVarFlow_Impl(const CoilHeatingLowTempRadiantVarFlow_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilHeatingLowTempRadiantVarFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    //virtual ModelObject clone(Model model) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> maximumHotWaterFlow() const;

    // OSOptionalQuantity getMaximumHotWaterFlow(bool returnIP=false) const;

    bool isMaximumHotWaterFlowDefaulted() const;

    bool isMaximumHotWaterFlowAutosized() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //Connection heatingWaterInletNode() const;

    //// TODO: Check return type. From object lists, some candidates are: Connection.
    //Connection heatingWaterOutletNode() const;

    double heatingControlThrottlingRange() const;

    //Quantity getHeatingControlThrottlingRange(bool returnIP=false) const;

    bool isHeatingControlThrottlingRangeDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> heatingControlTemperatureSchedule() const;

  boost::optional<double> autosizedMaximumHotWaterFlow() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumHotWaterFlow(boost::optional<double> maximumHotWaterFlow);

    //bool setMaximumHotWaterFlow(const OSOptionalQuantity& maximumHotWaterFlow);

    void resetMaximumHotWaterFlow();

    void autosizeMaximumHotWaterFlow();

    bool setHeatingControlThrottlingRange(double heatingControlThrottlingRange);

    //bool setHeatingControlThrottlingRange(const Quantity& heatingControlThrottlingRange);

    void resetHeatingControlThrottlingRange();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setHeatingControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingControlTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    bool addToNode(Node & node) override;

    // Used to find the ZoneHVAC that contains this coil
    boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantVarFlow");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    //boost::optional<Connection> optionalHeatingWaterInletNode() const;
    //boost::optional<Connection> optionalHeatingWaterOutletNode() const;

    //openstudio::OSOptionalQuantity maximumHotWaterFlow_SI() const;
    //openstudio::OSOptionalQuantity maximumHotWaterFlow_IP() const;
    //openstudio::Quantity heatingControlThrottlingRange_SI() const;
    //openstudio::Quantity heatingControlThrottlingRange_IP() const;

    //boost::optional<ModelObject> heatingWaterInletNodeAsModelObject() const;
    //boost::optional<ModelObject> heatingWaterOutletNodeAsModelObject() const;

    boost::optional<ModelObject> heatingControlTemperatureScheduleAsModelObject() const;

    //bool setHeatingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    //bool setHeatingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);

    bool setHeatingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

