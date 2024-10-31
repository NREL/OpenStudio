/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LOADPROFILEPLANT_IMPL_HPP
#define MODEL_LOADPROFILEPLANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;

  namespace detail {

    /** LoadProfilePlant_Impl is a StraightComponent_Impl that is the implementation class for LoadProfilePlant.*/
    class MODEL_API LoadProfilePlant_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      LoadProfilePlant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LoadProfilePlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LoadProfilePlant_Impl(const LoadProfilePlant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LoadProfilePlant_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule loadSchedule() const;

      double peakFlowRate() const;

      Schedule flowRateFractionSchedule() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      std::string plantLoopFluidType() const;

      double degreeofSubCooling() const;

      double degreeofLoopSubCooling() const;

      //@}
      /** @name Setters */
      //@{

      bool setLoadSchedule(Schedule& schedule);

      bool setPeakFlowRate(double peakFlowRate);

      bool setFlowRateFractionSchedule(Schedule& schedule);

      bool setPlantLoopFluidType(const std::string& plantLoopFluidType);

      bool setDegreeofSubCooling(double degreeofSubCooling);

      bool setDegreeofLoopSubCooling(double degreeofLoopSubCooling);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LoadProfilePlant");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalLoadSchedule() const;
      boost::optional<Schedule> optionalFlowRateFractionSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LOADPROFILEPLANT_IMPL_HPP
