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

#ifndef SHAREDGUICOMPONENTS_OSCOLLAPSIBLEVIEW_HPP
#define SHAREDGUICOMPONENTS_OSCOLLAPSIBLEVIEW_HPP

#include <QWidget>
#include <QPointer>
#include <QPushButton>

namespace openstudio {

class OSViewSwitcher;

// OSHeader is an empty QWidget that must be subclassed to show something interesting.
// The purpose of this class is to ensure that there is a toggle button to support OSCollapsibleView. 
// It is the responsibility of the subclass to insert the toggle button into a layout.
class OSHeader : public QPushButton
{
  Q_OBJECT

  public:

  OSHeader(QPushButton * button);

  virtual ~OSHeader() {}

  QPushButton * toggleButton;

  protected:

  void paintEvent(QPaintEvent *) override;
};

class OSCollapsibleView : public QWidget
{
  Q_OBJECT

  public:

  OSCollapsibleView(bool alwaysExpanded = false, QWidget * parent=nullptr);

  virtual ~OSCollapsibleView() {}

  void setHeader(QWidget * header);

  void setContent(QWidget * content);

  public slots:

  void expand();

  void collapse();

  void setExpanded(bool expanded);

  bool isExpanded();

  private:

  QPointer<OSHeader> m_osHeader;
  
  OSViewSwitcher * m_headerContainer;

  OSViewSwitcher * m_contentContainer;

  bool m_alwaysExpanded;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSCOLLAPSIBLEVIEW_HPP

