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

#ifndef MODEL_STANDARDOPAQUEMATERIAL_IMPL_HPP
#define MODEL_STANDARDOPAQUEMATERIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial_Impl.hpp"

namespace openstudio {
namespace model {

// forward declaration
class StandardOpaqueMaterial;

namespace detail {

  /** StandardOpaqueMaterial_Impl is a OpaqueMaterial_Impl that is the implementation class for StandardOpaqueMaterial.*/
  class MODEL_API StandardOpaqueMaterial_Impl : public OpaqueMaterial_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardOpaqueMaterial_Impl(const IdfObject& idfObject,
                  Model_Impl* model,
                  bool keepHandle);

    StandardOpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    StandardOpaqueMaterial_Impl(const StandardOpaqueMaterial_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    virtual ~StandardOpaqueMaterial_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    /** The conductance of the material in W/m^2*K. */
    virtual double thermalConductance() const override;

    /** The resistance of the material in m^2*K/W. */
    virtual double thermalResistance() const override;

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    virtual bool setThermalConductance(double value) override;

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    virtual bool setThermalResistance(double value) override;

    //@}
    /** @name Getters */
    //@{

    /** The conductivity of the material in W/m*K. */
    double thermalConductivity() const;

    /** The resistivity of the material in m*K/W. */
    double thermalResistivity() const;

    boost::optional<double> thermalReflectance() const;

    boost::optional<double> solarReflectance() const;

    boost::optional<double> visibleReflectance() const;

    boost::optional<double> getVisibleTransmittance() const;

    boost::optional<double> interiorVisibleAbsorptance() const;

    boost::optional<double> exteriorVisibleAbsorptance() const;

    std::string roughness() const;

    double thickness() const;

    double conductivity() const;

    double density() const;

    double specificHeat() const;

    double thermalAbsorptance() const;

    double solarAbsorptance() const;

    bool isSolarAbsorptanceDefaulted() const;

    double visibleAbsorptance() const;

    bool isVisibleAbsorptanceDefaulted() const;

    virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

    virtual std::vector<std::string> emsInternalVariableNames() const override;

    //@}
    /** @name Setters */
    //@{

    /** Sets the conductivity of the material in W/m*K, if possible. */
    bool setThermalConductivity(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    bool setThermalResistivity(double value);

    bool setThermalReflectance(boost::optional<double> value);

    bool setSolarAbsorptance(boost::optional<double> value);

    bool setSolarReflectance(boost::optional<double> value);

    bool setVisibleAbsorptance(boost::optional<double> value);

    bool setVisibleReflectance(boost::optional<double> value);

    bool setRoughness(std::string roughness);

    bool setThickness(double thickness);

    bool setConductivity(double conductivity);

    bool setDensity(double density);

    bool setSpecificHeat(double specificHeat);

    bool setThermalAbsorptance(double thermalAbsorptance);

    void resetThermalAbsorptance();

    bool isThermalAbsorptanceDefaulted() const;

    bool setSolarAbsorptance(double solarAbsorptance);

    void resetSolarAbsorptance();

    bool setVisibleAbsorptance(double visibleAbsorptance);

    void resetVisibleAbsorptance();

    //@}
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
     *  specificHeat() * density() * thickness(). */
    boost::optional<double> heatCapacity() const;

    //@}

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.StandardOpaqueMaterial");

    std::vector<std::string> roughnessValues() const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_STANDARDOPAQUEMATERIAL_IMPL_HPP

