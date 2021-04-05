#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int print_state(pid_t pid) {
  char buffer[4096];
  int ret = snprintf(buffer, sizeof(buffer), "/proc/%d/status", pid);
  if (ret < 0) {
    return ret;
  }
  int fd = open(buffer, O_RDONLY);
  if (fd < 0) {
    return fd;
  }
  ret = read(fd, buffer, sizeof(buffer));
  if (ret < 0) {
    return ret;
  }
  close(fd);

  char state_prefix[] = "State:\t";
  for (int i = 0; i < sizeof(buffer); ++i) {
    size_t remaining = sizeof(buffer) - i;
    size_t state_len = sizeof(state_prefix) - 1;
    if (remaining < state_len) {
      break;
    }
    if (strncmp(buffer + i, state_prefix, state_len) == 0) {
      for (int j = (i + state_len); j < sizeof(buffer); ++j) {
	ret = printf("%c", buffer[j]);
	if (ret < 0) {
	  return ret;
	}
	if (buffer[j] == '\n') {
	  break;
	}
      }
      break;
    }
  }

  return 0;
}

int main(int argc, char *argv[])
{
  pid_t pid = fork();
  if (pid == -1) {
    return errno;
  }
  if (pid == 0) {
    sleep(2);
  }
  else {
    int ret;
    sleep(1);
    printf("Child process state: ");
    ret = print_state(pid);
    if (ret < 0) { return errno; }
    sleep(2);
    printf("Child process state: ");
    ret = print_state(pid);
    if (ret < 0) { return errno; }
  }
  return 0;
}
