#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

int main(void)
{
	int fd, wd;
	char buffer[4096];

	fd = inotify_init();
	wd = inotify_add_watch(fd, "/",
		IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_CLOSE_WRITE);

	ssize_t len;
	char *ptr;
	const struct inotify_event *event;

	printf("MONITOR INICIADO\n");

	while(1) {
		len = read(fd, buffer, 4096);
		for(ptr = buffer;
			ptr < buffer + len;
			ptr += sizeof (struct inotify_event) + event->len) {
			if (event->mask & IN_CREATE)
				printf ("Creado: %s\n", event->name);
			else if (event->mask & IN_MODIFY)
				printf ("Modificado: %s\n", event->name);
			else if (event->mask & IN_DELETE)
				printf ("Eliminado: %s\n", event->name);
			else if (event->mask & IN_CLOSE_WRITE)
				printf ("Cerrado y escrito: %s\n", event->name);
			else
        		printf("Desconocido: %s\n", event->name);
		}
	}	
	return 0;
}