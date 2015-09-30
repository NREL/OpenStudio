/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef OPENSTUDIO_ICONLIBRARY_HPP
#define OPENSTUDIO_ICONLIBRARY_HPP

#include "OpenStudioAPI.hpp"

#include <QPixmap>
#include <map>

namespace openstudio {

/*! This class is a simple singleton that maps IddObjectType.value() to QPixmaps
 *
 *
 * Pretty straightforward. The class stores pointers, there is no real advantage to using pointers over by value semantics
 * since Qt is doing internal reference counting in QPixMap anyway. Just check the pointer when you do a find operation on
 * it, because it might return NULL.
 *
 */
class OPENSTUDIO_API IconLibrary
{

public:
  //! If the class in not instantiated, this call will instantiate it. (So don't ever worry about it, just call Instance when you want to use the class)
  static IconLibrary& Instance();
  /*! gets an Icon
   \param val should be an IddObjectType.value()
   \return either a pointer to the Icon, or NULL if it isn't found
  */
  const QPixmap* findIcon( unsigned int val ) const;
  /*! gets a  MiniIcon
   \param val should be an IddObjectType.value()
   \return either a pointer to the MiniIcon, or NULL if it isn't found
  */
  const QPixmap* findMiniIcon( unsigned int val ) const;

protected:
  
private:
  
  IconLibrary();
  IconLibrary(const IconLibrary&);
  IconLibrary& operator=(const IconLibrary&);
  
  static IconLibrary* s_instance;
  
  std::map<unsigned int, QPixmap*> m_icons;
  std::map<unsigned int, QPixmap*> m_miniIcons;
};

}

#endif // OPENSTUDIO_ICONLIBRARY_HPP
