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

#include "IGSpinBoxes.hpp"

#include "../utilities/core/Assert.hpp"

#include <QWheelEvent>
#include <string>

using std::string;


IGSpinBox::IGSpinBox(QWidget* parent):
  QSpinBox(parent)
{
}
void IGSpinBox::wheelEvent(QWheelEvent* event)
{
  event->ignore();
}




void IGSpinBox::triggered(bool trig)
{
  if( trig)
  {
    emit valueChanged( QString("%1").arg(value()) );
  }
  else
  {
    emit valueChanged( "AutoSize");
  }
}




IGDSpinBox::IGDSpinBox(InspectorGadget* ig, QWidget* parent):
  QDoubleSpinBox(parent),
  m_ig(ig),
  m_precision(false),
  m_floatType( InspectorGadget::UNFORMATED )
{
}
void IGDSpinBox::wheelEvent(QWheelEvent* event)
{
  event->ignore();
}

void IGDSpinBox::setDisplay(bool status)
{
  m_precision=!status;
  togglePrec();
}

bool IGDSpinBox::getPrec() const
{
  return m_precision;
}

void IGDSpinBox::setPrec( bool prec )
{
  m_precision = prec;
}

void IGDSpinBox::setFloatStyle( InspectorGadget::FLOAT_DISPLAY style )
{
  m_floatType = style;
}

InspectorGadget::FLOAT_DISPLAY IGDSpinBox::getFloatStyle() const
{
  return m_floatType;
}



void IGDSpinBox::triggered(bool trig)
{
  if( trig)
  {
    emit valueChanged( QString("%1").arg(value()) );
  }
  else
  {
    emit valueChanged( "AutoSize");
  }
}

void IGDSpinBox::togglePrec()
{
  QVariant v = property(InspectorGadget::s_indexSlotName);
  int index = v.toInt();

  string val = *(m_ig->m_workspaceObj->getString(index));

  try
  {
    double dResult = boost::lexical_cast<double>(val);
    if( m_precision ) {
      m_precision = false;
      setValue(dResult);
      setStyleSheet("color:black");
    }
    else  {
      m_precision = true;

      if( m_floatType == InspectorGadget::SCIENTIFIC ) {
        setDecimals( m_ig->m_precision );
      }
      else if ( m_floatType == InspectorGadget::FIXED ) {
        setDecimals( m_ig->m_precision );
      }
      else {
        setValue(dResult);
        setStyleSheet("color:black");
        return;//don't allow the next bit to change the text to aqua, return instead.
      }
      setValue(dResult);
      setStyleSheet("color:green");
    }
  }
  catch(...)
  {
    OS_ASSERT(false);
  }
}
