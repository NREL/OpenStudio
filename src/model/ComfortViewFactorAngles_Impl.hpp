/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMFORTVIEWFACTORANGLES_IMPL_HPP
#define MODEL_COMFORTVIEWFACTORANGLES_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ComfortViewFactorAngle;
  class Surface;

  namespace detail {
    class MODEL_API ComfortViewFactorAngles_Impl : public ModelObject_Impl
    {
     public:
      ComfortViewFactorAngles_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);
      ComfortViewFactorAngles_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);
      ComfortViewFactorAngles_Impl(const ComfortViewFactorAngles_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ComfortViewFactorAngles_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;
      virtual IddObjectType iddObjectType() const override;

      std::vector<ComfortViewFactorAngle> comfortViewFactorAngles() const;
      unsigned int numberofComfortViewFactorAngles() const;
      boost::optional<ComfortViewFactorAngle> getComfortViewFactorAngle(unsigned groupIndex) const;

      bool addComfortViewFactorAngle(const ComfortViewFactorAngle& comfortViewFactorAngle);
      bool addComfortViewFactorAngle(const Surface& surface, double angleFactor);
      bool removeComfortViewFactorAngle(unsigned groupIndex);
      void removeAllComfortViewFactorAngles();
    };
  }

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMFORTVIEWFACTORANGLES_IMPL_HPP