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

#ifndef MODEL_STANDARDSINFORMATIONCONSTRUCTION_IMPL_HPP
#define MODEL_STANDARDSINFORMATIONCONSTRUCTION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class StandardsInformationConstruction;
class ConstructionBase;
class Material;

namespace detail {

  class MODEL_API StandardsInformationConstruction_Impl : public ModelObject_Impl {
   public:
    /** Constructors and Destructors */
    //@{

    // constructor
    StandardsInformationConstruction_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    // construct from workspace
    StandardsInformationConstruction_Impl(
        const openstudio::detail::WorkspaceObject_Impl& other,
        Model_Impl* model,
        bool keepHandle);

    StandardsInformationConstruction_Impl(
        const StandardsInformationConstruction_Impl& other,
        Model_Impl* model,
        bool keepHandle);

    // virtual destructor
    virtual ~StandardsInformationConstruction_Impl(){}

    //@}
    /** @name Getters */
    //@{

    ConstructionBase construction() const;

    boost::optional<std::string> intendedSurfaceType() const;

    boost::optional<std::string> standardsConstructionType() const;

    std::vector<std::string> suggestedStandardsConstructionTypes() const;

    boost::optional<Material> perturbableLayer() const;

    std::string perturbableLayerType() const;

    bool isPerturbableLayerTypeDefaulted() const;

    boost::optional<std::string> otherPerturbableLayerType() const;

    std::vector<std::string> suggestedConstructionStandards() const;

    boost::optional<std::string> constructionStandard() const;

    std::vector<std::string> suggestedConstructionStandardSources() const;

    boost::optional<std::string> constructionStandardSource() const;

    boost::optional<std::string> fenestrationType() const;

    boost::optional<std::string> fenestrationAssemblyContext() const;

    boost::optional<std::string> fenestrationNumberOfPanes() const;

    boost::optional<std::string> fenestrationFrameType() const;

    boost::optional<std::string> fenestrationDividerType() const;

    boost::optional<std::string> fenestrationTint() const;

    boost::optional<std::string> fenestrationGasFill() const;

    bool fenestrationLowEmissivityCoating() const;

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const override;

    /// set the parent, child may have to call non-const methods on the parent
    virtual bool setParent(ParentObject& newParent) override;

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Setters */
    //@{

    bool setIntendedSurfaceType(const std::string& type);
    void resetIntendedSurfaceType();

    void setStandardsConstructionType(const std::string& type);
    void resetStandardsConstructionType();

    bool setPerturbableLayer(unsigned layerIndex);

    bool setPerturbableLayer(const Material& material);

    /** Clears all PerturbableLayer information, including type. */
    void resetPerturbableLayer();

    void setPerturbableLayerType(const std::string& type);

    void resetPerturbableLayerType();

    void setOtherPerturbableLayerType(const std::string& otherPerturbableLayerType);

    void resetOtherPerturbableLayerType();

    void setConstructionStandard(const std::string& constructionStandard);

    void resetConstructionStandard();

    void setConstructionStandardSource(const std::string& constructionStandardSource);

    void resetConstructionStandardSource();

    bool setFenestrationType(const std::string& fenestrationType);

    void resetFenestrationType();

    bool setFenestrationAssemblyContext(const std::string& fenestrationAssemblyContext);

    void resetFenestrationAssemblyContext();

    bool setFenestrationNumberOfPanes(const std::string& fenestrationNumberOfPanes);

    void resetFenestrationNumberOfPanes();

    bool setFenestrationFrameType(const std::string& fenestrationFrameType);

    void resetFenestrationFrameType();

    bool setFenestrationDividerType(const std::string& fenestrationDividerType);

    void resetFenestrationDividerType();

    bool setFenestrationTint(const std::string& fenestrationTint);

    void resetFenestrationTint();

    bool setFenestrationGasFill(const std::string& fenestrationGasFill);

    void resetFenestrationGasFill();

    void setFenestrationLowEmissivityCoating(bool fenestrationLowEmissivityCoating);

    void resetFenestrationLowEmissivityCoating();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.StandardsInformationConstruction");
  };
  
} // detail

} // model
} // openstudio 

#endif // MODEL_STANDARDSINFORMATIONCONSTRUCTION_IMPL_HPP
