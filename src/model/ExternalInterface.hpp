/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACE_HPP
#define MODEL_EXTERNALINTERFACE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ExternalInterface_Impl;

  }  // namespace detail

  /** ExternalInterface is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface'. */
  class MODEL_API ExternalInterface : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ExternalInterface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterface(const ExternalInterface& other) = default;
    ExternalInterface(ExternalInterface&& other) = default;
    ExternalInterface& operator=(const ExternalInterface&) = default;
    ExternalInterface& operator=(ExternalInterface&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> nameofExternalInterfaceValues();

    /** @name Getters */
    //@{

    std::string nameofExternalInterface() const;

    //@}
    /** @name Setters */
    //@{

    bool setNameofExternalInterface(const std::string& nameofExternalInterface);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterface_Impl;

    explicit ExternalInterface(std::shared_ptr<detail::ExternalInterface_Impl> impl);

    friend class detail::ExternalInterface_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit ExternalInterface(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterface");
  };

  /** \relates ExternalInterface*/
  using OptionalExternalInterface = boost::optional<ExternalInterface>;

  /** \relates ExternalInterface*/
  using ExternalInterfaceVector = std::vector<ExternalInterface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACE_HPP
