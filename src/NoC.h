/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2018 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file represents the top-level testbench
 */

#ifndef __NOXIMNOC_H__
#define __NOXIMNOC_H__

#include <systemc.h>
#include "Tile.h"
#include "GlobalRoutingTable.h"
#include "GlobalTrafficTable.h"
#include "Hub.h"
#include "Channel.h"
#include "TokenRing.h"

using namespace std;

template <typename T>
struct sc_signal_NSWE
{
    sc_signal<T> east;
    sc_signal<T> west;
    sc_signal<T> south;
    sc_signal<T> north;
};

template <typename T>
struct sc_signal_NSWEH
{
    sc_signal<T> east;
    sc_signal<T> west;
    sc_signal<T> south;
    sc_signal<T> north;
    sc_signal<T> to_hub;
    sc_signal<T> from_hub;
};


SC_MODULE(NoC)
{
    // I/O Ports
    sc_in_clk clock;		// The input clock for the NoC
    sc_in < bool > reset;	// The reset signal for the NoC

    // Signals mesh/switchBlocButterfly
    sc_signal_NSWEH<bool> **req;
    sc_signal_NSWEH<bool> **ack;
    sc_signal_NSWEH<TBufferFullStatus> **buffer_full_status;
    sc_signal_NSWEH<Flit> **flit;
    sc_signal_NSWE<int> **free_slots;

    // Signals core butterfly
    /*sc_signal_NSWEH<bool> *req_bf;
    sc_signal_NSWEH<bool> *ack_bf;
    sc_signal_NSWEH<TBufferFullStatus> *buffer_full_status_bf;
    sc_signal_NSWEH<Flit> *flit_bf;
    sc_signal_NSWE<int> *free_slots_bf;*/

    // NoP
    sc_signal_NSWE<NoP_data> **nop_data;
   // sc_signal_NSWE<NoP_data> *nop_data_bf;

    // Matrix of tiles
    Tile ***t;
    Tile ** core;

    map<int, Hub*> hub;
    map<int, Channel*> channel;

    TokenRing* token_ring;

    // Global tables
    GlobalRoutingTable grtable;
    GlobalTrafficTable gttable;


    // Constructor

    SC_CTOR(NoC) {

	// Build the Mesh
	buildMesh();
	
	GlobalParams::channel_selection = CHSEL_RANDOM;
	// out of yaml configuration (experimental features)
	//GlobalParams::channel_selection = CHSEL_FIRST_FREE;

	if (GlobalParams::ascii_monitor)
	{
	    SC_METHOD(asciiMonitor);
	    sensitive << clock.pos();
	}

    }

    // Support methods
    Tile *searchNode(const int id) const;

  private:

    void buildMesh();
    void buildButterfly();
    void buildCommon();
    void asciiMonitor();
    int * hub_connected_ports;
};

//Hub * dd;

#endif
