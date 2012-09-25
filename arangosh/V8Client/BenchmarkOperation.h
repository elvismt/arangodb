////////////////////////////////////////////////////////////////////////////////
/// @brief interface definition for benchmark operations
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triagens GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Jan Steemann
/// @author Copyright 2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_V8_CLIENT_BENCHMARK_OPERATION_H
#define TRIAGENS_V8_CLIENT_BENCHMARK_OPERATION_H 1

#include "Basics/Common.h"
#include "SimpleHttpClient/SimpleHttpClient.h"

using namespace std;
using namespace triagens::basics;
using namespace triagens::httpclient;
using namespace triagens::rest;

namespace triagens {
  namespace v8client {

// -----------------------------------------------------------------------------
// --SECTION--                                          class BenchmarkOperation
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup V8Client
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief simple interface for benchmark operations
////////////////////////////////////////////////////////////////////////////////

    struct BenchmarkOperation {

////////////////////////////////////////////////////////////////////////////////
/// @brief ctor, derived class can implemented something sensible
////////////////////////////////////////////////////////////////////////////////

      BenchmarkOperation () {
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief dtor, derived class can implemented something sensible
////////////////////////////////////////////////////////////////////////////////

      virtual ~BenchmarkOperation () {
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the URL of the operation to execute
////////////////////////////////////////////////////////////////////////////////

      virtual const string& url () = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief return the HTTP method of the operation to execute
////////////////////////////////////////////////////////////////////////////////

      virtual const HttpRequest::HttpRequestType type () = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief return the payload (body) of the HTTP request to execute 
////////////////////////////////////////////////////////////////////////////////
      
      virtual const char* payload (size_t*, const size_t) = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief return the HTTP headers for the oepration to execute
////////////////////////////////////////////////////////////////////////////////

      virtual const map<string, string>& headers () = 0;

    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
