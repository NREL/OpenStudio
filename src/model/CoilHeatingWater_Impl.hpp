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

#ifndef MODEL_COILHEATINGWATER_IMPL_HPP
#define MODEL_COILHEATINGWATER_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class MODEL_API CoilHeatingWater_Impl : public WaterToAirComponent_Impl {
      public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CoilHeatingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    CoilHeatingWater_Impl(const CoilHeatingWater_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~CoilHeatingWater_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove() override;

    virtual bool removeFromPlantLoop() override;

    virtual ModelObject clone(Model model) const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual bool addToNode(Node & node) override;

    virtual unsigned airInletPort() const override;

    virtual unsigned airOutletPort() const override;

    virtual unsigned waterInletPort() const override;

    virtual unsigned waterOutletPort() const override;

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    //@}
    /** @name Getters and Setters */
    //@{

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> uFactorTimesAreaValue();

    bool setUFactorTimesAreaValue( double value );

    bool isUFactorTimesAreaValueAutosized();

    void autosizeUFactorTimesAreaValue();

    boost::optional<double> maximumWaterFlowRate();

    bool setMaximumWaterFlowRate( double value );

    bool isMaximumWaterFlowRateAutosized();

    void autosizeMaximumWaterFlowRate();

    std::string performanceInputMethod();

    bool setPerformanceInputMethod( std::string value );

    boost::optional<double> ratedCapacity();

    bool setRatedCapacity( double value );

    bool isRatedCapacityAutosized();

    void autosizeRatedCapacity();

    double ratedInletWaterTemperature();

    bool setRatedInletWaterTemperature( double value );

    double ratedInletAirTemperature();

    bool setRatedInletAirTemperature( double value );

    double ratedOutletWaterTemperature();

    bool setRatedOutletWaterTemperature( double value );

    double ratedOutletAirTemperature();

    bool setRatedOutletAirTemperature( double value );

    double ratedRatioForAirAndWaterConvection();

    bool setRatedRatioForAirAndWaterConvection( double value );

  boost::optional<double> autosizedUFactorTimesAreaValue() const ;

  boost::optional<double> autosizedMaximumWaterFlowRate() const ;

  boost::optional<double> autosizedRatedCapacity() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Other */
    //@{

    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}

  private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWater");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATER_IMPL_HPP
