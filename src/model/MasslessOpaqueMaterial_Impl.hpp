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

      virtual ~MasslessOpaqueMaterial_Impl() = default;

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
