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
#include "IGLineEdit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include <sstream>
#include <string>
#include <limits>

#include <QWidget>
#include <QString>
#include <QVariant>
#include <QDoubleValidator>
#include <QIntValidator>

using std::ios;
using std::string;
using std::stringstream;

using namespace openstudio;

IGLineEdit::IGLineEdit(const QString& val, InspectorGadget* ig, QWidget* parent):
  QLineEdit(val,parent),
  m_ig(ig),
  m_precision(false),
  m_floatType(InspectorGadget::UNFORMATED),
  m_min(-std::numeric_limits<double>::max( )),
  m_max(std::numeric_limits<double>::max( ))
{

}

IGLineEdit::~IGLineEdit()
{
}

void IGLineEdit::setDisplay( bool status )
{
  m_precision=!status;
  togglePrec();
}

void IGLineEdit::setMin(double min)
{
  m_min=min;
}
void IGLineEdit::setMax(double max)
{
  m_max=max;
}

bool IGLineEdit::getPrec() const
{
  return m_precision;
}

void IGLineEdit::setPrec(bool prec )
{
  m_precision=prec;
}

void IGLineEdit::setStyle( InspectorGadget::FLOAT_DISPLAY style )
{
  m_floatType = style;
}

InspectorGadget::FLOAT_DISPLAY IGLineEdit::getStyle() const
{
  return m_floatType;
}


bool IGLineEdit::checkValue(QString& txt)
{
  const QValidator* v = validator();
  QString textVal = text();
  if( !v || !(textVal.compare("autosize",Qt::CaseInsensitive)) )
  {
    txt=textVal;
    return true;
  }

  const QRegExpValidator* dv = qobject_cast<const QRegExpValidator*>(v);
  if( dv )
  {
    bool ok;
    double val = textVal.toDouble(&ok);
    if( !ok ) val = 0.0;
    if( val < m_min )
    {
      val = m_min;
    }
    else if( val > m_max )
    {
      val = m_max;
    }
    txt = doubleToQString( val );
    int dummy=0;
    bool state = (dv->validate(txt,dummy) != QValidator::Invalid);
    OS_ASSERT(state);
    return state;
  }
  else if( const QIntValidator* iv = qobject_cast<const QIntValidator*>(v) )
  {
    int min = iv->bottom();
    int max = iv->top();
    bool ok;
    int val = textVal.toInt(&ok);
    if(!ok) val = 0;
    if( val < min )
    {
      val = min;
    }
    else if( val>max)
    {
      val = max;
    }
    txt = QString::number(val);
    int dummy=0;
    bool state = (iv->validate(txt,dummy) != QValidator::Invalid);
    OS_ASSERT(state);
    return state;
  }

  // shouldn't get here
  OS_ASSERT(false);
  return false;

}

QString IGLineEdit::doubleToQString( double v )
{
  if( m_floatType == InspectorGadget::SCIENTIFIC )
    {
      stringstream convert;
      convert.precision(m_ig->m_precision-1);
      convert.setf(ios::scientific,ios::floatfield);
      convert<<v;
      return QString::fromStdString( convert.str() );
    }
    else if ( m_floatType == InspectorGadget::FIXED )
    {
      stringstream convert;
      convert.precision(m_ig->m_precision);
      convert.setf(ios::fixed,ios::floatfield);
      convert<<v;
      return QString::fromStdString( convert.str() );
    }
    else
    {
      return QString::number(v,'g',14);
    }

}



void IGLineEdit::togglePrec()
{
  QVariant v = property(InspectorGadget::s_indexSlotName);
  int index = v.toInt();

  OptionalString s = m_ig->m_workspaceObj->getString(index);
  if(!s) return;
  string val = *s;

  QString temp = QString::fromStdString( val );
  if( m_precision )
  {
    m_precision = false;
    setText(temp);
    setStyleSheet("color:black");
  }
  else
  {
    m_precision = true;
    if( m_floatType == InspectorGadget::UNFORMATED )
    {
      setText(temp);
      setStyleSheet("color:black");
      return;
    }

    if( checkValue(temp) )
      setText(temp);
    setStyleSheet("color:green");
  }

}

void IGLineEdit::editDone()
{
  QString val;
  if( checkValue(val) )
  {
    setText(val); // emits textChanged but not textEdited 
    emit newValue(val);
  }
}


void IGLineEdit::hardsizeClicked(bool checked)
{
  if (!checked){
    return;
  }

  QString val;
  if( !checkValue(val) )
  {
    // do not allow empty string when hard sizing
    val = "0";
  }

  setText(val);
  emit textEdited(val);
}
