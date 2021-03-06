/*
 * network.h
 *
 *  Created on: Jul 29, 2017
 *      Author: wangyu
 */

#ifndef UDP2RAW_NETWORK_H_
#define UDP2RAW_NETWORK_H_

extern int raw_recv_fd;
extern int raw_send_fd;
extern int seq_mode;
extern int max_seq_mode;
extern int filter_port;
//extern u32_t bind_address_uint32;
extern int disable_bpf_filter;

extern int lower_level;
extern int lower_level_manual;
extern char if_name[100];
extern char dev[100];
extern unsigned char dest_hw_addr[];

extern int random_drop;

extern int ifindex;

extern char g_packet_buf[buf_len];
extern int g_packet_buf_len;
extern int g_packet_buf_cnt;

struct my_ip6hdr
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t traffic_class_high:4;
    uint8_t version:4;
    uint8_t flow_label_high:4;
    uint8_t traffic_class_low:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t version:4;
    uint8_t traffic_class_high:4;
    uint8_t traffic_class_low:4;
    uint8_t flow_label_high:4;
#else
# error	"Please fix this"
#endif
    u_int16_t flow_label_low;
    u_int16_t payload_len;
    uint8_t next_header;
    uint8_t hop_limit;

    struct in6_addr src;
    struct in6_addr dst;
  };

struct icmphdr
{
	uint8_t type;
	uint8_t code;
	uint16_t check_sum;
	uint16_t id;
	uint16_t seq;
};

struct pseudo_header {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

struct pseudo_header6 {
    struct in6_addr src;
    struct in6_addr dst;
    u_int32_t tcp_length;
    u_int16_t placeholder1;
    u_int8_t placeholder2;
    u_int8_t next_header;
};

struct packet_info_t  //todo change this to union
{
	uint8_t protocol;


	//u32_t src_ip;
	//u32_t dst_ip;
	my_ip_t new_src_ip;
	my_ip_t new_dst_ip;

	uint16_t src_port;
	uint16_t dst_port;

	//tcp_part:
	bool syn,ack,psh,rst;

	u32_t seq,ack_seq;

	u32_t ack_seq_counter;

	u32_t ts,ts_ack;


	uint16_t icmp_seq;

	bool has_ts;

	sockaddr_ll addr_ll;

	i32_t data_len;

	packet_info_t();
};

struct raw_info_t
{
	packet_info_t send_info;
	packet_info_t recv_info;
	//int last_send_len;
	//int last_recv_len;
	bool peek=0;
	//bool csum=1;
	u32_t reserved_send_seq;
	//uint32_t first_seq,first_ack_seq;
	int rst_received=0;
	bool disabled=0;

};//g_raw_info;


int init_raw_socket();

void init_filter(int port);

void remove_filter();

int init_ifindex(const char * if_name,int fd,int &index);

int find_lower_level_info(u32_t ip,u32_t &dest_ip,string &if_name,string &hw);

int get_src_adress(u32_t &ip,u32_t remote_ip_uint32,int remote_port);  //a trick to get src adress for a dest adress,so that we can use the src address in raw socket as source ip

int get_src_adress2(address_t &output_addr,address_t remote_addr);

int try_to_list_and_bind(int & bind_fd,u32_t local_ip_uint32,int port);  //try to bind to a port,may fail.

int client_bind_to_a_new_port(int & bind_fd,u32_t local_ip_uint32);//find a free port and bind to it.
int client_bind_to_a_new_port2(int &fd,const address_t& address);

int discard_raw_packet();
int pre_recv_raw_packet();

int send_raw_ip(raw_info_t &raw_info,const char * payload,int payloadlen);

int peek_raw(raw_info_t &peek_info);

int recv_raw_ip(raw_info_t &raw_info,char * &payload,int &payloadlen);

int send_raw_icmp(raw_info_t &raw_info, const char * payload, int payloadlen);

int send_raw_udp(raw_info_t &raw_info, const char * payload, int payloadlen);

int send_raw_tcp(raw_info_t &raw_info,const char * payload, int payloadlen);

int recv_raw_icmp(raw_info_t &raw_info, char *&payload, int &payloadlen);

int recv_raw_udp(raw_info_t &raw_info, char *&payload, int &payloadlen);

int recv_raw_tcp(raw_info_t &raw_info,char * &payload,int &payloadlen);

//int send_raw(raw_info_t &raw_info,const char * payload,int payloadlen);

//int recv_raw(raw_info_t &raw_info,char * &payload,int &payloadlen);

int send_raw0(raw_info_t &raw_info,const char * payload,int payloadlen);

int recv_raw0(raw_info_t &raw_info,char * &payload,int &payloadlen);

int after_send_raw0(raw_info_t &raw_info);

int after_recv_raw0(raw_info_t &raw_info);


#endif /* NETWORK_H_ */
