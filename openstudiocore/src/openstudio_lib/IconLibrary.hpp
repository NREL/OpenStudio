/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
