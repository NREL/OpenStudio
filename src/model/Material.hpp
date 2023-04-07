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

#ifndef MODEL_MATERIAL_HPP
#define MODEL_MATERIAL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class StandardsInformationMaterial;

  namespace detail {
    class Material_Impl;
  }

  /** A Material is a ResourceObject that serves as a base class for all objects that can be used
   *  in \link LayeredConstruction LayeredConstructions \endlink. */
  class MODEL_API Material : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Material() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Material(const Material& other) = default;
    Material(Material&& other) = default;
    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) = default;

    //@}
    /** @name Getters */
    //@{

    // Get the thickness of the material. Virtual implementation. For some materials, 0.0 is always returned.
    double thickness() const;

    // Get the visible transmittance of the material. Virtual implementation.
    boost::optional<double> getVisibleTransmittance() const;

    // Get the interiorVisibleAbsorptance of the material. Virtual implementation.
    boost::optional<double> interiorVisibleAbsorptance() const;

    // Get the exteriorVisibleAbsorptance of the material. Virtual implementation.
    boost::optional<double> exteriorVisibleAbsorptance() const;

    /** Returns this materials's standards information, constructing a new object if necessary. */
    StandardsInformationMaterial standardsInformation() const;

    //@}
    /** @name Setters */
    //@{

    // Set thickness to value (m). For some materials, false is always returned.
    bool setThickness(double value);

    //@}
   protected:
    /// @cond
    using ImplType = detail::Material_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::Material_Impl;

    Material(IddObjectType type, const Model& model);

    explicit Material(std::shared_ptr<detail::Material_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Material");
  };

  /** \relates Material */
  using OptionalMaterial = boost::optional<Material>;

  /** \relates Material */
  using MaterialVector = std::vector<Material>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIAL_HPP
