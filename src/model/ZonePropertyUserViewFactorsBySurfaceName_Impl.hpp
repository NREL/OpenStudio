/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_IMPL_HPP
#define MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Surface;
  class SubSurface;
  class InternalMass;
  class ViewFactor;

  namespace detail {

    /** ZonePropertyUserViewFactorsBySurfaceName_Impl is a ModelObject_Impl that is the implementation class for ZonePropertyUserViewFactorsBySurfaceName.*/
    class MODEL_API ZonePropertyUserViewFactorsBySurfaceName_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZonePropertyUserViewFactorsBySurfaceName_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZonePropertyUserViewFactorsBySurfaceName_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZonePropertyUserViewFactorsBySurfaceName_Impl(const ZonePropertyUserViewFactorsBySurfaceName_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZonePropertyUserViewFactorsBySurfaceName_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // Disallow cloning
      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Getters */
      //@{

      ThermalZone thermalZone() const;

      std::vector<ViewFactor> viewFactors() const;

      unsigned int numberofViewFactors() const;

      boost::optional<unsigned> viewFactorIndex(const ViewFactor& viewFactor) const;

      boost::optional<ViewFactor> getViewFactor(unsigned groupIndex) const;

      //@}
      /** @name Setters */
      //@{

      bool addViewFactor(const ViewFactor& viewFactor);

      bool addViewFactor(const Surface& fromSurface, const Surface& toSurface, double viewFactor);

      bool addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double viewFactor);

      bool addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double viewFactor);

      bool addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double viewFactor);

      bool addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double viewFactor);

      bool addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass, double viewFactor);

      bool addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toInernalMass, double viewFactor);

      bool addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double viewFactor);

      bool addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface, double viewFactor);

      bool addViewFactors(const std::vector<ViewFactor>& viewFactors);

      bool removeViewFactor(unsigned groupIndex);

      void removeAllViewFactors();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZonePropertyUserViewFactorsBySurfaceName");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_IMPL_HPP
