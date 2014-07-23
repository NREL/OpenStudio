/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_SPACETYPESVIEW_HPP
#define OPENSTUDIO_SPACETYPESVIEW_HPP

#include "ModelSubTabView.hpp"
#include "ModelObjectInspectorView.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/WorkspaceObject_Impl.hpp"

class QLabel;

namespace openstudio {

class SpaceTypesView : public ModelSubTabView
{
  Q_OBJECT

  public:

    SpaceTypesView(const openstudio::model::Model& model, 
                   QWidget * parent = 0);

    virtual ~SpaceTypesView() {}

};

} // openstudio

#endif // OPENSTUDIO_SPACETYPESVIEW_HPP
