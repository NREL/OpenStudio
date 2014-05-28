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

#ifndef SHAREDGUICOMPONENTS_COMPONENTLIST_HPP
#define SHAREDGUICOMPONENTS_COMPONENTLIST_HPP

#include <QWidget>

class QVBoxLayout;
class QButtonGroup;

namespace openstudio {

class Component;

class ComponentList : public QWidget {

  Q_OBJECT

public:
  ComponentList(QWidget * parent = nullptr);
  ComponentList(const std::vector<Component *> & components,
    QWidget * parent = nullptr);
  virtual ~ComponentList() {}
  // return the checked component in the list, NULL if list is empty
  Component * checkedComponent() const;
  // return the first component in the list, NULL if list is empty
  Component * firstComponent();
  // return the last component in the list, NULL if list is empty
  Component * lastComponent();
  // return all components in the list
  std::vector<Component *> components();
  // add a component
  void addComponent(Component * component);


protected:
  void paintEvent(QPaintEvent * event);

private:
  void createLayout();
  // set the components
  void setComponents(const std::vector<Component *> & components);

  QVBoxLayout * m_mainLayout;
  QButtonGroup * m_componentGroup;

signals:
  void componentClicked(bool checked);

private slots:
  void on_componentClicked(bool);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_COMPONENTLIST_HPP

