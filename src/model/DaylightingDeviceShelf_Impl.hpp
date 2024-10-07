/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP
#define MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class InteriorPartitionSurface;
  class ShadingSurface;
  class DaylightingDeviceShelf;

  namespace detail {

    /** DaylightingDeviceShelf_Impl is a ModelObject_Impl that is the implementation class for DaylightingDeviceShelf.*/
    class MODEL_API DaylightingDeviceShelf_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      DaylightingDeviceShelf_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DaylightingDeviceShelf_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DaylightingDeviceShelf_Impl(const DaylightingDeviceShelf_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DaylightingDeviceShelf_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<InteriorPartitionSurface> insideShelf() const;

      boost::optional<ShadingSurface> outsideShelf() const;

      boost::optional<double> viewFactortoOutsideShelf() const;

      //@}
      /** @name Setters */
      //@{

      bool setInsideShelf(const InteriorPartitionSurface& insideShelf);

      bool setOutsideShelf(const ShadingSurface& outsideShelf);

      bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

      void resetViewFactortoOutsideShelf();

      //@}

      /// Get the related sub surface.
      SubSurface subSurface() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DaylightingDeviceShelf");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP
