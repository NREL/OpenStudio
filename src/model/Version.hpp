/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_VERSION_HPP
#define MODEL_VERSION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {
  namespace detail {
    class Version_Impl;
  }

  class MODEL_API Version : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //{

    virtual ~Version() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Version(const Version& other) = default;
    Version(Version&& other) = default;
    Version& operator=(const Version&) = default;
    Version& operator=(Version&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //{

    std::string versionIdentifier() const;

    boost::optional<std::string> prereleaseIdentifier() const;

    //@}
   protected:
    /** @name Setters */
    //@{

    bool setVersionIdentifier(const std::string& s);
    bool setPrereleaseIdentifier(const std::string& s);

    //@}

    // constructor
    explicit Version(const Model& model);

    using ImplType = detail::Version_Impl;

    friend class detail::Model_Impl;
    friend class openstudio::IdfObject;
    friend class Model;

    // constructor
    explicit Version(std::shared_ptr<detail::Version_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.RunPeriod");
  };

  /** \relates Version */
  using OptionalVersion = boost::optional<Version>;

}  // namespace model
}  // namespace openstudio

#endif
