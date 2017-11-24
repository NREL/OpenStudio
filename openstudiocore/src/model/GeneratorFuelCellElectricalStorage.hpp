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

#ifndef MODEL_GENERATORFUELCELLELECTRICALSTORAGE_HPP
#define MODEL_GENERATORFUELCELLELECTRICALSTORAGE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class GeneratorFuelCellElectricalStorage_Impl;

} // detail

/** GeneratorFuelCellElectricalStorage is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:ElectricalStorage'. */
class MODEL_API GeneratorFuelCellElectricalStorage : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorFuelCellElectricalStorage(const Model& model);

  virtual ~GeneratorFuelCellElectricalStorage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> choiceofModelValues();

  /** @name Getters */
  //@{

  std::string choiceofModel() const;

  double nominalChargingEnergeticEfficiency() const;

  double nominalDischargingEnergeticEfficiency() const;

  double simpleMaximumCapacity() const;

  double simpleMaximumPowerDraw() const;

  double simpleMaximumPowerStore() const;

  double initialChargeState() const;

  //@}
  /** @name Setters */
  //@{

  bool setChoiceofModel(const std::string& choiceofModel);

  void resetChoiceofModel();

  bool setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency);

  void resetNominalChargingEnergeticEfficiency();

  bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);

  void resetNominalDischargingEnergeticEfficiency();

  bool setSimpleMaximumCapacity(double simpleMaximumCapacity);

  void resetSimpleMaximumCapacity();

  bool setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw);

  void resetSimpleMaximumPowerDraw();

  bool setSimpleMaximumPowerStore(double simpleMaximumPowerStore);

  void resetSimpleMaximumPowerStore();

  bool setInitialChargeState(double initialChargeState);

  void resetInitialChargeState();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorFuelCellElectricalStorage_Impl ImplType;

  explicit GeneratorFuelCellElectricalStorage(std::shared_ptr<detail::GeneratorFuelCellElectricalStorage_Impl> impl);

  friend class detail::GeneratorFuelCellElectricalStorage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorFuelCellElectricalStorage");
};

/** \relates GeneratorFuelCellElectricalStorage*/
typedef boost::optional<GeneratorFuelCellElectricalStorage> OptionalGeneratorFuelCellElectricalStorage;

/** \relates GeneratorFuelCellElectricalStorage*/
typedef std::vector<GeneratorFuelCellElectricalStorage> GeneratorFuelCellElectricalStorageVector;

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELLELECTRICALSTORAGE_HPP
