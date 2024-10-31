/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WINDOWDATAFILE_HPP
#define MODEL_WINDOWDATAFILE_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class WindowDataFile_Impl;
  }

  /** WindowDataFile is a ConstructionBase that wraps EnergyPlus IDD object
 *  'Construction:WindowDataFile'. WindowDataFile is an alternative to Construction that isFenestration. */
  class MODEL_API WindowDataFile : public ConstructionBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit WindowDataFile(const Model& model);

    virtual ~WindowDataFile() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WindowDataFile(const WindowDataFile& other) = default;
    WindowDataFile(WindowDataFile&& other) = default;
    WindowDataFile& operator=(const WindowDataFile&) = default;
    WindowDataFile& operator=(WindowDataFile&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
   protected:
    /// @cond
    using ImplType = detail::WindowDataFile_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::WindowDataFile_Impl;

    explicit WindowDataFile(std::shared_ptr<detail::WindowDataFile_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.WindowDataFile");
  };

  /** \relates WindowDataFile */
  using OptionalWindowDataFile = boost::optional<WindowDataFile>;

  /** \relates WindowDataFile */
  using WindowDataFileVector = std::vector<WindowDataFile>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WINDOWDATAFILE_HPP
