// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "superblock.h"

static const uint256 SUPER_BLOCK_HASH1 = uint256S("d4bac4d0aa87dd6fbdd461dec47abef1cb4b57167e2a3ee7dec75dc06ccab7c0");
static const uint256 SUPER_BLOCK_HASH2 = uint256S("46f8351cb49c5864b14ce7e484532507eb09cde709486025e2587d6840686889");
static const uint256 SUPER_BLOCK_HASH3 = uint256S("15cf01b793df0f15b519073659c6fe4b7d72b1331159ddbc04f7e3a0ac0ea972");

bool isSuperBlock(const CBlock& block) {
    return isSuperBlockHash(block.GetHash());
}

bool isSuperBlockHash(const uint256 hash) {
    return hash ==  SUPER_BLOCK_HASH1||
        hash ==  SUPER_BLOCK_HASH2 ||
        hash ==  SUPER_BLOCK_HASH3;
}
