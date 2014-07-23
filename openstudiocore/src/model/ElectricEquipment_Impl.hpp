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

#ifndef MODEL_ELECTRICEQUIPMENT_IMPL_HPP
#define MODEL_ELECTRICEQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ElectricEquipment;
class ElectricEquipmentDefinition;

namespace detail {

  /** ElectricEquipment_Impl is a SpaceLoadInstance_Impl that is the implementation class for ElectricEquipment.*/
  class MODEL_API ElectricEquipment_Impl : public SpaceLoadInstance_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool isScheduleDefaulted READ isScheduleDefaulted);
    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);
    Q_PROPERTY(std::string endUseSubcategory READ endUseSubcategory WRITE setEndUseSubcategory RESET resetEndUseSubcategory);
    Q_PROPERTY(bool isEndUseSubcategoryDefaulted READ isEndUseSubcategoryDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject RESET resetSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> electricEquipmentDefinition READ electricEquipmentDefinitionAsModelObject WRITE setElectricEquipmentDefinitionAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ElectricEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    ElectricEquipment_Impl(const ElectricEquipment_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~ElectricEquipment_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool hardSize();

    virtual bool hardApplySchedules();

    virtual double multiplier() const;

    virtual bool isMultiplierDefaulted() const;

    //@}
    /** @name Getters */
    //@{

    /** Gets the ElectricEquipmentDefinition object. */
    ElectricEquipmentDefinition electricEquipmentDefinition() const;

    /** Returns the (fractional) equipment schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the ElectricEquipmentDefinition object. */
    bool setElectricEquipmentDefinition(const ElectricEquipmentDefinition& definition);

    /** Sets the definition of this instance. */
    virtual bool setDefinition(const SpaceLoadDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> designLevel() const;

    boost::optional<double> powerPerFloorArea() const;

    boost::optional<double> powerPerPerson() const;

    /** Returns the design level represented by this instance, assuming floorArea (m^2) and 
     *  numPeople. */
    double getDesignLevel(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and 
     *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and 
     *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    //@}
   protected:    

    // index of the space name
    virtual int spaceIndex() const;

    // index of the definition name
    virtual int definitionIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.ElectricEquipment");

    boost::optional<ModelObject> scheduleAsModelObject() const;
    boost::optional<ModelObject> electricEquipmentDefinitionAsModelObject() const;

    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setElectricEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICEQUIPMENT_IMPL_HPP

