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

#ifndef MODEL_COILHEATINGDXMULTISPEED_IMPL_HPP
#define MODEL_COILHEATINGDXMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;
class CoilHeatingDXMultiSpeedStageData;
class ModelObjectList;

namespace detail {

  /** CoilHeatingDXMultiSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXMultiSpeed.*/
  class MODEL_API CoilHeatingDXMultiSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingDXMultiSpeed_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoilHeatingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoilHeatingDXMultiSpeed_Impl(const CoilHeatingDXMultiSpeed_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~CoilHeatingDXMultiSpeed_Impl() {}

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

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

    double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

    std::string defrostStrategy() const;

    std::string defrostControl() const;

    double defrostTimePeriodFraction() const;

    boost::optional<double> resistiveDefrostHeaterCapacity() const;

    bool isResistiveDefrostHeaterCapacityAutosized() const;

    bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

    std::string fuelType() const;

    int regionnumberforCalculatingHSPF() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    void setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    void setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor);

    void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

    void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

    bool setDefrostStrategy(std::string defrostStrategy);

    bool setDefrostControl(std::string defrostControl);

    bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

    bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

    void autosizeResistiveDefrostHeaterCapacity();

    void setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

    bool setFuelType(std::string fuelType);

    bool setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF);

    //@}
    /** @name Other */
    //@{

    bool setStageDataList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetStageDataList();

    boost::optional<ModelObjectList> stageDataList() const;

    std::vector<CoilHeatingDXMultiSpeedStageData> stages() const;

    bool addStage(const CoilHeatingDXMultiSpeedStageData& stage);

    void removeStage(const CoilHeatingDXMultiSpeedStageData& stage);

    void removeAllStages();

    std::vector<IdfObject> remove() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeed");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXMULTISPEED_IMPL_HPP

