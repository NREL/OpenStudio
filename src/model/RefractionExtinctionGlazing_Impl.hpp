/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~RefractionExtinctionGlazing_Impl() override = default;

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
