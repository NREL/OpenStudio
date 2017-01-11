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
