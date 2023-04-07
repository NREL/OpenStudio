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

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class CoilHeatingDXMultiSpeed;

  namespace detail {

    /** CoilHeatingDXMultiSpeedStageData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingDXMultiSpeedStageData.*/
    class MODEL_API CoilHeatingDXMultiSpeedStageData_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXMultiSpeedStageData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXMultiSpeedStageData_Impl(const CoilHeatingDXMultiSpeedStageData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXMultiSpeedStageData_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is used by any CoilHeatingDXMultiSpeed, remove the corresponding extensible group to avoid having 'blanks'
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> grossRatedHeatingCapacity() const;

      bool isGrossRatedHeatingCapacityAutosized() const;

      double grossRatedHeatingCOP() const;

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateAutosized() const;

      double ratedSupplyAirFanPowerPerVolumeFlowRate2017() const;

      double ratedSupplyAirFanPowerPerVolumeFlowRate2023() const;

      Curve heatingCapacityFunctionofTemperatureCurve() const;

      Curve heatingCapacityFunctionofFlowFractionCurve() const;

      Curve energyInputRatioFunctionofTemperatureCurve() const;

      Curve energyInputRatioFunctionofFlowFractionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      double ratedWasteHeatFractionofPowerInput() const;

      Curve wasteHeatFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setGrossRatedHeatingCapacity(boost::optional<double> grossRatedHeatingCapacity);

      void autosizeGrossRatedHeatingCapacity();

      bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setRatedSupplyAirFanPowerPerVolumeFlowRate2017(double ratedSupplyAirFanPowerPerVolumeFlowRate2017);

      bool setRatedSupplyAirFanPowerPerVolumeFlowRate2023(double ratedSupplyAirFanPowerPerVolumeFlowRate2023);

      bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

      bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedGrossRatedHeatingCapacity() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      void autosize();

      void applySizingValues();

      // Returns the CoilHeatingDXMultiSpeed that references it if any
      boost::optional<CoilHeatingDXMultiSpeed> parentCoil() const;

      // Used to determine the index of this performance data in the
      // list of stages in the parent object.
      boost::optional<std::tuple<int, CoilHeatingDXMultiSpeed>> stageIndexAndParentCoil() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeedStageData");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
      boost::optional<Curve> optionalWasteHeatFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
