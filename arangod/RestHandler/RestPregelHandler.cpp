////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2016 ArangoDB GmbH, Cologne, Germany
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
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Simon Grätzer
////////////////////////////////////////////////////////////////////////////////

#include "RestPregelHandler.h"

#include "ApplicationFeatures/ApplicationServer.h"
#include "Rest/HttpRequest.h"

#include <velocypack/Builder.h>
#include <velocypack/velocypack-aliases.h>

#include "Pregel/JobMapping.h"
#include "Pregel/Conductor.h"
#include "Pregel/Worker.h"
#include "Pregel/Utils.h"

using namespace arangodb;
using namespace arangodb::basics;
using namespace arangodb::rest;
using namespace arangodb::pregel;

RestPregelHandler::RestPregelHandler(GeneralRequest* request, GeneralResponse* response)
: RestVocbaseBaseHandler(request, response) {}

RestHandler::status RestPregelHandler::execute() {
  LOG(INFO) << "Received request\n";
  
  bool parseSuccess = true;
  std::shared_ptr<VPackBuilder> parsedBody =
  parseVelocyPackBody(&VPackOptions::Defaults, parseSuccess);
  VPackSlice body(parsedBody->start());// never nullptr
  
  if (!parseSuccess || !body.isObject()) {
    LOG(ERR) << "Bad request body\n";
    generateError(rest::ResponseCode::BAD,
                  TRI_ERROR_NOT_IMPLEMENTED, "illegal request for /_api/pregel");
  } else if (_request->requestType() == rest::RequestType::POST) {
    
    std::vector<std::string> const& suffix = _request->suffix();
    VPackSlice sExecutionNum = body.get(Utils::executionNumberKey);
    if (!sExecutionNum.isSmallInt() && !sExecutionNum.isInt()) {
      LOG(ERR) << "Invalid execution number";
      generateError(rest::ResponseCode::NOT_FOUND, TRI_ERROR_HTTP_NOT_FOUND);
      return status::DONE;
    }
    
    int executionNumber = sExecutionNum.getInt();

    if (suffix.size() != 1) {
      LOG(ERR) << "Invalid suffix";
      generateError(rest::ResponseCode::NOT_FOUND,
                    TRI_ERROR_HTTP_NOT_FOUND);
      return status::DONE;
    } else if (suffix[0] == "finishedGSS") {
      LOG(INFO) << "finishedGSS";
      Conductor *exe = JobMapping::instance()->conductor(executionNumber);
      if (exe) {
        exe->finishedGlobalStep(body);
      } else {
        LOG(ERR) << "Conductor not found\n";
      }
    } else if (suffix[0] == "nextGSS") {
      LOG(INFO) << "nextGSS";
      Worker *w = JobMapping::instance()->worker(executionNumber);
      if (!w) {// can happen if gss == 0
        LOG(INFO) << "creating worker";
        w = new Worker(executionNumber, _vocbase, body);
        JobMapping::instance()->addWorker(w, executionNumber);
      }
      w->nextGlobalStep(body);
    } else if (suffix[0] == "messages") {
      LOG(INFO) << "messages";
      Worker *exe = JobMapping::instance()->worker(executionNumber);
      if (exe) {
        exe->receivedMessages(body);
      }
    } else if (suffix[0] == "writeResults") {
      Worker *exe = JobMapping::instance()->worker(executionNumber);
      if (exe) {
        exe->writeResults();
      }
    }
    
    VPackBuilder result;
    result.add(VPackValue("thanks"));
    generateResult(rest::ResponseCode::OK, result.slice());
    
  } else {
    generateError(rest::ResponseCode::METHOD_NOT_ALLOWED,
                  TRI_ERROR_NOT_IMPLEMENTED, "illegal method for /_api/pregel");
  }
  
  return status::DONE;
}
