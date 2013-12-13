/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_DOCUMENT_DOCUMENTENUMS_HPP
#define UTILITIES_DOCUMENT_DOCUMENTENUMS_HPP

#include <utilities/core/Enum.hpp>

namespace openstudio {

/** \class DocumentFormat
 *  \brief Document output formats supported by OpenStudio.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(DocumentFormat,
          ((COUT))
          ((LaTeX))
          ((XHTML)) );
 *  \endcode */
OPENSTUDIO_ENUM(DocumentFormat,
          ((COUT))
          ((LaTeX))
          ((XHTML)) );

/** \class TableFormat
 *  \brief Table output formats supported by OpenStudio.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(TableFormat,
    ((CSV))
    ((JSON))
);
 *  \endcode */
OPENSTUDIO_ENUM(TableFormat,
    ((CSV))
    ((JSON))
);

} // openstudio

namespace boost { namespace serialization {
  template<class Archive>
    void save(Archive & ar, const openstudio::DocumentFormat& df, unsigned int version)
    {
      int value = df.value();
      ar & boost::serialization::make_nvp("value", value);
    }

  template<class Archive>
    void load(Archive & ar, openstudio::DocumentFormat& df, unsigned int version)
    {
      int value;
      ar & boost::serialization::make_nvp("value", value);
      df.setValue(value);
    }

  template<class Archive>
    inline void serialize(
        Archive & ar,
        openstudio::DocumentFormat& t,
        const unsigned int file_version
        )
    {
      split_free(ar, t, file_version);
    }
}}



#endif // UTILITIES_DOCUMENT_DOCUMENTENUMS_HPP
