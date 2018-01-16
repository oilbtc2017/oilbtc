// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SUPERBLOCK_H
#define BITCOIN_SUPERBLOCK_H


#include "primitives/block.h"


bool isSuperBlock(const CBlock& block); 
bool isSuperBlockHash(const uint256 hash);


#endif // BITCOIN_SUPERBLOCK_H
