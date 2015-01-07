/*
 结合这里的代码 看Linux内核 notify机制
 对netdevice 通知连的监听 
 */

#include <linux/netdevice.h>
#include <net/genetlink.h>

#include "datapath.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"

static int dp_device_event(struct notifier_block *unused, unsigned long event,
			   void *ptr)
{
	struct net_device *dev = ptr;
	struct vport *vport;
	/*OVS启动的时候 都不是internal的 所以进入else分支*/
	if (ovs_is_internal_dev(dev))
		vport = ovs_internal_dev_get_vport(dev);
	else
		vport = ovs_netdev_get_vport(dev);

	if (!vport)
		return NOTIFY_DONE;

	switch (event) {
	case NETDEV_UNREGISTER:
		if (!ovs_is_internal_dev(dev)) {
			struct sk_buff *notify;
			struct datapath *dp = vport->dp;

			notify = ovs_vport_cmd_build_info(vport, 0, 0,
							  OVS_VPORT_CMD_DEL);
			ovs_dp_detach_port(vport);
			if (IS_ERR(notify)) {
				netlink_set_err(GENL_SOCK(ovs_dp_get_net(dp)), 0,
						ovs_dp_vport_multicast_group.id,
						PTR_ERR(notify));
				break;
			}

			genlmsg_multicast_netns(ovs_dp_get_net(dp), notify, 0,
						ovs_dp_vport_multicast_group.id,
						GFP_KERNEL);
		}
		break;

	case NETDEV_CHANGENAME:   // netdevice.h
		if (vport->port_no != OVSP_LOCAL) {
			ovs_dp_sysfs_del_if(vport);
			ovs_dp_sysfs_add_if(vport);
		}
		break;
	}

	return NOTIFY_DONE;
}

struct notifier_block ovs_dp_device_notifier = {
	.notifier_call = dp_device_event
};
