// **********************************************************************
//
// Copyright (c) 2001
// MutableRealms, Inc.
// Huntsville, AL, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <Ice/Direct.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/Reference.h>
#include <Ice/Object.h>
#include <Ice/LocalException.h>

using namespace std;
using namespace Ice;
using namespace IceInternal;

IceInternal::Direct::Direct(const ObjectAdapterPtr& adapter, const ReferencePtr& ref, const char* operation) :
    _adapter(adapter),
    _reference(ref),
    _operation(operation)
{
    _servant = _adapter->identityToServant(_reference->identity);
    
    try
    {
	if (!_servant)
	{
	    string::size_type pos = _reference->identity.find('#');
	    if (pos != string::npos)
	    {
		_locator = _adapter->findServantLocator(_reference->identity.substr(0, pos));
	    }
	    else
	    {
		_locator = _adapter->findServantLocator("");
	    }

	    if (_locator)
	    {
		_servant = _locator->locate(_adapter, _reference->identity, _operation, _cookie);
	    }
	}
    }
    catch(...)
    {
	if (_locator && _servant)
	{
	    _locator->finished(_adapter, _reference->identity, _servant, _operation, _cookie);
	}
	throw;
    }

    if(!_servant)
    {
	throw ObjectNotExistException(__FILE__, __LINE__);
    }
}

IceInternal::Direct::~Direct()
{
    if (_locator && _servant)
    {
	_locator->finished(_adapter, _reference->identity, _servant, _operation, _cookie);
    }
}

const ObjectPtr&
IceInternal::Direct::servant()
{
    return _servant;
}
