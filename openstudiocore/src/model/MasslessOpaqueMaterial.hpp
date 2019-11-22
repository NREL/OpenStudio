/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_MASSLESSOPAQUEMATERIAL_HPP
#define MODEL_MASSLESSOPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class MasslessOpaqueMaterial_Impl;

} // detail

/** MasslessOpaqueMaterial is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:NoMass'. */
class MODEL_API MasslessOpaqueMaterial : public OpaqueMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit MasslessOpaqueMaterial(const Model& model,
    std::string roughness = "Smooth",
    double thermalResistance = 0.1);

  virtual ~MasslessOpaqueMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> roughnessValues();

  /** @name Getters */
  //@{

  std::string roughness() const;

  double thermalResistance() const;

  Quantity getThermalResistance(bool returnIP=false) const;

  boost::optional<double> thermalAbsorptance() const;

  Quantity getThermalAbsorptance(bool returnIP=false) const;

  bool isThermalAbsorptanceDefaulted() const;

  boost::optional<double> solarAbsorptance() const;

  Quantity getSolarAbsorptance(bool returnIP=false) const;

  bool isSolarAbsorptanceDefaulted() const;

  boost::optional<double> visibleAbsorptance() const;

  Quantity getVisibleAbsorptance(bool returnIP=false) const;

  bool isVisibleAbsorptanceDefaulted() const;

  double conductivity() const;

  Quantity getConductivity(bool returnIP=false) const;

  double density() const;

  Quantity getDensity(bool returnIP=false) const;

  double specificHeat() const;

  Quantity getSpecificHeat(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setRoughness(std::string roughness);

  bool setThermalResistance(double thermalResistance);

  bool setThermalResistance(const Quantity& thermalResistance);

  bool setThermalAbsorptance(double thermalAbsorptance);

  bool setThermalAbsorptance(const Quantity& thermalAbsorptance);

  void resetThermalAbsorptance();

  bool setSolarAbsorptance(double solarAbsorptance);

  bool setSolarAbsorptance(const Quantity& solarAbsorptance);

  void resetSolarAbsorptance();

  bool setVisibleAbsorptance(double visibleAbsorptance);

  bool setVisibleAbsorptance(const Quantity& visibleAbsorptance);

  void resetVisibleAbsorptance();

  bool setConductivity(double conductivity);

  bool setConductivity(const Quantity& conductivity);

  bool setDensity(double density);

  bool setDensity(const Quantity& density);

  bool setSpecificHeat(double specificHeat);

  bool setSpecificHeat(const Quantity& specificHeat);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::MasslessOpaqueMaterial_Impl ImplType;

  explicit MasslessOpaqueMaterial(std::shared_ptr<detail::MasslessOpaqueMaterial_Impl> impl);

  friend class detail::MasslessOpaqueMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.MasslessOpaqueMaterial");
};

/** \relates MasslessOpaqueMaterial*/
typedef boost::optional<MasslessOpaqueMaterial> OptionalMasslessOpaqueMaterial;

/** \relates MasslessOpaqueMaterial*/
typedef std::vector<MasslessOpaqueMaterial> MasslessOpaqueMaterialVector;

} // model
} // openstudio

#endif // MODEL_MASSLESSOPAQUEMATERIAL_HPP

