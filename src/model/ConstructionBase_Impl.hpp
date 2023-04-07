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

#ifndef MODEL_CONSTRUCTIONBASE_IMPL_HPP
#define MODEL_CONSTRUCTIONBASE_IMPL_HPP

#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class StandardsInformationConstruction;
  class RenderingColor;

  namespace detail {

    class MODEL_API ConstructionBase_Impl : public ResourceObject_Impl
    {

      // may require simulation results.

      // may require simulation results.

      // may require simulation results.

     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      ConstructionBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ConstructionBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ConstructionBase_Impl(const ConstructionBase_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ConstructionBase_Impl() = default;

      //@}
      /** @name Getters */
      //@{

      /** Returns this construction's standards information, constructing a new object if necessary. */
      StandardsInformationConstruction standardsInformation() const;

      virtual double getNetArea() const;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      //@}
      /** @name Setters */
      //@{

      /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include
     *  appropriate film coefficients. By default, assumes still air indoors and 15 mph outdoor air
     *  speed. */
      virtual bool setUFactor(double value);

      /** Set the u-factor of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a conductance. */
      virtual bool setUFactor(double value, double filmResistance);

      /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
      virtual bool setConductance(double value);

      /** Set the conductance of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a u-factor. */
      virtual bool setConductance(double value, double filmResistance);

      //@}
      /** @name Queries */
      //@{

      virtual bool isOpaque() const = 0;

      virtual bool isFenestration() const = 0;

      virtual bool isSolarDiffusing() const = 0;

      virtual bool isModelPartition() const = 0;

      /** Returns true if the construction has RoofVegetation as the outer layer. */
      virtual bool isGreenRoof() const;

      /** Get the u-factor of this construction. Includes film coefficients. */
      virtual boost::optional<double> uFactor() const;

      /** Get the u-factor of this construction. Includes filmResistance. */
      virtual boost::optional<double> uFactor(double filmResistance) const;

      /** Get the thermal conductance of this construction. Does not include film coefficients. */
      virtual boost::optional<double> thermalConductance() const;

      /** Get the thermal conductance of this construction. filmResistance is subtracted out of the
     *  thermalResistance if necessary. */
      virtual boost::optional<double> thermalConductance(double filmResistance) const;

      /** Get the heat capacity of this construction (J/m^2*K). Only works for
     *  \link LayeredConstruction LayeredConstructions \endlink of \link StandardOpaqueMaterial
     *  StandardOpaqueMaterials \endlink. */
      virtual boost::optional<double> heatCapacity() const;

      /// get interior visible absorptance (unitless)
      virtual boost::optional<double> interiorVisibleAbsorptance() const;

      /// get exterior visible absorptance (unitless)
      virtual boost::optional<double> exteriorVisibleAbsorptance() const;

      /// get visible transmittance (unitless)
      virtual boost::optional<double> visibleTransmittance() const;

      /// Returns the rendering color.
      virtual boost::optional<RenderingColor> renderingColor() const;

      /// Sets the rendering color.
      virtual bool setRenderingColor(const RenderingColor& renderingColor);

      /// Resets the rendering color.
      virtual void resetRenderingColor();

      /// Resets the rendering color.
      virtual int renderingColorIndex() const = 0;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.ConstructionBase");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONBASE_IMPL_HPP
