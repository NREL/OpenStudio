/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <utilities/bcl/BCL.hpp>
#include <utilities/data/Attribute.hpp>

#include <QAbstractButton>

class QCheckBox;
class QLabel;

namespace openstudio {

class BCLSearchResult;

class Component : public QAbstractButton
{
  Q_OBJECT

public:
  Component(const BCLSearchResult & bclSearchResult,
    bool showAbridgedView = true,
    bool showCheckBox = true,
    QWidget * parent = 0);
  Component(bool showAbridgedView = false,
    bool showCheckBox = false,
    QWidget * parent = 0);
  Component(const Component & other);
  virtual ~Component();
  Component & operator=(const Component & other);
  QCheckBox * checkBox();
  QLabel * msg();
  std::string uid() const;
  std::string versionId() const;
  void setChecked(bool checked);
  
protected:
  void paintEvent(QPaintEvent * event);

private:
  void createAbridgedLayout();
  void createCompleteLayout();
  void parseBCLSearchResult(const BCLSearchResult & bclSearchResult);

  QString m_componentType;
  QString m_name;
  QString m_uid;
  QString m_versionId;
  QString m_description;
  QString m_fidelityLevel;
  std::vector<Attribute> m_attributes;
  std::vector<BCLFile> m_files;
  std::vector<BCLProvenance> m_provenances;
  std::vector<std::string> m_tags;
  bool m_showAbridgedView;
  bool m_showCheckBox;
  bool m_available;
  bool m_updateAvailable;
  QCheckBox* m_checkBox;
  QLabel* m_msg;

signals:

private slots:

};

} // namespace openstudio

#endif // COMPONENT_H
