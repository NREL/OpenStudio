/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_OPAQUEMATERIAL_HPP
#define MODEL_OPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class OpaqueMaterial_Impl;
  }

  /** OpaqueMaterial is a Material that serves as a base class for all \link LayeredConstruction
 *  LayeredConstructions \endlink for which isOpaque. OpaqueMaterial also defines a number of
 *  virtually implemented attributes. */
  class MODEL_API OpaqueMaterial : public Material
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OpaqueMaterial() {}

    //@}
    /** @name Getters */
    //@{

    /** Thermal conductivity of the Material in W/m*K. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalConductivity' */
    double thermalConductivity() const;

    /** Thermal conductance of the Material in W/m^2*K. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalConductance' */
    double thermalConductance() const;

    /** Thermal resistivity of the Material in m*K/W. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalResistivity'. */
    double thermalResistivity() const;

    /** Thermal resistance of the Material in m^2*K/W. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalResistance'. */
    double thermalResistance() const;

    /** Thermal absorptance of the Material (dimensionless fraction).
   *
   *  Attribute name: 'thermalAbsorptance' */
    double thermalAbsorptance() const;

    /** Thermal reflectance of the Material (dimensionless fraction).
   *
   *  Attribute name: 'thermalReflectance' */
    boost::optional<double> thermalReflectance() const;

    /** Solar absorptance of the Material (dimensionless fraction).
   *
   *  Attribute name: 'solarAbsorptance' */
    double solarAbsorptance() const;

    /** Solar reflectance of the Material (dimensionless fraction).
   *
   *  Attribute name: 'solarReflectance' */
    boost::optional<double> solarReflectance() const;

    /** Visible absorptance of the Material (dimensionless fraction).
   *
   *  Attribute name: 'visibleAbsorptance' */
    double visibleAbsorptance() const;

    /** Visible reflectance of the Material (dimensionless fraction).
   *
   *  Attribute name: 'visibleReflectance' */
    boost::optional<double> visibleReflectance() const;

    //@}
    /** @name Setters */
    //@{

    /** Set the thermal conductivity (W/m*K).
   *
   *  Attribute name: 'thermalConductivity' */
    bool setThermalConductivity(double value);

    /** Set the thermal conductance (W/m^2*K).
   *
   *  Attribute name: 'thermalConductance' */
    bool setThermalConductance(double value);

    /** Set the thermal resistivity (m*K/W).
   *
   *  Attribute name: 'thermalResistivity' */
    bool setThermalResistivity(double value);

    /** Set the thermal resistance (m^2*K/W).
   *
   *  Attribute name: 'thermalResistance' */
    bool setThermalResistance(double value);

    /** Set the thermal absorptance (dimensionless fraction).
   *
   *  Attribute name: 'thermalAbsorptance' */
    bool setThermalAbsorptance(double value);

    /** Set the thermal reflectance (dimensionless fraction).
   *
   *  Attribute name: 'thermalReflectance' */
    bool setThermalReflectance(boost::optional<double> value);

    /** Set the solar absorptance (dimensionless fraction).
   *
   *  Attribute name: 'solarAbsorptance' */
    bool setSolarAbsorptance(boost::optional<double> value);

    /** Set the solar reflectance (dimensionless fraction).
   *
   *  Attribute name: 'solarReflectance' */
    bool setSolarReflectance(boost::optional<double> value);

    /** Set the visible absorptance (dimensionless fraction).
   *
   *  Attribute name: 'visibleAbsorptance' */
    bool setVisibleAbsorptance(boost::optional<double> value);

    /** Set the visible reflectance (dimensionless fraction).
   *
   *  Attribute name: 'visibleReflectance' */
    bool setVisibleReflectance(boost::optional<double> value);

    //@}
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
   *  specificHeat() * density() * thickness().
   *
   *  Attribute name: 'heatCapacity' */
    boost::optional<double> heatCapacity() const;

    //@}
   protected:
    /// @cond
    typedef detail::OpaqueMaterial_Impl ImplType;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::OpaqueMaterial_Impl;

    OpaqueMaterial(IddObjectType type, const Model& model);

    explicit OpaqueMaterial(std::shared_ptr<detail::OpaqueMaterial_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OpaqueMaterial");
  };

  /** \relates OpaqueMaterial */
  typedef boost::optional<OpaqueMaterial> OptionalOpaqueMaterial;

  /** \relates OpaqueMaterial */
  typedef std::vector<OpaqueMaterial> OpaqueMaterialVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OPAQUEMATERIAL_HPP
