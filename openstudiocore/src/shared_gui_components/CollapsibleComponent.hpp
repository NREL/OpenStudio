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

#ifndef SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENT_HPP
#define SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENT_HPP

#include <QAbstractButton>

#include "Component.hpp"

class QVBoxLayout;

namespace openstudio {

class CollapsibleComponentHeader;
class ComponentList;

class CollapsibleComponent : public QAbstractButton
{
  Q_OBJECT

public:
  CollapsibleComponent(CollapsibleComponentHeader * collapsibleComponentHeader,
    ComponentList * componentList,
    QWidget * parent = nullptr);
  virtual ~CollapsibleComponent() {}
  bool expanded() const;
  void setExpanded(bool expanded);
  CollapsibleComponentHeader * collapsibleComponentHeader() const;
  ComponentList * componentList() const;
  void setText(const QString& text);
  void setNumResults(int numResults);
  void setNumPages(int numPages);
  void firstPage();

protected:
  void paintEvent(QPaintEvent * event) override;

private:
  void createLayout();

  CollapsibleComponentHeader * m_collapsibleComponentHeader;
  ComponentList * m_componentList;
  QVBoxLayout * m_mainLayout;

signals:
  void headerClicked(bool);
  void componentClicked(bool);
  void getComponentsByPage(int pageNum);

private slots:
  void on_headerClicked(bool checked);
  void on_componentClicked(bool checked);
  void on_getComponentsByPage(int pageIdx);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENT_HPP
