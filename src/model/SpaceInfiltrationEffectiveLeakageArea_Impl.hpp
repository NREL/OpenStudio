/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP
#define MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class SpaceInfiltrationEffectiveLeakageArea;

  namespace detail {

    /** SpaceInfiltrationEffectiveLeakageArea_Impl is a SpaceLoad_Impl that is the implementation class for SpaceInfiltrationEffectiveLeakageArea.*/
    class MODEL_API SpaceInfiltrationEffectiveLeakageArea_Impl : public SpaceLoad_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SpaceInfiltrationEffectiveLeakageArea_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SpaceInfiltrationEffectiveLeakageArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SpaceInfiltrationEffectiveLeakageArea_Impl(const SpaceInfiltrationEffectiveLeakageArea_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceInfiltrationEffectiveLeakageArea_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual bool isAbsolute() const override;

      //@}
      /** @name Getters */
      //@{

      /** Returns the (fractional) infiltration schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
      boost::optional<Schedule> schedule() const;

      /** Returns true if this object does not specify a schedule directly. */
      bool isScheduleDefaulted() const;

      double effectiveAirLeakageArea() const;

      double stackCoefficient() const;

      double windCoefficient() const;

      //@}
      /** @name Setters */
      //@{

      /** Sets the (fractional) Schedule. */
      bool setSchedule(Schedule& schedule);

      /** Resets the (fractional) Schedule. */
      void resetSchedule();

      bool setEffectiveAirLeakageArea(double effectiveAirLeakageArea);

      bool setStackCoefficient(double stackCoefficient);

      bool setWindCoefficient(double windCoefficient);

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceInfiltrationEffectiveLeakageArea");

      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_IMPL_HPP
