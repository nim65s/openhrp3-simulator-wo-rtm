/*
 * Copyright (c) 2016, CNRS
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * LAAS
 */
/**
   @file SotController/server/server.cpp
*/

#include "Controller_impl.h"

#ifdef _WIN32
#include "winbase.h"
#else
#include <unistd.h>
#endif /* _WIN32 */

#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
  CORBA::ORB_var orb;
  try {
    orb = CORBA::ORB_init(argc, argv);
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);

    //
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();

    CosNaming::NamingContext_var cxT;
    CORBA::Object_var	nS = orb->resolve_initial_references("NameService");
    cxT = CosNaming::NamingContext::_narrow(nS);

    CORBA::Object_var ViewerFactory;
    OnlineViewer_impl* onlineViewerFactoryImpl = new OnlineViewer_impl(orb);
    onlineViewerFactory = onlineViewerFactoryImpl -> _this();
    CosNaming::Name nc;
    nc.length(1);
    nc[0].id = CORBA::string_dup("SamplePDController");
    nc[0].kind = CORBA::string_dup("");
    cxT -> rebind(nc, onlineViewerFactory);

    manager -> activate();
    cout << "ready" << endl;

    orb -> run();
  } catch (CORBA::SystemException& ex) {
    cerr << ex._rep_id() << endl;
    return 1;
  }
  orb->destroy();
  return 0;
}
