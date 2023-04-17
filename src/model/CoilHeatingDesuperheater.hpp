/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDESUPERHEATER_HPP
#define MODEL_COILHEATINGDESUPERHEATER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ModelObject;
  class AirflowNetworkEquivalentDuct;

  namespace detail {

    class CoilHeatingDesuperheater_Impl;

  }  // namespace detail

  /** CoilHeatingDesuperheater is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Desuperheater'.

  This coil can be used in air loop simulations but can’t be used by certain compound objects
  (e.g., AirLoopHVACUnitaryHeatPumpAirToAir, AirLoopHVACUnitaryHeatPumpWaterToAir, or DehumidifierDesiccantNoFans)
  or any air distribution equipment (e.g., AirTerminalSingleDuctConstantVolumeReheat, AirTerminalSingleDuctVAVReheat,
  or AirTerminalSingleDuctSeriesPIUReheat).
*/
  class MODEL_API CoilHeatingDesuperheater : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDesuperheater(const Model& model);

    virtual ~CoilHeatingDesuperheater() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDesuperheater(const CoilHeatingDesuperheater& other) = default;
    CoilHeatingDesuperheater(CoilHeatingDesuperheater&& other) = default;
    CoilHeatingDesuperheater& operator=(const CoilHeatingDesuperheater&) = default;
    CoilHeatingDesuperheater& operator=(CoilHeatingDesuperheater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double heatReclaimRecoveryEfficiency() const;

    bool isHeatReclaimRecoveryEfficiencyDefaulted() const;

    boost::optional<ModelObject> heatingSource() const;

    double parasiticElectricLoad() const;

    bool isParasiticElectricLoadDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setHeatReclaimRecoveryEfficiency(double heatReclaimRecoveryEfficiency);

    void resetHeatReclaimRecoveryEfficiency();

    /** \fn bool setHeatingSource(const ModelObject& heatingSource)
      \brief Sets the heating source from which the desuperheater heating coil recovers energy.
      \param[in] heatingSource The heating source.

      Valid inputs: \n
      \ref CoilCoolingDXSingleSpeed \n
      \ref CoilCoolingDXTwoSpeed \n
      \ref RefrigerationCompressorRack \n
      \ref RefrigerationCondenserAirCooled \n
      \ref RefrigerationCondenserEvaporativeCooled \n
      \ref RefrigerationCondenserWaterCooled \n
  */
    bool setHeatingSource(const ModelObject& modelObject);

    void resetHeatingSource();

    bool setParasiticElectricLoad(double parasiticElectricLoad);

    void resetParasiticElectricLoad();

    //@}
    /** @name Other */
    //@{

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDesuperheater_Impl;

    explicit CoilHeatingDesuperheater(std::shared_ptr<detail::CoilHeatingDesuperheater_Impl> impl);

    friend class detail::CoilHeatingDesuperheater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDesuperheater");
  };

  /** \relates CoilHeatingDesuperheater*/
  using OptionalCoilHeatingDesuperheater = boost::optional<CoilHeatingDesuperheater>;

  /** \relates CoilHeatingDesuperheater*/
  using CoilHeatingDesuperheaterVector = std::vector<CoilHeatingDesuperheater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDESUPERHEATER_HPP
