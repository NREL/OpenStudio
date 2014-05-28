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

#ifndef OPENSTUDIO_CONSTRUCTIONOBJECTVECTORCONTROLLER_HPP
#define OPENSTUDIO_CONSTRUCTIONOBJECTVECTORCONTROLLER_HPP

#include "ModelObjectVectorController.hpp"

namespace openstudio {

class ConstructionObjectVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  ConstructionObjectVectorController(QWidget * parentWidget);
  
  virtual ~ConstructionObjectVectorController() {}
    
  void setParentWidget(QWidget * parentWidget);

protected:
  
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  
  virtual std::vector<OSItemId> makeVector();
  
  virtual void onRemoveItem(OSItem* item);
  
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  
  virtual void onDrop(const OSItemId& itemId);

private:

  enum LayerType
  {
    FENESTRATION,
    OPAQUE,
    AIRWALL,
    UNKNOWN
  };

  LayerType getLayerType(IddObjectType iddObjectType);
  QWidget * parentWidget();

  QWidget * m_parentWidget;

};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONOBJECTVECTORCONTROLLER_HPP

