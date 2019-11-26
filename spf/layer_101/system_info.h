#include <sys/types.h>

struct system_info {
  pid_t pid;
  uid_t uid;
  gid_t gid;
  time_t startup_time;
  double sys_loads[3];
};
