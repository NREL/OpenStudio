/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEAIRCONTAMINANTBALANCE_IMPL_HPP
#define MODEL_ZONEAIRCONTAMINANTBALANCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** ZoneAirContaminantBalance_Impl is a ModelObject_Impl that is the implementation class for ZoneAirContaminantBalance.*/
    class MODEL_API ZoneAirContaminantBalance_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ZoneAirContaminantBalance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ZoneAirContaminantBalance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ZoneAirContaminantBalance_Impl(const ZoneAirContaminantBalance_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ZoneAirContaminantBalance_Impl() override = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      //@}

      /** @name Virtual Methods */
      //@{

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      bool carbonDioxideConcentration() const;

      bool isCarbonDioxideConcentrationDefaulted() const;

      boost::optional<Schedule> outdoorCarbonDioxideSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setCarbonDioxideConcentration(bool carbonDioxideConcentration);

      void resetCarbonDioxideConcentration();

      bool setOutdoorCarbonDioxideSchedule(Schedule& schedule);

      void resetOutdoorCarbonDioxideSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneAirContaminantBalance");

      boost::optional<ModelObject> outdoorCarbonDioxideScheduleAsModelObject() const;

      bool setOutdoorCarbonDioxideScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEAIRCONTAMINANTBALANCE_IMPL_HPP
