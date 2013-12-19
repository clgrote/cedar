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

    File:        DataSlot.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 29

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_DATA_SLOT_H
#define CEDAR_PROC_DATA_SLOT_H

// CEDAR INCLUDES
#include "cedar/processing/DataRole.h"
#include "cedar/auxiliaries/boostSignalsHelper.h"

// FORWARD DECLARATIONS
#include "cedar/processing/DataSlot.fwd.h"
#include "cedar/processing/DataConnection.fwd.h"
#include "cedar/processing/Group.fwd.h"
#include "cedar/processing/Connectable.fwd.h"
#include "cedar/processing/PromotedExternalData.fwd.h"
#include "cedar/processing/PromotedOwnedData.fwd.h"
#include "cedar/auxiliaries/Data.fwd.h"

// SYSTEM INCLUDES
#ifndef Q_MOC_RUN
  #include <boost/function.hpp>
  #include <boost/enable_shared_from_this.hpp>
  #include <boost/signals2/connection.hpp>
  #include <boost/signals2/signal.hpp>
#endif
#include <string>


/*!@brief This class represents data slots in connectable objects.
 *
 *        Connectable objects can have a number of DataSlots associated with them. These slots represent, e.g., inputs
 *        of the connectable and are used to define what data a connectable expects as input.
 */
class cedar::proc::DataSlot
:
public boost::enable_shared_from_this<cedar::proc::DataSlot>
{
  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------
  friend class cedar::proc::Connectable;
  friend class cedar::proc::DataConnection;
  friend class cedar::proc::Group;

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

public:
  /*! Enum describing the validity of the data connected to this slot.
   */
  enum VALIDITY
  {
    //! The data is valid.
    VALIDITY_VALID,
    //! The data may not be valid, but the step using it can be computed nonetheless.
    VALIDITY_WARNING,
    //! The data is erroneous, computing the corresponding step may explode things.
    VALIDITY_ERROR,
    //! The validity is unknown and needs to be determined before execution.
    VALIDITY_UNKNOWN
  };

  //! Type of the function that is called for type checks.
  typedef
    boost::function<VALIDITY (cedar::proc::ConstDataSlotPtr, cedar::aux::ConstDataPtr)>
    TypeCheckFunction;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  DataSlot(
            cedar::proc::DataRole::Id role,
            const std::string& name,
            cedar::proc::Connectable* pParent,
            bool isMandatory = true
          );

  //!@brief Destructor
  virtual ~DataSlot();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief get the encapsulated DataPtr managed by this slot
  virtual cedar::aux::DataPtr getData() = 0;

  //!@brief get the encapsulated DataPtr managed by this slot as const
  virtual cedar::aux::ConstDataPtr getData() const = 0;

  //!@brief get the role (input, output...) of this slot
  cedar::proc::DataRole::Id getRole() const;

  //!@brief get the name of this slot
  const std::string& getName() const;

  //!@brief get the name of this slot's parent
  const std::string& getParent() const;

  /*!@brief set some explanatory text for this slot
   *
   * @remarks Set to an empty string ("") to disable the text and use the name instead.
   */
  void setText(const std::string& text);

  //!@brief Returns the text to display to the user.
  const std::string& getText() const;

  //!@brief is this a mandatory connection? i.e. there must be at least one connection using this slot
  bool isMandatory() const;

  //!@brief typo version of getValidity() const
  CEDAR_DECLARE_DEPRECATED(VALIDITY getValidlity() const);

  //!@brief get the current validity of this slot
  virtual VALIDITY getValidity() const;

  //!@brief set the current validity of this slot
  virtual void setValidity(VALIDITY validity);

  //!@brief checks if this Connectable is the parent of this DataSlotItem
  bool isParent(cedar::proc::ConstConnectablePtr parent) const;

  //!@brief get the pointer of this slot's parent
  cedar::proc::Connectable* getParentPtr();

  //!@brief get the const pointer of this slot's parent
  const cedar::proc::Connectable* getParentPtr() const;

  /*!@brief Used for setting the type check for this object.
   *
   *        This method can be used to set the type check object for this slot by using the assignment operator, i.e.,
   *        slot->setCheck(...)
   */
  void setCheck(const TypeCheckFunction& check);

  /*!@brief Checks whether this slot has a validity check associated with it.
   * @see   setCheck
   */
  bool hasValidityCheck() const;

  /*!@brief Checks the validity of the given data for this slot.
   *
   * @throw cedar::proc::NoCheckException if no check function is set.
   */
  cedar::proc::DataSlot::VALIDITY checkValidityOf(cedar::aux::ConstDataPtr data) const;

  //--------------------------------------------------------------------------------------------------------------------
  // signals and slots
  //--------------------------------------------------------------------------------------------------------------------
public:
  CEDAR_DECLARE_SIGNAL(ValidityChanged, void());
public:
  CEDAR_DECLARE_SIGNAL(DataChanged, void());
public:
  CEDAR_DECLARE_SIGNAL(DataSet, void(cedar::aux::ConstDataPtr data));
public:
  CEDAR_DECLARE_SIGNAL(DataRemoved, void(cedar::aux::ConstDataPtr data));

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //! Set the internal DataPtr of this slot.
  void setData(cedar::aux::DataPtr data);

  //! Remove the DataPtr from this slot. Child classes decide how to handle this.
  void removeData(cedar::aux::ConstDataPtr data);

  //! Clears all data in the slot.
  void clear();

  //! Emits the DataChanged signal.
  void emitDataChanged()
  {
    this->signalDataChanged();
  }

  //! Emits the DataAdded signal.
  void emitDataSet(cedar::aux::ConstDataPtr data)
  {
    this->signalDataSet(data);
  }

  //! Emits the DataRemoved signal.
  void emitDataRemoved(cedar::aux::ConstDataPtr data)
  {
    this->signalDataRemoved(data);
  }

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief Set the name of the data slot
  void setName(const std::string& name);

  //! Returns the type check function object for this slot.
  const TypeCheckFunction& getCheck() const;

  //!@brief deprecated due to bad name, see resetParentPointer
  CEDAR_DECLARE_DEPRECATED(void deleteParentPointer());

  //!@brief sets the parent pointer to NULL
  void resetParentPointer();

  //! To be overridden by child classes.
  virtual void setDataInternal(cedar::aux::DataPtr data) = 0;

  //! To be overridden by child classes.
  virtual void removeDataInternal(cedar::aux::ConstDataPtr data) = 0;

  //!@brief Removes all data from the slot.
  virtual void clearInternal() = 0;

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //! The parent that owns the slot.
  cedar::proc::Connectable* mpParent;

private:
  //!@brief flag if this slot must be connected
  bool mMandatory;

  //!@brief the validity of this slot
  VALIDITY mValidity;

  //! Name of the slot, used to uniquely identify it among other slots of the same type in a step.
  std::string mName;

  //! Text of the slot, i.e., the text that is displayed to the user (ignored if empty).
  std::string mText;

  //! Role of the slot (input, output, ...)
  cedar::proc::DataRole::Id mRole;

  //! Promoted flag
  bool mIsPromoted;

  //! The function object holding a reference to the type check functions for this slot.
  TypeCheckFunction mTypeCheck;

}; // class cedar::proc::DataSlot

#endif // CEDAR_PROC_DATA_SLOT_H
