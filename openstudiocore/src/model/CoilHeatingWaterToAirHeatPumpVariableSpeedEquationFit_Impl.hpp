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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;
class ModelObjectList;

namespace detail {

  /** CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.*/
  class MODEL_API CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl : public WaterToAirComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle);

    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle);

    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle);

    virtual ~CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual unsigned airInletPort() override;

    virtual unsigned airOutletPort() override;

    virtual unsigned waterInletPort() override;

    virtual unsigned waterOutletPort() override;

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<IdfObject> remove() override;

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    //@}
    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel() const;

    bool isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    Curve energyPartLoadFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    void setNominalSpeedLevel(int nominalSpeedLevel);

    void setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel);

    void autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();

    void setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    void setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetSpeedDataList();

    boost::optional<ModelObjectList> speedDataList() const;

    std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

    bool addSpeed(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeSpeed(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeAllSpeeds();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP

