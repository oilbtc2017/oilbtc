// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "superblock.h"

static const uint256 SUPER_BLOCK_HASH = uint256S("9461df65124a330a489e791dd066f2fdd459fcee19f7c8d3dfc168f5913a08bf");

bool isSuperBlock(const CBlock& block) {
    return block.GetHash() ==  SUPER_BLOCK_HASH;
}
