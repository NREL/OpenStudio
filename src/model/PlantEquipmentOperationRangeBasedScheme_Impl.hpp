/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_IMPL_HPP

#include "PlantEquipmentOperationScheme_Impl.hpp"

namespace openstudio {
namespace model {

  class PlantLoop;

  namespace detail {

    class MODEL_API PlantEquipmentOperationRangeBasedScheme_Impl : public PlantEquipmentOperationScheme_Impl
    {
     public:
      PlantEquipmentOperationRangeBasedScheme_Impl(IddObjectType type, Model_Impl* model);

      PlantEquipmentOperationRangeBasedScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationRangeBasedScheme_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationRangeBasedScheme_Impl(const PlantEquipmentOperationRangeBasedScheme_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~PlantEquipmentOperationRangeBasedScheme_Impl() override = default;

      bool addLoadRange(double upperLimit, const std::vector<HVACComponent>& equipment);

      std::vector<HVACComponent> removeLoadRange(double upperLimit);

      std::vector<double> loadRangeUpperLimits() const;

      std::vector<HVACComponent> equipment(double upperLimit) const;

      bool addEquipment(double upperLimit, const HVACComponent& equipment);

      bool addEquipment(const HVACComponent& equipment);

      bool replaceEquipment(double upperLimit, const std::vector<HVACComponent>& equipment);

      bool replaceEquipment(const std::vector<HVACComponent>& equipment);

      bool removeEquipment(double upperLimit, const HVACComponent& equipment);

      bool removeEquipment(const HVACComponent& equipment);

      void clearLoadRanges();

      virtual double maximumUpperLimit() const = 0;

      virtual double minimumLowerLimit() const = 0;

      ModelObject clone(Model model) const override;

     private:
      REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationRangeBasedScheme");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_IMPL_HPP
