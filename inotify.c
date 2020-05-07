#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

static void displayInotifyEvent(struct inotify_event *i) {
	printf("    wd =%2d; ", i->wd);
	if (i->cookie > 0)
		printf("cookie =%4d; ", i->cookie);

	printf("mask = ");
	if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
	if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
	if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
	if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
	if (i->mask & IN_CREATE)        printf("IN_CREATE ");
	if (i->mask & IN_DELETE)        printf("IN_DELETE ");
	if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
	if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
	if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
	if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
	if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
	if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
	if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
	if (i->mask & IN_OPEN)          printf("IN_OPEN ");
	if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
	if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
	printf("\n");
 
	if (i->len > 0)
		printf("        name = %s\n", i->name);
}
 
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
 
int main(void) {
	int inotifyFd, wd, j;
	char buf[BUF_LEN] __attribute__ ((aligned(8)));
	ssize_t numRead;
	char *p;
	struct inotify_event *event;

	inotifyFd = inotify_init();                 /* Create inotify instance */
	if (inotifyFd == -1)
		printf("%s\n", );("inotify_init");

	wd = inotify_add_watch(inotifyFd, "/home/a201603014", IN_ALL_EVENTS);
	if (wd == -1)
		printf("inotify_add_watch");


	for (;;) {                                  /* Read events forever */
		numRead = read(inotifyFd, buf, BUF_LEN);
		if (numRead == 0)
			printf("read() from inotify fd returned 0!");
		if (numRead == -1)
			printf("read");
		printf("Read %ld bytes from inotify fd\n", (long) numRead);

		/* Process all of the events in buffer returned by read() */

		for (p = buf; p < buf + numRead; ) {
			event = (struct inotify_event *) p;
			displayInotifyEvent(event);
			p += sizeof(struct inotify_event) + event->len;
		}
	}

	exit(EXIT_SUCCESS);
}