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

#ifndef MODEL_CONSTRUCTIONBASE_HPP
#define MODEL_CONSTRUCTIONBASE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class StandardsInformationConstruction;
  class RenderingColor;

  namespace detail {
    class ConstructionBase_Impl;
  }

  /** ConstructionBase is a ResourceObject that serves as a base class for several objects that can
 *  be used to specify a construction for a PlanarSurface. */
  class MODEL_API ConstructionBase : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ConstructionBase() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConstructionBase(const ConstructionBase& other) = default;
    ConstructionBase(ConstructionBase&& other) = default;
    ConstructionBase& operator=(const ConstructionBase&) = default;
    ConstructionBase& operator=(ConstructionBase&&) = default;

    //@}
    /** @name Getters */
    //@{

    /** Returns this construction's standards information, constructing a new object if necessary. */
    StandardsInformationConstruction standardsInformation() const;

    //@}

    /** Returns the sum of the surface area of all planar surfaces that use this construction (m^2).
   *  If this construction is used by a Surface that contains one or more \link SubSurface
   *  SubSurfaces \endlink, than the SubSurface areas are subtracted out of the total.
   *  If two adjacent surfaces both reference the same construction then only the area of the largest
   *  adjacent surface will be counted.  If two adjacent surfaces reference different constructions then
   *  the net area of each adjacent surface will be included in the net area for their respective constructions.*/
    double getNetArea() const;

    /** Set the u-factor of this surface in W/m^2*K, if possible. Value should already include
   *  appropriate standard film coefficients, assuming still air indoors and 15 mph outdoor air speed.
   *  Does not consider any custom SurfacePropertyConvectionCoefficients. */
    bool setUFactor(double value);

    /** Set the u-factor of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
   *  used to convert value to a conductance. */
    bool setUFactor(double value, double filmResistance);

    /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
   *  coefficients. */
    bool setConductance(double value);

    /** Set the conductance of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
   *  used to convert value to a u-factor. */
    bool setConductance(double value, double filmResistance);

    bool isOpaque() const;

    bool isFenestration() const;

    /** Returns true if isFenestration and any material layers are solar diffusing. */
    bool isSolarDiffusing() const;

    bool isModelPartition() const;

    /** Returns true if the construction has RoofVegetation as the outer layer. */
    bool isGreenRoof() const;

    /** Get the u-factor of this construction (W/m^2*K).
     * Includes standard film coefficients, does not consider any custom SurfacePropertyConvectionCoefficients. */
    boost::optional<double> uFactor() const;

    /** Get the u-factor of this construction (W/m^2*K). Includes filmResistance. */
    boost::optional<double> uFactor(double filmResistance) const;

    /** Get the thermal conductance of this construction (W/m^2*K). Does not include film coefficients. */
    boost::optional<double> thermalConductance() const;

    /** Get the thermal conductance of this construction (W/m^2*K). filmResistance is subtracted out
    *  of the thermalResistance if necessary. */
    boost::optional<double> thermalConductance(double filmResistance) const;

    /** Get the heat capacity of this construction (J/m^2*K). Only works for
    *  \link LayeredConstruction LayeredConstructions \endlink of \link StandardOpaqueMaterial
    *  StandardOpaqueMaterials \endlink. */
    boost::optional<double> heatCapacity() const;

    /** Get the visible absorptance of the interior side of this construction (dimensionless). May
    *  require simulation ('EnvelopeSummary' table). */
    boost::optional<double> interiorVisibleAbsorptance() const;

    /** Get the visible absorptance of the exterior side of this construction (dimensionless). May
     *  require simulation ('EnvelopeSummary' table). */
    boost::optional<double> exteriorVisibleAbsorptance() const;

    /** Get the visible transmittance of this construction (dimensionless). May require simulation
     *  ('EnvelopeSummary' table). */
    boost::optional<double> visibleTransmittance() const;

    /// Returns the rendering color.
    boost::optional<RenderingColor> renderingColor() const;

    /// Sets the rendering color.
    bool setRenderingColor(const RenderingColor& renderingColor);

    /// Resets the rendering color.
    void resetRenderingColor();

   protected:
    /// @cond
    using ImplType = detail::ConstructionBase_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::ConstructionBase_Impl;

    ConstructionBase(IddObjectType type, const Model& model);

    explicit ConstructionBase(std::shared_ptr<detail::ConstructionBase_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ConstructionBase");
  };

  /** \relates ConstructionBase */
  using OptionalConstructionBase = boost::optional<ConstructionBase>;

  /** \relates ConstructionBase */
  using ConstructionBaseVector = std::vector<ConstructionBase>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONBASE_HPP
