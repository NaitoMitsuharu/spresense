#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>

typedef struct {
  char name[32];       // borad/sensor
  char type[16];       // type
  char path[128];       // device path
} board_info_t;

static int check_device_path(const char *path, board_info_t *boards, int *count,
                            const char *name, const char *type)
{
  if (access(path, F_OK) == 0) {
    if (*count < 64) {
      snprintf(boards[*count].name, sizeof(boards[*count].name), "%s", name);
      snprintf(boards[*count].type, sizeof(boards[*count].type), "%s", type);
      snprintf(boards[*count].path, sizeof(boards[*count].path), "%s", path);
      (*count)++;
      return 1;
    }
  }
  return 0;
}

static int scan_device_files(board_info_t *boards, int *count, int max_boards)
{
  struct {
    const char *path;
    const char *name;
    const char *type;
  } known_devices[] = {
    // bus
    { "/dev/i2c0", "I2C Bus 0", "bus" },
    { "/dev/i2c1", "I2C Bus 1", "bus" },
    { "/dev/spi0", "SPI Bus 0", "bus" },
    { "/dev/spi1", "SPI Bus 1", "bus" },

    // sensor
    { "/dev/imu0", "IMU Sensor", "sensor" },
    { "/dev/accel0", "Accelerometer", "sensor" },
    { "/dev/gyro0", "Gyroscope", "sensor" },
    { "/dev/mag0", "Magnetometer", "sensor" },
    { "/dev/compass0", "Compass", "sensor" },
    { "/dev/pressure0", "Pressure Sensor", "sensor" },
    { "/dev/baro0", "Barometer", "sensor" },
    { "/dev/temp0", "Temperature Sensor", "sensor" },
    { "/dev/humi0", "Humidity Sensor", "sensor" },
    { "/dev/light0", "Light Sensor", "sensor" },
    { "/dev/proximity0", "Proximity Sensor", "sensor" },
    { "/dev/gesture0", "Gesture Sensor", "sensor" },
    { "/dev/color0", "Color Sensor", "sensor" },
    { "/dev/uv0", "UV Sensor", "sensor" },
    { "/dev/gas0", "Gas Sensor", "sensor" },
    { "/dev/dust0", "Dust Sensor", "sensor" },
    { "/dev/motion0", "Motion Sensor", "sensor" },
    { "/dev/tof0", "ToF Sensor", "sensor" },
    { "/dev/range0", "Range Sensor", "sensor" },

    // display
    { "/dev/lcd0", "LCD Display", "display" },
    { "/dev/oled0", "OLED Display", "display" },
    { "/dev/epd0", "E-Paper Display", "display" },
    { "/dev/fb0", "Framebuffer", "display" },

    // audio
    { "/dev/audio0", "Audio Device", "audio" },
    { "/dev/pcm0", "PCM Audio", "audio" },
    { "/dev/mic0", "Microphone", "audio" },
    { "/dev/speaker0", "Speaker", "audio" },

    // camera
    { "/dev/camera0", "Camera", "camera" },
    { "/dev/video0", "Video Device", "camera" },

    // trans
    { "/dev/gnss0", "GNSS/GPS", "positioning" },
    { "/dev/lte0", "LTE Module", "communication" },
    { "/dev/bt0", "Bluetooth", "communication" },
    { "/dev/ble0", "BLE", "communication" },
    { "/dev/wifi0", "Wi-Fi", "communication" },
    { "/dev/lora0", "LoRa", "communication" },
    { "/dev/ttyS0", "Serial Port 0", "communication" },
    { "/dev/ttyS1", "Serial Port 1", "communication" },
    { "/dev/ttyUSB0", "USB Serial", "communication" },

    // I/O
    { "/dev/gpio0", "GPIO", "io" },
    { "/dev/pwm0", "PWM 0", "io" },
    { "/dev/pwm1", "PWM 1", "io" },
    { "/dev/adc0", "ADC 0", "io" },
    { "/dev/dac0", "DAC 0", "io" },
    { "/dev/i2s0", "I2S 0", "io" },

    // storage
    { "/dev/mmcsd0", "SD Card", "storage" },
    { "/dev/mtd0", "Flash Memory", "storage" },
    { "/dev/eeprom0", "EEPROM", "storage" },

    // system
    { "/dev/rtc0", "RTC", "time" },
    { "/dev/watchdog0", "Watchdog", "system" },
    { "/dev/pm0", "Power Management", "system" },
    { NULL, NULL, NULL }
  };

  for (int i = 0; known_devices[i].path != NULL; i++) {
    check_device_path(known_devices[i].path, boards, count,
                     known_devices[i].name, known_devices[i].type);
  }

  DIR *dir = opendir("/dev");
  if (!dir) {
    return -errno;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL && *count < max_boards) {
    if (entry->d_name == NULL) {
      continue;
    }
    char path[128];
    if (strlen(entry->d_name) + 6 > sizeof(path)) {
      continue;
    }
    snprintf(path, sizeof(path), "/dev/%s", entry->d_name);

    struct {
      const char *pattern;
      const char *name;
      const char *type;
    } patterns[] = {
      { "sensor", "Generic Sensor", "sensor" },
      { "imu", "IMU Device", "sensor" },
      { "accel", "Accelerometer", "sensor" },
      { "gyro", "Gyroscope", "sensor" },
      { "mag", "Magnetometer", "sensor" },
      { "gnss", "GNSS Device", "positioning" },
      { "lcd", "LCD Device", "display" },
      { "audio", "Audio Device", "audio" },
      { "camera", "Camera Device", "camera" },
      { NULL, NULL, NULL }
    };

    for (int i = 0; patterns[i].pattern != NULL; i++) {
      if (entry->d_name && strstr(entry->d_name, patterns[i].pattern) != NULL) {
        int already_added = 0;
        for (int j = 0; j < *count; j++) {
          if (strcmp(boards[j].path, path) == 0) {
            already_added = 1;
            break;
          }
        }

        if (!already_added) {
          snprintf(boards[*count].name, sizeof(boards[*count].name), "%s", patterns[i].name);
          snprintf(boards[*count].type, sizeof(boards[*count].type), "%s", patterns[i].type);
          snprintf(boards[*count].path, sizeof(boards[*count].path), "%s", path);
          (*count)++;
          break;
        }
      }
    }
  }

  closedir(dir);
  return 0;
}

static void output_json(board_info_t *boards, int count, int status)
{
  printf("JSON_BEGIN\n");
  printf("{\"cmd\":\"lsboard\",\"type\":\"res\",\"status\":%d,\"data\":[", status);

  for (int i = 0; i < count; i++) {
    printf("%s{\"name\":\"%s\",\"type\":\"%s\",\"path\":\"%s\"}",
           (i > 0) ? "," : "",
           boards[i].name,
           boards[i].type,
           boards[i].path);
  }

  printf("],\"config\":{}}\n");
  printf("JSON_END\n");
}

int lsboard_main(int argc, char **argv)
{
  board_info_t boards[32];
  int count = 0;
  int status = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf("Usage: %s\n", argv[0]);
      printf("List all connected boards and sensors in JSON format\n");
      return 0;
    }
  }
  if (scan_device_files(boards, &count, 64) < 0) {
    status = -EIO;
  }
  output_json(boards, count, status);

  return status == 0 ? 0 : 1;
}

#if defined(BUILD_MODULE)
int main(int argc, char **argv)
{
  return lsboard_main(argc, argv);
}
#endif
