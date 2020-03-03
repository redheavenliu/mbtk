#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <asm/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/route.h>
#define BUFLEN 20480

#define t_assert(x) { \
    if(!(x))  {err = -__LINE__;goto error;} \
}

/*Ctrl + C �˳�*/
static volatile int keepRunning = 1;

void intHandler(int dummy)
{
    keepRunning = 0;
}

/**
 * ����RTA,������tb
 */
void parse_rtattr(struct rtattr **tb, int max, struct rtattr *attr, int len)
{
    for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len))
    {
        if (attr->rta_type <= max)
        {
            tb[attr->rta_type] = attr;
        }
    }
}

/**
 * ��ʾ������Ϣ
 * �������䶯��ʱ�򴥷������Ϣ,�����/������,��/�������豸,����/���ýӿڵ�.
 */
void print_ifinfomsg(struct nlmsghdr *nh)
{
    int len;
    struct rtattr *tb[IFLA_MAX + 1];
    struct ifinfomsg *ifinfo;
    bzero(tb, sizeof(tb));
    ifinfo = NLMSG_DATA(nh);


    printf("nlmsghdr:%d,%d,%d,%d,%d\n",nh->nlmsg_len,
        nh->nlmsg_type,
        nh->nlmsg_flags,
        nh->nlmsg_seq,
        nh->nlmsg_pid);

    printf("ifinfomsg:%d,%d,%d,%d,%d,%d\n",ifinfo->ifi_family,
        ifinfo->__ifi_pad,
        ifinfo->ifi_type,
        ifinfo->ifi_index,
        ifinfo->ifi_flags,
        ifinfo->ifi_change);


    len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));
    parse_rtattr(tb, IFLA_MAX, IFLA_RTA (ifinfo), len);
    printf("%s: %s ", (nh->nlmsg_type==RTM_NEWLINK)?"NEWLINK":"DELLINK", (ifinfo->ifi_flags & IFF_UP) ? "up" : "down");

    // eth0
    if(tb[IFLA_IFNAME])
    {
        printf("%s", RTA_DATA(tb[IFLA_IFNAME]));
    }

    printf("\n");
}

/**
 * ��ʾ��ַ��Ϣ
 * ����ַ�䶯��ʱ�򴥷������Ϣ,����ͨ��DHCP��ȡ����ַ��
 */
void print_ifaddrmsg(struct nlmsghdr *nh)
{
    int len;
    struct rtattr *tb[IFA_MAX + 1];
    struct ifaddrmsg *ifaddr;
    char tmp[256];
    bzero(tb, sizeof(tb));
    ifaddr = NLMSG_DATA(nh);
    len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifaddr));
    parse_rtattr(tb, IFA_MAX, IFA_RTA (ifaddr), len);

    printf("%s ", (nh->nlmsg_type==RTM_NEWADDR)?"NEWADDR":"DELADDR");
    if (tb[IFA_LABEL] != NULL)
    {
        printf("%s ", RTA_DATA(tb[IFA_LABEL]));
    }
    if (tb[IFA_ADDRESS] != NULL)
    {
        inet_ntop(ifaddr->ifa_family, RTA_DATA(tb[IFA_ADDRESS]), tmp, sizeof(tmp));
        printf("%s ", tmp);
    }
    printf("\n");
}

/**
 * ��ʾ·����Ϣ
 * ��·�ɱ䶯��ʱ�򴥷������Ϣ
 */
void print_rtmsg(struct nlmsghdr *nh)
{
    int len;
    struct rtattr *tb[RTA_MAX + 1];
    struct rtmsg *rt;
    char tmp[256];
    bzero(tb, sizeof(tb));
    rt = NLMSG_DATA(nh);

    printf("nlmsghdr:%d,%d,%d,%d,%d\n",nh->nlmsg_len,
        nh->nlmsg_type,
        nh->nlmsg_flags,
        nh->nlmsg_seq,
        nh->nlmsg_pid);

    printf("rtmsg:%d,%d,%d,%d,%d,%d,%d,%d,%d\n",rt->rtm_family,
        rt->rtm_dst_len,
        rt->rtm_src_len,
        rt->rtm_tos,
        rt->rtm_table,
        rt->rtm_protocol,
        rt->rtm_scope,
        rt->rtm_type,
        rt->rtm_flags);

    len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*rt));
    parse_rtattr(tb, RTA_MAX, RTM_RTA(rt), len);
    printf("%s: ", (nh->nlmsg_type==RTM_NEWROUTE)?"NEWROUT":"DELROUT");
    if (tb[RTA_DST] != NULL)
    {
        inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_DST]), tmp, sizeof(tmp));
        printf("RTA_DST %s ", tmp);
    }
    if (tb[RTA_SRC] != NULL)
    {
        inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_SRC]), tmp, sizeof(tmp));
        printf("RTA_SRC %s ", tmp);
    }
    if (tb[RTA_GATEWAY] != NULL)
    {
        inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_GATEWAY]), tmp, sizeof(tmp));
        printf("RTA_GATEWAY %s ", tmp);
    }

    printf("\n");
}
int main(int argc, char *argv[])
{
    int socket_fd;
    int err = 0;
    fd_set rd_set;
    struct timeval timeout;
    int select_r;
    int read_r;
    struct sockaddr_nl sa;
    struct nlmsghdr *nh;


    int len = BUFLEN;
    char buff[2048];
    signal(SIGINT, intHandler);

    /*��NetLink Socket*/
    socket_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    t_assert(socket_fd > 0);
    t_assert(!setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &len, sizeof(len)));

    /*�趨�������Ͳ���Socket*/
    bzero(&sa, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;// | RTMGRP_IPV6_IFADDR | RTMGRP_IPV6_ROUTE;
    t_assert(!bind(socket_fd, (struct sockaddr *) &sa, sizeof(sa)));

    while (keepRunning)
    {
        FD_ZERO(&rd_set);
        FD_SET(socket_fd, &rd_set);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        select_r = select(socket_fd + 1, &rd_set, NULL, NULL, &timeout);
        if (select_r < 0)
        {
            perror("select");
        }
        else if (select_r > 0)
        {
            if (FD_ISSET(socket_fd, &rd_set))
            {
                read_r = read(socket_fd, buff, sizeof(buff));
                for (nh = (struct nlmsghdr *) buff; NLMSG_OK(nh, read_r); nh = NLMSG_NEXT(nh, read_r))
                {
                    switch (nh->nlmsg_type)
                    {
                        default:
                            /*�յ�Щ��ֵ���Ϣ*/
                            printf("nh->nlmsg_type = %d\n", nh->nlmsg_type);
                            break;
                        case NLMSG_DONE:
                        case NLMSG_ERROR:
                            break;
                        case RTM_NEWLINK:
                        case RTM_DELLINK:
                            print_ifinfomsg(nh);
                            break;
                        case RTM_NEWADDR:
                        case RTM_DELADDR:
                            print_ifaddrmsg(nh);
                            break;
                        case RTM_NEWROUTE:
                        case RTM_DELROUTE:
                            print_rtmsg(nh);
                            break;
                    }

                }
            }
        }
    }

    close(socket_fd);

error:
    if (err < 0)
    {
        printf("Error at line %d\nErrno=%d\n", -err, errno);
    }
    return err;
}


