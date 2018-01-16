/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP
#define MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class SpaceInfiltrationEffectiveLeakageArea;

namespace detail {

  /** SpaceInfiltrationEffectiveLeakageArea_Impl is a SpaceLoad_Impl that is the implementation class for SpaceInfiltrationEffectiveLeakageArea.*/
  class MODEL_API SpaceInfiltrationEffectiveLeakageArea_Impl : public SpaceLoad_Impl {














   public:
    /** @name Constructors and Destructors */
    //@{

    SpaceInfiltrationEffectiveLeakageArea_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle);

    SpaceInfiltrationEffectiveLeakageArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    SpaceInfiltrationEffectiveLeakageArea_Impl(const SpaceInfiltrationEffectiveLeakageArea_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    virtual ~SpaceInfiltrationEffectiveLeakageArea_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual bool hardSize() override;

    virtual bool hardApplySchedules() override;

    virtual bool isAbsolute() const override;

    //@}
    /** @name Getters */
    //@{

    /** Returns the (fractional) infiltration schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    double effectiveAirLeakageArea() const;

    Quantity getEffectiveAirLeakageArea(bool returnIP=false) const;

    double stackCoefficient() const;

    Quantity getStackCoefficient(bool returnIP=false) const;

    double windCoefficient() const;

    Quantity getWindCoefficient(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setEffectiveAirLeakageArea(double effectiveAirLeakageArea);

    bool setEffectiveAirLeakageArea(const Quantity& effectiveAirLeakageArea);

    bool setStackCoefficient(double stackCoefficient);

    bool setStackCoefficient(const Quantity& stackCoefficient);

    bool setWindCoefficient(double windCoefficient);

    bool setWindCoefficient(const Quantity& windCoefficient);

    //@}
   protected:

    // index of the space name
    virtual int spaceIndex() const override;

   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationEffectiveLeakageArea");

    openstudio::Quantity effectiveAirLeakageArea_SI() const;
    openstudio::Quantity effectiveAirLeakageArea_IP() const;
    openstudio::Quantity stackCoefficient_SI() const;
    openstudio::Quantity stackCoefficient_IP() const;
    openstudio::Quantity windCoefficient_SI() const;
    openstudio::Quantity windCoefficient_IP() const;

    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP

