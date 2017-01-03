/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_COILCOOLINGWATER_IMPL_HPP
#define MODEL_COILCOOLINGWATER_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class MODEL_API CoilCoolingWater_Impl : public WaterToAirComponent_Impl {
    

    
  public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CoilCoolingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    CoilCoolingWater_Impl(const CoilCoolingWater_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~CoilCoolingWater_Impl();

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<openstudio::IdfObject> remove() override;

    virtual bool removeFromPlantLoop() override;

    virtual ModelObject clone(Model model) const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned airInletPort() override;

    virtual unsigned airOutletPort() override;

    virtual unsigned waterInletPort() override;

    virtual unsigned waterOutletPort() override;

    virtual bool addToNode(Node & node) override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    //@}

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule & schedule );

    boost::optional<double> designWaterFlowRate();

    void setDesignWaterFlowRate( double value );

    bool isDesignWaterFlowRateAutosized();

    void autosizeDesignWaterFlowRate();

    boost::optional<double> designAirFlowRate();

    void setDesignAirFlowRate( double value );

    bool isDesignAirFlowRateAutosized();

    void autosizeDesignAirFlowRate();

    boost::optional<double> designInletWaterTemperature();

    void setDesignInletWaterTemperature( double value );

    bool isDesignInletWaterTemperatureAutosized();

    void autosizeDesignInletWaterTemperature();

    boost::optional<double> designInletAirTemperature();

    void setDesignInletAirTemperature( double value );

    bool isDesignInletAirTemperatureAutosized();

    void autosizeDesignInletAirTemperature();

    boost::optional<double> designOutletAirTemperature();

    void setDesignOutletAirTemperature( double value );

    bool isDesignOutletAirTemperatureAutosized();

    void autosizeDesignOutletAirTemperature();

    boost::optional<double> designInletAirHumidityRatio();

    void setDesignInletAirHumidityRatio( double value );

    bool isDesignInletAirHumidityRatioAutosized();

    void autosizeDesignInletAirHumidityRatio();

    boost::optional<double> designOutletAirHumidityRatio();

    void setDesignOutletAirHumidityRatio( double value );

    bool isDesignOutletAirHumidityRatioAutosized();

    void autosizeDesignOutletAirHumidityRatio();

    std::string typeOfAnalysis();

    void setTypeOfAnalysis( std::string value );

    std::string heatExchangerConfiguration();

    void setHeatExchangerConfiguration( std::string value );

  private:    
    REGISTER_LOGGER("openstudio.model.CoilCoolingWater");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;
  };

} // detail

} // model

} // openstudio

#endif // MODEL_COILCOOLINGWATER_IMPL_HPP
