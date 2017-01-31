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

#ifndef MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP
#define MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** ThermalStorageChilledWaterStratified_Impl is a WaterToWaterComponent_Impl that is the implementation class for ThermalStorageChilledWaterStratified.*/
  class MODEL_API ThermalStorageChilledWaterStratified_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ThermalStorageChilledWaterStratified_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    ThermalStorageChilledWaterStratified_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    ThermalStorageChilledWaterStratified_Impl(const ThermalStorageChilledWaterStratified_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~ThermalStorageChilledWaterStratified_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned supplyInletPort() override;

    virtual unsigned supplyOutletPort() override;

    virtual unsigned demandInletPort() override;

    virtual unsigned demandOutletPort() override;

    //@}
    /** @name Getters */
    //@{

    double tankVolume() const;

    double tankHeight() const;

    std::string tankShape() const;

    boost::optional<double> tankPerimeter() const;

    boost::optional<Schedule> setpointTemperatureSchedule() const;

    double deadbandTemperatureDifference() const;

    boost::optional<double> temperatureSensorHeight() const;

    boost::optional<double> minimumTemperatureLimit() const;

    boost::optional<double> nominalCoolingCapacity() const;

    std::string ambientTemperatureIndicator() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

    double useSideHeatTransferEffectiveness() const;

    boost::optional<Schedule> useSideAvailabilitySchedule() const;

    boost::optional<double> useSideInletHeight() const;

    bool isUseSideInletHeightAutocalculated() const;

    double useSideOutletHeight() const;

    boost::optional<double> useSideDesignFlowRate() const;

    bool isUseSideDesignFlowRateAutosized() const;

    double sourceSideHeatTransferEffectiveness() const;

    boost::optional<Schedule> sourceSideAvailabilitySchedule() const;

    double sourceSideInletHeight() const;

    boost::optional<double> sourceSideOutletHeight() const;

    bool isSourceSideOutletHeightAutocalculated() const;

    boost::optional<double> sourceSideDesignFlowRate() const;

    bool isSourceSideDesignFlowRateAutosized() const;

    double tankRecoveryTime() const;

    std::string inletMode() const;

    int numberofNodes() const;

    double additionalDestratificationConductivity() const;

    double node1AdditionalLossCoefficient() const;

    double node2AdditionalLossCoefficient() const;

    double node3AdditionalLossCoefficient() const;

    double node4AdditionalLossCoefficient() const;

    double node5AdditionalLossCoefficient() const;

    double node6AdditionalLossCoefficient() const;

    double node7AdditionalLossCoefficient() const;

    double node8AdditionalLossCoefficient() const;

    double node9AdditionalLossCoefficient() const;

    double node10AdditionalLossCoefficient() const;

    //@}
    /** @name Setters */
    //@{

    bool setTankVolume(double tankVolume);

    bool setTankHeight(double tankHeight);

    bool setTankShape(std::string tankShape);

    bool setTankPerimeter(boost::optional<double> tankPerimeter);

    void resetTankPerimeter();

    bool setSetpointTemperatureSchedule(Schedule& schedule);

    void resetSetpointTemperatureSchedule();

    bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

    bool setTemperatureSensorHeight(boost::optional<double> temperatureSensorHeight);

    void resetTemperatureSensorHeight();

    void setMinimumTemperatureLimit(boost::optional<double> minimumTemperatureLimit);

    void resetMinimumTemperatureLimit();

    void setNominalCoolingCapacity(boost::optional<double> nominalCoolingCapacity);

    void resetNominalCoolingCapacity();

    bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetAmbientTemperatureThermalZone();

    void setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

    void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

    bool setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness);

    bool setUseSideAvailabilitySchedule(Schedule& schedule);

    void resetUseSideAvailabilitySchedule();

    bool setUseSideInletHeight(boost::optional<double> useSideInletHeight);

    void autocalculateUseSideInletHeight();

    bool setUseSideOutletHeight(double useSideOutletHeight);

    bool setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate);

    void autosizeUseSideDesignFlowRate();

    bool setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness);

    bool setSourceSideAvailabilitySchedule(Schedule& schedule);

    void resetSourceSideAvailabilitySchedule();

    bool setSourceSideInletHeight(double sourceSideInletHeight);

    bool setSourceSideOutletHeight(boost::optional<double> sourceSideOutletHeight);

    void autocalculateSourceSideOutletHeight();

    bool setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate);

    void autosizeSourceSideDesignFlowRate();

    bool setTankRecoveryTime(double tankRecoveryTime);

    bool setInletMode(std::string inletMode);

    bool setNumberofNodes(int numberofNodes);

    bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

    void setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

    void setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

    void setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

    void setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

    void setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

    void setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

    void setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

    void setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

    void setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

    void setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ThermalStorageChilledWaterStratified");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
  };

} // detail

} // model
} // openstudio

#endif // MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP

