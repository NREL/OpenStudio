/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_BCL_BCLCOMPONENT_HPP
#define UTILITIES_BCL_BCLCOMPONENT_HPP

#include "../core/Optional.hpp"
#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../data/Attribute.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

// Add more methods for editing components, opening local component constructor

namespace openstudio {

class UTILITIES_API BCLComponent
{

 public:
  /** @name Constructors */
  //@{

  /// Default constructor for a new component
  BCLComponent();

  /// Constructor for downloaded components, path is to directory containing "component.xml" file
  BCLComponent(const openstudio::path& dir);

  // Note JM 2018-11-14: SHOULD BE REMOVED
  // DLM: TODO, this should really take an openstudio::path, other parts of this class should be converted to path as well
  /// Constructor for downloaded components, path is to directory containing "component.xml" file
  // BCLComponent(const std::string& dir);

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~BCLComponent() = default;

  //@}
  /** @name Getters */
  //@{

  openstudio::path directory() const;

  std::string uid() const;

  std::string versionId() const;

  std::string name() const;

  std::string description() const;

  // TODO: JM 2018-11-14
  // These should probably be a std::vector<openstudio::path> too
  std::vector<std::string> files() const;

  std::vector<std::string> files(const std::string& filetype) const;

  std::vector<std::string> filetypes() const;

  std::vector<Attribute> attributes() const;

  //@}
  /** @name Setters */
  //@{

  void setName(const std::string& name);

  //@}
  /** @name Operators */
  //@{

  /// Equality test - Compares the uids and versionIds of two separate components
  bool operator==(const BCLComponent& other) const;

  /// Negation of ==.
  bool operator!=(const BCLComponent& other) const;

  /// Less than
  bool operator<(const BCLComponent& other) const;

  //@}
 private:
  REGISTER_LOGGER("openstudio.BCLComponent");

  openstudio::path m_directory;
  std::string m_name;
  std::string m_uid;
  std::string m_versionId;
  std::string m_description;
  std::vector<std::string> m_files;
  std::vector<std::string> m_filetypes;
  std::vector<Attribute> m_attributes;
};

/** \relates BCLComponent */
using OptionalBCLComponent = boost::optional<BCLComponent>;

/** \relates BCLComponent */
using BCLComponentVector = std::vector<BCLComponent>;

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLCOMPONENT_HPP
