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

#ifndef OPENSTUDIO_OSGRIDVIEW_H
#define OPENSTUDIO_OSGRIDVIEW_H

#include <QAbstractButton>
#include <QWidget>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

class QGridLayout;
class QLabel;
class QString;

namespace openstudio{

class CollapsibleHeader;

class OSGridView : public QWidget
{
  Q_OBJECT

  public:

    OSGridView(std::vector<model::ModelObject> modelObjects, QWidget * parent = 0);

    virtual ~OSGridView() {};

    bool bindComboBox(int row, int column, model::ModelObject mo, std::string property, QString label = "");

    bool bindDouble(int row, int column, model::ModelObject mo, std::string property,std::string ipunits, std::string siunits);

    bool bindInteger(int row, int column, model::ModelObject mo, std::string property,std::string ipunits, std::string siunits);

    bool bindLineEdit(int row, int column, model::ModelObject mo, std::string property,std::string ipunits, std::string siunits);

    bool bindCheckBox(int row, int column, model::ModelObject mo, std::string property, QString label = "");
  
    bool bindUnsigned(int row, int column, model::ModelObject mo, std::string property, QString label = "");

    //void setCategories(std::vector<std::string>);

    //std::vector<std::string> categories();

    //void addWidget(int row, int column);

    //void removeWidget(int row, int column);

  private:

    void OSGridView::refresh(int row, int column);

    void OSGridView::refreshAll();

    CollapsibleHeader * m_collapsibleHeader;

    QWidget * m_body;

    QGridLayout * m_gridLayout;

    std::vector<std::string> m_categories;

    void addComboBoxColumn(std::string property, QString label);

    void addCheckBoxColumn(std::string property, QString label);

    void selectRow(model::ModelObject & modelObject);

    void addRows(std::vector<model::ModelObject> modelObjects);

    void addWidget(int row, int column, QWidget * widget );

    // Caller's job to delete
    void removeWidget( QWidget * widget );

    void selectRow(int row);

    void selectColumn(int row);

  signals:

    void rowClicked(int row);

    void columnClicked(int column);

    void cellClicked(int row, int column);

  private:

    std::vector<model::ModelObject> m_modelObjects;

    // A vector with pair for each column 
    // One item in the pair is property to bind to
    // The other item in the pair is the type of widget to show for that property
    // ComboBox, CheckBox, IntBox, etc
    std::vector< std::pair<std::string,std::string> > m_columnPropertyAndTypes;

    void refresh();

    void refreshRow(model::ModelObject modelObject);

    void refreshColumn(int columnId);

};

class CollapsibleHeader : public QAbstractButton
{
  Q_OBJECT

public:
  CollapsibleHeader(const std::string& text,
    QWidget * parent = 0);
  virtual ~CollapsibleHeader() {}
  void setChecked(bool isChecked);
  void setText(const QString& text);

protected:
  void paintEvent(QPaintEvent * event);
  QSize sizeHint() const;

private:
  void createLayout(const std::string& text);
  void setImage(bool isChecked);

  QLabel * m_text;
  QLabel * m_arrowLabel;
};

} // openstudio

#endif // OPENSTUDIO_OSGRIDVIEW_H

