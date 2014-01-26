/*======================================================================================================================

    Copyright 2011, 2012 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        Configurable.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 01 16

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/Configurable.h"
#include "cedar/auxiliaries/gui/Parameter.h"
#include "cedar/auxiliaries/TypeBasedFactory.h"
#include "cedar/auxiliaries/utilities.h"
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/Parameter.h"
#include "cedar/auxiliaries/ObjectParameter.h"
#include "cedar/auxiliaries/ObjectListParameter.h"

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QTreeWidgetItemIterator>
#include <QApplication>
#include <QList>
#include <QLabel>

#define PARAMETER_NAME_COLUMN 0
#define PARAMETER_CHANGED_FLAG_COLUMN 1
#define PARAMETER_EDITOR_COLUMN 2


//----------------------------------------------------------------------------------------------------------------------
// private class: DataDelegate
//----------------------------------------------------------------------------------------------------------------------

class cedar::aux::gui::Configurable::DataDelegate : public QStyledItemDelegate
{
public:
  DataDelegate(cedar::aux::ConfigurablePtr pConfigurable, cedar::aux::gui::Configurable* configurableWidget)
  :
  mpConfigurable(pConfigurable),
  mConfigurableWidget(configurableWidget)
  {
  }

  QWidget* createEditor(QWidget *pParent, const QStyleOptionViewItem& /*option*/, const QModelIndex &index) const
  {
    // Get the name of the parameter for which to return the edit widget.
    std::string parameter_path = index.data(Qt::UserRole).toString().toStdString();

    cedar::aux::ParameterPtr parameter = this->mpConfigurable->getParameter(parameter_path);

    QWidget* p_ret = NULL;
    try
    {
      cedar::aux::gui::Parameter *p_widget
        = cedar::aux::gui::ParameterFactorySingleton::getInstance()->get(parameter)->allocateRaw();
      p_widget->setParameter(parameter);
      QObject::connect(p_widget, SIGNAL(heightChanged()), this->mConfigurableWidget, SLOT(fitRowsToContents()), Qt::QueuedConnection);
      p_ret = p_widget;
    }
    catch (cedar::aux::UnknownTypeException& e)
    {
      QLabel* p_no_widget_label = new QLabel("(No widget for type)");
      p_no_widget_label->setToolTip(QString::fromStdString(e.exceptionInfo()));
      p_no_widget_label->setAlignment(Qt::AlignCenter);
      p_no_widget_label->setAutoFillBackground(true);
      p_no_widget_label->setStyleSheet("background-color: rgb(255, 200, 137)");
      p_ret = p_no_widget_label;
    }

    p_ret->setParent(pParent);

    return p_ret;
  }

private:
  cedar::aux::ConfigurablePtr mpConfigurable;

  cedar::aux::gui::Configurable* mConfigurableWidget;
};

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gui::Configurable::Configurable(QWidget* pParent)
:
QWidget(pParent)
{
  // create layout
  QVBoxLayout* p_layout = new QVBoxLayout();
  this->setLayout(p_layout);

  // insert property tree
  this->mpPropertyTree = new QTreeWidget();
  p_layout->addWidget(mpPropertyTree, 1);
  p_layout->setContentsMargins(0, 0, 0, 0);
  this->mpPropertyTree->setAlternatingRowColors(true);

  // setup header
  this->mpPropertyTree->setColumnCount(3);
  QStringList header_labels;
  header_labels << "Property" << "" << "Value";
  this->mpPropertyTree->setHeaderLabels(header_labels);

  // make first section stretch
  this->mpPropertyTree->header()->setResizeMode(PARAMETER_NAME_COLUMN, QHeaderView::Interactive);
  this->mpPropertyTree->header()->setResizeMode(PARAMETER_CHANGED_FLAG_COLUMN, QHeaderView::Fixed);
  this->mpPropertyTree->header()->setResizeMode(PARAMETER_EDITOR_COLUMN, QHeaderView::Stretch);
  this->mpPropertyTree->header()->resizeSection(PARAMETER_NAME_COLUMN, 150);
  this->mpPropertyTree->header()->resizeSection(PARAMETER_CHANGED_FLAG_COLUMN, 20);
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::gui::Configurable::fitRowsToContents()
{
  this->mpPropertyTree->doItemsLayout();
}

QTreeWidgetItem* cedar::aux::gui::Configurable::appendHeading(QTreeWidgetItem* pParent, const QString& text, int hLevel)
{
  QTreeWidgetItem* p_item = new QTreeWidgetItem();
  pParent->addChild(p_item);
  this->makeHeading(p_item, text, hLevel);
  return p_item;
}

void cedar::aux::gui::Configurable::makeHeading(QTreeWidgetItem* pItem, const QString& text, int hLevel)
{
  pItem->setFirstColumnSpanned(true);
  QFont font = pItem->font(0);
  switch (hLevel)
  {
    case 1:
    case 2:
      font.setBold(true);
      font.setPointSize(font.pointSize() + 3 - hLevel);
      pItem->setBackground(0, this->palette().brush(QPalette::Dark));
      pItem->setForeground(0, this->palette().brush(QPalette::Light));
      break;
    default:
      font.setItalic(true);
  }
  pItem->setFont(0, font);

  pItem->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
  pItem->setText(0, text);
}

void cedar::aux::gui::Configurable::display(cedar::aux::ConfigurablePtr configurable)
{
  this->clear();
  
  this->mpPropertyTree->setItemDelegateForColumn(PARAMETER_EDITOR_COLUMN, new cedar::aux::gui::Configurable::DataDelegate(configurable, this));

  std::string type_name = cedar::aux::objectTypeToString(configurable);
  type_name = cedar::aux::replace(type_name, "::", ".");
  auto p_item = this->appendHeading(this->mpPropertyTree->invisibleRootItem(), QString::fromStdString(type_name), 1);
  p_item->setExpanded(true);

  this->append(configurable, p_item, std::string());
}

void cedar::aux::gui::Configurable::append(cedar::aux::ConfigurablePtr configurable, QTreeWidgetItem* pItem, const std::string& pathSoFar)
{
  QTreeWidgetItem* advanced_node = new QTreeWidgetItem();
  // the advanced node needs to be added here already as some properties (e.g., setFirstColumnSpanned) will otherwise not work
  pItem->addChild(advanced_node);
  this->makeHeading(advanced_node, "advanced", 3);
  advanced_node->setExpanded(false);

  for (auto parameter : configurable->getParameters())
  {
    auto parent = pItem;
    if (parameter->isAdvanced())
    {
      parent = advanced_node;
    }
    this->append(parameter, parent, pathSoFar);
  }

  for (auto name_child_pair : configurable->configurableChildren())
  {
    const std::string& child_name = name_child_pair.first;
    cedar::aux::ConfigurablePtr child = name_child_pair.second;

    std::string path = pathSoFar;
    if (!path.empty())
    {
      path += ".";
    }
    path += child_name;

    QTreeWidgetItem* parent_node = pItem;
    if (child->isAdvanced())
    {
      parent_node = advanced_node;
    }

    auto child_item = this->appendHeading(parent_node, QString::fromStdString(child_name), 2);
    this->append(child, child_item, path);
    child_item->setExpanded(true);
  }

  if (advanced_node->childCount() == 0)
  {
    pItem->removeChild(advanced_node);
  }
}

void cedar::aux::gui::Configurable::append(cedar::aux::ParameterPtr parameter, QTreeWidgetItem* pNode, const std::string& pathSoFar)
{
  if (parameter->isHidden())
  {
    return;
  }

  std::string path = pathSoFar;
  if (!pathSoFar.empty())
  {
    path += ".";
  }
  path += parameter->getName();

  QTreeWidgetItem* parameter_item = new QTreeWidgetItem();
  parameter_item->setText(PARAMETER_NAME_COLUMN, QString::fromStdString(parameter->getName()));
  parameter_item->setData(PARAMETER_NAME_COLUMN, Qt::UserRole, QVariant::fromValue(static_cast<void*>(parameter.get())));
  parameter_item->setData(PARAMETER_EDITOR_COLUMN, Qt::UserRole, QString::fromStdString(path));
  pNode->addChild(parameter_item);
  this->mpPropertyTree->openPersistentEditor(parameter_item, PARAMETER_EDITOR_COLUMN);
  this->updateChangeState(parameter_item, parameter.get());

  parameter_item->setTextAlignment(PARAMETER_CHANGED_FLAG_COLUMN, Qt::AlignHCenter | Qt::AlignVCenter);
  QFont changed_font = parameter_item->font(PARAMETER_CHANGED_FLAG_COLUMN);
  changed_font.setBold(true);
  parameter_item->setFont(PARAMETER_CHANGED_FLAG_COLUMN, changed_font);

  QObject::connect(parameter.get(), SIGNAL(changedFlagChanged()), this, SLOT(parameterChangeFlagChanged()));

  // check if parameter is an object parameter
  if (auto object_parameter = boost::dynamic_pointer_cast<cedar::aux::ObjectParameter>(parameter))
  {
    this->append(object_parameter->getConfigurable(), parameter_item, path);
    QObject::connect(object_parameter.get(), SIGNAL(valueChanged()), this, SLOT(objectParameterValueChanged()));
    parameter_item->setExpanded(true);
  }

  // check if parameter is an object list parameter
  if (auto list_parameter = boost::dynamic_pointer_cast<cedar::aux::ObjectListParameter>(parameter))
  {
    QObject::connect(list_parameter.get(), SIGNAL(valueChanged()), this, SLOT(objectListParameterValueChanged()));

    this->appendObjectListParameter(list_parameter, parameter_item, path);
    parameter_item->setExpanded(true);
  }
}

void cedar::aux::gui::Configurable::appendObjectListParameter
(
  cedar::aux::ObjectListParameterPtr objectListParameter,
  QTreeWidgetItem* pParent,
  const std::string& path
)
{
  for (size_t i = 0; i < objectListParameter->size(); ++i)
  {
    cedar::aux::ConfigurablePtr configurable = objectListParameter->configurableAt(i);
    std::string subpath = path + "[" + cedar::aux::toString(i) + "]";
    QString label = QString(QString::fromStdString(objectListParameter->getName()) + " [%1]").arg(i);
    auto sub_item = this->appendHeading(pParent, label, 2);
    this->append(configurable, sub_item, subpath);
    sub_item->setExpanded(true);
  }
}

void cedar::aux::gui::Configurable::objectListParameterValueChanged()
{
  auto p_object_list_parameter = dynamic_cast<cedar::aux::ObjectListParameter*>(QObject::sender());
  CEDAR_DEBUG_ASSERT(p_object_list_parameter != nullptr);

  auto item = this->getItemForParameter(p_object_list_parameter);

  if (item == nullptr)
  {
    cedar::aux::LogSingleton::getInstance()->debugMessage
    (
      "Could not update object parameter \"" + p_object_list_parameter->getName() + "\": corresponding item not found.",
      "void cedar::aux::gui::Configurable::objectListParameterValueChanged()"
    );
    return;
  }

  // iterate in reverse because we erase children, thus changing the index of all following children
  for (int child_index = item->childCount() - 1; child_index >= 0; --child_index)
  {
    auto child_item = item->child(child_index);
    this->disconnect(child_item);
    item->removeChild(child_item);
  }

  this->appendObjectListParameter(p_object_list_parameter, item, this->getPathFromItem(item).toStdString());
}

void cedar::aux::gui::Configurable::objectParameterValueChanged()
{
  auto p_object_parameter = dynamic_cast<cedar::aux::ObjectParameter*>(QObject::sender());
  CEDAR_DEBUG_ASSERT(p_object_parameter != nullptr);

  auto item = this->getItemForParameter(p_object_parameter);

  if (item == nullptr)
  {
    cedar::aux::LogSingleton::getInstance()->debugMessage
    (
      "Could not update object parameter \"" + p_object_parameter->getName() + "\": corresponding item not found.",
      "void cedar::aux::gui::Configurable::parameterChangeFlagChanged()"
    );
    return;
  }

  // iterate in reverse because we erase children, thus changing the index of all following children
  for (int child_index = item->childCount() - 1; child_index >= 0; --child_index)
  {
    auto child_item = item->child(child_index);
    this->disconnect(child_item);
    item->removeChild(child_item);
  }
  
  this->append(p_object_parameter->getConfigurable(), item, this->getPathFromItem(item).toStdString());
}

QString cedar::aux::gui::Configurable::getPathFromItem(QTreeWidgetItem* item)
{
  return item->data(PARAMETER_EDITOR_COLUMN, Qt::UserRole).toString();
}

void cedar::aux::gui::Configurable::disconnect(QTreeWidgetItem* item)
{
  auto parameter = static_cast<cedar::aux::Parameter*>(item->data(PARAMETER_NAME_COLUMN, Qt::UserRole).value<void*>());
  if (parameter != nullptr)
  {
    QObject::disconnect(parameter, SIGNAL(changedFlagChanged()), this, SLOT(parameterChangeFlagChanged()));

    if (auto object_parameter = dynamic_cast<cedar::aux::ObjectParameter*>(parameter))
    {
      QObject::disconnect(object_parameter, SIGNAL(valueChanged()), this, SLOT(objectParameterValueChanged()));
    }

    if (auto list_parameter = dynamic_cast<cedar::aux::ObjectListParameter*>(parameter))
    {
      QObject::disconnect(list_parameter, SIGNAL(valueChanged()), this, SLOT(objectListParameterValueChanged()));
    }
  }
}

void cedar::aux::gui::Configurable::clear()
{
  for (QTreeWidgetItemIterator iter(this->mpPropertyTree); *iter != nullptr; ++iter)
  {
    this->disconnect(*iter);
  }

  this->mpPropertyTree->clear();
  QAbstractItemDelegate * p_delegate = this->mpPropertyTree->itemDelegateForColumn(PARAMETER_EDITOR_COLUMN);
  if (p_delegate != NULL)
  {
    delete p_delegate;
  }
}

void cedar::aux::gui::Configurable::parameterChangeFlagChanged()
{
  cedar::aux::Parameter *p_parameter = dynamic_cast<cedar::aux::Parameter*>(QObject::sender());
  CEDAR_DEBUG_ASSERT(p_parameter != nullptr);

  auto item = this->getItemForParameter(p_parameter);

  if (item != nullptr)
  {
    this->updateChangeState(item, p_parameter);
  }
  else
  {
    cedar::aux::LogSingleton::getInstance()->debugMessage
    (
      "Could not update changed state of parameter \"" + p_parameter->getName() + "\": corresponding item not found.",
      "void cedar::aux::gui::Configurable::parameterChangeFlagChanged()"
    );
  }
}

QTreeWidgetItem* cedar::aux::gui::Configurable::getItemForParameter(cedar::aux::Parameter* parameter)
{
  CEDAR_DEBUG_ASSERT(parameter != nullptr);

  for (QTreeWidgetItemIterator iter(this->mpPropertyTree); *iter != nullptr; ++iter)
  {
    auto item = *iter;
    if (item->data(PARAMETER_NAME_COLUMN, Qt::UserRole).value<void*>() == static_cast<void*>(parameter))
    {
      return item;
    }
  }

  return nullptr;
}

void cedar::aux::gui::Configurable::updateChangeState(QTreeWidgetItem* item, cedar::aux::Parameter* pParameter)
{
  if (pParameter->isChanged())
  {
    item->setText(PARAMETER_CHANGED_FLAG_COLUMN, "*");
  }
  else
  {
    item->setText(PARAMETER_CHANGED_FLAG_COLUMN, "");
  }
}