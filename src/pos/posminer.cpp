// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pos/posminer.h"
#include "amount.h"
#include "chain.h"
#include "chainparams.h"
#include "coins.h"
#include "consensus/consensus.h"
#include "consensus/tx_verify.h"
#include "consensus/merkle.h"
#include "consensus/validation.h"
#include "hash.h"
#include "validation.h"
#include "primitives/transaction.h"
#include "script/standard.h"
#include "timedata.h"
#include "txmempool.h"
#include "util.h"
#include "utilmoneystr.h"
#include "validationinterface.h"
#include "pos/pos.h"
#include "miner.h"
#include "wallet/wallet.h"

#include <algorithm>
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>
#include <utility>

int64_t nLastCoinStakeSearchInterval = 0;

static bool isMining = false;
boost::mutex mutex_;

//Oilcoin:Gerald
UniValue minePosBlock(CWallet *pwallet) {

    if (pwallet->IsLocked()){
        return NullUniValue;
    }

    UniValue blockHashes(UniValue::VOBJ);
    CReserveKey reservekey(pwallet);
    if(pwallet->HaveAvailableCoinsForStaking() == false ) {
        LogPrintf("wallet doesn't have available coins for staking\n");
        return blockHashes;
    }

    int64_t nTotalFees = 0;
    std::unique_ptr<CBlockTemplate> pblocktemplate(BlockAssembler(Params())
            .CreateEmptyBlock(reservekey.reserveScript, &nTotalFees));
    if (!pblocktemplate.get())
        return NullUniValue;

    CBlockIndex* pindexPrev =  chainActive.Tip();
    uint32_t beginningTime=GetAdjustedTime();
    beginningTime &= ~STAKE_TIMESTAMP_MASK;

    for(uint32_t i=beginningTime;i<beginningTime + POS_MINING_TIMES; i+=STAKE_TIMESTAMP_MASK+1) {
        static int64_t nLastCoinStakeSearchTime = GetAdjustedTime(); // startup timestamp
        nLastCoinStakeSearchInterval = i - nLastCoinStakeSearchTime;

        pblocktemplate->block.nTime = i;
        std::shared_ptr<CBlock> pblock = std::make_shared<CBlock>(pblocktemplate->block);

        if (SignBlock(pblock, *pwallet, nTotalFees, i) == false ) {
            continue;
        }

        if (chainActive.Tip()->GetBlockHash() != pblock->hashPrevBlock) {
            LogPrintf("minePosBlock: Valid future PoS block was orphaned before becoming valid");
            break;
        }

        std::unique_ptr<CBlockTemplate> pblocktemplatefilled(
                BlockAssembler(Params()).CreateNewPosBlock(pblock->vtx[1]->vout[1].scriptPubKey, true, &nTotalFees,
                    i, FutureDrift(GetAdjustedTime()) - STAKE_TIME_BUFFER));
        if (!pblocktemplatefilled.get()){
            return NullUniValue;
        }

        if (chainActive.Tip()->GetBlockHash() != pblock->hashPrevBlock) {
            //another block was received while building ours, scrap progress
            LogPrintf("minePosBlock: Valid future PoS block was orphaned before becoming valid");
            break;
        }

        std::shared_ptr<CBlock> pblockfilled = std::make_shared<CBlock>(pblocktemplatefilled->block);
        if (SignBlock(pblockfilled, *pwallet, nTotalFees, i) == false ) {
            break;
        }

        bool validBlock = false;
        while(!validBlock) {
            if (chainActive.Tip()->GetBlockHash() != pblockfilled->hashPrevBlock) {
                LogPrintf("minePosBlock: Valid future PoS block was orphaned before becoming valid");
                break;
            }

            if (pblockfilled->GetBlockTime() <= pindexPrev->GetBlockTime() ||
                    FutureDrift(pblockfilled->GetBlockTime()) < pindexPrev->GetBlockTime()) {
                LogPrintf("minePosBlock: Valid PoS block took too long to create and has expired");
                break; 
            }

            if (pblockfilled->GetBlockTime() > FutureDrift(GetAdjustedTime())) {
                MilliSleep(3000);
                continue;
            }
            validBlock = true;
        }

        if(validBlock) {
            CheckStake(pblockfilled, *pwallet);
            nLastCoinStakeSearchTime = pblockfilled->GetBlockTime();
            blockHashes.push_back(Pair("blockhash",pblockfilled->GetHash().GetHex()));
        }
    }

    return blockHashes;
}

//Oilcoin:new-rpc pos create:lf getPosMiningstatus
UniValue getPosMiningstatus(){
    boost::mutex::scoped_lock lock(mutex_);
    UniValue ret(UniValue::VOBJ);
    ret.push_back(Pair("ismining",isMining));
    return ret;
}

//Oilcoin:new-rpc pos create:lf startPosMiningThread
UniValue startPosMiningThread(CWallet *pwallet){
    boost::mutex::scoped_lock lock(mutex_);
    UniValue ret(UniValue::VOBJ);
    if(isMining){
        ret.push_back(Pair("startMing",false));
    }else {
        isMining = true;
        StakePOS(isMining,pwallet);
    }
    ret.push_back(Pair("startMing",true));
    return ret;
}

//Oilcoin:new-rpc pos create:lf stopPosMiningThread
UniValue stopPosMiningThread(){
    boost::mutex::scoped_lock lock(mutex_);
    isMining = false;
    UniValue ret(UniValue::VOBJ);
    ret.push_back(Pair("stopMing", isMining));
    return ret;
}

//Oilcoin:new-rpc pos create:lf StakeMinerThread
void StakeMinerThread(CWallet *pwallet){
    SetThreadPriority(THREAD_PRIORITY_LOWEST);
    // Make this thread recognisable as the mining thread
    RenameThread("Oilcoin-miner");
	//启动线程无限循环
    while (isMining)
    {
		//检查钱包锁定-》sleep
        while (pwallet->IsLocked())
        {
            nLastCoinStakeSearchInterval = 0;
            MilliSleep(10000);
        }
        if(!minePosBlock(pwallet).isNull()){
            boost::mutex::scoped_lock lock(mutex_);
            //isMining = false;
        }else {
            MilliSleep(1000);
        }
        
    }
}

void StakePOS(bool fStake, CWallet *pwallet){
    static boost::thread_group* stakeThread = NULL;

   if (stakeThread != NULL)
    {
        stakeThread->interrupt_all();
       delete stakeThread;
        stakeThread = NULL;
    }

   if(fStake)
    {
       stakeThread = new boost::thread_group();
       stakeThread->create_thread(boost::bind(&StakeMinerThread, pwallet));
    }
}
