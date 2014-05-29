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

#ifndef MODEL_BUILDINGSTORY_IMPL_HPP
#define MODEL_BUILDINGSTORY_IMPL_HPP

#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class SpaceType;
class DefaultConstructionSet;
class DefaultScheduleSet;
class Space;
class RenderingColor;
class BuildingStory;

namespace detail {

  /** BuildingStory_Impl is a ModelObject_Impl that is the implementation class for BuildingStory.*/
  class MODEL_API BuildingStory_Impl : public ModelObject_Impl {
    Q_OBJECT;
    
    Q_PROPERTY(boost::optional<double> nominalZCoordinate READ nominalZCoordinate WRITE setNominalZCoordinate RESET resetNominalZCoordinate);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalZCoordinate_SI READ nominalZCoordinate_SI WRITE setNominalZCoordinate RESET resetNominalZCoordinate);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalZCoordinate_IP READ nominalZCoordinate_IP WRITE setNominalZCoordinate RESET resetNominalZCoordinate);
    
    Q_PROPERTY(double nominalFloortoFloorHeight READ nominalFloortoFloorHeight WRITE setNominalFloortoFloorHeight RESET resetNominalFloortoFloorHeight);
    Q_PROPERTY(openstudio::Quantity nominalFloortoFloorHeight_SI READ nominalFloortoFloorHeight_SI WRITE setNominalFloortoFloorHeight RESET resetNominalFloortoFloorHeight);
    Q_PROPERTY(openstudio::Quantity nominalFloortoFloorHeight_IP READ nominalFloortoFloorHeight_IP WRITE setNominalFloortoFloorHeight RESET resetNominalFloortoFloorHeight);
    Q_PROPERTY(bool isNominalFloortoFloorHeightDefaulted READ isNominalFloortoFloorHeightDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultConstructionSet READ defaultConstructionSetAsModelObject WRITE setDefaultConstructionSetAsModelObject RESET resetDefaultConstructionSet);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultScheduleSet READ defaultScheduleSetAsModelObject WRITE setDefaultScheduleSetAsModelObject RESET resetDefaultScheduleSet);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> renderingColor READ renderingColorAsModelObject WRITE setRenderingColorAsModelObject RESET resetRenderingColor);
   public:
    /** @name Constructors and Destructors */
    //@{

    BuildingStory_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    BuildingStory_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    BuildingStory_Impl(const BuildingStory_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    virtual ~BuildingStory_Impl() {}

    //@}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    boost::optional<double> nominalZCoordinate() const;
    
    OSOptionalQuantity getNominalZCoordinate(bool returnIP=false) const;   

    double nominalFloortoFloorHeight() const;
    
    Quantity getNominalFloortoFloorHeight(bool returnIP=false) const;

    bool isNominalFloortoFloorHeightDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setNominalZCoordinate(boost::optional<double> nominalZCoordinate);

    bool setNominalZCoordinate(const OSOptionalQuantity& nominalZCoordinate);

    void setNominalZCoordinate(double nominalZCoordinate);

    void resetNominalZCoordinate();

    bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);
    
    bool setNominalFloortoFloorHeight(const Quantity& nominalFloortoFloorHeight);    

    void resetNominalFloortoFloorHeight();

    //@}

    /// Gets all spaces on this building story.
    std::vector<Space> spaces() const;

    /// Returns the default construction set that this space references directly.
    boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

    /// Sets the default construction set for this space directly.
    bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

    /// Resets the default construction set for this space.
    void resetDefaultConstructionSet();

    /// Returns the default schedule set that this space references directly.
    boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

    /// Sets the default schedule set for this space directly.
    bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

    /// Resets the default schedule set for this space.
    void resetDefaultScheduleSet();

    /// Returns the rendering color.
    boost::optional<RenderingColor> renderingColor() const;

    /// Sets the rendering color.
    bool setRenderingColor(const RenderingColor& renderingColor);

    /// Resets the rendering color.
    void resetRenderingColor();

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.BuildingStory");

    openstudio::OSOptionalQuantity nominalZCoordinate_SI() const;
    openstudio::OSOptionalQuantity nominalZCoordinate_IP() const;
    openstudio::Quantity nominalFloortoFloorHeight_SI() const;
    openstudio::Quantity nominalFloortoFloorHeight_IP() const;

    boost::optional<ModelObject> defaultConstructionSetAsModelObject() const;
    boost::optional<ModelObject> defaultScheduleSetAsModelObject() const;
    boost::optional<ModelObject> renderingColorAsModelObject() const;

    bool setDefaultConstructionSetAsModelObject(boost::optional<ModelObject> modelObject);
    bool setDefaultScheduleSetAsModelObject(boost::optional<ModelObject> modelObject);
    bool setRenderingColorAsModelObject(boost::optional<ModelObject> modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_BUILDINGSTORY_IMPL_HPP

