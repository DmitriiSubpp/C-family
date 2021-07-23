#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include "pack_dump.h"

void pcap_err(const char *failed_in, const char *errbuf)
{
	printf("error in %s. %s\n", failed_in, errbuf);
	exit(1);
}

int main()
{
	struct pcap_pkthdr header;
	const unsigned char *packet;
	
	char errbuf[PCAP_ERRBUF_SIZE];
	char *device;
	
	pcap_t *pcap_handle;

	pcap_if_t *devs;
	pcap_findalldevs(&devs, errbuf);
	
	if (devs->name == NULL) pcap_err("pcap_findalldevs", errbuf);
		
	printf("sniffing on device: %s\n", devs->name);
	
	pcap_handle = pcap_open_live("any", 4096, 1, 1, errbuf); 
	// a divice arg of "any" or NULL can be used for capture packets from all interfaces
	if (pcap_handle == NULL) pcap_err("pcap_open_live", errbuf);
	
	for (int i = 0; i < 10; i++)
	{
		packet = pcap_next(pcap_handle, &header);
		if (packet == NULL) printf("packet not found\n");
		printf("got new package with len %d bytes\n", header.len);
		dump(packet, header.len);
	}
	pcap_close(pcap_handle);
}



