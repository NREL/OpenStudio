/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGDEVICELIGHTWELL_IMPL_HPP
#define MODEL_DAYLIGHTINGDEVICELIGHTWELL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SubSurface;

  namespace detail {

    /** DaylightingDeviceLightWell_Impl is a ModelObject_Impl that is the implementation class for DaylightingDeviceLightWell.*/
    class MODEL_API DaylightingDeviceLightWell_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      DaylightingDeviceLightWell_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DaylightingDeviceLightWell_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DaylightingDeviceLightWell_Impl(const DaylightingDeviceLightWell_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DaylightingDeviceLightWell_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      SubSurface subSurface() const;

      double heightofWell() const;

      double perimeterofBottomofWell() const;

      double areaofBottomofWell() const;

      double visibleReflectanceofWellWalls() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeightofWell(double heightofWell);

      bool setPerimeterofBottomofWell(double perimeterofBottomofWell);

      bool setAreaofBottomofWell(double areaofBottomofWell);

      bool setVisibleReflectanceofWellWalls(double visibleReflectanceofWellWalls);

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DaylightingDeviceLightWell");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICELIGHTWELL_IMPL_HPP
