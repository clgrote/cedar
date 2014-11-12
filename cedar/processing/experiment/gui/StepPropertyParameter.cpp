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

    File:        StepPropertyParameter.cpp

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.rub.de
    Date:        2014 03 13

    Description: Source file for the class cedar::proc::experiment::gui::StepPropertyParameter.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/experiment/gui/StepPropertyParameter.h"
#include "cedar/processing/experiment/StepPropertyParameter.h"
#include "cedar/processing/experiment/Supervisor.h"
#include "cedar/auxiliaries/TypeBasedFactory.h"
#include "cedar/auxiliaries/Singleton.h"
#include "cedar/auxiliaries/gui/Parameter.h"

// SYSTEM INCLUDES
#include <QFormLayout>
#include <QLabel>


//----------------------------------------------------------------------------------------------------------------------
// associate aux::gui parameter with the aux parameter
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registered = cedar::aux::gui::ParameterFactorySingleton::getInstance()->add
      <
        cedar::proc::experiment::StepPropertyParameter,
        cedar::proc::experiment::gui::StepPropertyParameter
      >();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::experiment::gui::StepPropertyParameter::StepPropertyParameter(QWidget *pParent)
:
cedar::aux::gui::Parameter(pParent),
mpStep(new QComboBox()),
mpProperty(new QComboBox()),
mpPropertyCopy(nullptr)
{
  this->mpStep->setEditable(true);
  this->mpStep->setInsertPolicy(QComboBox::NoInsert);

  this->mpProperty->setEditable(true);
  this->mpProperty->setInsertPolicy(QComboBox::NoInsert);

  auto layout = new QFormLayout();
  layout->setMargin(0);
  this->setLayout(layout);
  layout->addRow(new QLabel("Step"), mpStep);
  layout->addRow(new QLabel("Property"), mpProperty);
  layout->addRow(new QLabel("Value"), new QLabel("(select an element and parameter first)"));
  QObject::connect(this, SIGNAL(parameterPointerChanged()), this, SLOT(parameterPointerChanged()));
}

cedar::proc::experiment::gui::StepPropertyParameter::~StepPropertyParameter()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------


void cedar::proc::experiment::gui::StepPropertyParameter::parameterPointerChanged()
{
  updateSteps();
  cedar::proc::experiment::StepPropertyParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::proc::experiment::StepPropertyParameter>(this->getParameter());
  if (parameter && parameter->getStep())
  {
    this->mpStep->setCurrentIndex(this->mpStep->findText((parameter->getStep()->getName().c_str())));
  }
  stepChanged();
  updateProperties();
  this->mpProperty->setCurrentIndex(this->mpProperty->findText((parameter->getProperty().c_str())));
  updateValue();
  connect(this->mpStep, SIGNAL(currentIndexChanged(int)), this, SLOT(stepChanged()));
  connect(this->mpProperty, SIGNAL(currentIndexChanged(int)), this, SLOT(propertyChanged()));
}

void cedar::proc::experiment::gui::StepPropertyParameter::stepChanged()
{
  QString text = mpStep->currentText();
  cedar::proc::experiment::StepPropertyParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::proc::experiment::StepPropertyParameter>(this->getParameter());

  parameter->setStep(text.toStdString());

  updateProperties();
}

void cedar::proc::experiment::gui::StepPropertyParameter::propertyChanged()
{

  QString text = mpProperty->currentText();
  cedar::proc::experiment::StepPropertyParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::proc::experiment::StepPropertyParameter>(this->getParameter());
  parameter->setProperty(text.toStdString());

  updateValue();
}

void cedar::proc::experiment::gui::StepPropertyParameter::updateSteps()
{
  mpStep->clear();
  std::vector<std::string> steps = SupervisorSingleton::getInstance()->getExperiment()->getGroupSteps();
  for (std::string step : steps)
  {
    mpStep->addItem(QString::fromStdString(step));
  }
}

void cedar::proc::experiment::gui::StepPropertyParameter::updateProperties()
{
  std::string index = mpStep->currentText().toStdString();
  if (index == "")
     return;

  cedar::proc::experiment::StepPropertyParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::proc::experiment::StepPropertyParameter>(this->getParameter());

  std::vector<std::string> properties;
  switch (parameter->getType())
  {
    case cedar::proc::experiment::StepPropertyParameter::PARAMETER:
    {
      properties = parameter->getListOfParameters();
      break;
    }
    case cedar::proc::experiment::StepPropertyParameter::OUTPUT:
    {
      properties = parameter->getListOfData(cedar::proc::DataRole::OUTPUT);
      break;
    }
    case cedar::proc::experiment::StepPropertyParameter::BUFFER:
    {
      properties = parameter->getListOfData(cedar::proc::DataRole::BUFFER);
      break;
    }
  }

  mpProperty->clear();
  for (std::string property : properties)
  {
    mpProperty->addItem(QString::fromStdString(property));
  }

}

void cedar::proc::experiment::gui::StepPropertyParameter::updateValue()
{
  cedar::proc::experiment::StepPropertyParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::proc::experiment::StepPropertyParameter>(this->getParameter());

  switch(parameter->getType())
  {
    case cedar::proc::experiment::StepPropertyParameter::PARAMETER:
    {
      auto layout = cedar::aux::asserted_cast<QFormLayout*>(this->layout());
      cedar::aux::ParameterPtr parameter_copy = parameter->getParameterCopy();

      if (!parameter_copy)
      {
        return;
      }
      if (this->mpPropertyCopy)
      {
        delete this->mpPropertyCopy;
        this->mpPropertyCopy = nullptr;
      }
      else
      {
        delete layout->itemAt(2, QFormLayout::FieldRole)->widget();
      }
      cedar::aux::gui::Parameter* parameter_widget =
          cedar::aux::gui::ParameterFactorySingleton::getInstance()->get(parameter_copy)->allocateRaw();
//      parameter_widget->setParent(this);
      parameter_widget->setParameter(parameter_copy);
      mpPropertyCopy = parameter_widget;
      layout->setWidget(2, QFormLayout::FieldRole, mpPropertyCopy);
      break;
    }
    case cedar::proc::experiment::StepPropertyParameter::OUTPUT:
    {
      break;
    }
    case cedar::proc::experiment::StepPropertyParameter::BUFFER:
    {
      break;
    }
  }
}

