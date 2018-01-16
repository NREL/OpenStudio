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

#ifndef MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP
#define MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** DesignSpecificationZoneAirDistribution_Impl is a ModelObject_Impl that is the implementation class for DesignSpecificationZoneAirDistribution.*/
  class MODEL_API DesignSpecificationZoneAirDistribution_Impl : public ModelObject_Impl {


















   public:
    /** @name Constructors and Destructors */
    //@{

    DesignSpecificationZoneAirDistribution_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    DesignSpecificationZoneAirDistribution_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    DesignSpecificationZoneAirDistribution_Impl(const DesignSpecificationZoneAirDistribution_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~DesignSpecificationZoneAirDistribution_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    double zoneAirDistributionEffectivenessinCoolingMode() const;

    Quantity getZoneAirDistributionEffectivenessinCoolingMode(bool returnIP=false) const;

    bool isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    double zoneAirDistributionEffectivenessinHeatingMode() const;

    Quantity getZoneAirDistributionEffectivenessinHeatingMode(bool returnIP=false) const;

    bool isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    boost::optional<Schedule> zoneAirDistributionEffectivenessSchedule() const;

    double zoneSecondaryRecirculationFraction() const;

    Quantity getZoneSecondaryRecirculationFraction(bool returnIP=false) const;

    bool isZoneSecondaryRecirculationFractionDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode);

    bool setZoneAirDistributionEffectivenessinCoolingMode(const Quantity& zoneAirDistributionEffectivenessinCoolingMode);

    void resetZoneAirDistributionEffectivenessinCoolingMode();

    bool setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode);

    bool setZoneAirDistributionEffectivenessinHeatingMode(const Quantity& zoneAirDistributionEffectivenessinHeatingMode);

    void resetZoneAirDistributionEffectivenessinHeatingMode();

    bool setZoneAirDistributionEffectivenessSchedule(Schedule& schedule);

    void resetZoneAirDistributionEffectivenessSchedule();

    bool setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction);

    bool setZoneSecondaryRecirculationFraction(const Quantity& zoneSecondaryRecirculationFraction);

    void resetZoneSecondaryRecirculationFraction();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.DesignSpecificationZoneAirDistribution");

    openstudio::Quantity zoneAirDistributionEffectivenessinCoolingMode_SI() const;
    openstudio::Quantity zoneAirDistributionEffectivenessinCoolingMode_IP() const;
    openstudio::Quantity zoneAirDistributionEffectivenessinHeatingMode_SI() const;
    openstudio::Quantity zoneAirDistributionEffectivenessinHeatingMode_IP() const;
    openstudio::Quantity zoneSecondaryRecirculationFraction_SI() const;
    openstudio::Quantity zoneSecondaryRecirculationFraction_IP() const;

    boost::optional<ModelObject> zoneAirDistributionEffectivenessScheduleAsModelObject() const;

    bool setZoneAirDistributionEffectivenessScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP

