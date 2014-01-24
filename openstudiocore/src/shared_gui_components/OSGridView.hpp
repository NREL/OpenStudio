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

#include "OSGridController.hpp"

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

class QGridLayout;
class QLabel;
class QString;

namespace openstudio{

class OSCollapsibleView;

class OSGridView : public QWidget
{
  Q_OBJECT

  public:

    OSGridView(IddObjectType iddObjectType, const model::Model & model, QWidget * parent = 0); 

    OSGridView(std::vector<model::ModelObject> modelObjects, QWidget * parent = 0);

    virtual ~OSGridView() {}; 
     
    void setGridController(OSGridController * gridController);

    OSGridController * gridController() const;

    void setCategories(std::vector<QString> categories);

    std::vector<QString> categories();

    void setCategoryFields(const QString & category, std::vector<QString> fields);

    std::vector<QString> categoryFields(const QString & category);

    //void addComboBoxColumn(std::string property, QString label);

    //void addCheckBoxColumn(std::string property, QString label);

    //void addRows(std::vector<model::ModelObject> modelObjects);

    //bool bindComboBox(int row, int column, model::ModelObject mo, std::string property, QString label = "");

    //bool bindDouble(int row, int column, model::ModelObject mo, std::string property,std::string ipunits, std::string siunits);

    //bool bindInteger(int row, int column, model::ModelObject mo, std::string property,std::string ipunits, std::string siunits);

    //bool bindLineEdit(int row, int column, model::ModelObject mo, std::string property,std::string ipunits, std::string siunits);

    //bool bindCheckBox(int row, int column, model::ModelObject mo, std::string property, QString label = "");
  
    //bool bindUnsigned(int row, int column, model::ModelObject mo, std::string property, QString label = "");

  signals:

    void cellClicked(int row, int column);

    void rowClicked(int row);

    void columnClicked(int column);

  private slots:

    void refresh(int row, int column);

    void refreshAll(); 

    void addWidget(int row, int column);

    void removeWidget(int row, int column);
    
    void setVerticalHeader(bool visible, QString title);

    void setHorizontalHeader(std::vector<QWidget *> widgets);

    void setHorizontalHeader(std::vector<QString> names);

    void selectCategory(int category);

  private:

    void refreshRow(model::ModelObject modelObject);

    void refreshColumn(int columnId);

    void selectCell(int row, int column);

    void selectRow(int row);

    void selectColumn(int column);

    OSCollapsibleView * m_CollapsibleView;

    QGridLayout * m_gridLayout;

    OSGridController * m_gridController;

    std::vector<model::ModelObject> m_modelObjects;

    model::Model m_model;

    std::map<QString,std::vector<QString>> m_categoriesAndFields;

};

} // openstudio

#endif // OPENSTUDIO_OSGRIDVIEW_H

