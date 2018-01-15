// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POS_MINER_H
#define BITCOIN_POS_MINER_H

#include <univalue.h>
#include "wallet/wallet.h"
#include "primitives/block.h"

static const int POS_MINING_TIMES=100;

//Oilcoin:new-rpc pos create:lf
UniValue startPosMiningThread(CWallet *pwallet);
UniValue stopPosMiningThread();
UniValue getPosMiningstatus();
void StakePOS(bool fStake, CWallet *pwallet);
UniValue minePosBlock(CWallet *pwallet);

#endif // BITCOIN_POS_MINER_H
