// **********************************************************************
//
// Copyright (c) 2001
// MutableRealms, Inc.
// Huntsville, AL, USA
//
// All Rights Reserved
//
// **********************************************************************

class NestedServer extends Ice.Application
{
    public int
    run(String[] args)
    {
        Ice.ObjectAdapter adapter =
            communicator().createObjectAdapter("NestedServerAdapter");
        NestedPrx self = NestedPrxHelper.uncheckedCast(
            adapter.createProxy(Ice.Util.stringToIdentity("nestedServer")));
        adapter.add(new NestedI(self),
                    Ice.Util.stringToIdentity("nestedServer"));
        adapter.activate();
        communicator().waitForShutdown();
        return 0;
    }
}
