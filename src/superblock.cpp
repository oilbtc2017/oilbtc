// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "superblock.h"

static const uint256 SUPER_BLOCK_HASH = uint256S("48a9df367d75e52e538a80c18813a7765c3d46c934576d4e48aa11ea039db171");

bool isSuperBlock(const CBlock& block) {
    return block.GetHash() ==  SUPER_BLOCK_HASH;
}
