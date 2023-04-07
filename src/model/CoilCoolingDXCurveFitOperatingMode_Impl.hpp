/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_IMPL_HPP
#define MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilCoolingDXCurveFitPerformance;
  class CoilCoolingDXCurveFitSpeed;

  namespace detail {

    /** CoilCoolingDXCurveFitOperatingMode_Impl is a ResourceObject_Impl that is the implementation class for CoilCoolingDXCurveFitOperatingMode.*/
    class MODEL_API CoilCoolingDXCurveFitOperatingMode_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXCurveFitOperatingMode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitOperatingMode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitOperatingMode_Impl(const CoilCoolingDXCurveFitOperatingMode_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXCurveFitOperatingMode_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is still used by at least one CoilCoolingDXCurveFitPerformance as a BaseOperatingMode, refuse to remove to avoid putting
      // the CoilCoolingDXCurveFitPerformance(s) in a broken state
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> ratedGrossTotalCoolingCapacity() const;
      bool isRatedGrossTotalCoolingCapacityAutosized() const;

      boost::optional<double> ratedEvaporatorAirFlowRate() const;
      bool isRatedEvaporatorAirFlowRateAutosized() const;

      boost::optional<double> ratedCondenserAirFlowRate() const;
      bool isRatedCondenserAirFlowRateAutosized() const;

      double maximumCyclingRate() const;

      double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

      double latentCapacityTimeConstant() const;

      double nominalTimeforCondensateRemovaltoBegin() const;

      bool applyLatentDegradationtoSpeedsGreaterthan1() const;

      std::string condenserType() const;

      boost::optional<double> nominalEvaporativeCondenserPumpPower() const;
      bool isNominalEvaporativeCondenserPumpPowerAutosized() const;

      unsigned nominalSpeedNumber() const;
      bool isNominalSpeedNumberDefaulted() const;

      std::vector<CoilCoolingDXCurveFitPerformance> coilCoolingDXCurveFitPerformances() const;

      std::vector<CoilCoolingDXCurveFitSpeed> speeds() const;

      // Extensible: Speeds
      boost::optional<unsigned> speedIndex(const CoilCoolingDXCurveFitSpeed& speed) const;
      unsigned numberOfSpeeds() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedGrossTotalCoolingCapacity(double ratedGrossTotalCoolingCapacity);
      void autosizeRatedGrossTotalCoolingCapacity();

      bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);
      void autosizeRatedEvaporatorAirFlowRate();

      bool setRatedCondenserAirFlowRate(double ratedCondenserAirFlowRate);
      void autosizeRatedCondenserAirFlowRate();

      bool setMaximumCyclingRate(double maximumCyclingRate);

      bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
        double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

      bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

      bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

      bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

      bool setCondenserType(const std::string& condenserType);

      bool setNominalEvaporativeCondenserPumpPower(double nominalEvaporativeCondenserPumpPower);

      void autosizeNominalEvaporativeCondenserPumpPower();

      bool setNominalSpeedNumber(unsigned nominalSpeedNumber);
      void resetNominalSpeedNumber();

      // Extensible: Speeds
      bool addSpeed(const CoilCoolingDXCurveFitSpeed& speed);
      bool addSpeed(const CoilCoolingDXCurveFitSpeed& speed, unsigned index);
      bool setSpeedIndex(const CoilCoolingDXCurveFitSpeed& speed, unsigned index);
      bool setSpeeds(const std::vector<CoilCoolingDXCurveFitSpeed>& speeds);
      void removeAllSpeeds();
      bool removeSpeed(const CoilCoolingDXCurveFitSpeed& speed);
      bool removeSpeed(unsigned index);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedRatedGrossTotalCoolingCapacity();

      boost::optional<double> autosizedRatedEvaporatorAirFlowRate();

      boost::optional<double> autosizedRatedCondenserAirFlowRate();

      boost::optional<double> autosizedNominalEvaporativeCondenserPumpPower();

      void autosize();

      void applySizingValues();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitOperatingMode");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_IMPL_HPP
