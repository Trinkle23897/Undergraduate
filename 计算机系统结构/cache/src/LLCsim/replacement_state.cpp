#include "replacement_state.h"

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

/*
** This file implements the cache replacement state. Users can enhance the code
** below to develop their cache replacement ideas.
**
*/


////////////////////////////////////////////////////////////////////////////////
// The replacement state constructor:                                         //
// Inputs: number of sets, associativity, and replacement policy to use       //
// Outputs: None                                                              //
//                                                                            //
// DO NOT CHANGE THE CONSTRUCTOR PROTOTYPE                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
CACHE_REPLACEMENT_STATE::CACHE_REPLACEMENT_STATE (UINT32 _sets, UINT32 _assoc, UINT32 _pol)
{

	numsets    = _sets;
	assoc      = _assoc;
	replPolicy = _pol;

	mytimer    = 0;

	InitReplacementState();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This function initializes the replacement policy hardware by creating      //
// storage for the replacement state on a per-line/per-cache basis.           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void CACHE_REPLACEMENT_STATE::InitReplacementState()
{
	// Create the state for sets, then create the state for the ways
	repl  = new LINE_REPLACEMENT_STATE* [numsets];

	// ensure that we were able to create replacement state
	assert(repl);

	// Create the state for the sets
	for (UINT32 setIndex=0; setIndex<numsets; setIndex++) 
	{
		repl[setIndex]  = new LINE_REPLACEMENT_STATE[assoc];

		for (UINT32 way=0; way<assoc; way++) 
		{
			// initialize stack position (for true LRU)
			repl[setIndex][way].LRUpos = way;
		}
	}

	// Contestants:  ADD INITIALIZATION FOR YOUR HARDWARE HERE
	clock_ptr = new UINT32[numsets];
	self_ptr = new UINT32[numsets];
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This function is called by the cache on every cache miss. The input        //
// arguments are the thread id, set index, pointers to ways in current set    //
// and the associativity.  We are also providing the PC, physical address,    //
// and accesstype should you wish to use them at victim selection time.       //
// The return value is the physical way index for the line being replaced.    //
// Return -1 if you wish to bypass LLC.                                       //
//                                                                            //
// vicSet is the current set. You can access the contents of the set by       //
// indexing using the wayID which ranges from 0 to assoc-1 e.g. vicSet[0]     //
// is the first way and vicSet[4] is the 4th physical way of the cache.       //
// Elements of LINE_STATE are defined in crc_cache_defs.h                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
INT32 CACHE_REPLACEMENT_STATE::GetVictimInSet (UINT32 tid, UINT32 setIndex, const LINE_STATE *vicSet, UINT32 assoc,
											   Addr_t PC, Addr_t paddr, UINT32 accessType)
{
	// If no invalid lines, then replace based on replacement policy
	if (replPolicy == CRC_REPL_LRU) 
		return Get_LRU_Victim(setIndex);
	else if (replPolicy == CRC_REPL_RANDOM)
		return Get_Random_Victim(setIndex);
	else if (replPolicy == CRC_REPL_FIFO)
		return Get_FIFO_Victim(setIndex);
	else if (replPolicy == CRC_REPL_LFU)
		return Get_LFU_Victim(setIndex);
	else if (replPolicy == CRC_REPL_CLOCK)
		return Get_CLOCK_Victim(setIndex);
	else if (replPolicy == CRC_REPL_SELF)
		return Get_SELF_Victim(setIndex);
	// We should never get here
	assert(0);

	return -1; // Returning -1 bypasses the LLC
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This function is called by the cache after every cache hit/miss            //
// The arguments are: the set index, the physical way of the cache,           //
// the pointer to the physical line (should contestants need access           //
// to information of the line filled or hit upon), the thread id              //
// of the request, the PC of the request, the accesstype, and finall          //
// whether the line was a cachehit or not (cacheHit=true implies hit)         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void CACHE_REPLACEMENT_STATE::UpdateReplacementState (
	UINT32 setIndex, INT32 updateWayID, const LINE_STATE *currLine, 
	UINT32 tid, Addr_t PC, UINT32 accessType, bool cacheHit)
{
	// What replacement policy?
	if (replPolicy == CRC_REPL_LRU) 
		UpdateLRU(setIndex, updateWayID);
	else if (replPolicy == CRC_REPL_RANDOM)
		; // Random replacement requires no replacement state update
	else if (replPolicy == CRC_REPL_FIFO)
		UpdateFIFO(setIndex, updateWayID);
	else if (replPolicy == CRC_REPL_LFU)
		UpdateLFU(setIndex, updateWayID);
	else if (replPolicy == CRC_REPL_CLOCK)
		UpdateCLOCK(setIndex, updateWayID);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//////// HELPER FUNCTIONS FOR REPLACEMENT UPDATE AND VICTIM SELECTION //////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This function finds the LRU victim in the cache set by returning the       //
// cache block at the bottom of the LRU stack. Top of LRU stack is '0'        //
// while bottom of LRU stack is 'assoc-1'                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
INT32 CACHE_REPLACEMENT_STATE::Get_LRU_Victim(UINT32 setIndex) {
	for (UINT32 w = 0; w < assoc; ++w) 
		if (repl[setIndex][w].LRUpos == assoc - 1)
			return w;
	return -1;
}

INT32 CACHE_REPLACEMENT_STATE::Get_FIFO_Victim(UINT32 setIndex) {
	INT32 id = 0;
	for (UINT32 w = 0; w < assoc; ++w)
		if (repl[setIndex][w].time < repl[setIndex][id].time)
			id = w;
	repl[setIndex][id].time = mytimer;
	return id;
}

INT32 CACHE_REPLACEMENT_STATE::Get_LFU_Victim(UINT32 setIndex) {
	INT32 id = 0;
	for (UINT32 w = 0; w < assoc; ++w)
		if (repl[setIndex][w].LFUcnt < repl[setIndex][id].LFUcnt)
			id = w;
	repl[setIndex][id].LFUcnt = 0;
	return id;
}

INT32 CACHE_REPLACEMENT_STATE::Get_CLOCK_Victim(UINT32 setIndex) {
	UINT32 ptr = clock_ptr[setIndex];
	for (; repl[setIndex][ptr].CLOCK; ptr = (ptr + 1) % assoc)
		repl[setIndex][ptr].CLOCK = 0;
	clock_ptr[setIndex] = (ptr + 1) % assoc;
	return ptr;
}

INT32 CACHE_REPLACEMENT_STATE::Get_SELF_Victim(UINT32 setIndex) {
	INT32 id = (self_ptr[setIndex] / assoc) & 1 ? assoc - 1 - self_ptr[setIndex] % assoc : self_ptr[setIndex] % assoc;
	++self_ptr[setIndex];
	return id;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This function finds a random victim in the cache set                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
INT32 CACHE_REPLACEMENT_STATE::Get_Random_Victim(UINT32 setIndex)
{
	INT32 way = (rand() % assoc);
	return way;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This function implements the LRU update routine for the traditional        //
// LRU replacement policy. The arguments to the function are the physical     //
// way and set index.                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void CACHE_REPLACEMENT_STATE::UpdateLRU(UINT32 setIndex, INT32 updateWayID) {
	// Determine current LRU stack position
	UINT32 currLRUpos = repl[setIndex][updateWayID].LRUpos;
	// Update the stack position of all lines before the current line
	// Update implies incremeting their stack positions by one
	for (UINT32 way=0; way<assoc; way++) 
		if (repl[setIndex][way].LRUpos < currLRUpos)
			repl[setIndex][way].LRUpos++;
	// Set the LRU stack position of new line to be zero
	repl[setIndex][updateWayID].LRUpos = 0;
}

void CACHE_REPLACEMENT_STATE::UpdateLFU(UINT32 setIndex, INT32 updateWayID) {
	++repl[setIndex][updateWayID].LFUcnt;
}

void CACHE_REPLACEMENT_STATE::UpdateFIFO(UINT32 setIndex, INT32 updateWayID) {
	// repl[setIndex][updateWayID].time = mytimer;
}

void CACHE_REPLACEMENT_STATE::UpdateCLOCK(UINT32 setIndex, INT32 updateWayID) {
	repl[setIndex][updateWayID].CLOCK = 1;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// The function prints the statistics for the cache                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
ostream & CACHE_REPLACEMENT_STATE::PrintStats(ostream &out)
{

	out<<"=========================================================="<<endl;
	out<<"=========== Replacement Policy Statistics ================"<<endl;
	out<<"=========================================================="<<endl;

	// CONTESTANTS:  Insert your statistics printing here

	return out;
	
}

