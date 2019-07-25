/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        PythonScript.cpp

    Maintainer:  Lars Janssen
    Email:       lars.janssen@ini.rub.de
    Date:        2019 04 01

    Description: Source file for the class PythonScript.

    Credits:

======================================================================================================================*/

/*
The MIT License (MIT)

Copyright (c) 2014 Yati Sagade

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


// CEDAR CONFIGURATION

// CLASS HEADER
#include "cedar/processing/steps/PythonScript.h"

// CEDAR INCLUDES
//#include "cedar/processing/typecheck/Matrix.h"
#include "cedar/processing/ElementDeclaration.h"
#include "cedar/processing/GroupDeclaration.h"
#include "cedar/processing/gui/Settings.h"


// SYSTEM INCLUDES
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/algorithm/string.hpp>

#include "numpy/ndarrayobject.h"

#include <QReadWriteLock>


//----------------------------------------------------------------------------------------------------------------------
// register the class
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool declare()
  {
    using cedar::proc::ElementDeclarationPtr;
    using cedar::proc::ElementDeclarationTemplate;

    ElementDeclarationPtr declaration
    (
      new ElementDeclarationTemplate<cedar::proc::steps::PythonScript>
      (
        "Programming",
        "cedar.processing.steps.PythonScript"
      )
    );
    declaration->setIconPath(":/steps/python_script.svg");
    declaration->setDescription
    (
      "In-Runtime Python Script-editor to develop new steps from scratch."
    );

    declaration->declare();

    return true;
  }

  bool declared = declare();
}

// static member declaration
int cedar::proc::steps::PythonScript::executionFailed = 0;
std::vector<PyThreadState*> cedar::proc::steps::PythonScript::threadStates;
QMutex cedar::proc::steps::PythonScript::mutex;
std::string cedar::proc::steps::PythonScript::nameOfExecutingStep = "";

int NDArrayConverter::failmsg(const char *fmt, ...)
{
  char str[1000];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(str, sizeof(str), fmt, ap);
  va_end(ap);

  PyErr_SetString(PyExc_TypeError, str);

  cedar::aux::LogSingleton::getInstance()->error
          (
                  str,
                  "void cedar::proc::steps::PythonScript::executePythonScript()",
                  (this->pythonScript != nullptr) ? this->pythonScript->getName():"Python Script"
          );
  cedar::proc::steps::PythonScript::executionFailed = 1;

  return 0;
}

class PyAllowThreads
{
public:
    PyAllowThreads() : _state(PyEval_SaveThread()) {}
    ~PyAllowThreads()
    {
        PyEval_RestoreThread(_state);
    }
private:
    PyThreadState* _state;
};

class PyEnsureGIL
{
public:
    PyEnsureGIL() : _state(PyGILState_Ensure()) 
    {
    }
    ~PyEnsureGIL()
    {
      PyGILState_Release(_state);
    }
private:
    PyGILState_STATE _state;
};

class NumpyAllocator : public cv::MatAllocator
{
public:

    const MatAllocator* stdAllocator;
    NumpyAllocator() { stdAllocator = cv::Mat::getStdAllocator(); }
    ~NumpyAllocator() {}

    void allocate(int dims, const int* sizes, int type, int*& refcount,
                  uchar*& datastart, uchar*& data, size_t* step)
    {
      //PyEnsureGIL gil;
      int depth = CV_MAT_DEPTH(type);
      int cn = CV_MAT_CN(type);
      const int f = (int)(sizeof(size_t)/8);
      int typenum = depth == CV_8U ? NPY_UBYTE : depth == CV_8S ? NPY_BYTE :
                                                 depth == CV_16U ? NPY_USHORT : depth == CV_16S ? NPY_SHORT :
                                                                                depth == CV_32S ? NPY_INT : depth == CV_32F ? NPY_FLOAT :
                                                                                                            depth == CV_64F ? NPY_DOUBLE : f*NPY_ULONGLONG + (f^1)*NPY_UINT;
      int i;
      npy_intp _sizes[CV_MAX_DIM+1];
      for( i = 0; i < dims; i++ )
      {
        _sizes[i] = sizes[i];
      }

      if( cn > 1 )
      {
        _sizes[dims++] = cn;
      }
      PyObject* o = PyArray_SimpleNew(dims, _sizes, typenum);
      if(!o)
      {
        CV_Error_(CV_StsError, ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));
      }
      refcount = refcountFromPyObject(o);
      PyArrayObject* oCasted = reinterpret_cast<PyArrayObject*>(o);
      npy_intp* _strides = PyArray_STRIDES(oCasted); // added oCasted
      for( i = 0; i < dims - (cn > 1); i++ )
        step[i] = (size_t)_strides[i];
      datastart = data = (uchar*)PyArray_DATA(oCasted); // added oCasted
    }

    cv::UMatData* allocate(PyObject* o, int dims, const int* sizes, int type, size_t* step) const
    {
      cv::UMatData* u = new cv::UMatData(this);
      u->data = u->origdata = (uchar*)PyArray_DATA((PyArrayObject*) o);
      npy_intp* _strides = PyArray_STRIDES((PyArrayObject*) o);
      for( int i = 0; i < dims - 1; i++ )
        step[i] = (size_t)_strides[i];
      step[dims-1] = CV_ELEM_SIZE(type);
      u->size = sizes[0]*step[0];
      u->userdata = o;
      return u;
    }

    cv::UMatData* allocate(int dims0, const int* sizes, int type, void* data, size_t* step, int flags, cv::UMatUsageFlags usageFlags) const// CV_OVERRIDE
    {
      if( data != 0 )
      {
        // issue #6969: CV_Error(Error::StsAssert, "The data should normally be NULL!");
        // probably this is safe to do in such extreme case
        return stdAllocator->allocate(dims0, sizes, type, data, step, flags, usageFlags);
      }
      //PyEnsureGIL gil;

      int depth = CV_MAT_DEPTH(type);
      int cn = CV_MAT_CN(type);
      const int f = (int)(sizeof(size_t)/8);
      int typenum = depth == CV_8U ? NPY_UBYTE : depth == CV_8S ? NPY_BYTE :
                                                 depth == CV_16U ? NPY_USHORT : depth == CV_16S ? NPY_SHORT :
                                                                                depth == CV_32S ? NPY_INT : depth == CV_32F ? NPY_FLOAT :
                                                                                                            depth == CV_64F ? NPY_DOUBLE : f*NPY_ULONGLONG + (f^1)*NPY_UINT;
      int i, dims = dims0;
      cv::AutoBuffer<npy_intp> _sizes(dims + 1);
      for( i = 0; i < dims; i++ )
        _sizes[i] = sizes[i];
      if( cn > 1 )
        _sizes[dims++] = cn;
      PyObject* o = PyArray_SimpleNew(dims, _sizes, typenum);
      if(!o)
        CV_Error_(cv::Error::StsError, ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));
      return allocate(o, dims0, sizes, type, step);
    }

    bool allocate(cv::UMatData* u, int accessFlags, cv::UMatUsageFlags usageFlags) const //CV_OVERRIDE
    {
      return stdAllocator->allocate(u, accessFlags, usageFlags);
    }

    void deallocate(int* refcount, uchar*, uchar*)
    {
      //PyEnsureGIL gil;
      if( !refcount )
        return;

      PyObject* o = pyObjectFromRefcount(refcount);
      Py_INCREF(o);
      Py_DECREF(o);
    }

    void deallocate(cv::UMatData* u) const //CV_OVERRIDE
    {
      if(!u)
        return;
      //PyEnsureGIL gil;
      CV_Assert(u->urefcount >= 0);
      CV_Assert(u->refcount >= 0);
      if(u->refcount == 0)
      {
        PyObject* o = (PyObject*)u->userdata;
        Py_XDECREF(o);
        delete u;
      }
    }

};

NumpyAllocator g_numpyAllocator;


// static member decleration
int NDArrayConverter::isInitialized = 0;


NDArrayConverter::NDArrayConverter()
:
pythonScript(nullptr)
{
  init();
}

NDArrayConverter::NDArrayConverter(cedar::proc::steps::PythonScript* pScript)
:
pythonScript(pScript)
{
  init();
}


void NDArrayConverter::init()
{
  if(!NDArrayConverter::isInitialized)
  {
    NDArrayConverter::isInitialized = 1;
    _import_array();
  }
}

const char * NDArrayConverter::typenumToString(int typenum) {
  switch(typenum){
    case NPY_BOOL:
      return "boolean (NPY_BOOL)";
    case NPY_UBYTE:
      return "unsigned byte (NPY_UBYTE)";
    case NPY_BYTE:
      return "byte (NPY_BYTE)";
    case NPY_USHORT:
      return "unsigned short (NPY_USHORT)";
    case NPY_SHORT:
      return "short (NPY_SHORT)";
    case NPY_UINT:
      return "unsigned int (NPY_UINT)";
    case NPY_INT:
      return "int (NPY_INT)";
    case NPY_ULONGLONG:
      return "unsigned long long (NPY_ULONGLONG)";
    case NPY_LONGLONG:
      return "long long (NPY_LONGLONG)";
    case NPY_HALF:
      return "float16 (NPY_HALF)";
    case NPY_FLOAT:
      return "float32 (NPY_FLOAT)";
    case NPY_DOUBLE:
      return "double (NPY_DOUBLE)";
    case NPY_LONGDOUBLE:
      return "long double (NPY_LONGDOUBLE)";
    case NPY_CFLOAT:
      return "complex float32 (NPY_CFLOAT)";
    case NPY_CDOUBLE:
      return "complex double (NPY_CDOUBLE)";
    case NPY_CLONGDOUBLE:
      return "complex long double (NPY_CLONGDOUBLE)";
    case NPY_DATETIME:
      return "datetime (NPY_DATETIME)";
    case NPY_TIMEDELTA:
      return "timedelta (NPY_TIMEDELTA)";
    case NPY_STRING:
      return "string (NPY_STRING)";
    case NPY_UNICODE:
      return "unicode (NPY_UNICODE)";
    case NPY_OBJECT:
      return "object (NPY_OBJECT)";
    case NPY_VOID:
      return "void (NPY_VOID)";
    case NPY_NOTYPE:
      return "notype (NPY_NOTYPE)";
    case NPY_USERDEF:
      return "user defined (NPY_USERDEF)";
    default:
      "unknown";
  }

}

cv::Mat NDArrayConverter::toMat(PyObject* o, int index)
{
  cv::Mat m;
  bool allowND = true, doMultiChannelTypeConversion = false;
  if(!o || o == Py_None)
  {
    //TODO failmsg here?
    return cv::Mat::zeros(1,1,CV_32F);
  }

  if( PyInt_Check(o) )
  {
    double v[] = {static_cast<double>(PyInt_AsLong((PyObject*)o))};
    m = cv::Mat(1, 1, CV_64F, v).clone();
    return m;
  }
  if( PyFloat_Check(o) )
  {
    double v[] = {PyFloat_AsDouble((PyObject*)o)};
    m = cv::Mat(1, 1, CV_64F, v).clone();
    return m;
  }
  if( PyTuple_Check(o) )
  {
    int i, sz = (int)PyTuple_Size((PyObject*)o);
    m = cv::Mat(sz, 1, CV_64F);
    for( i = 0; i < sz; i++ )
    {
      PyObject* oi = PyTuple_GetItem(o, i);
      if( PyInt_Check(oi) )
      {
        m.at<double>(i) = (double) PyInt_AsLong(oi);
      }
      else if( PyFloat_Check(oi) )
      {
        m.at<double>(i) = (double) PyFloat_AsDouble(oi);
      }
      else
      {
        failmsg("Object in pc.outputs[%d] is not a numerical tuple.", index);
        return cv::Mat::zeros(1,1,CV_32F);
      }
    }
    return m;
  }

  if( !PyArray_Check(o) )
  {
    failmsg("Object in pc.outputs[%d] is not a numpy array, neither a scalar.", index);
    return cv::Mat::zeros(1,1,CV_32F);
  }

  PyArrayObject* oarr = (PyArrayObject*) o;

  bool needcopy = false, needcast = false;
  int typenum = PyArray_TYPE(oarr), new_typenum = typenum;
  int type = typenum == NPY_UBYTE ? CV_8U :
             typenum == NPY_BYTE ? CV_8S :
             typenum == NPY_USHORT ? CV_16U :
             typenum == NPY_SHORT ? CV_16S :
             typenum == NPY_INT ? CV_32S :
             typenum == NPY_INT32 ? CV_32S :
             typenum == NPY_FLOAT ? CV_32F :
             typenum == NPY_DOUBLE ? CV_64F : -1;

  if( type < 0 )
  {
    if( typenum == NPY_INT64 || typenum == NPY_UINT64 || typenum == NPY_LONG )
    {
      needcopy = needcast = true;
      new_typenum = NPY_INT;
      type = CV_32S;
    }
    else
    {
      failmsg("Data type (%s) of matrix in pc.outputs[%d] is not supported.", typenumToString(typenum), index);
      return cv::Mat::zeros(1,1,CV_32F);
    }
  }

#ifndef CV_MAX_DIM
  const int CV_MAX_DIM = 32;
#endif

  int ndims = PyArray_NDIM(oarr);
  if(ndims >= CV_MAX_DIM)
  {
    failmsg("Dimension of matrix in pc.outputs[%d] is too high. (%d)", index, ndims);
    return cv::Mat::zeros(1,1,CV_32F);
  }

  int size[CV_MAX_DIM+1];
  size_t step[CV_MAX_DIM+1];
  size_t elemsize = CV_ELEM_SIZE1(type);
  const npy_intp* _sizes = PyArray_DIMS(oarr);
  const npy_intp* _strides = PyArray_STRIDES(oarr);
  bool ismultichannel = ndims == 3 && _sizes[2] <= CV_CN_MAX;

  for( int i = ndims-1; i >= 0 && !needcopy; i-- )
  {
    // these checks handle cases of
    //  a) multi-dimensional (ndims > 2) arrays, as well as simpler 1- and 2-dimensional cases
    //  b) transposed arrays, where _strides[] elements go in non-descending order
    //  c) flipped arrays, where some of _strides[] elements are negative
    // the _sizes[i] > 1 is needed to avoid spurious copies when NPY_RELAXED_STRIDES is set
    if( (i == ndims-1 && _sizes[i] > 1 && (size_t)_strides[i] != elemsize) ||
        (i < ndims-1 && _sizes[i] > 1 && _strides[i] < _strides[i+1]) )
    {
      needcopy = true;
    }
  }

  if( ismultichannel && _strides[1] != (npy_intp)elemsize*_sizes[2] )
  {
    needcopy = true;
  }

  if (needcopy)
  {
    if( needcast )
    {
      o = PyArray_Cast(oarr, new_typenum);
      oarr = (PyArrayObject*) o;
    }
    else
    {
      oarr = PyArray_GETCONTIGUOUS(oarr);
      o = (PyObject*) oarr;
    }

    _strides = PyArray_STRIDES(oarr);
  }

  // Normalize strides in case NPY_RELAXED_STRIDES is set
  size_t default_step = elemsize;
  for ( int i = ndims - 1; i >= 0; --i )
  {
    size[i] = (int)_sizes[i];
    if ( size[i] > 1 )
    {
      step[i] = (size_t)_strides[i];
      default_step = step[i] * size[i];
    }
    else
    {
      step[i] = default_step;
      default_step *= size[i];
    }
  }

  // handle degenerate case
  if(ndims == 0)
  {
    failmsg("Matrix in pc.outputs[%d] has 0 dimensions.", index);
    return cv::Mat::zeros(1,1,CV_32F);

    // Could check the following:
    //size[ndims] = 1;
    //step[ndims] = elemsize;
    //ndims++;
  }

  if( doMultiChannelTypeConversion && ismultichannel )
  {
    ndims--;
    type |= CV_MAKETYPE(0, size[2]);
    std::cout << "Matrix has multiple channels... Do conversion... type |=" <<
                 " CV_MAKETYPE(0, size[2]) has computed " << type << std::endl;
    //TODO implement a type check after oring (currently this statement is never true)
  }

  if( ndims > 2 && !allowND )
  {
    failmsg("Matrix in pc.outputs[%d] has more than 2 dimensions. (%d) (This is currently not supported)", index, ndims);
    return cv::Mat::zeros(1,1,CV_32F);
  }

  m = cv::Mat(ndims, size, type, PyArray_DATA(oarr), step);

  //CV2
  //if( m.data )
  //{
  //  m.refcount = refcountFromPyObject(o);
  //  m.addref(); // protect the original numpy array from deallocation
  //  // (since Mat destructor will decrement the reference counter)
  //}

  //CV3
  m.u = g_numpyAllocator.allocate(o, ndims, size, type, step);
  m.addref();
  if( !needcopy )
  {
    Py_INCREF(o);
  }




  m.allocator = &g_numpyAllocator;
  return m;
}

PyObject* NDArrayConverter::toNDArray(const cv::Mat& m)
{
  if( !m.data )
  {
    Py_RETURN_NONE;
  }
  cv::Mat temp, *p = (cv::Mat*)&m;

  //CV2
  //if(!p->refcount || p->allocator != &g_numpyAllocator)

  //CV3
  if(!p->u || p->allocator != &g_numpyAllocator)

  {
    temp.allocator = &g_numpyAllocator;
    try
    {
      PyAllowThreads allowThreads;
      m.copyTo(temp);
    }
    catch (const cv::Exception &e)
    {
      PyErr_SetString(0, e.what());
      return 0;
    }
    //m.copyTo(temp);

    p = &temp;
  }
  //CV2
  //p->addref();
  //return pyObjectFromRefcount(&p->u->refcount);

  //CV3
  PyObject* o = (PyObject*)p->u->userdata;
  Py_INCREF(o);
  return o;
}



void printFromPython(PyObject* obj)
{
  char const* text = boost::python::extract<const char*>(obj);

  std::string name = cedar::proc::steps::PythonScript::nameOfExecutingStep;
  boost::trim(name);
  if(name == "") name = "Python Script";

  cedar::aux::LogSingleton::getInstance()->message
        (
          std::string("[PythonScript] ").append(text),
          "void cedar::proc::steps::PythonScript::printFromPython(char const* text)",
          name
        );
   
}

struct pylist_converter
{
    static void* convertible(PyObject* object)
    {
      if (!PyList_Check(object)) {
        return nullptr;
      }
      return object;
    }

    static void construct(PyObject* object, boost::python::converter::rvalue_from_python_stage1_data* data)
    {
      typedef boost::python::converter::rvalue_from_python_storage<std::list<PyObject*>> storage_type;
      void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

      data->convertible = new (storage) std::list<PyObject*>();

      std::list<PyObject*>* l = (std::list<PyObject*>*)(storage);

      int sz = PySequence_Size(object);
      for (int i = 0; i < sz; ++i) {
        l->push_back(PyList_GetItem(object, i));
      }
    }
};

struct std_list_to_python
{
    static PyObject* convert(std::list<boost::python::handle<>> const& l)
    {
      boost::python::list result;
      for (auto const& value : l) {
        result.append(value);
      }
      return boost::python::incref(result.ptr());
    }
};

BOOST_PYTHON_MODULE(pycedar)
{
  // Tried to expose vectors of cv::Mat
  // boost::python::class_<std::vector<boost::python::handle<> > >("MatVec")
  //       .def(boost::python::vector_indexing_suite<std::vector<boost::python::handle<> > >());

  boost::python::to_python_converter<std::list<boost::python::handle<>>, std_list_to_python, false>();

  boost::python::converter::registry::push_back(&pylist_converter::convertible
          ,&pylist_converter::construct
          ,boost::python::type_id<std::list<PyObject*>>());
        
  boost::python::def("messagePrint", &printFromPython);
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::steps::PythonScript::PythonScript()
:
PythonScript(false)
{  
}


cedar::proc::steps::PythonScript::PythonScript(bool isLooped)
:
cedar::proc::Step(isLooped)
,
mOutput(new cedar::aux::MatData(cv::Mat::zeros(1, 1, CV_32F))),

mInputs(1, cedar::aux::MatDataPtr()),
mOutputs(1, cedar::aux::MatDataPtr(new cedar::aux::MatData(cv::Mat::zeros(1, 1, CV_32F)))),

// Declare Properties
_codeStringForSavingArchitecture (new cedar::aux::StringParameter(this, "code", "import numpy as np\nimport pycedar as pc\n\n#Print to messages tab:\n# pc.messagePrint('text')\n# pc.messagePrint(str(...))\n\n#Inputs:\n# pc.inputs[0]\n# pc.inputs[1]\n# ...\n\n#Outputs:\n# pc.outputs[0]\n# pc.outputs[1]\n# ...\n\n\ninput = pc.inputs[0]\n\npc.outputs[0] = input * 2\n")),
_mNumberOfInputs (new cedar::aux::UIntParameter(this, "number of inputs", 1,1,255)),
_mNumberOfOutputs (new cedar::aux::UIntParameter(this, "number of outputs", 1,1,255)),
_hasScriptFile (new cedar::aux::BoolParameter(this, "use script file", false)),
_autoConvertDoubleToFloat (new cedar::aux::BoolParameter(this, "auto-convert double output matrices to float", true)),
_scriptFile (new cedar::aux::FileParameter(this, "script file path", cedar::aux::FileParameter::READ))
{
  this->declareInput(makeInputSlotName(0), false);
  
  this->declareOutput(makeOutputSlotName(0), mOutputs[0]);
  
  QObject::connect(this->_hasScriptFile.get(), SIGNAL(valueChanged()), this, SLOT(hasScriptFileChanged()));
  QObject::connect(_mNumberOfInputs.get(), SIGNAL(valueChanged()), this, SLOT(numberOfInputsChanged()));
  QObject::connect(_mNumberOfOutputs.get(), SIGNAL(valueChanged()), this, SLOT(numberOfOutputsChanged()));

  this->_scriptFile->setConstant(true);
  this->_codeStringForSavingArchitecture->setHidden(true);
  this->_autoConvertDoubleToFloat->markAdvanced(true);
  
  cedar::proc::steps::PythonScript::executionFailed = 0;
  
  PyImport_AppendInittab("pycedar", &initpycedar);
  
  Py_Initialize();
  PyEval_InitThreads();

  threadID = threadStates.size();

  if(threadID == 0)
  {
    threadStates.push_back(PyThreadState_Get());
  }
  else
  {
    threadStates.push_back(Py_NewInterpreter());
  }
}



cedar::proc::steps::PythonScript::~PythonScript() { }

template<typename T>
cv::Mat cedar::proc::steps::PythonScript::convert3DMatToFloat(cv::Mat &mat)
{
  if(mat.dims != 3) return cv::Mat::zeros(1,1,CV_32F);
  int size[] = {mat.size[0], mat.size[1], mat.size[2]};
  cv::Mat output(3, size, CV_32F);
  for (int i = 0; i < size[0]; ++i)
  {
    for (int j = 0; j < size[1]; ++j)
    {
      for (int k = 0; k < size[2]; ++k)
      {
        output.at<float>(i,j,k) = static_cast<float>(mat.at<T>(i,j,k));
      }
    }
  }
  return output;
}

void cedar::proc::steps::PythonScript::hasScriptFileChanged()
{
  if(this->_hasScriptFile->getValue())
  {
    this->_scriptFile->setConstant(false);
  }
  else
  {
    this->_scriptFile->setConstant(true);
  }
}

void cedar::proc::steps::PythonScript::numberOfInputsChanged()
{
  unsigned newsize = _mNumberOfInputs->getValue();
  if (newsize == mInputs.size())
  {
    return;
  }
  else if (newsize < mInputs.size())
  {
    // delete unused slots
    for (unsigned i=newsize; i<mInputs.size(); i++)
    {
      removeInputSlot(makeInputSlotName(i));
    }
  }
  else if (newsize > mInputs.size())
  {
    // declare new input slots
    for (unsigned i=mInputs.size(); i<newsize; i++)
    {
      this->declareInput(makeInputSlotName(i), false);
    }
  }
  // resize inputs vector
  mInputs.resize(newsize);
}

void cedar::proc::steps::PythonScript::exportStepAsTemplate()
{
  cedar::proc::GroupDeclarationPtr python_declaration
          (
                  new cedar::proc::GroupDeclaration
                          (
                                  "PScript",
                                  "resource://groupTemplates/fieldTemplates.json",
                                  "python script template1",
                                  "DFT Templates"
                          )
          );
  python_declaration->setIconPath(":/cedar/dynamics/gui/steps/field_1d_active.svg");
  python_declaration->declare();

  // calls the reset() function of ElementList, so the template is visible
  cedar::proc::gui::SettingsSingleton::getInstance()->emitElementListViewResetSignal();

}

void cedar::proc::steps::PythonScript::numberOfOutputsChanged()
{
  unsigned newsize = _mNumberOfOutputs->getValue();
  unsigned oldsize = mOutputs.size();
  if (newsize == oldsize)
  {
    return;
  }
  
  // resize outputs vector
  mOutputs.resize(newsize);
  
  if (newsize < oldsize)
  {
    // delete unused slots
    for (unsigned i=newsize; i<oldsize; i++)
    {
      removeOutputSlot(makeOutputSlotName(i));
    }
  }
  else if (newsize > oldsize)
  {
    // declare new input slots
    for (unsigned i=oldsize; i<newsize; i++)
    {
      mOutputs[i] = cedar::aux::MatDataPtr(new cedar::aux::MatData(cv::Mat::zeros(1, 1, CV_32F)));
      this->declareOutput(makeOutputSlotName(i), mOutputs[i]);
    }
  }
  
}

std::string cedar::proc::steps::PythonScript::makeInputSlotName(const int i)
{
    std::stringstream s;
    s << "input " << i;
    return s.str();
}

std::string cedar::proc::steps::PythonScript::makeOutputSlotName(const int i)
{
    std::stringstream s;
    s << "output " << i;
    return s.str();
}


//TODO allow 0 inputs / outputs in GUI

void cedar::proc::steps::PythonScript::executePythonScript()
{
  mutex.lock();

  isExecuting = 1;
  cedar::proc::steps::PythonScript::executionFailed = 0;

  nameOfExecutingStep = this->getName();

  // Swap to the interpreter of this specific PythonScript step
  if(threadStates.size() > threadID)
  {
    PyThreadState_Swap(threadStates.at(threadID));
  }
  else
  {
    std::cout << "No thread state for thread ID " << threadID << std::endl;
  }
  
  // Python...
  try{

    // Loading main module
    boost::python::object main_module = boost::python::import("__main__");
    boost::python::object main_namespace = main_module.attr("__dict__");

    // Loading pycedar module
    boost::python::object pycedar_module( (boost::python::handle<>(PyImport_ImportModule("pycedar"))) );

    std::list<boost::python::handle<>> inputsList;
    NDArrayConverter cvt(this);
    for(unsigned int i = 0; i < mInputs.size(); i++)
    {
      cedar::aux::ConstDataPtr inputMatrixPointer = this->getInputSlot(makeInputSlotName(i))->getData();
      if(inputMatrixPointer != 0)
      {
        // Lock input matrix
        auto mat_data = boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(inputMatrixPointer);
        QReadLocker input_l(&mat_data->getLock());
        cv::Mat inputMatrix = mat_data->getData().clone();

        // Unlock input matrix
        input_l.unlock();
        // Convert input matrix to numpy array
        PyObject* inputMatrix_np = cvt.toNDArray(inputMatrix);
        // Include it to Python
        boost::python::handle<> inputMatrix_np_handle(inputMatrix_np);
        inputsList.push_back(inputMatrix_np_handle);

        // Old way: Setting the Input to pc.input1, pc.input2 etc.
        /*

        boost::python::object inputMatrix_object(inputMatrix_np_handle);
        // Setting the input directly in the global pycedar scope
        const char* attrName = std::string("input").append(std::to_string(i + 1)).c_str();
        boost::python::scope(pycedar_module).attr(attrName) = inputMatrix_object;

        */
      }
      else
      {
        // Old way: Setting the Input to pc.input1, pc.input2 etc.
        /*

        const char* attrName = std::string("input").append(std::to_string(i + 1)).c_str();
        boost::python::scope(pycedar_module).attr(attrName) = -1;

        */

        PyObject* matrix_np = cvt.toNDArray(cv::Mat::zeros(1, 1, CV_32F));
        boost::python::handle<> matrix_np_handle(matrix_np);
        inputsList.push_back(matrix_np_handle);

      }
    }
    boost::python::scope(pycedar_module).attr("inputs") = inputsList;
    // Adding empty matrices to the output list so that the length of the list in python is correctly set
    std::list<boost::python::handle<>> outputsList;
    boost::python::handle<> hand(cvt.toNDArray( cv::Mat::zeros(1, 1, CV_32F) ));

    for(unsigned int i = 0; i < mOutputs.size(); i++)
      outputsList.push_back(hand);

    boost::python::scope(pycedar_module).attr("outputs") = outputsList;
    // Old way: Setting the Output to pc.output1, pc.output2 etc.
    /*

    // Remove output data of old calls
    for(unsigned int i = 0; i < mOutputs.size(); i++){
      const char* attrName = std::string("output").append(std::to_string(i + 1)).c_str();

      // Test if the output object exists in the pycedar scope
      try
      {
        boost::python::scope(pycedar_module).attr(attrName);
      }
      catch(const boost::python::error_already_set&)
      {
        continue;
      }

      // Create empty matrix
      PyObject* emptyMatrix_np = cvt.toNDArray(cv::Mat::zeros(1, 1, CV_32F));

      // Include it to Python
      boost::python::handle<> emptyMatrix_np_handle(emptyMatrix_np);
      boost::python::object emptyMatrix_object(emptyMatrix_np_handle);

      boost::python::scope(pycedar_module).attr(attrName) = emptyMatrix_object;
    }

    */

    bool executedFromFile = false;
    // Execute script from the specified file, if the hasScriptFile Parameter is set and the file exists
    if(this->_hasScriptFile->getValue())
    {
      cedar::aux::Path path = this->_scriptFile->getPath();
      if(path.exists()){
        boost::python::exec_file(path.toString().c_str(), main_namespace);
        executedFromFile = true;
      }
    }
    
    if(!executedFromFile)
    {
      std::string inputString = this->_codeStringForSavingArchitecture->getValue();
      boost::python::str inputString_py(inputString);
      
      boost::python::exec(boost::python::extract<char const*>(inputString_py), main_namespace);
    }

    // Old way: Setting the Output to pc.output1, pc.output2 etc.
    /*

    // relegate output from python to the step output
    for(unsigned int i = 0; i < mOutputs.size(); i++){
      const char* attrName = std::string("output").append(std::to_string(i + 1)).c_str();
      boost::python::object output_object;

      // Test if the output object exists in the pycedar scope
      try
      {
        output_object = boost::python::scope(pycedar_module).attr(attrName);
      }
      catch(const boost::python::error_already_set&)
      {
        continue;
      }

      PyObject * outputPointer = output_object.ptr();
      cv::Mat &outputNodeMatrix = mOutputs[i]->getData();
      outputNodeMatrix = cvt.toMat(outputPointer);
      // Convert 1D and 2D matrices of type CV_64F (double) to CV_32F (float)
      if (this->_autoConvertDoubleToFloat->getValue())
      {
        if (outputNodeMatrix.dims <= 2 &&
            outputNodeMatrix.type() == CV_64F)
        {
          outputNodeMatrix.convertTo(outputNodeMatrix, CV_32F); // added
        }
      }
    }

    */

    // Get the outputs from python
    std::list<PyObject*> list = boost::python::extract<std::list<PyObject*>>(boost::python::scope(pycedar_module).attr("outputs"));

    int i = 0;
    for (auto const& outputPointer : list) {
      if(i >= mOutputs.size()) break;

      cv::Mat &outputNodeMatrix = mOutputs[i]->getData();
      outputNodeMatrix = cvt.toMat(outputPointer, i);

      // Convert 1D and 2D matrices of type CV_64F (double) to CV_32F (float)
      if (this->_autoConvertDoubleToFloat->getValue())
      {
        if (outputNodeMatrix.dims <= 2 &&
            outputNodeMatrix.type() == CV_64F)
        {
          outputNodeMatrix.convertTo(outputNodeMatrix, CV_32F); // added
        }
        else if (outputNodeMatrix.dims == 3 &&
             outputNodeMatrix.type() == CV_64F)
        {
          outputNodeMatrix = convert3DMatToFloat<double>(outputNodeMatrix);
        }
      }

      i++;
    }
    
  }
  catch(const boost::python::error_already_set&){

    cedar::proc::steps::PythonScript::executionFailed = 1;
    
    if(PyErr_Occurred() == 0) std::cout << "No PyErr occured!" << std::endl;

    // Try to extract the error message from python
    std::string errorMessage = "Undefined Error";
    try{
      PyObject *exc,*val,*tb;
      PyErr_Fetch(&exc,&val,&tb);
      PyErr_NormalizeException(&exc,&val,&tb);
      boost::python::handle<> hexc(exc),hval(boost::python::allow_null(val)),htb(boost::python::allow_null(tb));
      if(!hval)
      {
        errorMessage = boost::python::extract<std::string>(boost::python::str(hexc));
      }
      else
      {
        // Get Error message
        boost::python::object traceback(boost::python::import("traceback"));
        boost::python::object format_exception(traceback.attr("format_exception"));
        boost::python::object formatted_list(format_exception(hexc,hval,htb));
        boost::python::object formatted(boost::python::str("").join(formatted_list));

        errorMessage = boost::python::extract<std::string>(formatted);

        // Cutoff empty line at the end
        if(errorMessage.at(errorMessage.length() - 1) == '\n') errorMessage = errorMessage.substr(0, errorMessage.length() - 1);

        // Extract line number
        std::string substring = errorMessage.substr(errorMessage.find("line") + 4, errorMessage.length());
        std::stringstream stream(substring);
        long lineno;
        stream >> lineno;
        if(stream) emit errorMessageLineNumberChanged(lineno);
      }
    }
    catch(const std::exception& e)
    {
      errorMessage = e.what();
    }
    catch(const boost::python::error_already_set&)
    {
      errorMessage = "Undefined error";
    }

    // Post the error in the Log section of cedar
    cedar::aux::LogSingleton::getInstance()->error
      (
        errorMessage,
        "void cedar::proc::steps::PythonScript::executePythonScript()",
        this->getName()
      );

  }
  
  if(cedar::proc::steps::PythonScript::executionFailed) this->setState(cedar::proc::Triggerable::STATE_EXCEPTION, "An exception occured");
  else this->setState(cedar::proc::Triggerable::STATE_UNKNOWN, "");
  
  isExecuting = 0;
  mutex.unlock();
}

void cedar::proc::steps::PythonScript::inputConnectionChanged(const std::string& inputName)
{
  // Put input in the corresponding field in mInputs
  cedar::aux::ConstMatDataPtr input = boost::dynamic_pointer_cast<const cedar::aux::MatData>(getInput(inputName));

  unsigned slot_id;
  std::istringstream(inputName.substr(5)) >> slot_id;

  if (slot_id < mInputs.size())
  {
    mInputs[slot_id] = input;
  }
}

void cedar::proc::steps::PythonScript::compute(const cedar::proc::Arguments&)
{
  if(!isExecuting)
  {
    executePythonScript();
  }
}

// Method is called when the executeButton in the CodeWidget is clicked
void cedar::proc::steps::PythonScript::executeButtonClicked(){
  if(!isExecuting)
  {
    executePythonScript();
  }
}
