/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef MODEL_CONSTRUCTIONBASESTANDARDSINFORMATION_IMPL_HPP
#define MODEL_CONSTRUCTIONBASESTANDARDSINFORMATION_IMPL_HPP

#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

class ConstructionBaseStandardsInformation;
class ConstructionBase;
class Material;

namespace detail {

  class MODEL_API ConstructionBaseStandardsInformation_Impl : public ModelObject_Impl {
   public:
    /** Constructors and Destructors */
    //@{

    // constructor
    ConstructionBaseStandardsInformation_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    // construct from workspace
    ConstructionBaseStandardsInformation_Impl(
        const openstudio::detail::WorkspaceObject_Impl& other,
        Model_Impl* model,
        bool keepHandle);

    ConstructionBaseStandardsInformation_Impl(
        const ConstructionBaseStandardsInformation_Impl& other,
        Model_Impl* model,
        bool keepHandle);

    // virtual destructor
    virtual ~ConstructionBaseStandardsInformation_Impl(){}

    //@}
    /** @name Getters */
    //@{

    boost::optional<ConstructionBase> construction() const;

    std::string intendedSurfaceType() const;

    std::string constructionType() const;

    boost::optional<Material> perturbableLayer() const;

    std::string perturbableLayerType() const;

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Setters */
    //@{

    void setConstruction(const ConstructionBase& construction);

    bool setIntendedSurfaceType(const std::string& type);

    void setConstructionType(const std::string& type);

    bool setPerturbableLayer(unsigned layerIndex);

    bool setPerturbableLayer(const Material& material);

    /** Clears all PerturbableLayer information, including type. */
    void resetPerturbableLayer();

    void setPerturbableLayerType(const std::string& type);

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ConstructionBaseStandardsInformation");
  };
  
} // detail

} // model
} // openstudio 

#endif // MODEL_CONSTRUCTIONBASESTANDARDSINFORMATION_IMPL_HPP
