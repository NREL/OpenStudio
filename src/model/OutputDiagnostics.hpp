/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTDIAGNOSTICS_HPP
#define MODEL_OUTPUTDIAGNOSTICS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputDiagnostics_Impl;

  }  // namespace detail

  /** OutputDiagnostics is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:Diagnostics'. */
  class MODEL_API OutputDiagnostics : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputDiagnostics() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputDiagnostics(const OutputDiagnostics& other) = default;
    OutputDiagnostics(OutputDiagnostics&& other) = default;
    OutputDiagnostics& operator=(const OutputDiagnostics&) = default;
    OutputDiagnostics& operator=(OutputDiagnostics&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> keyValues();
    static std::vector<std::string> validKeyValues();

    /** @name Getters */
    //@{

    std::vector<std::string> keys() const;

    //@}
    /** @name Setters */
    //@{

    // Return false if key isn't valid. If is already present, not added twice (ensures unicity) and logs an info in that case too
    bool addKey(const std::string& key);

    // Calls clearKeys, then for each k, addKey(k)
    bool setKeys(const std::vector<std::string>& keys);

    // Just a convenience function to add 'DisplayExtraWarnings' as a key since it's so common
    bool enableDisplayExtraWarnings();

    // Clears out every key
    void clearKeys();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputDiagnostics_Impl;

    explicit OutputDiagnostics(std::shared_ptr<detail::OutputDiagnostics_Impl> impl);

    friend class detail::OutputDiagnostics_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputDiagnostics(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputDiagnostics");
  };

  /** \relates OutputDiagnostics*/
  using OptionalOutputDiagnostics = boost::optional<OutputDiagnostics>;

  /** \relates OutputDiagnostics*/
  using OutputDiagnosticsVector = std::vector<OutputDiagnostics>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTDIAGNOSTICS_HPP
