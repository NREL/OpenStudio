/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADINGCONTROL_IMPL_HPP
#define MODEL_SHADINGCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Construction;
  class ShadingMaterial;
  class Schedule;
  class SubSurface;

  namespace detail {

    /** ShadingControl_Impl is a ResourceObject_Impl that is the implementation class for ShadingControl.*/
    class MODEL_API ShadingControl_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ShadingControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ShadingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ShadingControl_Impl(const ShadingControl_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ShadingControl_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // If cloning in the same model, will keep on referencing the same SubSurfaces as the original
      // If cloning in another model, will clear out its extensible Sub Surface Name group
      virtual ModelObject clone(Model model) const override;

      //@}

      static bool isControlTypeValueNeedingSetpoint1(const std::string& controlType);
      static bool isControlTypeValueNeedingSetpoint2(const std::string& controlType);

      // **Allows** a schedule
      static bool isControlTypeValueAllowingSchedule(const std::string& controlType);
      // **Requires** a Schedule
      static bool isControlTypeValueRequiringSchedule(const std::string& controlType);

      // **Allows** a slat angle control
      static bool isTypeValueAllowingSlatAngleControl(const std::string& type);

      /** @name Getters */
      //@{

      std::string shadingType() const;

      boost::optional<Construction> construction() const;

      boost::optional<ShadingMaterial> shadingMaterial() const;

      std::string shadingControlType() const;

      bool isShadingControlTypeDefaulted() const;

      boost::optional<Schedule> schedule() const;

      boost::optional<double> setpoint() const;

      bool isSetpointDefaulted() const;

      bool glareControlIsActive() const;

      std::string typeofSlatAngleControlforBlinds() const;

      bool isTypeofSlatAngleControlforBlindsDefaulted() const;

      boost::optional<Schedule> slatAngleSchedule() const;

      boost::optional<double> setpoint2() const;

      std::string multipleSurfaceControlType() const;

      //@}
      /** @name Setters */
      //@{

      bool setShadingType(const std::string& shadingType);

      bool setShadingControlType(const std::string& shadingControlType);

      void resetShadingControlType();

      bool setSchedule(const Schedule& schedule);

      void resetSchedule();

      bool setSetpoint(double setpoint);

      void resetSetpoint();

      bool setGlareControlIsActive(bool glareControlIsActive);

      void resetGlareControlIsActive();

      bool setTypeofSlatAngleControlforBlinds(const std::string& typeofSlatAngleControlforBlinds);

      void resetTypeofSlatAngleControlforBlinds();

      bool setSlatAngleSchedule(const Schedule& slatAngleSchedule);

      void resetSlatAngleSchedule();

      bool setSetpoint2(double setpoint2);

      // Impl only
      void resetSetpoint2();

      bool setMultipleSurfaceControlType(const std::string& multipleSurfaceControlType);

      //@}
      /** @name Other */
      //@{

      // Extensible: Surfaces
      std::vector<SubSurface> subSurfaces() const;
      unsigned numberofSubSurfaces() const;
      boost::optional<unsigned> subSurfaceIndex(const SubSurface& subSurface) const;

      bool addSubSurface(const SubSurface& subSurface);
      bool addSubSurface(const SubSurface& subSurface, unsigned index);
      bool setSubSurfaceIndex(const SubSurface& subSurface, unsigned index);
      bool removeSubSurface(const SubSurface& subSurface);
      bool removeSubSurface(unsigned index);
      // Bulk operations
      bool addSubSurfaces(const std::vector<SubSurface>& subSurfaces);
      // Clears existing first, then bulk add
      bool setSubSurfaces(const std::vector<SubSurface>& subSurfaces);
      void removeAllSubSurfaces();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ShadingControl");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGCONTROL_IMPL_HPP
