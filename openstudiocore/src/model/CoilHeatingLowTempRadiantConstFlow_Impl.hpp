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

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoilHeatingLowTempRadiantConstFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingLowTempRadiantConstFlow.*/
  class MODEL_API CoilHeatingLowTempRadiantConstFlow_Impl : public StraightComponent_Impl {

    public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilHeatingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilHeatingLowTempRadiantConstFlow_Impl(const CoilHeatingLowTempRadiantConstFlow_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~CoilHeatingLowTempRadiantConstFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;


    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> heatingHighWaterTemperatureSchedule() const;

    boost::optional<Schedule> heatingLowWaterTemperatureSchedule() const;

    boost::optional<Schedule> heatingHighControlTemperatureSchedule() const;

    boost::optional<Schedule> heatingLowControlTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingHighWaterTemperatureSchedule(Schedule& schedule);

    void resetHeatingHighWaterTemperatureSchedule();

    bool setHeatingLowWaterTemperatureSchedule(Schedule& schedule);

    void resetHeatingLowWaterTemperatureSchedule();

    bool setHeatingHighControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingHighControlTemperatureSchedule();

    bool setHeatingLowControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingLowControlTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantConstFlow");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals

    boost::optional<ModelObject> heatingWaterInletNodeAsModelObject() const;
    boost::optional<ModelObject> heatingWaterOutletNodeAsModelObject() const;
    boost::optional<ModelObject> heatingHighWaterTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingLowWaterTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingHighControlTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingLowControlTemperatureScheduleAsModelObject() const;

    bool setHeatingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

