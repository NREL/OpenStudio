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

#ifndef SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENTLIST_HPP
#define SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENTLIST_HPP

#include "Component.hpp"

class QButtonGroup;
class QHBoxLayout;
class QVBoxLayout;

namespace openstudio {

class Component;
class CollapsibleComponent;
class CollapsibleComponentHeader;

class CollapsibleComponentList : public QWidget
{
  Q_OBJECT

public:
  CollapsibleComponentList(QWidget * parent = nullptr);
  CollapsibleComponentList(const std::vector<CollapsibleComponent *> & collapsibleComponents,
    QWidget * parent = nullptr);
  virtual ~CollapsibleComponentList() {}
  // return the checked component in the list, NULL if list is empty
  Component * checkedComponent() const;
  // return the checked CollapsibleComponent in the list, NULL if list is empty
  CollapsibleComponent * checkedCollapsibleComponent() const;
  // return the first CollapsibleComponent in the list, NULL if list is empty
  CollapsibleComponent * firstCollapsibleComponent();
  // return the last CollapsibleComponent in the list, NULL if list is empty
  CollapsibleComponent * lastCollapsibleComponent();
  // return all collapsibleComponents in the list
  std::vector<CollapsibleComponent *> collapsibleComponents();
  // return all components in the list
  std::vector<Component *> components();
  // add a collapsibleComponent
  void addCollapsibleComponent(CollapsibleComponent * collapsibleComponent);
  // set the title text
  void setText(const QString& text);
  // set the page nav's numeric value displayed of BCL results
  void setNumResults(int numResults);
  // set the page nav's numeric value displayed of BCL pages
  void setNumPages(int numPages);
  ///! set the page nav to display first page (pageIdx = 0)
  void firstPage();

protected:
  void paintEvent( QPaintEvent * event ) override;

private:
  void createLayout();
  // set the CollapsibleComponents
  void setCollapsibleComponents(const std::vector<CollapsibleComponent *> & collapsibleComponents);

  QVBoxLayout * m_mainLayout;
  QHBoxLayout * m_contentLayout;
  QButtonGroup * m_collapsibleComponentGroup;

signals:
  void headerClicked(bool checked);
  void componentClicked(bool checked);
  void collapsibleComponentClicked(bool checked);
  void getComponentsByPage(int pageNum);

private slots:
  void on_headerClicked(bool checked);
  void on_componentClicked(bool checked);
  void on_collapsibleComponentClicked(bool checked);
  void on_getComponentsByPage(int pageIdx);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENTLIST_HPP
