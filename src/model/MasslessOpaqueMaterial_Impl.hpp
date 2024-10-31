/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MASSLESSOPAQUEMATERIAL_IMPL_HPP
#define MODEL_MASSLESSOPAQUEMATERIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** MasslessOpaqueMaterial_Impl is a OpaqueMaterial_Impl that is the implementation class for MasslessOpaqueMaterial.*/
    class MODEL_API MasslessOpaqueMaterial_Impl : public OpaqueMaterial_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      MasslessOpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      MasslessOpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      MasslessOpaqueMaterial_Impl(const MasslessOpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~MasslessOpaqueMaterial_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      std::string roughness() const;

      /** The conductivity of the material in W/m*K. */
      virtual double thermalConductivity() const override;

      /** The conductance of the material in W/m^2*K. */
      virtual double thermalConductance() const override;

      /** The resistivity of the material in m*K/W. */
      virtual double thermalResistivity() const override;

      /** The resistance of the material in m^2*K/W. */
      virtual double thermalResistance() const override;

      virtual double thermalAbsorptance() const override;

      virtual boost::optional<double> thermalReflectance() const override;

      virtual double solarAbsorptance() const override;

      virtual boost::optional<double> solarReflectance() const override;

      virtual double visibleAbsorptance() const override;

      virtual boost::optional<double> visibleReflectance() const override;

      bool isThermalAbsorptanceDefaulted() const;

      bool isSolarAbsorptanceDefaulted() const;

      bool isVisibleAbsorptanceDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setRoughness(const std::string& roughness);

      /** Set thickness to value (m). */
      virtual bool setThickness(double value) override;

      /** Sets the conductivity of the material in W/m*K, if possible. */
      virtual bool setThermalConductivity(double value) override;

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      virtual bool setThermalConductance(double value) override;

      /** Sets the resistivity of the material in m*K/W, if possible. */
      virtual bool setThermalResistivity(double value) override;

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      virtual bool setThermalResistance(double thermalResistance) override;

      virtual bool setThermalAbsorptance(boost::optional<double> value);

      virtual bool setThermalReflectance(boost::optional<double> value) override;

      virtual bool setSolarAbsorptance(boost::optional<double> value) override;

      virtual bool setSolarReflectance(boost::optional<double> value) override;

      virtual bool setVisibleAbsorptance(boost::optional<double> value) override;

      virtual bool setVisibleReflectance(boost::optional<double> value) override;

      bool setThermalAbsorptance(double thermalAbsorptance) override;

      void resetThermalAbsorptance();

      bool setSolarAbsorptance(double solarAbsorptance);

      void resetSolarAbsorptance();

      bool setVisibleAbsorptance(double visibleAbsorptance);

      void resetVisibleAbsorptance();

      // if material property moisture penetration depth settings already exists, do nothing and return nil; creates the material property moisture penetration depth settings if it does not already exist and return it;
      boost::optional<MaterialPropertyMoisturePenetrationDepthSettings>
        createMaterialPropertyMoisturePenetrationDepthSettings(double waterVaporDiffusionResistanceFactor, double moistureEquationCoefficientA,
                                                               double moistureEquationCoefficientB, double moistureEquationCoefficientC,
                                                               double moistureEquationCoefficientD, double coatingLayerThickness,
                                                               double coatingLayerWaterVaporDiffusionResistanceFactor);

      // returns the material property moisture penetration depth settings if set
      boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> materialPropertyMoisturePenetrationDepthSettings() const;

      // resets the material property moisture penetration depth settings
      void resetMaterialPropertyMoisturePenetrationDepthSettings();

      // if material property phase change already exists, do nothing and return nil; creates the material property phase change if it does not already exist and return it;
      boost::optional<MaterialPropertyPhaseChange> createMaterialPropertyPhaseChange();
      boost::optional<MaterialPropertyPhaseChange> createMaterialPropertyPhaseChange(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys);

      // returns the material property phase change if set
      boost::optional<MaterialPropertyPhaseChange> materialPropertyPhaseChange() const;

      // resets the material property phase change
      void resetMaterialPropertyPhaseChange();

      // if material property phase change hysteresis already exists, do nothing and return nil; creates the material property phase change hysteresis if it does not already exist and return it
      boost::optional<MaterialPropertyPhaseChangeHysteresis> createMaterialPropertyPhaseChangeHysteresis();

      // returns the material property phase change hysteresis if set
      boost::optional<MaterialPropertyPhaseChangeHysteresis> materialPropertyPhaseChangeHysteresis() const;

      // resets the material property phase change hysteresis
      void resetMaterialPropertyPhaseChangeHysteresis();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MasslessOpaqueMaterial");

      std::vector<std::string> roughnessValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MASSLESSOPAQUEMATERIAL_IMPL_HPP
