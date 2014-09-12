
#ifndef VPORT_INTERNAL_DEV_H
#define VPORT_INTERNAL_DEV_H 1

#include "datapath.h"
#include "vport.h"

int ovs_is_internal_dev(const struct net_device *);
struct vport *ovs_internal_dev_get_vport(struct net_device *);

#endif /* vport-internal_dev.h */
