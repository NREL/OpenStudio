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

#ifndef STANDARDSINTERFACE_TITLE24INTERFACE_HPP
#define STANDARDSINTERFACE_TITLE24INTERFACE_HPP

#include <standardsinterface/StandardsInterfaceAPI.hpp>
#include <standardsinterface/OpenStudioStandardsInterface.hpp>

namespace openstudio {
namespace standardsinterface {

class STANDARDSINTERFACE_API Title24Interface : public OpenStudioStandardsInterface {
 public:
  /** @name Constructors and Destructors*/
  //@{

  /** Construct an instance of the interface from a model. Sets the climate zone institution
   *  to "CEC". */
  Title24Interface(const model::Model& model=model::Model());

  /** Create a Title24Interface by wrapping a disconnected copy of model. */
  Title24Interface clone() const;

  /** Virtual destructor. */
  virtual ~Title24Interface() {}

  //@}

 private:
  REGISTER_LOGGER("standardsinterface.Title24Interface");
};

} // standardsinterface
} // openstudio

#endif // STANDARDSINTERFACE_TITLE24INTERFACE_HPP
