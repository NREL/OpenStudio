/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGER_IMPL_HPP
#define MODEL_SETPOINTMANAGER_IMPL_HPP

#include "HVACComponent_Impl.hpp"
#include <boost/optional.hpp>

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API SetpointManager_Impl : public HVACComponent_Impl
    {
     public:
      SetpointManager_Impl(IddObjectType type, Model_Impl* model);

      SetpointManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManager_Impl(const SetpointManager_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~SetpointManager_Impl() override = default;

      /** This method will delete any existing SPM with the same controlVariable, and check if placing the SPM is allowed:
     * <ul>
     *  <li>Airside: only allows placement of the SPM on the supply side of an AirLoopHVAC,
     *      or the node of an AirLoopHVACOutdoorAirSystem EXCEPT on the outdoorAirNode or reliefAirNode</li>
     *  <li>Waterside: if isAllowedOnPlantLoop() returns true, it will allow connection on the supply side,
     *  or the demand side EXCEPT between the demand splitter and demand mixer (i.e. not allowed on a demand branch)</li>
     * </ul>.
     */
      virtual bool addToNode(Node& node) override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::string controlVariable() const = 0;

      virtual bool setControlVariable(const std::string& value) = 0;

      virtual boost::optional<Node> setpointNode() const = 0;

      virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      virtual boost::optional<PlantLoop> plantLoop() const override;

      virtual boost::optional<Loop> loop() const override;

      virtual boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const override;

      /** Defaults to false. Must be overriden by derived classes that can be actually placed on a PlantLoop to return true.
     * If true, addToNode will allow connection on a PlantLoop (supply side, or demand side EXCEPT between splitter and mixer) */
      virtual bool isAllowedOnPlantLoop() const;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      /** This is used to set the field SetpointNodeName.
      * Use addToNode to add a setpoint manager to a node. */
      virtual bool setSetpointNode(const Node& node) = 0;

      /** This is used to reset the field SetpointNodeName. */
      virtual void resetSetpointNode() = 0;

      REGISTER_LOGGER("openstudio.model.SetpointManager");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGER_IMPL_HPP
