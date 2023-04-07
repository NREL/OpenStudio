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

#ifndef MODEL_MODELPARTITIONMATERIAL_IMPL_HPP
#define MODEL_MODELPARTITIONMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class ModelPartitionMaterial;

  namespace detail {

    class MODEL_API ModelPartitionMaterial_Impl : public Material_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      ModelPartitionMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ModelPartitionMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ModelPartitionMaterial_Impl(const ModelPartitionMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ModelPartitionMaterial_Impl() = default;

      //@}
      /** @name Getters */
      //@{

      /** Get the thickness of the material. Always returns 0.0. */
      virtual double thickness() const override;

      /** The conductivity of the material in W/m*K. */
      virtual double thermalConductivity() const;

      /** The conductance of the material in W/m^2*K. */
      virtual double thermalConductance() const;

      /** The resistivity of the material in m*K/W. */
      virtual double thermalResistivity() const;

      /** The resistance of the material in m^2*K/W. */
      virtual double thermalResistance() const;

      virtual double thermalTransmittance() const;

      virtual double thermalAbsorptance() const;

      virtual double thermalReflectance() const;

      virtual double solarTransmittance() const;

      virtual double solarAbsorptance() const;

      virtual double solarReflectance() const;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      virtual double visibleAbsorptance() const;

      virtual double visibleReflectance() const;

      //@}
      /** @name Setters */
      //@{

      /** Set thickness to value (m). Always returns false. */
      virtual bool setThickness(double value) override;

      /** Sets the conductivity of the material in W/m*K, if possible. */
      virtual bool setThermalConductivity(double value);

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      virtual bool setThermalConductance(double value);

      /** Sets the resistivity of the material in m*K/W, if possible. */
      virtual bool setThermalResistivity(double value);

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      virtual bool setThermalResistance(double value);

      virtual bool setThermalTransmittance(double value);

      virtual bool setThermalAbsorptance(double value);

      virtual bool setThermalReflectance(double value);

      virtual bool setSolarTransmittance(double value);

      virtual bool setSolarAbsorptance(double value);

      virtual bool setSolarReflectance(double value);

      virtual bool setVisibleTransmittance(double value);

      virtual bool setVisibleAbsorptance(double value);

      virtual bool setVisibleReflectance(double value);

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.ModelPartitionMaterial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MODELPARTITIONMATERIAL_IMPL_HPP
