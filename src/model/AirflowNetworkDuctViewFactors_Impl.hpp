/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // Foward declaration
  class AirflowNetworkLinkage;
  class PlanarSurface;
  class ViewFactorData;

  namespace detail {

    /** AirflowNetworkDuctViewFactors_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkDuctViewFactors.*/
    class MODEL_API AirflowNetworkDuctViewFactors_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkDuctViewFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkDuctViewFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkDuctViewFactors_Impl(const AirflowNetworkDuctViewFactors_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkDuctViewFactors_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      AirflowNetworkLinkage linkage() const;

      double ductSurfaceExposureFraction() const;

      bool isDuctSurfaceExposureFractionDefaulted() const;

      double ductSurfaceEmittance() const;

      bool isDuctSurfaceEmittanceDefaulted() const;

      boost::optional<double> getViewFactor(const PlanarSurface& surf) const;

      std::vector<ViewFactorData> viewFactors() const;

      //@}
      /** @name Setters */
      //@{

      bool setLinkage(const AirflowNetworkLinkage& linkage);

      bool setDuctSurfaceExposureFraction(double ductSurfaceExposureFraction);

      void resetDuctSurfaceExposureFraction();

      bool setDuctSurfaceEmittance(double ductSurfaceEmittance);

      void resetDuctSurfaceEmittance();

      bool setViewFactor(const PlanarSurface& surf, double F);
      bool removeViewFactor(const PlanarSurface& surf);
      void resetViewFactors();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkDuctViewFactors");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<AirflowNetworkLinkage> optionalLinkage() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_IMPL_HPP
