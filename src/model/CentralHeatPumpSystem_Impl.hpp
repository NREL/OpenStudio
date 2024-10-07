/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ModelObjectList;
  class CentralHeatPumpSystemModule;

  namespace detail {

    /** CentralHeatPumpSystem_Impl is a WaterToWaterComponent_Impl that is the implementation class for CentralHeatPumpSystem.*/
    class MODEL_API CentralHeatPumpSystem_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CentralHeatPumpSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CentralHeatPumpSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CentralHeatPumpSystem_Impl(const CentralHeatPumpSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CentralHeatPumpSystem_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // CoolingLoop
      virtual unsigned supplyInletPort() const override;
      virtual unsigned supplyOutletPort() const override;
      // SourceLoop
      virtual unsigned demandInletPort() const override;
      virtual unsigned demandOutletPort() const override;
      // HeatingLoop
      virtual unsigned tertiaryInletPort() const override;
      virtual unsigned tertiaryOutletPort() const override;

      /* This function will perform a check if trying to add it to a node that is on the supply side of a plant loop.
     * If:
     *     - the node is on the supply side of a loop
     *     - the CentralHeatPumpSystem already has a cooling loop (supply side)
     *     - the node isn't on the cooling loop itself
     *     - the CentralHeatPumpSystem doesn't have a heating (tertiary) loop,
     * then it tries to add it to the Tertiary loop.
     * In all other cases, it will call the base class' method WaterToWaterComponent_Impl::addToNode()
     */
      virtual bool addToNode(Node& node) override;

      /* Restricts addToTertiaryNode to a node that is on the supply side of a plant loop (tertiary = Heating Loop) */
      virtual bool addToTertiaryNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string controlMethod() const;

      double ancillaryPower() const;

      boost::optional<Schedule> ancillaryOperationSchedule() const;

      boost::optional<ModelObjectList> chillerHeaterModuleList() const;

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

      bool setChillerHeaterModuleList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetChillerHeaterModuleList();

      /** Convenience Function to return the Cooling Loop **/
      boost::optional<PlantLoop> coolingPlantLoop() const;

      /** Convenience Function to return the Source Loop **/
      boost::optional<PlantLoop> sourcePlantLoop() const;

      /** Convenience Function to return the Heating Loop **/
      boost::optional<PlantLoop> heatingPlantLoop() const;

      // TODO: Need to override the clone, allowableChildTypes and children methods
      ModelObject clone(Model model) const override;

      // std::vector<IddObjectType> allowableChildTypes() const override;

      // std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP
