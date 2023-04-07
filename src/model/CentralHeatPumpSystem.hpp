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

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ModelObjectList;
  class CentralHeatPumpSystemModule;

  namespace detail {

    class CentralHeatPumpSystem_Impl;

  }  // namespace detail

  /** CentralHeatPumpSystem is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:CentralHeatPumpSystem'. */
  class MODEL_API CentralHeatPumpSystem : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /* This constructor adds a chiller heater module list to store your CentralHeatPumpSystemModules, but it doesn't add a CentralHeatPumpSystemModule
   * At least one CentralHeatPumpSystemModule is needed for this object to work correctly (and be translated) to EnergyPlus.
   */
    explicit CentralHeatPumpSystem(const Model& model);

    virtual ~CentralHeatPumpSystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CentralHeatPumpSystem(const CentralHeatPumpSystem& other) = default;
    CentralHeatPumpSystem(CentralHeatPumpSystem&& other) = default;
    CentralHeatPumpSystem& operator=(const CentralHeatPumpSystem&) = default;
    CentralHeatPumpSystem& operator=(CentralHeatPumpSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlMethodValues();

    /** @name Getters */
    //@{

    std::string controlMethod() const;

    double ancillaryPower() const;

    boost::optional<Schedule> ancillaryOperationSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlMethod(const std::string& controlMethod);

    bool setAncillaryPower(double ancillaryPower);

    bool setAncillaryOperationSchedule(Schedule& schedule);

    void resetAncillaryOperationSchedule();

    //@}
    /** @name Other */
    //@{

    bool addModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule);

    void removeModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule);

    void removeAllModules();

    std::vector<CentralHeatPumpSystemModule> modules() const;

    /** Convenience Function to return the Cooling Loop **/
    boost::optional<PlantLoop> coolingPlantLoop() const;

    /** Convenience Function to return the Source Loop **/
    boost::optional<PlantLoop> sourcePlantLoop() const;

    /** Convenience Function to return the Heating Loop **/
    boost::optional<PlantLoop> heatingPlantLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CentralHeatPumpSystem_Impl;

    explicit CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl);

    friend class detail::CentralHeatPumpSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");
  };

  /** \relates CentralHeatPumpSystem*/
  using OptionalCentralHeatPumpSystem = boost::optional<CentralHeatPumpSystem>;

  /** \relates CentralHeatPumpSystem*/
  using CentralHeatPumpSystemVector = std::vector<CentralHeatPumpSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CENTRALHEATPUMPSYSTEM_HPP
