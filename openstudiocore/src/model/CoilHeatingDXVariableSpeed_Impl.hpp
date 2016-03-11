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

#ifndef MODEL_COILHEATINGDXVARIABLESPEED_IMPL_HPP
#define MODEL_COILHEATINGDXVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class CoilHeatingDXVariableSpeedSpeedData;
class ModelObjectList;

namespace detail {

  /** CoilHeatingDXVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXVariableSpeed.*/
  class MODEL_API CoilHeatingDXVariableSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingDXVariableSpeed_Impl(const IdfObject& idfObject,
                                    Model_Impl* model,
                                    bool keepHandle);

    CoilHeatingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    CoilHeatingDXVariableSpeed_Impl(const CoilHeatingDXVariableSpeed_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    virtual ~CoilHeatingDXVariableSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

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

    boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel() const;

    bool isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    Curve energyPartLoadFractionCurve() const;

    boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

    double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    std::string defrostStrategy() const;

    std::string defrostControl() const;

    double defrostTimePeriodFraction() const;

    boost::optional<double> resistiveDefrostHeaterCapacity() const;

    bool isResistiveDefrostHeaterCapacityAutosized() const;

    //@}
    /** @name Setters */
    //@{

    void setNominalSpeedLevel(int nominalSpeedLevel);

    void setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel);

    void autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();

    void setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

    void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    void setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor);

    void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

    bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setDefrostStrategy(std::string defrostStrategy);

    bool setDefrostControl(std::string defrostControl);

    bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

    bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

    void autosizeResistiveDefrostHeaterCapacity();

    //@}
    /** @name Other */
    //@{

    bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetSpeedDataList();

    boost::optional<ModelObjectList> speedDataList() const;

    std::vector<CoilHeatingDXVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    std::vector<IdfObject> remove() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeed");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXVARIABLESPEED_IMPL_HPP

