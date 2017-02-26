/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_HPP
#define MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Connection;
class Node;
class GeneratorFuelCell;

namespace detail {

  class GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl;

} // detail

/** GeneratorFuelCellExhaustGasToWaterHeatExchanger is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:ExhaustGasToWaterHeatExchanger'. */
class MODEL_API GeneratorFuelCellExhaustGasToWaterHeatExchanger : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorFuelCellExhaustGasToWaterHeatExchanger(const Model& model);

  explicit GeneratorFuelCellExhaustGasToWaterHeatExchanger(const Model& model,
                                                           const Node& exhaustOutletAirNode);

  virtual ~GeneratorFuelCellExhaustGasToWaterHeatExchanger() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatExchangerCalculationMethodValues();

  /** @name Getters */
  //@{

  double heatRecoveryWaterMaximumFlowRate() const;

  boost::optional<Node> exhaustOutletAirNode() const;

  std::string heatExchangerCalculationMethod() const;

  boost::optional<double> method1HeatExchangerEffectiveness() const;

  boost::optional<double> method2Parameterhxs0() const;

  boost::optional<double> method2Parameterhxs1() const;

  boost::optional<double> method2Parameterhxs2() const;

  boost::optional<double> method2Parameterhxs3() const;

  boost::optional<double> method2Parameterhxs4() const;

  boost::optional<double> method3h0GasCoefficient() const;

  boost::optional<double> method3NdotGasRefCoefficient() const;

  boost::optional<double> method3nCoefficient() const;

  boost::optional<double> method3GasArea() const;

  boost::optional<double> method3h0WaterCoefficient() const;

  boost::optional<double> method3NdotWaterrefCoefficient() const;

  boost::optional<double> method3mCoefficient() const;

  boost::optional<double> method3WaterArea() const;

  boost::optional<double> method3FAdjustmentFactor() const;

  boost::optional<double> method4hxl1Coefficient() const;

  boost::optional<double> method4hxl2Coefficient() const;

  boost::optional<double> method4CondensationThreshold() const;

  // Return optional parent generator
  GeneratorFuelCell fuelCell() const;

  //@}
  /** @name Setters */
  //@{

  void setHeatRecoveryWaterMaximumFlowRate(double heatRecoveryWaterMaximumFlowRate);

  void resetHeatRecoveryWaterMaximumFlowRate();

  bool setExhaustOutletAirNode(const Node& node);

  void resetExhaustOutletAirNode();

  bool setHeatExchangerCalculationMethod(const std::string& heatExchangerCalculationMethod);

  void resetHeatExchangerCalculationMethod();

  void setMethod1HeatExchangerEffectiveness(double method1HeatExchangerEffectiveness);

  void resetMethod1HeatExchangerEffectiveness();

  void setMethod2Parameterhxs0(double method2Parameterhxs0);

  void resetMethod2Parameterhxs0();

  void setMethod2Parameterhxs1(double method2Parameterhxs1);

  void resetMethod2Parameterhxs1();

  void setMethod2Parameterhxs2(double method2Parameterhxs2);

  void resetMethod2Parameterhxs2();

  void setMethod2Parameterhxs3(double method2Parameterhxs3);

  void resetMethod2Parameterhxs3();

  void setMethod2Parameterhxs4(double method2Parameterhxs4);

  void resetMethod2Parameterhxs4();

  void setMethod3h0GasCoefficient(double method3h0GasCoefficient);

  void resetMethod3h0GasCoefficient();

  void setMethod3NdotGasRefCoefficient(double method3NdotGasRefCoefficient);

  void resetMethod3NdotGasRefCoefficient();

  void setMethod3nCoefficient(double method3nCoefficient);

  void resetMethod3nCoefficient();

  void setMethod3GasArea(double method3GasArea);

  void resetMethod3GasArea();

  void setMethod3h0WaterCoefficient(double method3h0WaterCoefficient);

  void resetMethod3h0WaterCoefficient();

  void setMethod3NdotWaterrefCoefficient(double method3NdotWaterrefCoefficient);

  void resetMethod3NdotWaterrefCoefficient();

  void setMethod3mCoefficient(double method3mCoefficient);

  void resetMethod3mCoefficient();

  void setMethod3WaterArea(double method3WaterArea);

  void resetMethod3WaterArea();

  void setMethod3FAdjustmentFactor(double method3FAdjustmentFactor);

  void resetMethod3FAdjustmentFactor();

  void setMethod4hxl1Coefficient(double method4hxl1Coefficient);

  void resetMethod4hxl1Coefficient();

  void setMethod4hxl2Coefficient(double method4hxl2Coefficient);

  void resetMethod4hxl2Coefficient();

  void setMethod4CondensationThreshold(double method4CondensationThreshold);

  void resetMethod4CondensationThreshold();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl ImplType;

  explicit GeneratorFuelCellExhaustGasToWaterHeatExchanger(std::shared_ptr<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl> impl);

  friend class detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorFuelCellExhaustGasToWaterHeatExchanger");
};

/** \relates GeneratorFuelCellExhaustGasToWaterHeatExchanger*/
typedef boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger> OptionalGeneratorFuelCellExhaustGasToWaterHeatExchanger;

/** \relates GeneratorFuelCellExhaustGasToWaterHeatExchanger*/
typedef std::vector<GeneratorFuelCellExhaustGasToWaterHeatExchanger> GeneratorFuelCellExhaustGasToWaterHeatExchangerVector;

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_HPP

