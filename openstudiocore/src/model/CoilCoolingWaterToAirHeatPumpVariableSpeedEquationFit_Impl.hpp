/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;
class ModelObjectList;

namespace detail {

  /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.*/
  class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl : public WaterToAirComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle);

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle);

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle);

    virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl() {}

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

    boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

    bool useHotGasReheat() const;

    Curve energyPartLoadFractionCurve() const;

  boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const ;

  boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const ;

  boost::optional<double> autosizedRatedWaterFlowRateAtSelectedNominalSpeedLevel() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

    void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

    bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

    bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

    bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

    bool setUseHotGasReheat(bool useHotGasReheat);

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetSpeedDataList();

    boost::optional<ModelObjectList> speedDataList() const;

    std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

    bool addSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeAllSpeeds();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP
