/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PROGRAMCONTROL_HPP
#define MODEL_PROGRAMCONTROL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ProgramControl_Impl;

  }  // namespace detail

  /** ProgramControl is a ModelObject that wraps the OpenStudio IDD object 'OS:ProgramControl'. */
  class MODEL_API ProgramControl : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ProgramControl(const Model& model);

    virtual ~ProgramControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ProgramControl(const ProgramControl& other) = default;
    ProgramControl(ProgramControl&& other) = default;
    ProgramControl& operator=(const ProgramControl&) = default;
    ProgramControl& operator=(ProgramControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<int> numberofThreadsAllowed() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberofThreadsAllowed(int numberofThreadsAllowed);

    void resetNumberofThreadsAllowed();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ProgramControl_Impl;

    explicit ProgramControl(std::shared_ptr<detail::ProgramControl_Impl> impl);

    friend class detail::ProgramControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ProgramControl");
  };

  /** \relates ProgramControl*/
  using OptionalProgramControl = boost::optional<ProgramControl>;

  /** \relates ProgramControl*/
  using ProgramControlVector = std::vector<ProgramControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PROGRAMCONTROL_HPP
