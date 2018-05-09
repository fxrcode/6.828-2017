#include "ns.h"
#include "inc/lib.h"

extern union Nsipc nsipcbuf;

void
input(envid_t ns_envid)
{
	binaryname = "ns_input";

	// LAB 6: Your code here:
	// 	- read a packet from the device driver
	//	- send it to the network server
	// Hint: When you IPC a page to the network server, it will be
	// reading from it for a while, so don't immediately receive
	// another packet in to the same physical page.
	uint32_t req;
	int perm, r;
	while (1) {
		// 1. call sys_pkt_recv
		// 2. ipc_send to network core
		memset(&nsipcbuf, 0, sizeof(union Nsipc));
		sys_pkt_recv(&nsipcbuf);
		while((r = sys_ipc_try_send(ns_envid, NSREQ_INPUT,
					    &nsipcbuf,
					    PTE_U | PTE_P)) != 0) {
			if(r == -E_IPC_NOT_RECV)
				sys_yield();
			else
				panic("ipc_send: %e", r);
		}
	}
}
