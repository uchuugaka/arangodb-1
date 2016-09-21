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
/// @author Dr. Frank Celler
////////////////////////////////////////////////////////////////////////////////

#include "RestEngine.h"

#include "GeneralServer/RestHandler.h"
#include "Logger/Logger.h"

using namespace arangodb;
using namespace arangodb::rest;

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

RestEngine::RestEngine() {}

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

void RestEngine::asyncRun(std::shared_ptr<rest::RestHandler> handler) {
  while (true) {
    int res = TRI_ERROR_INTERNAL;

    switch (_state) {
      case State::PREPARE:
        res = handler->prepareEngine();
        break;

      case State::EXECUTE:
        res = handler->executeEngine();
        break;

      case State::RUN:
        res = handler->runEngine();
        break;

      case State::FINALIZE:
        res = handler->finalizeEngine();
        break;

      case State::DONE:
      case State::FAILED:
        return;
    }

    if (res != TRI_ERROR_NO_ERROR) {
      return;
    }
  }
}

RestStatus RestEngine::syncRun(std::shared_ptr<rest::RestHandler> handler) {}

void RestEngine::appendRestStatus(std::shared_ptr<RestStatusElement> element) {
  while (element.get() != nullptr) {
    _elements.emplace_back(element);
    element = element->previous();
  }
}
