// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "superblock.h"

static const uint256 SUPER_BLOCK_HASH1 = uint256S("335aa7e944cd5fa05838e295fb898131e14daba0152dcd9c9cb7f07ccebfa0c3");
static const uint256 SUPER_BLOCK_HASH2 = uint256S("76daf69059b30f1a5f97c5fb7e1d934a2231771a5238f08c74fa8afedb932b82");
static const uint256 SUPER_BLOCK_HASH3 = uint256S("7591ece9a3ed9f2f3b52d16dc0e660bf3de3f2802151ee8ec748bf007a639db2");

bool isSuperBlock(const CBlock& block) {
    return isSuperBlockHash(block.GetHash());
}

bool isSuperBlockHash(const uint256 hash) {
    return hash ==  SUPER_BLOCK_HASH1||
        hash ==  SUPER_BLOCK_HASH2 ||
        hash ==  SUPER_BLOCK_HASH3;
}
