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

#include "StartupView.hpp"
#include "OpenStudioApp.hpp"
#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>
#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QListView>
#include <QTextEdit>
#include <QMessageBox>
#include <QPainterPath>
#include <QPolygon>
#include <QPolygonF>

namespace openstudio {

StartupView::StartupView( QWidget * parent ) 
  : QWidget( parent )
{
  m_templateListModel = std::shared_ptr<TemplateListModel>( new TemplateListModel() );

  setStyleSheet("openstudio--StartupView { background: #E6E6E6; }");
  
#ifdef Q_OS_MAC
  setWindowFlags(Qt::FramelessWindowHint);
#else
  setWindowFlags(Qt::CustomizeWindowHint);
#endif

  auto recentProjectsView = new QWidget();
  recentProjectsView->setStyleSheet("QWidget { background: #F2F2F2; }");
  auto recentProjectsLayout = new QVBoxLayout();
  recentProjectsLayout->setContentsMargins(10,10,10,10);
  QLabel * recentProjectsLabel = new QLabel("Recent Projects");
  recentProjectsLabel->setStyleSheet("QLabel { font: bold }");
  recentProjectsLayout->addWidget(recentProjectsLabel,0,Qt::AlignTop);
  recentProjectsView->setLayout(recentProjectsLayout);

  auto openButton = new QToolButton();
  openButton->setText("Open File");
  openButton->setStyleSheet("QToolButton { font: bold; }");
  openButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  QIcon openIcon(":/images/open_file.png");
  openButton->setIcon(openIcon);
  openButton->setIconSize(QSize(40,40));
  connect( openButton, SIGNAL(clicked()), this, SIGNAL(openClicked()) );

  auto importButton = new QToolButton();
  importButton->setText("Import Idf");
  importButton->setStyleSheet("QToolButton { font: bold; }");
  importButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  QIcon importIcon(":/images/import_file.png");
  importButton->setIcon(importIcon);
  importButton->setIconSize(QSize(40,40));
  connect( importButton, SIGNAL(clicked()), this, SIGNAL(importClicked()) );
/*  
  QToolButton * importSDDButton = new QToolButton();
  importSDDButton->setText("Import SDD");
  importSDDButton->setStyleSheet("QToolButton { font: bold; }");
  importSDDButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  QIcon importSDDIcon(":/images/import_file.png");
  importSDDButton->setIcon(importSDDIcon);
  importSDDButton->setIconSize(QSize(40,40));
  connect( importSDDButton, SIGNAL(clicked()), this, SIGNAL(importSDDClicked()) );
*/
  auto projectChooserView = new QWidget();
  projectChooserView->setFixedWidth(238);
  projectChooserView->setStyleSheet("QWidget { background: #F2F2F2; }");
  auto projectChooserLayout = new QVBoxLayout();
  projectChooserLayout->setContentsMargins(10,10,10,10);
  QLabel * projectChooserLabel = new QLabel("Create New From Template");
  projectChooserLabel->setStyleSheet("QLabel { font: bold }");
  projectChooserLayout->addWidget(projectChooserLabel,0,Qt::AlignTop);
  m_listView = new QListView();
  m_listView->setViewMode(QListView::IconMode);
  m_listView->setModel(m_templateListModel.get());
  m_listView->setFocusPolicy(Qt::NoFocus);
  m_listView->setFlow(QListView::LeftToRight);
  m_listView->setUniformItemSizes(true);
  m_listView->setSelectionMode(QAbstractItemView::SingleSelection);
  projectChooserLayout->addWidget(m_listView);
  projectChooserView->setLayout(projectChooserLayout);

  m_projectDetailView = new QWidget();
  m_projectDetailView->setStyleSheet("QWidget { background: #F2F2F2; }");
  auto projectDetailLayout = new QVBoxLayout();
  projectDetailLayout->setContentsMargins(10,10,10,10);
  m_projectDetailView->setLayout(projectDetailLayout);

  auto footerView = new QWidget();
  footerView->setObjectName("FooterView");
  footerView->setStyleSheet("QWidget#FooterView { background: #E6E6E6; }");
  footerView->setMaximumHeight(50);
  footerView->setMinimumHeight(50);

  auto cancelButton = new QPushButton();
  cancelButton->setObjectName("StandardGrayButton");
  cancelButton->setMinimumSize(QSize(99,28));
  #ifdef OPENSTUDIO_PLUGIN
    cancelButton->setText("Cancel");
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(hide()) );
  #else
    #ifdef Q_OS_MAC
      cancelButton->setText("Quit");
    #else
      cancelButton->setText("Exit");
    #endif
    connect( cancelButton, SIGNAL(clicked()), OpenStudioApp::instance(), SLOT(quit()) );
  #endif
  cancelButton->setStyleSheet("QPushButton { font: bold; }");

  auto chooseButton = new QPushButton();
  chooseButton->setObjectName("StandardBlueButton");
  chooseButton->setText("Choose");
  chooseButton->setMinimumSize(QSize(99,28));
  connect( chooseButton, SIGNAL(clicked()), this, SLOT(newFromTemplateSlot()) );
  chooseButton->setStyleSheet("QPushButton { font: bold; }");

  auto hFooterLayout = new QHBoxLayout();
  hFooterLayout->setSpacing(25);
  hFooterLayout->setContentsMargins(0,0,0,0);
  hFooterLayout->addStretch();
  hFooterLayout->addWidget(cancelButton);
  hFooterLayout->addWidget(chooseButton);
  footerView->setLayout(hFooterLayout);

  auto hLayout = new QHBoxLayout();
  auto vLayout = new QVBoxLayout();

  auto vOpenLayout = new QVBoxLayout();
  vOpenLayout->addWidget(recentProjectsView);
  vOpenLayout->addWidget(openButton);
  vOpenLayout->addWidget(importButton);
  //vOpenLayout->addWidget(importSDDButton);

  hLayout->addLayout(vOpenLayout);
  hLayout->addWidget(projectChooserView);
  hLayout->addWidget(m_projectDetailView,1);

  vLayout->addSpacing(50);
  vLayout->addLayout(hLayout);
  vLayout->addWidget(footerView);

  setLayout(vLayout);

  connect(m_listView,SIGNAL(clicked( const QModelIndex &)),this,SLOT( showDetailsForItem( const QModelIndex & ) ));

  m_listView->setCurrentIndex(m_templateListModel->index(0,0));
  showDetailsForItem(m_templateListModel->index(0,0));
}

void StartupView::resizeEvent(QResizeEvent * event)
{
#ifdef Q_OS_MAC
  QPainterPath path;
  path.addRoundedRect(rect(),9.0,9.0);
  QPolygon p = path.toFillPolygon().toPolygon();
  QRegion region(p);
  setMask(region);
#endif
}

void StartupView::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);

  QImage leftHeader = QImage(":/images/image_header.png");
  painter.drawImage(0,0,leftHeader);  

  QImage centerHeader = QImage(":/images/header-backgnd-1px-wide.png");

  int i;
  for( i = leftHeader.width();
       i < width();
       i++ )
  {
    painter.drawImage(i,0,centerHeader);  
  }
}

QSize StartupView::sizeHint() const
{
  return QSize(800,500);
}

void StartupView::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) 
  {
    if( event->y() < 50 )
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
        _move = true;
    }
    else
    {
      _move = false;
    }
  }
}

void StartupView::mouseMoveEvent(QMouseEvent *event)
{
  if(event->buttons() & Qt::LeftButton) 
  {
    if( _move )
    {
      move(event->globalPos() - dragPosition);
      event->accept();
    }
  }
}

void StartupView::newFromTemplateSlot()
{
  switch(m_listView->currentIndex().row()) 
  {
    case NEWFROMTEMPLATE_EMPTY:
    {
      emit newFromTemplate( NEWFROMTEMPLATE_EMPTY );
      break;
    }
    case NEWFROMTEMPLATE_WIZARD:
    {
      QMessageBox message(this);
      message.setText("Sorry the wizard option is not available yet.");
      message.exec();
      break;
    }
  }
}

void StartupView::showDetailsForItem( const QModelIndex & index )
{
  QLayout * layout = m_projectDetailView->layout();

  for( int i = 0; i < layout->count(); i++ )
  {
    delete layout->itemAt(i)->widget();
    layout->removeItem(layout->itemAt(i));
  }

  QString name = m_templateListModel->data(index,Qt::ToolTipRole).toString();

  QString description = m_templateListModel->data(index,Qt::WhatsThisRole).toString();

  if( ! name.isEmpty() )
  {
    auto nameLabel = new QLabel(name);

    nameLabel->setStyleSheet("QLabel { font: bold }");

    layout->addWidget(nameLabel);
  }

  if( ! description.isEmpty() )
  {
    auto descriptionLabel = new QTextEdit(description);
    
    descriptionLabel->setStyleSheet("QTextEdit { border: none; }");
    
    descriptionLabel->setReadOnly(true);

    layout->addWidget(descriptionLabel);
  }

}

TemplateListModel::TemplateListModel(QObject * parent)
  : QAbstractListModel(parent)
{
}

int TemplateListModel::rowCount( const QModelIndex & parent ) const
{
  return 2;
}

QVariant TemplateListModel::data( const QModelIndex & index, int role ) const
{
  if(!index.isValid())
  {
    return QVariant();
  }
  else if( role == Qt::DecorationRole )
  {
    switch( index.row() )
    {
      case NEWFROMTEMPLATE_EMPTY:
      {
        return QPixmap(":/images/osm-icon-open.png").scaled(85,85,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      }
      case NEWFROMTEMPLATE_WIZARD:
      {
        return QPixmap(":/images/wizard.png").scaled(85,85,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      }
      default:
      {
        return QVariant();
      }
    }
  }
  else if( role == Qt::SizeHintRole )
  {
    return QSize(100,100);
  }
  else if( role == Qt::ToolTipRole ) 
  {
    switch( index.row() )
    {
      case NEWFROMTEMPLATE_EMPTY:
      {
        return QString("Empty Building Model");
      }
      case NEWFROMTEMPLATE_WIZARD:
      {
        return QString("Building Model From Wizard");
      }
      default:
      {
        return QVariant();
      }
    }
  }
  else if( role == Qt::WhatsThisRole ) 
  {
    switch( index.row() )
    {
      case NEWFROMTEMPLATE_EMPTY:
      {
        return QString("Create a new empty building model.  Start here if you want to add your own Zones and HVAC Systems.");
      }
      case NEWFROMTEMPLATE_WIZARD:
      {
        return QString("Create a new building model using a wizard.  \
This option will create a complete building model through a series of basic inputs.");
      }
      default:
      {
        return QVariant();
      }
    }
  }
  else
  {
    return QVariant();
  }
}

Qt::ItemFlags TemplateListModel::flags(const QModelIndex &index) const
{
  return (Qt::ItemIsEnabled | Qt::ItemIsSelectable );
}

} // openstudio

