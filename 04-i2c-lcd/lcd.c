#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define LCD_ADDR 0x27
#define LCD_CHR  1
#define LCD_CMD  0

#define LCD_LINE_1 0x80
#define LCD_LINE_2 0xC0

#define ENABLE 0x04
#define BACKLIGHT 0x08

int fd;

void lcd_byte(int bits, int mode) {
    int bits_high = mode | (bits & 0xF0) | BACKLIGHT;
    int bits_low  = mode | ((bits << 4) & 0xF0) | BACKLIGHT;

    write(fd, (char[]){bits_high}, 1);
    write(fd, (char[]){bits_high | ENABLE}, 1);
    write(fd, (char[]){bits_high & ~ENABLE}, 1);

    write(fd, (char[]){bits_low}, 1);
    write(fd, (char[]){bits_low | ENABLE}, 1);
    write(fd, (char[]){bits_low & ~ENABLE}, 1);
}

void lcd_init() {
    lcd_byte(0x33, LCD_CMD);
    lcd_byte(0x32, LCD_CMD);
    lcd_byte(0x06, LCD_CMD);
    lcd_byte(0x0C, LCD_CMD);
    lcd_byte(0x28, LCD_CMD);
    lcd_byte(0x01, LCD_CMD);
    usleep(500000);
}

void lcd_string(const char *message, int line) {
    lcd_byte(line, LCD_CMD);
    while (*message) {
        lcd_byte(*message, LCD_CHR);
        message++;
    }
}

void lcd_clear() {
    lcd_byte(0x01, LCD_CMD);
    usleep(500000);
}

int main() {
    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) {
        perror("open i2c");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0) {
        perror("ioctl");
        return 1;
    }

    lcd_init();
    lcd_string("Hola Lucas!", LCD_LINE_1);
    lcd_string("Raspberry Pi", LCD_LINE_2);

    sleep(3);
    lcd_clear();
    lcd_string("I2C funciona", LCD_LINE_1);

    close(fd);
    return 0;
}
