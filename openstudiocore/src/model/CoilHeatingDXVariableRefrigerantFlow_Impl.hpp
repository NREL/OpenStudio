/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {

  /** CoilHeatingDXVariableRefrigerantFlow_Impl is a HVACComponent_Impl that is the implementation class for CoilHeatingDXVariableRefrigerantFlow.*/
  class MODEL_API CoilHeatingDXVariableRefrigerantFlow_Impl : public HVACComponent_Impl {

   public:

    CoilHeatingDXVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    CoilHeatingDXVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    CoilHeatingDXVariableRefrigerantFlow_Impl(const CoilHeatingDXVariableRefrigerantFlow_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~CoilHeatingDXVariableRefrigerantFlow_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    Schedule availabilitySchedule() const;

    boost::optional<double> ratedTotalHeatingCapacity() const;

    bool isRatedTotalHeatingCapacityAutosized() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    Curve heatingCapacityRatioModifierFunctionofTemperatureCurve() const;

    Curve heatingCapacityModifierFunctionofFlowFractionCurve() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity);

    void autosizeRatedTotalHeatingCapacity();

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool setHeatingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCapacityModifierFunctionofFlowFractionCurve(const Curve& curve);

    ModelObject clone(Model model) const override;

    std::vector<ModelObject> children() const override;

  boost::optional<double> autosizedRatedTotalHeatingCapacity() const ;

  boost::optional<double> autosizedRatedAirFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

   protected:

   private:

    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableRefrigerantFlow");

    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<Curve> optionalHeatingCapacityRatioModifierFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingCapacityModifierFunctionofFlowFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP

