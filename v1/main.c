#include<stdio.h>
#include<stdlib.h>
#include<sys/inotify.h>
#include<unistd.h>
#include<limits.h>
#include<string.h>
#include<errno.h>
#include <linux/limits.h>


#define BUFF_LEN (10* (sizeof(struct inotify_event) + NAME_MAX + 1))


void copy_file(char *src_path,char *dest_path) {
    char command[256 + PATH_MAX * 2 ];
    snprintf(command,sizeof(command),"./itrans/itrans -eh ./itrans/map.txt %s %s",src_path,dest_path);

    int ret = system(command);

    if(ret == -1) {
        perror("Error excecuting the command");
    }else{
        printf("Command executed sucessfully\n");
    }
}


void handle_event(struct inotify_event *event,char *src,char *dest) {
    if (event->mask & IN_MODIFY) {
        printf("File modified: %s\n", event->name);
        
        char src_path[PATH_MAX];
        char dest_path[PATH_MAX];
        snprintf(src_path, PATH_MAX, "%s/%s", src, event->name);
        snprintf(dest_path, PATH_MAX, "%s/%s", dest, event->name);
        
        copy_file(src_path, dest_path);
    }
}

int main(int argc, char * argv[]){
    if (argc < 2) {
        fprintf(stderr, "Directory not provided for monitoring\n");
        return 0;
    }
    if (argc < 3) {
        fprintf(stderr, "Destination directory not provided\n");
        return 0;
    }

    char *source_dir = argv[1];
    char *destination_dir = argv[2];

    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(inotify_fd,source_dir, IN_CREATE | IN_MODIFY | IN_DELETE );
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Monitoring directory: %s\n",source_dir);

    char buff[BUFF_LEN] __attribute__((aligned(8)));
    ssize_t num_read;

    while(1){
        num_read = read(inotify_fd,buff,BUFF_LEN);
        if (num_read == 0) {
            fprintf(stderr, "read() from inotify fd returned 0!");
            exit(EXIT_FAILURE);
        }
        if(num_read == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }

        char *ptr = buff;

        while(ptr < buff + num_read) {
            struct inotify_event *event = (struct inotify_event *)ptr;
            handle_event(event,source_dir,destination_dir);
            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}