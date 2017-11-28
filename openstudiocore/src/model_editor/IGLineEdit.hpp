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
