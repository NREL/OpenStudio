/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_CLOUD_OSSERVER_HPP
#define UTILITIES_CLOUD_OSSERVER_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <QObject>

namespace openstudio{

  /// OSServer is a class for accessing the rails server started on machines provided by a CloudProvider.
  class UTILITIES_API OSServer : public QObject {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor
    OSServer();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OSServer();

    //@}
    /** @name Class members */
    //@{

    //@}
  private:

  };

} // openstudio

#endif // UTILITIES_BCL_LOCALBCL_HPP
