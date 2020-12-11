/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    explicit EnergyManagementSystemActuator(const ModelObject& modelObject, const std::string& actuatedComponentType,
                                            const std::string& actuatedComponentControlType);
    /**These constructors below are for SpaceloadInstances that are defined in SpaceTypes that are used in Spaces.
     * Upon translation, the SpaceLoadInstances use ZoneLists which are not avail in OS
     * The ZoneListName is the SpaceType name
     * The Zone's are the Space->ThermalZone names
     * So to attach to a future zone, use the TZ or the Space that the SpaceLoadInstance will operate on
  **/
    explicit EnergyManagementSystemActuator(const ModelObject& modelObject, const std::string& actuatedComponentType,
                                            const std::string& actuatedComponentControlType, const ThermalZone& thermalZone);

    explicit EnergyManagementSystemActuator(const ModelObject& modelObject, const std::string& actuatedComponentType,
                                            const std::string& actuatedComponentControlType, const Space& space);

    virtual ~EnergyManagementSystemActuator() {}

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<ModelObject> actuatedComponent() const;

    std::string actuatedComponentControlType() const;

    std::string actuatedComponentType() const;

    boost::optional<ModelObject> zoneName() const;

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

    void resetZoneName();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    typedef detail::EnergyManagementSystemActuator_Impl ImplType;

    explicit EnergyManagementSystemActuator(std::shared_ptr<detail::EnergyManagementSystemActuator_Impl> impl);

    friend class detail::EnergyManagementSystemActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemActuator");
    //These are for the ReveseTranslator
    explicit EnergyManagementSystemActuator(const ModelObject& modelObject);

    explicit EnergyManagementSystemActuator(const Model& model);
  };

  /** \relates EnergyManagementSystemActuator*/
  typedef boost::optional<EnergyManagementSystemActuator> OptionalEnergyManagementSystemActuator;

  /** \relates EnergyManagementSystemActuator*/
  typedef std::vector<EnergyManagementSystemActuator> EnergyManagementSystemActuatorVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_HPP
