#include ../include/ethertrace.h

//function to enable promiscuous mode 
int set_promisc(const char *iface, bool enable){
    if (iface == NULL || iface[0] == '\0') {
        errno = EINVAL;
        perror("invalid interface name");
        return -1;
    }

    size_t iface_len = strlen(iface);

    if (iface_len >= IFNAMSIZ) {
        errno = ENAMETOOLONG;
        perror("interface name too long");
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket(AF_INET, SOCK_DGRAM)");
        return -1;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));

    memcpy(ifr.ifr_name, iface, iface_len + 1);

    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("ioctl(SIOCGIFFLAGS)");
        close(sockfd);
        return -1;
    }

    bool currently_enabled =
        (ifr.ifr_flags & IFF_PROMISC) != 0;

    // Nothing needs to be changed.
    if (currently_enabled == enable) {
        close(sockfd);
        return 0;
    }

    if (enable) {
        ifr.ifr_flags |= IFF_PROMISC;
    } else {
        ifr.ifr_flags &= (short)~IFF_PROMISC;
    }

    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
        if (enable) {
            perror("ioctl(SIOCSIFFLAGS) — need root or CAP_NET_ADMIN");
        } else {
            perror("ioctl(SIOCSIFFLAGS) — need root or CAP_NET_ADMIN");
        }

        close(sockfd);
        return -1;
    }

    close(sockfd);
    return 0;
}


