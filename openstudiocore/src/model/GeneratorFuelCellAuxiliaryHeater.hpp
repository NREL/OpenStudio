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

#ifndef MODEL_GENERATORFUELCELLAUXILIARYHEATER_HPP
#define MODEL_GENERATORFUELCELLAUXILIARYHEATER_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class ThermalZone;
class GeneratorFuelCell;

namespace detail {

  class GeneratorFuelCellAuxiliaryHeater_Impl;

} // detail

/** GeneratorFuelCellAuxiliaryHeater is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:AuxiliaryHeater'. */
class MODEL_API GeneratorFuelCellAuxiliaryHeater : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorFuelCellAuxiliaryHeater(const Model& model);

  virtual ~GeneratorFuelCellAuxiliaryHeater() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> skinLossDestinationValues();

  static std::vector<std::string> heatingCapacityUnitsValues();

  /** @name Getters */
  //@{

  double excessAirRatio() const;

  double ancillaryPowerConstantTerm() const;

  double ancillaryPowerLinearTerm() const;

  double skinLossUFactorTimesAreaValue() const;

  std::string skinLossDestination() const;

  boost::optional<ThermalZone> zonetoReceiveSkinLosses() const;

  std::string heatingCapacityUnits() const;

  double maximumHeatingCapacityinWatts() const;

  double minimumHeatingCapacityinWatts() const;

  double maximumHeatingCapacityinKmolperSecond() const;

  double minimumHeatingCapacityinKmolperSecond() const;

  // Return optional parent generator
  GeneratorFuelCell fuelCell() const;

  //@}
  /** @name Setters */
  //@{

  bool setExcessAirRatio(double excessAirRatio);

  void resetExcessAirRatio();

  bool setAncillaryPowerConstantTerm(double ancillaryPowerConstantTerm);

  void resetAncillaryPowerConstantTerm();

  bool setAncillaryPowerLinearTerm(double ancillaryPowerLinearTerm);

  void resetAncillaryPowerLinearTerm();

  bool setSkinLossUFactorTimesAreaValue(double skinLossUFactorTimesAreaValue);

  void resetSkinLossUFactorTimesAreaValue();

  bool setSkinLossDestination(const std::string& skinLossDestination);

  void resetSkinLossDestination();

  bool setZonetoReceiveSkinLosses(const ThermalZone& zone);

  void resetZonetoReceiveSkinLosses();

  bool setHeatingCapacityUnits(const std::string& heatingCapacityUnits);

  void resetHeatingCapacityUnits();

  bool setMaximumHeatingCapacityinWatts(double maximumHeatingCapacityinWatts);

  void resetMaximumHeatingCapacityinWatts();

  bool setMinimumHeatingCapacityinWatts(double minimumHeatingCapacityinWatts);

  void resetMinimumHeatingCapacityinWatts();

  bool setMaximumHeatingCapacityinKmolperSecond(double maximumHeatingCapacityinKmolperSecond);

  void resetMaximumHeatingCapacityinKmolperSecond();

  bool setMinimumHeatingCapacityinKmolperSecond(double minimumHeatingCapacityinKmolperSecond);

  void resetMinimumHeatingCapacityinKmolperSecond();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorFuelCellAuxiliaryHeater_Impl ImplType;

  explicit GeneratorFuelCellAuxiliaryHeater(std::shared_ptr<detail::GeneratorFuelCellAuxiliaryHeater_Impl> impl);

  friend class detail::GeneratorFuelCellAuxiliaryHeater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAuxiliaryHeater");
};

/** \relates GeneratorFuelCellAuxiliaryHeater*/
typedef boost::optional<GeneratorFuelCellAuxiliaryHeater> OptionalGeneratorFuelCellAuxiliaryHeater;

/** \relates GeneratorFuelCellAuxiliaryHeater*/
typedef std::vector<GeneratorFuelCellAuxiliaryHeater> GeneratorFuelCellAuxiliaryHeaterVector;

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELLAUXILIARYHEATER_HPP
