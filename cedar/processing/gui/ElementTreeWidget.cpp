/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        ElementTreeWidget.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2014 10 21

    Description: Source file for the class cedar::proc::gui::ElementTreeWidget.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/gui/ElementTreeWidget.h"
#include "cedar/processing/gui/Group.h"
#include "cedar/auxiliaries/casts.h"

// SYSTEM INCLUDES
#include <boost/bind.hpp>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::ElementTreeWidget::ElementTreeWidget(QWidget* pParent)
:
QTreeWidget(pParent),
mNameColumn(0)
{
  QObject::connect(this, SIGNAL(elementAddedSignal(QString)), this, SLOT(elementAdded(QString)));
  QObject::connect(this, SIGNAL(elementRemovedSignal(QString)), this, SLOT(elementRemoved(QString)));
}

cedar::proc::gui::ElementTreeWidget::~ElementTreeWidget()
{
  this->disconnectSignals();
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::gui::ElementTreeWidget::setFilter(const boost::function<bool(cedar::proc::ConstElementPtr)>& filter)
{
  this->mFilter = filter;
}

void cedar::proc::gui::ElementTreeWidget::setGroup(cedar::proc::GroupPtr group)
{
  this->mGroup = group;

  this->disconnectSignals();

  this->clear();
  this->mElementNames.clear();

  this->addGroup(this->mGroup);
}

void cedar::proc::gui::ElementTreeWidget::addGroup(cedar::proc::GroupPtr group)
{
  for (const auto& name_element_pair : group->getElements())
  {
    this->translateElementAddedSignal(name_element_pair.second);
  }

  this->mElementAddedConnections.push_back
  (
    group->connectToNewElementAddedSignal
    (
      boost::bind(&cedar::proc::gui::ElementTreeWidget::translateElementAddedSignal, this, _1)
    )
  );

  this->mElementRemovedConnections.push_back
  (
    group->connectToElementRemovedSignal
    (
      boost::bind(&cedar::proc::gui::ElementTreeWidget::translateElementRemovedSignal, this, _1)
    )
  );
}

void cedar::proc::gui::ElementTreeWidget::addElement(cedar::proc::ElementPtr element)
{
  std::string path = this->mGroup->findPath(element);
  this->mElementNames[element.get()] = QString::fromStdString(path);

  this->connectRenameSignal(element);

  auto p_item = new QTreeWidgetItem();
  p_item->setText(this->mNameColumn, QString::fromStdString(element->getName()));
  p_item->setData(this->mNameColumn, Qt::UserRole, QString::fromStdString(path));
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

  // this retrieves the group item (or creates it if it does not exist)
  auto group_item = this->getGroupItem(path);
  group_item->addChild(p_item);

  this->signalElementAdded(p_item, element);
}

void cedar::proc::gui::ElementTreeWidget::disconnectSignals()
{
  for (auto connection : this->mElementAddedConnections)
  {
    connection.disconnect();
  }
  for (auto connection : this->mElementRemovedConnections)
  {
    connection.disconnect();
  }
}

void cedar::proc::gui::ElementTreeWidget::translateElementAddedSignal(cedar::proc::ElementPtr element)
{
  std::string path = this->mGroup->findPath(element);
  emit elementAddedSignal(QString::fromStdString(path));
}

void cedar::proc::gui::ElementTreeWidget::elementAdded(QString elementName)
{
  auto element = this->mGroup->getElement(elementName.toStdString());

  if (auto group = boost::dynamic_pointer_cast<cedar::proc::Group>(element))
  {
    std::string path = this->mGroup->findPath(group);

    this->connectRenameSignal(group);

    this->addGroup(group);
  }
  else
  {
    if (!this->mFilter || this->mFilter(element))
    {
      this->addElement(element);
    }
  }
}

void cedar::proc::gui::ElementTreeWidget::connectRenameSignal(cedar::proc::ElementPtr element)
{
  cedar::aux::ParameterPtr name = element->getParameter("name");
  QObject::connect(name.get(), SIGNAL(valueChanged()), this, SLOT(elementNameChanged()));
}

void cedar::proc::gui::ElementTreeWidget::elementNameChanged()
{
  auto name = cedar::aux::asserted_cast<cedar::aux::Parameter*>(QObject::sender());
  auto element = dynamic_cast<cedar::proc::Element*>(name->getOwner());

  auto iter = this->mElementNames.find(element);
  if (iter == this->mElementNames.end())
  {
    return; // can happen if the element is being deleted
  }

  QString old_name = iter->second;
  QString new_name = QString::fromStdString(this->mGroup->findPath(element));

  auto items = this->findItems(old_name, 0, this->mNameColumn);

  for (int i = 0; i < items.size(); ++i)
  {
    items.at(i)->setText(this->mNameColumn, new_name);
  }

  this->mElementNames[element] = new_name;
}

void cedar::proc::gui::ElementTreeWidget::translateElementRemovedSignal(cedar::proc::ConstElementPtr element)
{
  auto iter = this->mElementNames.find(element.get());
  if (iter != this->mElementNames.end())
  {
    QString name = iter->second;
    this->mElementNames.erase(iter);

    emit elementRemovedSignal(name);
  }
}

void cedar::proc::gui::ElementTreeWidget::elementRemoved(QString elementName)
{
  auto items = this->findItems(elementName, 0, this->mNameColumn);

  QTreeWidgetItemIterator it(this);
  while (*it)
  {
    QTreeWidgetItem* p_item = *it;
    if (p_item->data(this->mNameColumn, Qt::UserRole).toString() == elementName)
    {
      delete p_item;
      return;
    }
    ++it;
  }
}

QTreeWidgetItem* cedar::proc::gui::ElementTreeWidget::getGroupItem(const std::string& elementPath)
{
  std::vector<std::string> subpaths;
  cedar::aux::split(elementPath, ".", subpaths);

  CEDAR_ASSERT(!subpaths.empty());

  subpaths.pop_back();

  QTreeWidgetItem* p_last = this->invisibleRootItem();
  for (const auto& subpath : subpaths)
  {
    QTreeWidgetItem* p_item = nullptr;
    for (int i = 0; i < p_last->childCount(); ++i)
    {
      auto child = p_last->child(i);
      if (child->text(this->mNameColumn).toStdString() == subpath)
      {
        p_item = child;
      }
    }
    if (p_item == nullptr)
    {
      p_item = new QTreeWidgetItem();
      p_item->setText(this->mNameColumn, QString::fromStdString(subpath));
      p_last->addChild(p_item);
      p_item->setExpanded(true);
    }
    p_last = p_item;
  }

  return p_last;
}
