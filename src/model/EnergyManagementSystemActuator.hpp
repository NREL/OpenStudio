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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

  class ThermalZone;
  class Space;

  namespace detail {

    class EnergyManagementSystemActuator_Impl;

  }  // namespace detail

  /** EnergyManagementSystemActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Actuator'. */
  class MODEL_API EnergyManagementSystemActuator : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor to use for any actuated component that is not a SpaceLoadInstance */
    explicit EnergyManagementSystemActuator(const ModelObject& actuatedComponent, const std::string& actuatedComponentType,
                                            const std::string& actuatedComponentControlType);
    /**These constructors below are for SpaceloadInstances that are defined in SpaceTypes that are used in Spaces.
     * Upon translation, the SpaceLoadInstances use ZoneLists which are not avail in OS
     * The ZoneListName is the SpaceType name
     * The Zone's are the Space->ThermalZone names
     * So to attach to a future zone, use the TZ or the Space that the SpaceLoadInstance will operate on **/

    // TODO: the first parameter should be a SpaceLoadInstance, not a ModelObject
    /** Constructor that will only accept a SpaceLoadInstance as the actuated component. Use this version if do not plan on disabling the Space
     * Translation in the Forward Translator */
    explicit EnergyManagementSystemActuator(const ModelObject& actuatedComponent, const std::string& actuatedComponentType,
                                            const std::string& actuatedComponentControlType, const Space& space);

    // TODO: the first parameter should be a SpaceLoadInstance, not a ModelObject
    /** Constructor that will only accept a SpaceLoadInstance as the actuated component. Use this version if you plan on disabling the Space
     * Translation in the Forward Translator */
    explicit EnergyManagementSystemActuator(const ModelObject& actuatedComponent, const std::string& actuatedComponentType,
                                            const std::string& actuatedComponentControlType, const ThermalZone& thermalZone);

    virtual ~EnergyManagementSystemActuator() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemActuator(const EnergyManagementSystemActuator& other) = default;
    EnergyManagementSystemActuator(EnergyManagementSystemActuator&& other) = default;
    EnergyManagementSystemActuator& operator=(const EnergyManagementSystemActuator&) = default;
    EnergyManagementSystemActuator& operator=(EnergyManagementSystemActuator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<ModelObject> actuatedComponent() const;  // TODO: this should NOT be an optional

    std::string actuatedComponentControlType() const;

    std::string actuatedComponentType() const;

    OS_DEPRECATED boost::optional<ModelObject> zoneName() const;

    boost::optional<ModelObject> zoneOrSpace() const;
    boost::optional<ThermalZone> thermalZone() const;
    boost::optional<Space> space() const;

    //@}
    /** @name Setters */
    //@{

    bool setActuatedComponent(const ModelObject& modelObject);

    bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

    bool setActuatedComponentType(const std::string& actuatedComponentType);
    //set the ZoneName field to the ThermalZone's names
    bool setThermalZone(const ThermalZone& thermalZone);
    //set the ZoneName field to the Space's ThermalZone's name
    bool setSpace(const Space& space);

    OS_DEPRECATED void resetZoneName();
    void resetZoneOrSpace();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemActuator_Impl;

    explicit EnergyManagementSystemActuator(std::shared_ptr<detail::EnergyManagementSystemActuator_Impl> impl);

    friend class detail::EnergyManagementSystemActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    // These are for the ReverseTranslator
    explicit EnergyManagementSystemActuator(const ModelObject& actuatedComponent);

    explicit EnergyManagementSystemActuator(const Model& model);

    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemActuator");
  };

  /** \relates EnergyManagementSystemActuator*/
  using OptionalEnergyManagementSystemActuator = boost::optional<EnergyManagementSystemActuator>;

  /** \relates EnergyManagementSystemActuator*/
  using EnergyManagementSystemActuatorVector = std::vector<EnergyManagementSystemActuator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP
