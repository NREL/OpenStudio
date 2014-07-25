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

#ifndef MODEL_EXTERIORLIGHTS_IMPL_HPP
#define MODEL_EXTERIORLIGHTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class ExteriorLightsDefinition;
class Schedule;
class Facility;

namespace detail {

  /** ExteriorLights_Impl is a ModelObject_Impl that is the implementation class for ExteriorLights.*/
  class MODEL_API ExteriorLights_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string controlOption READ controlOption WRITE setControlOption RESET resetControlOption);
    Q_PROPERTY(bool isControlOptionDefaulted READ isControlOptionDefaulted);
    Q_PROPERTY(std::vector<std::string> controlOptionValues READ controlOptionValues);

    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);

    Q_PROPERTY(std::string endUseSubcategory READ endUseSubcategory WRITE setEndUseSubcategory RESET resetEndUseSubcategory);
    Q_PROPERTY(bool isEndUseSubcategoryDefaulted READ isEndUseSubcategoryDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> exteriorLightsDefinition READ exteriorLightsDefinitionAsModelObject WRITE setExteriorLightsDefinitionAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> facility READ facilityAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    ExteriorLights_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ExteriorLights_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    ExteriorLights_Impl(const ExteriorLights_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~ExteriorLights_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    ExteriorLightsDefinition exteriorLightsDefinition() const;

    Schedule schedule() const;

    std::string controlOption() const;

    bool isControlOptionDefaulted() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition);

    bool setSchedule(Schedule& schedule);

    bool setControlOption(std::string controlOption);

    void resetControlOption();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    Facility facility() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLights");

    std::vector<std::string> controlOptionValues() const;

    boost::optional<ModelObject> exteriorLightsDefinitionAsModelObject() const;
    boost::optional<ModelObject> scheduleAsModelObject() const;
    boost::optional<ModelObject> facilityAsModelObject() const;

    bool setExteriorLightsDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EXTERIORLIGHTS_IMPL_HPP

