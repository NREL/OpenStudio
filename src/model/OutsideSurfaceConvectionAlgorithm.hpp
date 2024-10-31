/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTSIDESURFACECONVECTIONALGORITHM_HPP
#define MODEL_OUTSIDESURFACECONVECTIONALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class OutsideSurfaceConvectionAlgorithm_Impl;
  }  // namespace detail

  /** OutsideSurfaceConvectionAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "SurfaceConvectionAlgorithm:Outside".
 *
 *  OutsideSurfaceConvectionAlgorithm is a unique object that specifies the global model used for surface convection at the outside face of all the
 *  heat transfer surfaces in the model.  This global algorithm may be overridden for specific Surface, SurfaceList, or Zone objects.
 *  OutsideSurfaceConvectionAlgorithm does not have a public constructor because it is a unique ModelObject.
 *  To get the OutsideSurfaceConvectionAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<OutsideSurfaceConvectionAlgorithm>().
 *  To get the OutsideSurfaceConvectionAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<OutsideSurfaceConvectionAlgorithm>().
 */
  class MODEL_API OutsideSurfaceConvectionAlgorithm : public ModelObject
  {
   public:
    virtual ~OutsideSurfaceConvectionAlgorithm() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutsideSurfaceConvectionAlgorithm(const OutsideSurfaceConvectionAlgorithm& other) = default;
    OutsideSurfaceConvectionAlgorithm(OutsideSurfaceConvectionAlgorithm&& other) = default;
    OutsideSurfaceConvectionAlgorithm& operator=(const OutsideSurfaceConvectionAlgorithm&) = default;
    OutsideSurfaceConvectionAlgorithm& operator=(OutsideSurfaceConvectionAlgorithm&&) = default;

    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validAlgorithmValues();

    //@}
    /** @name Getters */
    //@{

    std::string algorithm() const;

    bool isAlgorithmDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAlgorithm(const std::string& algorithm);

    void resetAlgorithm();

    //@}
   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new OutsideSurfaceConvectionAlgorithm object in the model.
    explicit OutsideSurfaceConvectionAlgorithm(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::OutsideSurfaceConvectionAlgorithm_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit OutsideSurfaceConvectionAlgorithm(std::shared_ptr<detail::OutsideSurfaceConvectionAlgorithm_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.OutsideSurfaceConvectionAlgorithm");

    /// @endcond
  };

  /** \relates OutsideSurfaceConvectionAlgorithm */
  using OptionalOutsideSurfaceConvectionAlgorithm = boost::optional<OutsideSurfaceConvectionAlgorithm>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTSIDESURFACECONVECTIONALGORITHM_HPP
