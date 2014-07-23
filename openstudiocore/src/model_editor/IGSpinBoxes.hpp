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

#ifndef MODELEDITOR_IGSPINBOXES_HPP
#define MODELEDITOR_IGSPINBOXES_HPP

#include "ModelEditorAPI.hpp"
#include "InspectorGadget.hpp"

#include <QSpinBox>
#include <QDoubleSpinBox>

class QWheelEvent;



class MODELEDITOR_API IGSpinBox : public QSpinBox
{
  Q_OBJECT;
  
 public:
  IGSpinBox(QWidget* parent);

  virtual void wheelEvent(QWheelEvent* event);

  public slots:
  void triggered(bool);//the radio button got triggered and calls this slot
  
};

class MODELEDITOR_API IGDSpinBox : public QDoubleSpinBox
{
  Q_OBJECT;

  public:
  IGDSpinBox(InspectorGadget* ig,QWidget* parent);

  virtual void wheelEvent(QWheelEvent* event);

  void setDisplay( bool status );

  bool getPrec() const;
  void setPrec( bool );
  void setFloatStyle( InspectorGadget::FLOAT_DISPLAY style );
  InspectorGadget::FLOAT_DISPLAY getFloatStyle() const;
  public slots:
  void triggered(bool);//the radio button got triggered and calls this slot
  void togglePrec();

 private:
  InspectorGadget* m_ig;
  bool m_precision;
  InspectorGadget::FLOAT_DISPLAY m_floatType;

};


#endif // MODELEDITOR_IGSPINBOXES_HPP
