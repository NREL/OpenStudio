/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP
#define MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Material;

namespace detail {

  /** StandardsInformationMaterial_Impl is a ModelObject_Impl that is the implementation class for StandardsInformationMaterial.*/
  class MODEL_API StandardsInformationMaterial_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsInformationMaterial_Impl(const IdfObject& idfObject,
                                      Model_Impl* model,
                                      bool keepHandle);

    StandardsInformationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    StandardsInformationMaterial_Impl(const StandardsInformationMaterial_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    virtual ~StandardsInformationMaterial_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const override;

    virtual bool setParent(ParentObject& newParent) override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    Material material() const;

    boost::optional<std::string> materialStandard() const;
    std::vector<std::string> suggestedMaterialStandards() const;

    boost::optional<std::string> materialStandardSource() const;
    std::vector<std::string> suggestedMaterialStandardSources() const;

    boost::optional<std::string> standardsCategory() const;
    std::vector<std::string> suggestedStandardsCategories() const;
    bool isCompositeMaterial() const;

    boost::optional<std::string> standardsIdentifier() const;
    std::vector<std::string> suggestedStandardsIdentifiers() const;

    boost::optional<std::string> compositeFramingMaterial() const;
    std::vector<std::string> suggestedCompositeFramingMaterials() const;

    boost::optional<std::string> compositeFramingConfiguration() const;
    std::vector<std::string> suggestedCompositeFramingConfigurations() const;

    boost::optional<std::string> compositeFramingDepth() const;
    std::vector<std::string> suggestedCompositeFramingDepths() const;

    boost::optional<std::string> compositeFramingSize() const;
    std::vector<std::string> suggestedCompositeFramingSizes() const;

    boost::optional<std::string> compositeCavityInsulation() const;
    std::vector<std::string> suggestedCompositeCavityInsulations() const;

    //@}
    /** @name Setters */
    //@{

    void setMaterialStandard(const std::string& materialStandard);
    void resetMaterialStandard();

    void setMaterialStandardSource(const std::string& materialStandardSource);
    void resetMaterialStandardSource();

    void setStandardsCategory(const std::string& standardsCategory);
    void resetStandardsCategory();

    void setStandardsIdentifier(const std::string& standardsIdentifier);
    void resetStandardsIdentifier();

    void setCompositeFramingMaterial(const std::string& compositeFramingMaterial);
    void resetCompositeFramingMaterial();

    void setCompositeFramingConfiguration(const std::string& compositeFramingConfiguration);
    void resetCompositeFramingConfiguration();

    void setCompositeFramingDepth(const std::string& compositeFramingDepth);
    void resetCompositeFramingDepth();

    void setCompositeFramingSize(const std::string& compositeFramingSize);
    void resetCompositeFramingSize();

    void setCompositeCavityInsulation(const std::string& compositeCavityInsulation);
    void resetCompositeCavityInsulation();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.StandardsInformationMaterial");

    void parseStandardsMap() const;
    static QMap<QString, QVariant> m_standardsMap;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP

