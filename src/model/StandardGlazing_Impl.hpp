/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STANDARDGLAZING_IMPL_HPP
#define MODEL_STANDARDGLAZING_IMPL_HPP

#include "ModelAPI.hpp"
#include "Glazing_Impl.hpp"

namespace openstudio {
namespace model {

  class MaterialPropertyGlazingSpectralData;

  namespace detail {

    /** StandardGlazing_Impl is a Glazing_Impl that is the implementation class for StandardGlazing.*/
    class MODEL_API StandardGlazing_Impl : public Glazing_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      StandardGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      StandardGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      StandardGlazing_Impl(const StandardGlazing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~StandardGlazing_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string opticalDataType() const;

      boost::optional<std::string> windowGlassSpectralDataSetName() const;

      boost::optional<MaterialPropertyGlazingSpectralData> windowGlassSpectralDataSet() const;

      /** Get the thickness of the material. */
      virtual double thickness() const override;

      double solarTransmittance() const;

      boost::optional<double> solarTransmittanceatNormalIncidence() const;

      boost::optional<double> frontSideSolarReflectanceatNormalIncidence() const;

      boost::optional<double> backSideSolarReflectanceatNormalIncidence() const;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      boost::optional<double> visibleTransmittanceatNormalIncidence() const;

      boost::optional<double> frontSideVisibleReflectanceatNormalIncidence() const;

      boost::optional<double> backSideVisibleReflectanceatNormalIncidence() const;

      double infraredTransmittance() const;

      double infraredTransmittanceatNormalIncidence() const;

      bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

      double frontSideInfraredHemisphericalEmissivity() const;

      bool isFrontSideInfraredHemisphericalEmissivityDefaulted() const;

      double backSideInfraredHemisphericalEmissivity() const;

      bool isBackSideInfraredHemisphericalEmissivityDefaulted() const;

      double conductivity() const;

      bool isConductivityDefaulted() const;

      double dirtCorrectionFactorforSolarandVisibleTransmittance() const;

      bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const;

      bool solarDiffusing() const;

      bool isSolarDiffusingDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setOpticalDataType(const std::string& opticalDataType);

      bool setWindowGlassSpectralDataSetName(boost::optional<std::string> windowGlassSpectralDataSetName);
      void resetWindowGlassSpectralDataSetName();

      bool setWindowGlassSpectralDataSet(const MaterialPropertyGlazingSpectralData& spectralData);
      void resetWindowGlassSpectralDataSet();

      /** Set thickness to value (m). */
      virtual bool setThickness(double thickness) override;

      bool setSolarTransmittance(double value);

      bool setSolarTransmittanceatNormalIncidence(boost::optional<double> solarTransmittanceatNormalIncidence);

      void resetSolarTransmittanceatNormalIncidence();

      bool setFrontSideSolarReflectanceatNormalIncidence(boost::optional<double> frontSideSolarReflectanceatNormalIncidence);

      void resetFrontSideSolarReflectanceatNormalIncidence();

      bool setBackSideSolarReflectanceatNormalIncidence(boost::optional<double> backSideSolarReflectanceatNormalIncidence);

      void resetBackSideSolarReflectanceatNormalIncidence();

      bool setVisibleTransmittance(double value);

      bool setVisibleTransmittanceatNormalIncidence(boost::optional<double> visibleTransmittanceatNormalIncidence);

      void resetVisibleTransmittanceatNormalIncidence();

      bool setFrontSideVisibleReflectanceatNormalIncidence(boost::optional<double> frontSideVisibleReflectanceatNormalIncidence);

      void resetFrontSideVisibleReflectanceatNormalIncidence();

      bool setBackSideVisibleReflectanceatNormalIncidence(boost::optional<double> backSideVisibleReflectanceatNormalIncidence);

      void resetBackSideVisibleReflectanceatNormalIncidence();

      bool setInfraredTransmittance(double value);

      bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

      void resetInfraredTransmittanceatNormalIncidence();

      bool setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity);

      void resetFrontSideInfraredHemisphericalEmissivity();

      bool setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity);

      void resetBackSideInfraredHemisphericalEmissivity();

      bool setConductivity(double conductivity);

      void resetConductivity();

      bool setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance);

      void resetDirtCorrectionFactorforSolarandVisibleTransmittance();

      bool setSolarDiffusing(bool solarDiffusing);

      void resetSolarDiffusing();

      //@}
      /** @name Other */
      //@{

      /** The conductivity of the material in W/m*K. */
      double thermalConductivity() const;

      /** The conductance of the material in W/m^2*K. */
      double thermalConductance() const;

      /** The resistivity of the material in m*K/W. */
      double thermalResistivity() const;

      /** The resistance of the material in m^2*K/W. */
      double thermalResistance() const;

      double interiorVisibleReflectance() const;

      double exteriorVisibleReflectance() const;

      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      /** Sets the conductivity of the material in W/m*K, if possible. */
      bool setThermalConductivity(double value);

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      bool setThermalConductance(double value);

      /** Sets the resistivity of the material in m*K/W, if possible. */
      bool setThermalResistivity(double value);

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      bool setThermalResistance(double value);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.StandardGlazing");

      std::vector<std::string> opticalDataTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STANDARDGLAZING_IMPL_HPP
