#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int main() {
    int fd = open("/dev/serial0", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct termios config;
    tcgetattr(fd, &config);

    cfsetispeed(&config, B115200);
    cfsetospeed(&config, B115200);

    config.c_cflag &= ~PARENB;
    config.c_cflag &= ~CSTOPB;
    config.c_cflag &= ~CSIZE;
    config.c_cflag |= CS8;
    config.c_cflag |= CREAD | CLOCAL;

    config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    config.c_iflag &= ~(IXON | IXOFF | IXANY);
    config.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &config);

    const char *msg = "Hola desde C en la Pi!\n";
    write(fd, msg, strlen(msg));
    printf("Mensaje enviado: %s", msg);

    close(fd);
    return 0;
}
