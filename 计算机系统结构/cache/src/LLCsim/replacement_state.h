#ifndef REPL_STATE_H
#define REPL_STATE_H

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is distributed as part of the Cache Replacement Championship     //
// workshop held in conjunction with ISCA'2010.                               //
//                                                                            //
//                                                                            //
// Everyone is granted permission to copy, modify, and/or re-distribute       //
// this software.                                                             //
//                                                                            //
// Please contact Aamer Jaleel <ajaleel@gmail.com> should you have any        //
// questions                                                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cassert>
#include "utils.h"
#include "crc_cache_defs.h"

// Replacement Policies Supported
typedef enum 
{
	CRC_REPL_LRU        = 0,
	CRC_REPL_RANDOM     = 1,
	CRC_REPL_FIFO       = 2,
	CRC_REPL_LFU        = 3,
	CRC_REPL_CLOCK      = 4,
	CRC_REPL_SELF       = 5,
} ReplacemntPolicy;

// Replacement State Per Cache Line
typedef struct
{
	uint64_t time;
	UINT32 LRUpos;
	UINT32 LFUcnt;
	UINT32 CLOCK;
	// CONTESTANTS: Add extra state per cache line here

} LINE_REPLACEMENT_STATE;


// The implementation for the cache replacement policy
class CACHE_REPLACEMENT_STATE
{

  private:
	UINT32 numsets;
	UINT32 assoc;
	UINT32 replPolicy;
	
	LINE_REPLACEMENT_STATE **repl;

	UINT32* clock_ptr;
	UINT32* self_ptr;
	COUNTER mytimer;  // tracks # of references to the cache

	// CONTESTANTS:  Add extra state for cache here

  public:

	// The constructor CAN NOT be changed
	CACHE_REPLACEMENT_STATE(UINT32 _sets, UINT32 _assoc, UINT32 _pol);

	INT32  GetVictimInSet(UINT32 tid, UINT32 setIndex, const LINE_STATE *vicSet, UINT32 assoc, Addr_t PC, Addr_t paddr, UINT32 accessType);
	void   UpdateReplacementState(UINT32 setIndex, INT32 updateWayID);

	void   SetReplacementPolicy(UINT32 _pol) { replPolicy = _pol; } 
	void   IncrementTimer() { mytimer++; } 

	void   UpdateReplacementState (UINT32 setIndex, INT32 updateWayID, const LINE_STATE *currLine, 
								   UINT32 tid, Addr_t PC, UINT32 accessType, bool cacheHit);

	ostream&   PrintStats (ostream &out);

  private:
	
	void   InitReplacementState();
	INT32  Get_Random_Victim(UINT32 setIndex);
	INT32  Get_LRU_Victim(UINT32 setIndex);
	INT32  Get_LFU_Victim(UINT32 setIndex);
	INT32  Get_FIFO_Victim(UINT32 setIndex);
	INT32  Get_CLOCK_Victim(UINT32 setIndex);
	INT32  Get_SELF_Victim(UINT32 setIndex);
	void   UpdateLRU(UINT32 setIndex, INT32 updateWayID);
	void   UpdateLFU(UINT32 setIndex, INT32 updateWayID);
	void   UpdateFIFO(UINT32 setIndex, INT32 updateWayID);
	void   UpdateCLOCK(UINT32 setIndex, INT32 updateWayID);
};


#endif
