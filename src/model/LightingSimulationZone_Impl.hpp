/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTINGSIMULATIONZONE_IMPL_HPP
#define MODEL_LIGHTINGSIMULATIONZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RenderingColor;
  class Space;
  class LightingSimulationZone;

  namespace detail {

    /** LightingSimulationZone_Impl is a ModelObject_Impl that is the implementation class for LightingSimulationZone.*/
    class MODEL_API LightingSimulationZone_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      LightingSimulationZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LightingSimulationZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LightingSimulationZone_Impl(const LightingSimulationZone_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LightingSimulationZone_Impl() override = default;

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      //@}
      /** @name Setters */
      //@{

      //@}

      /// Returns the rendering color.
      boost::optional<RenderingColor> renderingColor() const;

      /// Sets the rendering color.
      bool setRenderingColor(const RenderingColor& renderingColor);

      /// Resets the rendering color.
      void resetRenderingColor();

      /// Returns all spaces in this lighting simulation zone.
      std::vector<Space> spaces() const;

      /// Adds a spaces to this lighting simulation zone.
      bool addSpace(const Space& space);

      /// Resets spaces in this lighting simulation zone.
      void resetSpaces();

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LightingSimulationZone");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGSIMULATIONZONE_IMPL_HPP
