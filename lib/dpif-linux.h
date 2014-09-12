
#ifndef DPIF_LINUX_H
#define DPIF_LINUX_H 1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <linux/openvswitch.h>

struct ofpbuf;

/*代表ovs中虚拟端口信息，比如说构造一个vport，而后通过netlink
  发送到内核空间，看函数 dpif_linux_port_add
*/
struct dpif_linux_vport {
    /* Generic Netlink header. */
    uint8_t cmd;

    /* ovs_vport header. */
    int dp_ifindex;
    uint32_t port_no;                      /* UINT32_MAX if unknown. */
    enum ovs_vport_type type;

    /* Attributes.
     *
     * The 'stats' member points to 64-bit data that might only be aligned on
     * 32-bit boundaries, so use get_unaligned_u64() to access its values.
     */
    const char *name;                      /* OVS_VPORT_ATTR_NAME. */
    const uint32_t *upcall_pid;            /* OVS_VPORT_ATTR_UPCALL_PID. */
    const struct ovs_vport_stats *stats;   /* OVS_VPORT_ATTR_STATS. */
    const uint8_t *address;                /* OVS_VPORT_ATTR_ADDRESS. */
    const struct nlattr *options;          /* OVS_VPORT_ATTR_OPTIONS. */
    size_t options_len;
};

void dpif_linux_vport_init(struct dpif_linux_vport *);

int dpif_linux_vport_transact(const struct dpif_linux_vport *request,
                              struct dpif_linux_vport *reply,
                              struct ofpbuf **bufp);
int dpif_linux_vport_get(const char *name, struct dpif_linux_vport *reply,
                         struct ofpbuf **bufp);

bool dpif_linux_is_internal_device(const char *name);

int dpif_linux_vport_send(int dp_ifindex, uint32_t port_no,
                          const void *data, size_t size);

#endif /* dpif-linux.h */
