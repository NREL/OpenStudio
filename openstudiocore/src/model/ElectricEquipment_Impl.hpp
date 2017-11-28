/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual bool hardSize() override;

    virtual bool hardApplySchedules() override;

    virtual double multiplier() const override;

    virtual bool isMultiplierDefaulted() const override;

    virtual bool isAbsolute() const override;

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
    virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(std::string endUseSubcategory);

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
    virtual int spaceIndex() const override;

    // index of the definition name
    virtual int definitionIndex() const override;

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

