/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~EnergyManagementSystemActuator() override = default;
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

    OS_DEPRECATED(3, 6, 0) boost::optional<ModelObject> zoneName() const;

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

    OS_DEPRECATED(3, 6, 0) void resetZoneName();
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
