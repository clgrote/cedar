/*======================================================================================================================

    Copyright 2011 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

// LOCAL INCLUDES
#include "cedar/processing/gui/TriggerItem.h"
#include "cedar/processing/gui/StepItem.h"
#include "cedar/processing/gui/exceptions.h"
#include "cedar/processing/LoopedTrigger.h"
#include "cedar/processing/Manager.h"
#include "cedar/auxiliaries/Data.h"
#include "cedar/auxiliaries/utilities.h"
#include "cedar/processing/Trigger.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QPainter>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <iostream>

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
  this->setTrigger(trigger);
  this->construct();
}

void cedar::proc::gui::TriggerItem::construct()
{
  this->setFlags(this->flags() | QGraphicsItem::ItemIsSelectable
                               | QGraphicsItem::ItemIsMovable
                               | QGraphicsItem::ItemSendsGeometryChanges
                               );

  QGraphicsDropShadowEffect *p_effect = new QGraphicsDropShadowEffect();
  p_effect->setBlurRadius(5.0);
  p_effect->setOffset(3.0, 3.0);
  this->setGraphicsEffect(p_effect);
}

cedar::proc::gui::TriggerItem::~TriggerItem()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::gui::TriggerItem::disconnect(cedar::proc::gui::GraphicsBase* pListener)
{
  switch (pListener->getGroup())
  {
    case cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_STEP:
    {
      cedar::proc::gui::StepItem *p_step = cedar::aux::asserted_cast<cedar::proc::gui::StepItem*>(pListener);
      CEDAR_DEBUG_ASSERT(this->getTrigger()->isListener(p_step->getStep()));
      this->getTrigger()->removeListener(p_step->getStep());
      break;
    }

    case cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER:
    {
      cedar::proc::gui::TriggerItem *p_trigger = cedar::aux::asserted_cast<cedar::proc::gui::TriggerItem*>(pListener);
      CEDAR_DEBUG_ASSERT(this->getTrigger()->isListener(p_trigger->getTrigger()));
      this->getTrigger()->removeTrigger(p_trigger->getTrigger());
      break;
    }

    default:
      // should never happen: triggers can only be connected to steps and other triggers.
      CEDAR_DEBUG_ASSERT(false);
      break;
  }
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

cedar::proc::gui::ConnectValidity
  cedar::proc::gui::TriggerItem::canConnectTo(GraphicsBase* pTarget) const
{
  // a trigger cannot connect to its parent
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
    if(this->mTrigger->isListener(p_step_item->getStep()))
    {
      return cedar::proc::gui::CONNECT_NO;
    }
  }

  if (cedar::proc::gui::TriggerItem *p_trigger_item = dynamic_cast<cedar::proc::gui::TriggerItem*>(pTarget))
  {
    if(this->mTrigger->isListener(p_trigger_item->getTrigger()))
    {
      return cedar::proc::gui::CONNECT_NO;
    }
  }

  return cedar::proc::gui::CONNECT_YES;
}

void cedar::proc::gui::TriggerItem::setTrigger(cedar::proc::TriggerPtr trigger)
{
  this->mTrigger = trigger;
  this->mClassId = cedar::proc::Manager::getInstance().triggers().getDeclarationOf(mTrigger);
  
  std::string tool_tip = this->mTrigger->getName() + " (" + this->mClassId->getClassName() + ")";
  this->setToolTip(tool_tip.c_str());
}

void cedar::proc::gui::TriggerItem::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
  this->cedar::proc::gui::GraphicsBase::readConfiguration(node);
  try
  {
    std::string trigger_name = node.get<std::string>("trigger");
    cedar::proc::TriggerPtr trigger = cedar::proc::Manager::getInstance().triggers().get(trigger_name);
    this->setTrigger(trigger);
  }
  catch (const boost::property_tree::ptree_bad_path&)
  {
    CEDAR_THROW(cedar::proc::gui::InvalidTriggerNameException, "Cannot read TriggerItem from file: no trigger name given.");
  }
}

void cedar::proc::gui::TriggerItem::writeConfiguration(cedar::aux::ConfigurationNode& root)
{
  root.put("trigger", this->mTrigger->getName());
  this->cedar::proc::gui::GraphicsBase::writeConfiguration(root);
}

void cedar::proc::gui::TriggerItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
  if (cedar::proc::LoopedTrigger* p_looped_trigger = dynamic_cast<cedar::proc::LoopedTrigger*>(this->mTrigger.get()))
  {
    QMenu menu;
    QAction *p_start = menu.addAction("start");
    QAction *p_stop = menu.addAction("stop");

    if (p_looped_trigger->isRunning())
    {
      p_start->setEnabled(false);
    }
    else
    {
      p_stop->setEnabled(false);
    }

    QAction *a = menu.exec(event->screenPos());

    if (a == p_start)
    {
      p_looped_trigger->startTrigger();
    }
    else if (a == p_stop)
    {
      p_looped_trigger->stopTrigger();
    }
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

void cedar::proc::gui::TriggerItem::connectTo(cedar::proc::gui::StepItem *pTarget)
{
  cedar::proc::Manager::getInstance().connect(this->getTrigger(), pTarget->getStep());
  /*!@todo check that this connection isn't added twice; the check above doesn't to this because during file loading,
   *       the "real" connections are already read via cedar::proc::Network, and then added to the ui afterwards using
   *       this function.
   */
  this->scene()->addItem(new Connection(this, pTarget));
}

void cedar::proc::gui::TriggerItem::connectTo(cedar::proc::gui::TriggerItem *pTarget)
{
  cedar::proc::Manager::getInstance().connect(this->getTrigger(), pTarget->getTrigger());
  /*!@todo check that this connection isn't added twice; the check above doesn't to this because during file loading,
   *       the "real" connections are already read via cedar::proc::Network, and then added to the ui afterwards using
   *       this function.
   */
  this->scene()->addItem(new Connection(this, pTarget));
}
