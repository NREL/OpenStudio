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

#ifndef MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP
#define MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {

  /** EvaporativeCoolerIndirectResearchSpecial_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeCoolerIndirectResearchSpecial.*/
  class MODEL_API EvaporativeCoolerIndirectResearchSpecial_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EvaporativeCoolerIndirectResearchSpecial_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    EvaporativeCoolerIndirectResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    EvaporativeCoolerIndirectResearchSpecial_Impl(const EvaporativeCoolerIndirectResearchSpecial_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~EvaporativeCoolerIndirectResearchSpecial_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    double coolerMaximumEffectiveness() const;

    boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

    bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

    boost::optional<double> secondaryFanFlowRate() const;

    bool isSecondaryFanFlowRateAutosized() const;

    double secondaryFanTotalEfficiency() const;

    double secondaryFanDeltaPressure() const;

    double dewpointEffectivenessFactor() const;

    boost::optional<double> driftLossFraction() const;

    boost::optional<double> blowdownConcentrationRatio() const;

    boost::optional<Curve> wetbulbEffectivenessFlowRatioModifierCurve() const;

    boost::optional<double> coolerDrybulbDesignEffectiveness() const;

    boost::optional<Curve> drybulbEffectivenessFlowRatioModifierCurve() const;

    double waterPumpPowerSizingFactor() const;

    boost::optional<Curve> waterPumpPowerModifierCurve() const;

    double secondaryAirFlowScalingFactor() const;

    boost::optional<double> secondaryAirFanDesignPower() const;

    bool isSecondaryAirFanDesignPowerAutosized() const;

    boost::optional<Curve> secondaryAirFanPowerModifierCurve() const;

    boost::optional<double> primaryDesignAirFlowRate() const;

    bool isPrimaryDesignAirFlowRateAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCoolerMaximumEffectiveness(double coolerMaximumEffectiveness);

    void setRecirculatingWaterPumpPowerConsumption(double recirculatingWaterPumpPowerConsumption);

    void autosizeRecirculatingWaterPumpPowerConsumption();

    bool setSecondaryFanFlowRate(boost::optional<double> secondaryFanFlowRate);

    void resetSecondaryFanFlowRate();

    void autosizeSecondaryFanFlowRate();

    bool setSecondaryFanTotalEfficiency(double secondaryFanTotalEfficiency);

    bool setSecondaryFanDeltaPressure(double secondaryFanDeltaPressure);

    void setDewpointEffectivenessFactor(double dewpointEffectivenessFactor);

    bool setDriftLossFraction(boost::optional<double> driftLossFraction);

    void resetDriftLossFraction();

    bool setBlowdownConcentrationRatio(boost::optional<double> blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    bool setWetbulbEffectivenessFlowRatioModifierCurve(const boost::optional<Curve>& curve);

    void resetWetbulbEffectivenessFlowRatioModifierCurve();

    bool setCoolerDrybulbDesignEffectiveness(boost::optional<double> coolerDrybulbDesignEffectiveness);

    void resetCoolerDrybulbDesignEffectiveness();

    bool setDrybulbEffectivenessFlowRatioModifierCurve(const boost::optional<Curve>& curve);

    void resetDrybulbEffectivenessFlowRatioModifierCurve();

    void setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

    bool setWaterPumpPowerModifierCurve(const boost::optional<Curve>& curve);

    void resetWaterPumpPowerModifierCurve();

    void setSecondaryAirFlowScalingFactor(double secondaryAirFlowScalingFactor);

    void setSecondaryAirFanDesignPower(boost::optional<double> secondaryAirFanDesignPower);

    void autosizeSecondaryAirFanDesignPower();

    bool setSecondaryAirFanPowerModifierCurve(const boost::optional<Curve>& curve);

    void resetSecondaryAirFanPowerModifierCurve();

    bool setPrimaryDesignAirFlowRate(boost::optional<double> primaryDesignAirFlowRate);

    void resetPrimaryDesignAirFlowRate();

    void autosizePrimaryDesignAirFlowRate();

    //@}
    /** @name Other */
    //@{
    unsigned inletPort() override;

    unsigned outletPort() override;

    bool addToNode(Node & node) override;

    bool setReliefAirInletNode(const Node & node);

    boost::optional<Node> reliefAirInletNode() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeCoolerIndirectResearchSpecial");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP

