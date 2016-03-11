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

#ifndef MODEL_COILCOOLINGDXVARIABLESPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
// class WaterStorageTank;
class Schedule;
class CoilCoolingDXVariableSpeedSpeedData;
class ModelObjectList;

namespace detail {

  /** CoilCoolingDXVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXVariableSpeed.*/
  class MODEL_API CoilCoolingDXVariableSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingDXVariableSpeed_Impl(const IdfObject& idfObject,
                                    Model_Impl* model,
                                    bool keepHandle);

    CoilCoolingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    CoilCoolingDXVariableSpeed_Impl(const CoilCoolingDXVariableSpeed_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    virtual ~CoilCoolingDXVariableSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

    Curve energyPartLoadFractionCurve() const;

    // boost::optional<std::string> condenserAirInletNodeName() const;

    std::string condenserType() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    //@}
    /** @name Setters */
    //@{

    void setNominalSpeedLevel(int nominalSpeedLevel);

    void setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

    void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

    void setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

    bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    // void setCondenserAirInletNodeName(boost::optional<std::string> condenserAirInletNodeName);

    // void resetCondenserAirInletNodeName();

    bool setCondenserType(std::string condenserType);

    bool setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    // bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    // bool setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    // void resetCondensateCollectionWaterStorageTank();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    //@}
    /** @name Other */
    //@{

    bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetSpeedDataList();

    boost::optional<ModelObjectList> speedDataList() const;

    std::vector<CoilCoolingDXVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    std::vector<IdfObject> remove() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeed");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLESPEED_IMPL_HPP

