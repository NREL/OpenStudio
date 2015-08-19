/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP
#define MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Schedule;
class ThermalZone;
class Connection;
class Connection;
class Schedule;
class Connection;
class Connection;
class Schedule;

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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    double tankVolume() const;

    double tankHeight() const;

    std::string tankShape() const;

    boost::optional<double> tankPerimeter() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> setpointTemperatureSchedule() const;

    double deadbandTemperatureDifference() const;

    boost::optional<double> temperatureSensorHeight() const;

    boost::optional<double> minimumTemperatureLimit() const;

    boost::optional<double> nominalCoolingCapacity() const;

    std::string ambientTemperatureIndicator() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> ambientTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection useSideInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection useSideOutletNode() const;

    double useSideHeatTransferEffectiveness() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> useSideAvailabilitySchedule() const;

    boost::optional<double> useSideInletHeight() const;

    bool isUseSideInletHeightAutocalculated() const;

    double useSideOutletHeight() const;

    boost::optional<double> useSideDesignFlowRate() const;

    bool isUseSideDesignFlowRateAutosized() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection sourceSideInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection sourceSideOutletNode() const;

    double sourceSideHeatTransferEffectiveness() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
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

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
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

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetAmbientTemperatureThermalZone();

    void setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

    void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setUseSideInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setUseSideOutletNode(const Connection& connection);

    bool setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setUseSideAvailabilitySchedule(Schedule& schedule);

    void resetUseSideAvailabilitySchedule();

    bool setUseSideInletHeight(boost::optional<double> useSideInletHeight);

    void autocalculateUseSideInletHeight();

    bool setUseSideOutletHeight(double useSideOutletHeight);

    bool setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate);

    void autosizeUseSideDesignFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setSourceSideInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setSourceSideOutletNode(const Connection& connection);

    bool setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
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

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalUseSideInletNode() const;
    boost::optional<Connection> optionalUseSideOutletNode() const;
    boost::optional<Connection> optionalSourceSideInletNode() const;
    boost::optional<Connection> optionalSourceSideOutletNode() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP

