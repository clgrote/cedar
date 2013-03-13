/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        RobotCard.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 03 04

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/devices/gui/RobotCard.h"
#include "cedar/devices/RobotManager.h"

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDropEvent>
#include <QListWidget>
#include <QModelIndex>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::gui::RobotCard::RobotCard(const QString& robotName)
{
  // associate robot with this card
  this->mRobot = cedar::dev::RobotManagerSingleton::getInstance()->getRobot(robotName.toStdString());

  this->mRobotRemovedConnection
    = cedar::dev::RobotManagerSingleton::getInstance()->connectToRobotRemovedSignal
      (
        boost::bind
        (
          &cedar::dev::gui::RobotCard::robotRemoved,
          this,
          _1
        )
      );

  // build user interface
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  this->setFixedWidth(200);
  this->setFrameShape(QFrame::StyledPanel);

  auto p_outer_layout = new QVBoxLayout();
  p_outer_layout->setContentsMargins(2, 2, 2, 2);

  // header
  auto p_header_layout = new QHBoxLayout();
  p_header_layout->setContentsMargins(2, 2, 2, 2);
  p_outer_layout->addLayout(p_header_layout, 0);
  mpRobotNameEdit = new QLineEdit();
  p_header_layout->addWidget(mpRobotNameEdit);
  mpRobotNameEdit->setText(robotName);
  auto p_recycle_button = new QPushButton("d");
  p_recycle_button->setFixedWidth(24);
  p_recycle_button->setFixedHeight(24);
  p_header_layout->addWidget(p_recycle_button);

  // center
  auto p_drop_area = new cedar::dev::gui::RobotCardIconHolder();
  p_outer_layout->addWidget(p_drop_area, 1);

  // footer
  mpConfigurationSelector = new QComboBox();
  p_outer_layout->addWidget(mpConfigurationSelector, 0);

  this->setLayout(p_outer_layout);

  QObject::connect(p_drop_area, SIGNAL(robotDropped(const QString&)), this, SLOT(robotDropped(const QString&)));
  QObject::connect(mpConfigurationSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedConfigurationChanged(int)));
  QObject::connect(p_recycle_button, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}

cedar::dev::gui::RobotCardIconHolder::RobotCardIconHolder()
{
  this->setAcceptDrops(true);
  this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  this->setBackgroundRole(QPalette::Light);
  this->setMinimumHeight(200);
}

cedar::dev::gui::RobotCard::~RobotCard()
{
  this->mRobotRemovedConnection.disconnect();
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::gui::RobotCard::deleteClicked()
{
  cedar::dev::RobotManagerSingleton::getInstance()->removeRobot(this->getRobotName());
}

void cedar::dev::gui::RobotCard::robotRemoved(const std::string& robotName)
{
  if (robotName == this->getRobotName())
  {
    this->deleteLater();
  }
}

std::string cedar::dev::gui::RobotCard::getRobotName() const
{
  return this->mpRobotNameEdit->text().toStdString();
}

void cedar::dev::gui::RobotCard::selectedConfigurationChanged(int index)
{
  // index 0 is the selection hint
  QString combo_text = this->mpConfigurationSelector->itemText(index);
  if (index == 0 && combo_text == "-- select to instantiate --")
  {
    return;
  }

  auto robot_template
    = cedar::dev::RobotManagerSingleton::getInstance()->getRobotTemplate(this->getRobotTemplateName());

  std::string configuration = robot_template.getConfiguration(combo_text.toStdString());
  cedar::dev::RobotManagerSingleton::getInstance()->loadRobotConfigurationFromResource(this->getRobotName(), configuration);
}

void cedar::dev::gui::RobotCard::robotDropped(const QString& robotName)
{
  auto robot_template = cedar::dev::RobotManagerSingleton::getInstance()->getRobotTemplate(robotName.toStdString());

  this->mRobotTemplateName = robotName.toStdString();

  this->mpConfigurationSelector->clear();

  this->mpConfigurationSelector->addItem("-- select to instantiate --");

  auto configuration_names = robot_template.getConfigurationNames();

  for (size_t i = 0; i < configuration_names.size(); ++i)
  {
    this->mpConfigurationSelector->addItem(QString::fromStdString(configuration_names.at(i)));
  }
}

void cedar::dev::gui::RobotCardIconHolder::setRobotFromTemplate(const std::string& name)
{
  auto robot_template = cedar::dev::RobotManagerSingleton::getInstance()->getRobotTemplate(name);

  QIcon icon(QString::fromStdString(robot_template.getIconPath()));
  this->setPixmap(icon.pixmap(QSize(200, 200)));
}

void cedar::dev::gui::RobotCardIconHolder::dragEnterEvent(QDragEnterEvent* pEvent)
{
  QListWidgetItem *p_item = this->itemFromMime(pEvent);
  if (p_item != NULL)
  {
    pEvent->acceptProposedAction();
  }
}

void cedar::dev::gui::RobotCardIconHolder::dropEvent(QDropEvent* pEvent)
{
  QListWidgetItem *p_item = this->itemFromMime(pEvent);

  if (p_item != NULL)
  {
    pEvent->acceptProposedAction();
    QString robot_name = p_item->data(Qt::UserRole).toString();

    this->setRobotFromTemplate(robot_name.toStdString());

    emit robotDropped(robot_name);
  }
}

QListWidgetItem* cedar::dev::gui::RobotCardIconHolder::itemFromMime(QDropEvent* pEvent)
{
  auto p_source = dynamic_cast<QListWidget*>(pEvent->source());

  if (!p_source)
  {
    return NULL;
  }

  const QMimeData *p_mime_data = pEvent->mimeData();

  if (!p_mime_data)
  {
    return NULL;
  }

  QByteArray item_data = pEvent->mimeData()->data("application/x-qabstractitemmodeldatalist");
  QDataStream stream(&item_data, QIODevice::ReadOnly);

  int r, c;
  QMap<int, QVariant> v;
  stream >> r >> c >> v;

  return p_source->item(r);
}
