/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_HPP
#define MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include <vector>
#include <unordered_map>
#include "PlanarSurface.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkLinkage;

  namespace detail {

    class AirflowNetworkDuctViewFactors_Impl;

  }  // namespace detail

  /** This class implements a single point of a viewFactors, meant to replace std::pair<PlanarSurface, double> */
  class MODEL_API ViewFactorData
  {
   public:
    ViewFactorData(const PlanarSurface& s, double y);

    PlanarSurface planarSurface() const;
    double viewFactor() const;

   private:
    openstudio::model::PlanarSurface m_planarSurface;
    double m_viewFactor;
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::ViewFactorData& vf);

  /** AirflowNetworkDuctViewFactors is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkDuctViewFactors'. */
  class MODEL_API AirflowNetworkDuctViewFactors : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirflowNetworkDuctViewFactors(const Model& model);

    virtual ~AirflowNetworkDuctViewFactors() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDuctViewFactors(const AirflowNetworkDuctViewFactors& other) = default;
    AirflowNetworkDuctViewFactors(AirflowNetworkDuctViewFactors&& other) = default;
    AirflowNetworkDuctViewFactors& operator=(const AirflowNetworkDuctViewFactors&) = default;
    AirflowNetworkDuctViewFactors& operator=(AirflowNetworkDuctViewFactors&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    AirflowNetworkLinkage linkage() const;

    double ductSurfaceExposureFraction() const;

    bool isDuctSurfaceExposureFractionDefaulted() const;

    double ductSurfaceEmittance() const;

    bool isDuctSurfaceEmittanceDefaulted() const;

    boost::optional<double> getViewFactor(const PlanarSurface& surf) const;

    std::vector<ViewFactorData> viewFactors() const;

    //std::unordered_map<PlanarSurface, double> viewFactorMap() const;

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
    /// @cond
    using ImplType = detail::AirflowNetworkDuctViewFactors_Impl;

    explicit AirflowNetworkDuctViewFactors(std::shared_ptr<detail::AirflowNetworkDuctViewFactors_Impl> impl);

    friend class detail::AirflowNetworkDuctViewFactors_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDuctViewFactors");
  };

  /** \relates AirflowNetworkDuctViewFactors*/
  using OptionalAirflowNetworkDuctViewFactors = boost::optional<AirflowNetworkDuctViewFactors>;

  /** \relates AirflowNetworkDuctViewFactors*/
  using AirflowNetworkDuctViewFactorsVector = std::vector<AirflowNetworkDuctViewFactors>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_HPP
