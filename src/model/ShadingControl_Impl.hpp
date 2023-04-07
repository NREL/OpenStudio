/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~ShadingControl_Impl() = default;

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
