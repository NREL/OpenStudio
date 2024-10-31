/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DISTRICTHEATINGWATER_IMPL_HPP
#define MODEL_DISTRICTHEATINGWATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    /** DistrictHeatingWater_Impl is a StraightComponent_Impl that is the implementation class for DistrictHeatingWater.*/

    class MODEL_API DistrictHeatingWater_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DistrictHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DistrictHeatingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DistrictHeatingWater_Impl(const DistrictHeatingWater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DistrictHeatingWater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      Schedule capacityFractionSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalCapacity(double nominalCapacity);

      void autosizeNominalCapacity();

      bool setCapacityFractionSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedNominalCapacity() const;

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DistrictHeatingWater");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_DISTRICTHEATINGWATER_IMPL_HPP
