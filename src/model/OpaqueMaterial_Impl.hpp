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

#ifndef MODEL_OPAQUEMATERIAL_IMPL_HPP
#define MODEL_OPAQUEMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class OpaqueMaterial;

  namespace detail {

    class MODEL_API OpaqueMaterial_Impl : public Material_Impl
    {

     public:
      // Construct completely new object.
      OpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      OpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      OpaqueMaterial_Impl(const OpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OpaqueMaterial_Impl() = default;

      /** @name Getters */
      //@{

      /** The conductivity of the material in W/m*K. */
      virtual double thermalConductivity() const = 0;

      /** The conductance of the material in W/m^2*K. */
      virtual double thermalConductance() const = 0;

      /** The resistivity of the material in m*K/W. */
      virtual double thermalResistivity() const = 0;

      /** The resistance of the material in m^2*K/W. */
      virtual double thermalResistance() const = 0;

      virtual double thermalAbsorptance() const = 0;

      virtual boost::optional<double> thermalReflectance() const = 0;

      virtual double solarAbsorptance() const = 0;

      virtual boost::optional<double> solarReflectance() const = 0;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      virtual double visibleAbsorptance() const = 0;

      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      virtual boost::optional<double> visibleReflectance() const = 0;

      //@}
      /** @name Setters */
      //@{

      /** Sets the conductivity of the material in W/m*K, if possible. */
      virtual bool setThermalConductivity(double value) = 0;

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      virtual bool setThermalConductance(double value) = 0;

      /** Sets the resistivity of the material in m*K/W, if possible. */
      virtual bool setThermalResistivity(double value) = 0;

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      virtual bool setThermalResistance(double value) = 0;

      virtual bool setThermalAbsorptance(double value) = 0;

      virtual bool setThermalReflectance(boost::optional<double> value) = 0;

      virtual bool setSolarAbsorptance(boost::optional<double> value) = 0;

      virtual bool setSolarReflectance(boost::optional<double> value) = 0;

      virtual bool setVisibleAbsorptance(boost::optional<double> value) = 0;

      virtual bool setVisibleReflectance(boost::optional<double> value) = 0;

      //@}
      /** @name Queries */
      //@{

      /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
     *  specificHeat() * density() * thickness(). */
      virtual boost::optional<double> heatCapacity() const;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.OpaqueMaterial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OPAQUEMATERIAL_IMPL_HPP
