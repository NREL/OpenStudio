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
#ifndef MODELEDITOR_BRIDGECLASSES_HPP
#define MODELEDITOR_BRIDGECLASSES_HPP

#include "ModelEditorAPI.hpp"

#include <QObject>

class MODELEDITOR_API ComboHighlightBridge : public QObject
{

  Q_OBJECT ;

 public:

  ComboHighlightBridge(QObject* parent);

  QObject* bridgeSender();

 public slots:
  void highlighted(const QString& val);
  void activated(const QString& val);
 signals:
  void highlightEmitted( const QString& val);
  void activatedEmitted(const QString& val);

 private:
  QObject* m_source;//original source object

};



#endif // MODELEDITOR_BRIDGECLASSES_HPP
