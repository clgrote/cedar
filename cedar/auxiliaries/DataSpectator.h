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

    File:        DataSpectator.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.ruhr-uni-bochum.de
    Date:        2013 07 01

    Description: The Recorder uses this class to observe the registered DataPtr.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_AUX_DATASPECTATOR_H_
#define CEDAR_AUX_DATASPECTATOR_H_

// CEDAR INCLUDES
#include "cedar/processing/namespace.h"
#include "cedar/auxiliaries/Data.h"
#include "cedar/auxiliaries/LoopedThread.h"

// SYSTEM INCLUDES
#include<QTime>
#include<string>
#include <fstream>
#include <sys/stat.h>


  /*!@brief The Recorder uses this class to observe the registered DataPtr.
   *      This class copy the observed DataPtr on each time step and stores the copy with time stamp in a queue. The
   *      recorder can access this queue and write the elements to disk.
   */



class cedar::aux::DataSpectator : public cedar::aux::LoopedThread{
  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------

  /* Recorder should be the only class that could create a DataSpectator. THus the constructor is private,
   * Recorder must be a friend class.
   */
  friend Recorder;

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
  private:
  //!@brief A data structure to store all the DataPtr with time stamp in a list.
    struct recordData
    {
      int mRecordTime;
      cedar::aux::DataPtr mData;
    };

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  public:
    ~DataSpectator();
  private:
    /*!@brief The private Constructor. Can only called by friend classes such as cedar::aux::Recorder. recordIntv should
     * be passed in ms. name is a unique name for this DataPtr, so a file with this name can be created in the output
     * dictionary.*/
    DataSpectator(cedar::aux::DataPtr toSpectate, int recordIntv, std::string name);


  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
  public:
    //!@brief Gets the DataPtr.
    cedar::aux::DataPtr data();

    //!@brief Starts the DataSpectator: Before starting the output file will be opened and the header be written.
    void applyStart();

    //!@brief Stops the DataSpactator. Before stopping all RecordDatas in the queue will be written to disk.
    void applyStop(bool suppressWarning);

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
  protected:
    // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
  private:
    //!@brief Calls record() each time step. Inherited from Looped Thread.
    void step(double time);

    //!@brief Writes the header for the DataPtr to the output file.
    void writeHeader();

    //!@brief Writes the first element of the RecordData queue to the output file.
    void writeOneRecordData();

    //!@brief Writes the whole RecordData queue to the output file.
    void writeAllRecordData();

    //!@brief Copies the DataPtr and stores it as new RecordData in the queue.
    void record();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------

  public:
    //!@brief The unique name of the DataPtr. Will be used to name the file.
    std::string mName;

  protected:
    // none yet

  private:
    //!@brief The pointer to the Data.
    cedar::aux::DataPtr mData;

    //!@brief The path for storing the recorded Data. This is the full output path including the file name.
    std::string mOutputPath;

    //!@brief The output stream to mOutputPath.
    boost::shared_ptr<std::ofstream> mpOutputStream;

    //!@brief The Lock for mOutputStream.
    QReadWriteLock* mpOfstreamLock;

    //!@brief This queue will be write to disk every time step is called.
    std::list<boost::shared_ptr<recordData>> mDataQueue;

    //!@brief Locks mDataQueue.
    QReadWriteLock* mpQueueLock;

};

#endif // CEDAR_AUX_DATASPECTATOR_H_
