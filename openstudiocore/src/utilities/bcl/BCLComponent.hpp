/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_BCL_BCLCOMPONENT_HPP
#define UTILITIES_BCL_BCLCOMPONENT_HPP

#include "../core/Optional.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../data/Attribute.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

// Add more methods for editing components, opening local component constructor

namespace openstudio{

  class UTILITIES_API BCLComponent {

  public:

    /** @name Constructors */
    //@{

    /// Default constructor for a new component
    BCLComponent();

    // DLM: TODO, this should really take an openstudio::path, other parts of this class should be converted to path as well
    /// Constructor for downloaded components, path is to directory containing "component.xml" file
    BCLComponent(const std::string& dir);

    //@}
    /** @name Destructor */
    //@{

    /// Virtual destructor
    virtual ~BCLComponent();

    //@}
    /** @name Getters */
    //@{

    std::string directory() const;

    std::string uid() const;

    std::string versionId() const;

    std::string name() const;

    std::string description() const;

    std::vector<std::string> files() const;

    std::vector<std::string> files(std::string filetype) const;

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

//@}
  private:
    std::string m_directory;
    std::string m_name;
    std::string m_uid;
    std::string m_versionId;
    std::string m_description;
    std::vector<std::string> m_files;
    std::vector<std::string> m_filetypes;
    std::vector<Attribute> m_attributes;
  };

  /** \relates BCLComponent */
  typedef boost::optional<BCLComponent> OptionalBCLComponent;

  /** \relates BCLComponent */
  typedef std::vector<BCLComponent> BCLComponentVector;

} // openstudio

#endif // UTILITIES_BCL_BCLCOMPONENT_HPP
