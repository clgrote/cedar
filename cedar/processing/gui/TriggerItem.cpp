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

    File:        StepItem.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 11

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/processing/gui/TriggerItem.h"
#include "cedar/processing/gui/StepItem.h"
#include "cedar/processing/gui/Scene.h"
#include "cedar/processing/gui/Settings.h"
#include "cedar/processing/gui/exceptions.h"
#include "cedar/processing/LoopedTrigger.h"
#include "cedar/processing/Manager.h"
#include "cedar/processing/Trigger.h"
#include "cedar/processing/ElementDeclaration.h"
#include "cedar/processing/DeclarationRegistry.h"
#include "cedar/auxiliaries/Data.h"
#include "cedar/auxiliaries/utilities.h"
#include "cedar/auxiliaries/CallFunctionInThread.h"
#include "cedar/auxiliaries/Singleton.h"
#include "cedar/auxiliaries/sleepFunctions.h"
#include "cedar/auxiliaries/Log.h"
#include "cedar/auxiliaries/casts.h"
#include "cedar/units/Time.h"
#include "cedar/units/prefixes.h"

// SYSTEM INCLUDES
#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <iostream>
#include <string>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::TriggerItem::TriggerItem()
:
cedar::proc::gui::GraphicsBase(30, 30,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_STEP
                               | cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER,
                               cedar::proc::gui::GraphicsBase::BASE_SHAPE_ROUND
                               )
{
  cedar::aux::LogSingleton::getInstance()->allocating(this);
  this->construct();
}


cedar::proc::gui::TriggerItem::TriggerItem(cedar::proc::TriggerPtr trigger)
:
cedar::proc::gui::GraphicsBase(30, 30,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_STEP
                               | cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER,
                               cedar::proc::gui::GraphicsBase::BASE_SHAPE_ROUND
                               )
{
  cedar::aux::LogSingleton::getInstance()->allocating(this);
  this->setTrigger(trigger);
  this->construct();
}

void cedar::proc::gui::TriggerItem::construct()
{
  this->setFlags(this->flags() | QGraphicsItem::ItemIsSelectable
                               | QGraphicsItem::ItemIsMovable
                               | QGraphicsItem::ItemSendsGeometryChanges
                               );

  if (cedar::proc::gui::SettingsSingleton::getInstance()->useGraphicsItemShadowEffects())
  {
    QGraphicsDropShadowEffect *p_effect = new QGraphicsDropShadowEffect();
    p_effect->setBlurRadius(5.0);
    p_effect->setOffset(3.0, 3.0);
    this->setGraphicsEffect(p_effect);
  }
}

cedar::proc::gui::TriggerItem::~TriggerItem()
{
  cedar::aux::LogSingleton::getInstance()->freeing(this);

  if (this->scene())
  {
    cedar::aux::asserted_cast<cedar::proc::gui::Scene*>(this->scene())->removeTriggerItem(this);
  }
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::gui::TriggerItem::disconnect(cedar::proc::gui::GraphicsBase* /*pListener*/)
{
}

void cedar::proc::gui::TriggerItem::isDocked(bool docked)
{
  if (docked)
  {
    this->setFlags(static_cast<QGraphicsItem::GraphicsItemFlags>(this->flags() - QGraphicsItem::ItemIsSelectable
                                                                               - QGraphicsItem::ItemIsMovable)
                                 );
  }
  else
  {
    this->setFlags(this->flags() | QGraphicsItem::ItemIsSelectable
                                 | QGraphicsItem::ItemIsMovable
                                 );
  }
}

cedar::proc::gui::ConnectValidity cedar::proc::gui::TriggerItem::canConnectTo(GraphicsBase* pTarget) const
{
  // a trigger cannot connect to its parent (e.g., the step that owns it)
  if (pTarget == this->parentItem())
  {
    return cedar::proc::gui::CONNECT_NO;
  }

  if (this->cedar::proc::gui::GraphicsBase::canConnectTo(pTarget) == cedar::proc::gui::CONNECT_NO)
  {
    return cedar::proc::gui::CONNECT_NO;
  }

  if (cedar::proc::gui::StepItem *p_step_item = dynamic_cast<cedar::proc::gui::StepItem*>(pTarget))
  {
    if(p_step_item->getStep()->getParentTrigger() || this->mTrigger->isListener(p_step_item->getStep()))
    {
      return cedar::proc::gui::CONNECT_NO;
    }
    // ... source and target are not in the same network
    else if (this->getTrigger()->getNetwork() != p_step_item->getStep()->getNetwork())
    {
      return cedar::proc::gui::CONNECT_NO;
    }
  }

  if (cedar::proc::gui::TriggerItem *p_trigger_item = dynamic_cast<cedar::proc::gui::TriggerItem*>(pTarget))
  {
    // a trigger cannot be connected to a trigger if the target trigger is owned by a step (i.e., has a parent item) or
    // if it is already a listener of the target
    if
    (
      dynamic_cast<StepItem*>(p_trigger_item->parentItem()) != NULL
        || this->mTrigger->isListener(p_trigger_item->getTrigger())
    )
    {
      return cedar::proc::gui::CONNECT_NO;
    }
    // ... source and target are not in the same network
    else if (this->getTrigger()->getNetwork() != p_trigger_item->getTrigger()->getNetwork())
    {
      return cedar::proc::gui::CONNECT_NO;
    }
  }

  return cedar::proc::gui::CONNECT_YES;
}

void cedar::proc::gui::TriggerItem::setTrigger(cedar::proc::TriggerPtr trigger)
{
  this->setElement(trigger);
  this->mTrigger = trigger;
  this->mClassId = cedar::proc::ElementManagerSingleton::getInstance()->getDeclarationOf(mTrigger);
  
  std::string tool_tip = this->mTrigger->getName() + " (" + this->mClassId->getClassName() + ")";
  this->setToolTip(tool_tip.c_str());

  if (auto looped_trigger = boost::dynamic_pointer_cast<cedar::proc::LoopedTrigger>(this->mTrigger))
  {
    QObject::connect(looped_trigger.get(), SIGNAL(triggerStarting()), this, SLOT(triggerStateChanging()));
    QObject::connect(looped_trigger.get(), SIGNAL(triggerStopping()), this, SLOT(triggerStateChanging()));
    QObject::connect(looped_trigger.get(), SIGNAL(triggerStarted()), this, SLOT(triggerStarted()));
    QObject::connect(looped_trigger.get(), SIGNAL(triggerStopped()), this, SLOT(triggerStopped()));

    this->mStartCaller = cedar::aux::CallFunctionInThreadPtr
        (
          new cedar::aux::CallFunctionInThread
          (
            boost::bind(&cedar::proc::LoopedTrigger::start, looped_trigger)
          )
        );

    this->mStopCaller = cedar::aux::CallFunctionInThreadPtr
        (
          new cedar::aux::CallFunctionInThread
          (
            boost::bind(&cedar::proc::LoopedTrigger::stop, looped_trigger, UINT_MAX, false)
          )
        );
  }
}

void cedar::proc::gui::TriggerItem::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
  this->cedar::proc::gui::GraphicsBase::readConfiguration(node);
}

void cedar::proc::gui::TriggerItem::writeConfiguration(cedar::aux::ConfigurationNode& root) const
{
  root.put("trigger", this->mTrigger->getName());
  this->cedar::proc::gui::GraphicsBase::writeConfiguration(root);
}

void cedar::proc::gui::TriggerItem::triggerStateChanging()
{
  this->setFillColor(mValidityColorWarning);
}

void cedar::proc::gui::TriggerItem::triggerStarted()
{
  this->setFillColor(mValidityColorValid);
}

void cedar::proc::gui::TriggerItem::triggerStopped()
{
  this->setFillColor(mDefaultFillColor);
}

void cedar::proc::gui::TriggerItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  cedar::proc::gui::Scene *p_scene = dynamic_cast<cedar::proc::gui::Scene*>(this->scene());
  CEDAR_DEBUG_ASSERT(p_scene);

  if (auto looped_trigger = boost::dynamic_pointer_cast<cedar::proc::LoopedTrigger>(this->mTrigger))
  {
    QMenu menu;
    QAction *p_start = menu.addAction("start");
    QAction *p_stop = menu.addAction("stop");
    QAction *p_single = menu.addAction("single step");

    menu.addSeparator();
    p_scene->networkGroupingContextMenuEvent(menu);

    if (looped_trigger->isRunningNolocking())
    {
      p_start->setEnabled(false);
      p_single->setEnabled(false);
    }
    else
    {
      p_stop->setEnabled(false);
    }

    QAction *a = menu.exec(event->screenPos());

    if (a == p_start)
    {
      CEDAR_DEBUG_ASSERT(this->mStartCaller);

      // calls looped_trigger->start()
      this->mStartCaller->start();
    }
    else if (a == p_stop)
    {
      CEDAR_DEBUG_ASSERT(this->mStopCaller);

      // calls looped_trigger->stop()
      this->mStopCaller->start();
    }
    else if (a == p_single)
    {
      looped_trigger->step(looped_trigger->getSimulatedTimeParameter());
    }
  }
  else
  {
    QMenu menu;
    p_scene->networkGroupingContextMenuEvent(menu);
    menu.exec(event->screenPos());
  }
}

void cedar::proc::gui::TriggerItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
  painter->save(); // save current painter settings

  this->paintFrame(painter, style, widget);
  
  painter->restore(); // restore saved painter settings
}

cedar::proc::TriggerPtr cedar::proc::gui::TriggerItem::getTrigger()
{
  return this->mTrigger;
}

cedar::proc::ConstTriggerPtr cedar::proc::gui::TriggerItem::getTrigger() const
{
  return this->mTrigger;
}

cedar::proc::gui::Connection* cedar::proc::gui::TriggerItem::connectTo(cedar::proc::gui::GraphicsBase *pTarget)
{
  return new Connection(this, pTarget);
}
