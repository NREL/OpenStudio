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

#ifndef MODEL_REFRACTIONEXTINCTIONGLAZING_IMPL_HPP
#define MODEL_REFRACTIONEXTINCTIONGLAZING_IMPL_HPP

#include "ModelAPI.hpp"
#include "Glazing_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** RefractionExtinctionGlazing_Impl is a Glazing_Impl that is the implementation class for RefractionExtinctionGlazing.*/
    class MODEL_API RefractionExtinctionGlazing_Impl : public Glazing_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      RefractionExtinctionGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefractionExtinctionGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefractionExtinctionGlazing_Impl(const RefractionExtinctionGlazing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefractionExtinctionGlazing_Impl() = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double thickness() const override;

      double thermalConductivity() const;

      double thermalConductance() const;

      double thermalResistivity() const;

      double thermalResistance() const;

      double thermalTransmittance() const;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      /** This model assumes that the front- and back-side properties are identical. */
      double visibleAbsorptance() const;

      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      double solarIndexofRefraction() const;

      double solarExtinctionCoefficient() const;

      double visibleIndexofRefraction() const;

      double visibleExtinctionCoefficient() const;

      double infraredTransmittanceatNormalIncidence() const;

      bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

      double infraredHemisphericalEmissivity() const;

      bool isInfraredHemisphericalEmissivityDefaulted() const;

      virtual double conductivity() const;

      virtual bool isConductivityDefaulted() const;

      virtual double dirtCorrectionFactorforSolarandVisibleTransmittance() const;

      virtual bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const;

      bool solarDiffusing() const;

      bool isSolarDiffusingDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setThermalConductivity(double value);

      bool setThermalConductance(double value);

      bool setThermalResistivity(double value);

      bool setThermalResistance(double value);

      bool setThermalTransmittance(double value);

      bool setThickness(double thickness) override;

      bool setSolarIndexofRefraction(double solarIndexofRefraction);

      bool setSolarExtinctionCoefficient(double solarExtinctionCoefficient);

      bool setVisibleIndexofRefraction(double visibleIndexofRefraction);

      bool setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient);

      bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

      void resetInfraredTransmittanceatNormalIncidence();

      bool setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity);

      void resetInfraredHemisphericalEmissivity();

      virtual bool setConductivity(double conductivity);

      virtual void resetConductivity();

      virtual bool setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance);

      virtual void resetDirtCorrectionFactorforSolarandVisibleTransmittance();

      bool setSolarDiffusing(bool solarDiffusing);

      void resetSolarDiffusing();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefractionExtinctionGlazing");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRACTIONEXTINCTIONGLAZING_IMPL_HPP
