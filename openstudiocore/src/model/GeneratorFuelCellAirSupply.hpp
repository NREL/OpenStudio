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

#ifndef MODEL_GENERATORFUELCELLAIRSUPPLY_HPP
#define MODEL_GENERATORFUELCELLAIRSUPPLY_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class CurveCubic;
class CurveQuadratic;

namespace detail {

  class GeneratorFuelCellAirSupply_Impl;

} // detail

/** GeneratorFuelCellAirSupply is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:AirSupply'. */
class MODEL_API GeneratorFuelCellAirSupply : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorFuelCellAirSupply(const Model& model);

  virtual ~GeneratorFuelCellAirSupply() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> airSupplyRateCalculationModeValues();

  static std::vector<std::string> airIntakeHeatRecoveryModeValues();

  static std::vector<std::string> airSupplyConstituentModeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: CubicCurves, UniVariateTables.
  boost::optional<CurveCubic> blowerPowerCurve() const;

  boost::optional<double> blowerHeatLossFactor() const;

  std::string airSupplyRateCalculationMode() const;

  boost::optional<double> stoichiometricRatio() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  boost::optional<CurveQuadratic> airRateFunctionofElectricPowerCurve() const;

  boost::optional<double> airRateAirTemperatureCoefficient() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  boost::optional<CurveQuadratic> airRateFunctionofFuelRateCurve() const;

  std::string airIntakeHeatRecoveryMode() const;

  std::string airSupplyConstituentMode() const;

  boost::optional<double> numberofUserDefinedConstituents() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirInletNode(const Connection& connection);

  void resetAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: CubicCurves, UniVariateTables.
  bool setBlowerPowerCurve(const CurveCubic& cubicCurves);

  void resetBlowerPowerCurve();

  bool setBlowerHeatLossFactor(double blowerHeatLossFactor);

  void resetBlowerHeatLossFactor();

  bool setAirSupplyRateCalculationMode(const std::string& airSupplyRateCalculationMode);

  void setStoichiometricRatio(double stoichiometricRatio);

  void resetStoichiometricRatio();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  bool setAirRateFunctionofElectricPowerCurve(const CurveQuadratic& quadraticCurves);

  void resetAirRateFunctionofElectricPowerCurve();

  void setAirRateAirTemperatureCoefficient(double airRateAirTemperatureCoefficient);

  void resetAirRateAirTemperatureCoefficient();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  bool setAirRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

  void resetAirRateFunctionofFuelRateCurve();

  bool setAirIntakeHeatRecoveryMode(const std::string& airIntakeHeatRecoveryMode);

  bool setAirSupplyConstituentMode(const std::string& airSupplyConstituentMode);

  bool setNumberofUserDefinedConstituents(double numberofUserDefinedConstituents);

  void resetNumberofUserDefinedConstituents();

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorFuelCellAirSupply_Impl ImplType;

  explicit GeneratorFuelCellAirSupply(std::shared_ptr<detail::GeneratorFuelCellAirSupply_Impl> impl);

  friend class detail::GeneratorFuelCellAirSupply_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAirSupply");
};

/** \relates GeneratorFuelCellAirSupply*/
typedef boost::optional<GeneratorFuelCellAirSupply> OptionalGeneratorFuelCellAirSupply;

/** \relates GeneratorFuelCellAirSupply*/
typedef std::vector<GeneratorFuelCellAirSupply> GeneratorFuelCellAirSupplyVector;

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELLAIRSUPPLY_HPP

