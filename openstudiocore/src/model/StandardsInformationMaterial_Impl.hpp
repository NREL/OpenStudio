/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Material.
    Material material() const;

    boost::optional<std::string> materialStandard() const;

    boost::optional<std::string> materialStandardSource() const;

    boost::optional<std::string> standardsCategory() const;

    boost::optional<std::string> standardsIdentifier() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Material.
    bool setMaterial(const Material& material);

    void setMaterialStandard(boost::optional<std::string> materialStandard);

    void resetMaterialStandard();

    void setMaterialStandardSource(boost::optional<std::string> materialStandardSource);

    void resetMaterialStandardSource();

    void setStandardsCategory(boost::optional<std::string> standardsCategory);

    void resetStandardsCategory();

    void setStandardsIdentifier(boost::optional<std::string> standardsIdentifier);

    void resetStandardsIdentifier();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.StandardsInformationMaterial");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Material> optionalMaterial() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP

