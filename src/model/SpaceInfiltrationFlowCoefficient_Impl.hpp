/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_IMPL_HPP
#define MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class SpaceInfiltrationFlowCoefficient;

  namespace detail {

    /** SpaceInfiltrationFlowCoefficient_Impl is a SpaceLoad_Impl that is the implementation class for SpaceInfiltrationFlowCoefficient.*/
    class MODEL_API SpaceInfiltrationFlowCoefficient_Impl : public SpaceLoad_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SpaceInfiltrationFlowCoefficient_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SpaceInfiltrationFlowCoefficient_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SpaceInfiltrationFlowCoefficient_Impl(const SpaceInfiltrationFlowCoefficient_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceInfiltrationFlowCoefficient_Impl() override = default;

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

      double flowCoefficient() const;

      double stackCoefficient() const;

      double pressureExponent() const;

      double windCoefficient() const;

      double shelterFactor() const;

      //@}
      /** @name Setters */
      //@{

      /** Sets the (fractional) Schedule. */
      bool setSchedule(Schedule& schedule);

      /** Resets the (fractional) Schedule. */
      void resetSchedule();

      bool setFlowCoefficient(double flowCoefficient);

      bool setStackCoefficient(double stackCoefficient);

      bool setPressureExponent(double pressureExponent);

      bool setWindCoefficient(double windCoefficient);

      bool setShelterFactor(double shelterFactor);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceInfiltrationFlowCoefficient");

      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_IMPL_HPP
