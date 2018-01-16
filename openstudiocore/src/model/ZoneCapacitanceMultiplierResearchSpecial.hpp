/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP
#define MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class ZoneCapacitanceMultiplierResearchSpecial_Impl;

} // detail

/** ZoneCapacitanceMultiplierResearchSpecial is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneCapacitanceMultiplier:ResearchSpecial'. */
class MODEL_API ZoneCapacitanceMultiplierResearchSpecial : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ZoneCapacitanceMultiplierResearchSpecial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double temperatureCapacityMultiplier() const;

  Quantity getTemperatureCapacityMultiplier(bool returnIP=false) const;

  bool isTemperatureCapacityMultiplierDefaulted() const;

  double humidityCapacityMultiplier() const;

  Quantity getHumidityCapacityMultiplier(bool returnIP=false) const;

  bool isHumidityCapacityMultiplierDefaulted() const;

  double carbonDioxideCapacityMultiplier() const;

  Quantity getCarbonDioxideCapacityMultiplier(bool returnIP=false) const;

  bool isCarbonDioxideCapacityMultiplierDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier);

  bool setTemperatureCapacityMultiplier(const Quantity& temperatureCapacityMultiplier);

  void resetTemperatureCapacityMultiplier();

  bool setHumidityCapacityMultiplier(double humidityCapacityMultiplier);

  bool setHumidityCapacityMultiplier(const Quantity& humidityCapacityMultiplier);

  void resetHumidityCapacityMultiplier();

  bool setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier);

  bool setCarbonDioxideCapacityMultiplier(const Quantity& carbonDioxideCapacityMultiplier);

  void resetCarbonDioxideCapacityMultiplier();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneCapacitanceMultiplierResearchSpecial_Impl ImplType;

  explicit ZoneCapacitanceMultiplierResearchSpecial(std::shared_ptr<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl> impl);

  friend class detail::ZoneCapacitanceMultiplierResearchSpecial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit ZoneCapacitanceMultiplierResearchSpecial(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneCapacitanceMultiplierResearchSpecial");
};

/** \relates ZoneCapacitanceMultiplierResearchSpecial*/
typedef boost::optional<ZoneCapacitanceMultiplierResearchSpecial> OptionalZoneCapacitanceMultiplierResearchSpecial;

/** \relates ZoneCapacitanceMultiplierResearchSpecial*/
typedef std::vector<ZoneCapacitanceMultiplierResearchSpecial> ZoneCapacitanceMultiplierResearchSpecialVector;

} // model
} // openstudio

#endif // MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP

