#include <sys/types.h>

struct system_info {
  pid_t pid;
  uid_t uid;
  gid_t gid;
  time_t startup_time;
  double sys_loads[3];
};

#define MSGTYPE_QUERY 1
#define MSGTYPE_REPLY 2
typedef struct {
  long mtype;
  char mtext[sizeof(struct system_info)];
} msgbuf_t;
