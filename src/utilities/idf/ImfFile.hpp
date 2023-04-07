/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_IMFFILE_HPP
#define UTILITIES_IDF_IMFFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "IdfObject.hpp"

#include "../idd/IddObject.hpp"
#include "../idd/IddFileAndFactoryWrapper.hpp"

#include "../core/Path.hpp"
#include "../core/Containers.hpp"

#include <string>
#include <ostream>
#include <vector>
#include <map>

namespace openstudio {

/** ImfFile represents an Imf file. ImfFile does not have any copy
*   or assignment operators so it is not a shared object.  ImfFile
*   does not support advanced imf syntax, only sectioning by
*   \code
*   ##def Schedules[]
*   \endcode
*   , etc.
*/
class UTILITIES_API ImfFile
{
 public:
  /** @name Constructors */
  //@{

  /// default constructor
  ImfFile(IddFileType iddFileType = IddFileType("EnergyPlus"));

  //@}
  /** @name Getters */
  //@{

  /** Get the IddFile. If one is not explicitly held by this object, returns one from
   *  IddFactory per the documentation for this class. */
  IddFile iddFile() const;

  /** Get the IddFile's IddFileType. */
  IddFileType iddFileType() const;

  /** Return the names of all sections. */
  StringVector sectionNames() const;

  /** Get a section by name */
  IdfObjectVector section(const std::string& section) const;

  //@}
  /** @name Queries */
  //@{

  /** Return the number of sections in the ImfFile */
  unsigned numSections() const;

  //@}
  /** @name Serialization */
  //@{

  /// load file from std::istream. assumes objects are in sub-idd file fileType of IddFactory.
  static boost::optional<ImfFile> load(std::istream& is, IddFileType iddFileType);

  /// load file from std::istream. use user-supplied IddFile.
  static boost::optional<ImfFile> load(std::istream& is, const IddFile& iddFile);

  /// load from file path
  static boost::optional<ImfFile> load(const openstudio::path& p, IddFileType iddFileType);

  /// load from file path
  static boost::optional<ImfFile> load(const openstudio::path& p, const IddFile& iddFile);

  /// print file to std::stream
  std::ostream& print(std::ostream& os) const;

  /** Save IddFile to path. Will construct parent folder, but no further up the chain. Will
   *  only overwrite an existing file if overwrite==true. If no extension is provided will use
   *  'imf'. */
  // cppcheck-suppress functionConst
  bool save(const openstudio::path& p, bool overwrite = false);

  //@}

 private:
  // store all idf objects in a map by section
  using SectionMapType = std::map<std::string, IdfObjectVector>;
  SectionMapType m_sectionMap;
  IddFileAndFactoryWrapper m_iddFileAndFactoryWrapper;

  bool m_load(std::istream& is);

  // configure logging
  REGISTER_LOGGER("utilities.idf.ImfFile");
};

/// optional imf file
using OptionalImfFile = boost::optional<ImfFile>;

/// vector of imf file
using ImfFileVector = std::vector<ImfFile>;

// ostream operator<<
UTILITIES_API std::ostream& operator<<(std::ostream& os, const ImfFile& imfFile);

}  // namespace openstudio

#endif  //UTILITIES_IDF_IMFFILE_HPP
