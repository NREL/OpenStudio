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

#ifndef MODELEDITOR_IGLINEEDIT_HPP
#define MODELEDITOR_IGLINEEDIT_HPP

#include "ModelEditorAPI.hpp"
#include "InspectorGadget.hpp"

#include <QLineEdit>



class  MODELEDITOR_API IGLineEdit : public QLineEdit
{
  Q_OBJECT;
  Q_PROPERTY( bool precision READ getPrec WRITE setPrec );
  

 public:
  IGLineEdit(const QString& val, InspectorGadget* ig, QWidget* parent);
  virtual ~IGLineEdit();

  void setDisplay(bool status);

  void setMin(double min);
  void setMax(double max);

  bool getPrec() const;
  void setPrec( bool );
  void setStyle( InspectorGadget::FLOAT_DISPLAY style );
  InspectorGadget::FLOAT_DISPLAY getStyle() const;

  bool checkValue(QString& value);

  QString doubleToQString( double v );
  //QString intToQString( int v );

 public slots:
  void togglePrec();

  void editDone();

  void hardsizeClicked(bool checked);

 signals:

  void newValue(const QString& text);

 private:
  InspectorGadget* m_ig;
  bool m_precision;
  InspectorGadget::FLOAT_DISPLAY m_floatType;

  double m_min;
  double m_max;

};
  

#endif // MODELEDITOR_IGLINEEDIT_HPP
