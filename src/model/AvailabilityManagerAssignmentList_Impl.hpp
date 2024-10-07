/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Model;
  class AvailabilityManager;
  class Loop;
  class AirLoopHVAC;
  class PlantLoop;
  class ZoneHVACComponent;

  namespace detail {

    /** AvailabilityManagerAssignmentList_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerAssignmentList.*/
    class MODEL_API AvailabilityManagerAssignmentList_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerAssignmentList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerAssignmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerAssignmentList_Impl(const AvailabilityManagerAssignmentList_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerAssignmentList_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Getters */
      //@{

      unsigned availabilityManagerPriority(const AvailabilityManager& avm) const;

      std::vector<AvailabilityManager> availabilityManagers() const;

      boost::optional<Loop> loop() const;
      boost::optional<AirLoopHVAC> airLoopHVAC() const;
      boost::optional<PlantLoop> plantLoop() const;
      boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilityManagers(const std::vector<AvailabilityManager>& avms);
      void resetAvailabilityManagers();

      bool addAvailabilityManager(const AvailabilityManager& avm);
      bool addAvailabilityManager(const AvailabilityManager& avm, unsigned priority);

      bool removeAvailabilityManager(const AvailabilityManager& avm);
      bool removeAvailabilityManager(unsigned priority);

      bool setAvailabilityManagerPriority(const AvailabilityManager& avm, unsigned priority);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerAssignmentList");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_IMPL_HPP
