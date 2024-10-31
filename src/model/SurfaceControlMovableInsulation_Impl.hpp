/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACECONTROLMOVABLEINSULATION_IMPL_HPP
#define MODEL_SURFACECONTROLMOVABLEINSULATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Surface;
  class Material;
  class Schedule;

  namespace detail {

    /** SurfaceControlMovableInsulation_Impl is a ModelObject_Impl that is the implementation class for SurfaceControlMovableInsulation.*/
    class MODEL_API SurfaceControlMovableInsulation_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfaceControlMovableInsulation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfaceControlMovableInsulation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfaceControlMovableInsulation_Impl(const SurfaceControlMovableInsulation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfaceControlMovableInsulation_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      std::string insulationType() const;

      Surface surface() const;

      Material material() const;

      Schedule schedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setInsulationType(const std::string& insulationType);

      bool setSurface(const Surface& surface);

      bool setMaterial(const Material& material);

      bool setSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfaceControlMovableInsulation");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACECONTROLMOVABLEINSULATION_IMPL_HPP
