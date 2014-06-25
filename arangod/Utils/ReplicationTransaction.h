////////////////////////////////////////////////////////////////////////////////
/// @brief replication transaction
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
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
/// @author Jan Steemann
/// @author Copyright 2014, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_UTILS_REPLICATION_TRANSACTION_H
#define ARANGODB_UTILS_REPLICATION_TRANSACTION_H 1

#include "Basics/Common.h"

#include "Utils/RestTransactionContext.h"
#include "Utils/Transaction.h"
#include "VocBase/server.h"
#include "VocBase/transaction.h"

struct TRI_vocbase_s;

namespace triagens {
  namespace arango {

    class ReplicationTransaction : public Transaction<RestTransactionContext> {

// -----------------------------------------------------------------------------
// --SECTION--                                      class ReplicationTransaction
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief create the transaction
////////////////////////////////////////////////////////////////////////////////

        ReplicationTransaction (struct TRI_vocbase_s* vocbase)
          : Transaction<RestTransactionContext>(vocbase) {
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief end the transaction
////////////////////////////////////////////////////////////////////////////////

        ~ReplicationTransaction () {
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief get a collection by id
/// this will automatically add the collection to the transaction 
////////////////////////////////////////////////////////////////////////////////

        inline TRI_transaction_collection_t* trxCollection (TRI_voc_cid_t cid) {
          TRI_ASSERT(cid > 0);

          TRI_transaction_collection_t* trxCollection = TRI_GetCollectionTransaction(this->_trx, cid, TRI_TRANSACTION_WRITE);

          if (trxCollection == nullptr) {
            int res = TRI_AddCollectionTransaction(this->_trx, cid, TRI_TRANSACTION_WRITE, 0, true);

            if (res == TRI_ERROR_NO_ERROR) {
              res = TRI_LockCollectionTransaction(trxCollection, TRI_TRANSACTION_WRITE, 0);
            }
              
            if (res != TRI_ERROR_NO_ERROR) {
              return nullptr;
            }
          
            trxCollection = TRI_GetCollectionTransaction(this->_trx, cid, TRI_TRANSACTION_WRITE);
          }
 
          return nullptr;
        }

    };

  }
}

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
