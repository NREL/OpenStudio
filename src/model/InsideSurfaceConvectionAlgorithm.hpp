/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP
#define MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class InsideSurfaceConvectionAlgorithm_Impl;
  }  // namespace detail

  /** InsideSurfaceConvectionAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "SurfaceConvectionAlgorithm:Inside".
 *
 *  InsideSurfaceConvectionAlgorithm is a unique object that specifies the global model used for surface convection at the inside face of all the
 *  heat transfer surfaces in the model.  This global algorithm may be overridden for specific Surface, SurfaceList, or Zone objects.
 *  InsideSurfaceConvectionAlgorithm does not have a public constructor because it is a unique ModelObject.
 *  To get the InsideSurfaceConvectionAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<InsideSurfaceConvectionAlgorithm>().
 *  To get the InsideSurfaceConvectionAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<InsideSurfaceConvectionAlgorithm>().
 */
  class MODEL_API InsideSurfaceConvectionAlgorithm : public ModelObject
  {
   public:
    virtual ~InsideSurfaceConvectionAlgorithm() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InsideSurfaceConvectionAlgorithm(const InsideSurfaceConvectionAlgorithm& other) = default;
    InsideSurfaceConvectionAlgorithm(InsideSurfaceConvectionAlgorithm&& other) = default;
    InsideSurfaceConvectionAlgorithm& operator=(const InsideSurfaceConvectionAlgorithm&) = default;
    InsideSurfaceConvectionAlgorithm& operator=(InsideSurfaceConvectionAlgorithm&&) = default;

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

    /// Constructs a new InsideSurfaceConvectionAlgorithm object in the model.
    explicit InsideSurfaceConvectionAlgorithm(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::InsideSurfaceConvectionAlgorithm_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit InsideSurfaceConvectionAlgorithm(std::shared_ptr<detail::InsideSurfaceConvectionAlgorithm_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.InsideSurfaceConvectionAlgorithm");

    /// @endcond
  };

  /** \relates InsideSurfaceConvectionAlgorithm */
  using OptionalInsideSurfaceConvectionAlgorithm = boost::optional<InsideSurfaceConvectionAlgorithm>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP
