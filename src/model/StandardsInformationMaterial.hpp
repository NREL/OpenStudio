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

#ifndef MODEL_STANDARDSINFORMATIONMATERIAL_HPP
#define MODEL_STANDARDSINFORMATIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Material;

  namespace detail {
    class StandardsInformationMaterial_Impl;
    class Material_Impl;
  }  // namespace detail

  /** StandardsInformationMaterial is a ModelObject that wraps the OpenStudio IDD object 'OS:StandardsInformation:Material'. */
  class MODEL_API StandardsInformationMaterial : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~StandardsInformationMaterial() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    StandardsInformationMaterial(const StandardsInformationMaterial& other) = default;
    StandardsInformationMaterial(StandardsInformationMaterial&& other) = default;
    StandardsInformationMaterial& operator=(const StandardsInformationMaterial&) = default;
    StandardsInformationMaterial& operator=(StandardsInformationMaterial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the material that this standards information object refers to. */
    Material material() const;

    /** Returns the standard that specifies this material, e.g. 'CEC Title24-2013'. */
    boost::optional<std::string> materialStandard() const;

    /** Returns a list of suggested material standards. */
    std::vector<std::string> suggestedMaterialStandards() const;

    /** Identifies the table or section in the standard which specifies this material. */
    boost::optional<std::string> materialStandardSource() const;

    /** Returns a list of suggestions for material standard source based on material standard. */
    std::vector<std::string> suggestedMaterialStandardSources() const;

    /** Returns the category of this material, e.g. 'Plastering Materials' 'Wood Framed Wall', or 'Roofing'. */
    boost::optional<std::string> standardsCategory() const;

    /** Returns a list of suggestions for standards category, filters list by material standard if it is set. */
    std::vector<std::string> suggestedStandardsCategories() const;

    /** Returns true if this standards category represents a composite layer (such as framing with cavity insulation). */
    bool isCompositeMaterial() const;

    /** Returns the id of this material within a standard, e.g. 'Wood siding - 1/2 in.'.*/
    boost::optional<std::string> standardsIdentifier() const;

    /** Returns a list of suggestions for standards identifier, filters list by material standard and standards category if set. */
    std::vector<std::string> suggestedStandardsIdentifiers() const;

    /** Returns the framing material for a composite layer, e.g. 'Wood' or 'Metal'. */
    boost::optional<std::string> compositeFramingMaterial() const;

    /** Returns a list of suggestions for composite framing material, filters list by material standard and standards category if set. */
    std::vector<std::string> suggestedCompositeFramingMaterials() const;

    /** Returns the framing configuration for a composite layer, e.g. 'Wall16inOC'. */
    boost::optional<std::string> compositeFramingConfiguration() const;

    /** Returns a list of suggestions for composite framing configurations, filters list by material standard and standards category if set. */
    std::vector<std::string> suggestedCompositeFramingConfigurations() const;

    /** Returns the framing depth for a composite layer, e.g. '3_5In'. */
    boost::optional<std::string> compositeFramingDepth() const;

    /** Returns a list of suggestions for composite framing depths, filters list by material standard and standards category if set. */
    std::vector<std::string> suggestedCompositeFramingDepths() const;

    /** Returns the framing size for a composite layer, e.g. '2x4'. */
    boost::optional<std::string> compositeFramingSize() const;

    /** Returns a list of suggestions for composite framing sizes, filters list by material standard and standards category if set. */
    std::vector<std::string> suggestedCompositeFramingSizes() const;

    /** Returns the cavity insulation for a composite layer, e.g. '30'*/
    boost::optional<std::string> compositeCavityInsulation() const;

    /** Returns a list of suggestions for cavity insulation, filters list by material standard and standards category if set. */
    std::vector<std::string> suggestedCompositeCavityInsulations() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaterialStandard(const std::string& materialStandard);
    void resetMaterialStandard();

    bool setMaterialStandardSource(const std::string& materialStandardSource);
    void resetMaterialStandardSource();

    bool setStandardsCategory(const std::string& standardsCategory);
    void resetStandardsCategory();

    bool setStandardsIdentifier(const std::string& standardsIdentifier);
    void resetStandardsIdentifier();

    bool setCompositeFramingMaterial(const std::string& compositeFramingMaterial);
    void resetCompositeFramingMaterial();

    bool setCompositeFramingConfiguration(const std::string& compositeFramingConfiguration);
    void resetCompositeFramingConfiguration();

    bool setCompositeFramingDepth(const std::string& compositeFramingDepth);
    void resetCompositeFramingDepth();

    bool setCompositeFramingSize(const std::string& compositeFramingSize);
    void resetCompositeFramingSize();

    bool setCompositeCavityInsulation(const std::string& compositeCavityInsulation);
    void resetCompositeCavityInsulation();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::StandardsInformationMaterial_Impl;

    explicit StandardsInformationMaterial(const Material& material);

    explicit StandardsInformationMaterial(std::shared_ptr<detail::StandardsInformationMaterial_Impl> impl);

    friend class detail::StandardsInformationMaterial_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class openstudio::model::Material;
    friend class openstudio::model::detail::Material_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.StandardsInformationMaterial");
  };

  /** \relates StandardsInformationMaterial*/
  using OptionalStandardsInformationMaterial = boost::optional<StandardsInformationMaterial>;

  /** \relates StandardsInformationMaterial*/
  using StandardsInformationMaterialVector = std::vector<StandardsInformationMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STANDARDSINFORMATIONMATERIAL_HPP
