#include <nuttx/config.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <nuttx/sensors/cxd5602pwbimu.h>

#define IMU_BOARD_PATH      "/dev/imu0"

static bool g_running = false;
static pthread_t g_thread_id;
static struct timespec g_start_time;
static int g_samplerate = 100;  /* Default: 100Hz */
static int g_accel_range = 2;   /* Default: ±2g */
static int g_gyro_range = 250;  /* Default: ±250dps */
static int g_fifo = 1;          /* Default: FIFO enabled with threshold 1 */

static void show_usage(const char *progname) {
  printf("Usage: %s [start|stop] [options]\n", progname);
  printf("Commands:\n");
  printf("  start     Start IMU data logging\n");
  printf("  stop      Stop IMU data logging\n");
  printf("Options (for start command):\n");
  printf("  -r RATE   Specify sample rate (Hz) (15, 30, 60, 120, 240, 480, 960, 1920)\n");
  printf("  -a RANGE  Specify accelerometer dynamic range (2, 4, 8, 16)\n");
  printf("  -g RANGE  Specify gyroscope dynamic range (125, 250, 500, 1000, 2000, 4000)\n");
  printf("  -f FIFO   Specify FIFO threshold\n");
  printf("  -h        Show this help message\n");
}

static int start_sensing(int fd, int rate, int adrange, int gdrange, int nfifos) {
  cxd5602pwbimu_range_t range;
  int ret;

  /*
   * Set sampling rate. Available values (Hz) are below.
   *
   * 15, 30, 60, 120, 240, 480, 960, 1920
   */
  ret = ioctl(fd, SNIOC_SSAMPRATE, rate);
  if (ret) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to set sampling rate\", \"config\":{}}\n", ret);
    return ret;
  }

  /*
   * Set dynamic ranges for accelerometer and gyroscope.
   * Available values are below.
   *
   * accel: 2, 4, 8, 16
   * gyro: 125, 250, 500, 1000, 2000, 4000
   */
  range.accel = adrange;
  range.gyro = gdrange;
  ret = ioctl(fd, SNIOC_SDRANGE, (unsigned long)(uintptr_t)&range);
  if (ret) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to set dynamic range\", \"config\":{}}\n", ret);
    return ret;
  }

  /*
   * Set hardware FIFO threshold.
   */
  ret = ioctl(fd, SNIOC_SFIFOTHRESH, nfifos);
  if (ret) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to set FIFO threshold\", \"config\":{}}\n", ret);
    return ret;
  }

  /*
   * Start sensing
   */
  ret = ioctl(fd, SNIOC_ENABLE, 1);
  if (ret) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to enable sensor\", \"config\":{}}\n", ret);
    return ret;
  }

  return 0;
}

static void *imu_data_thread(void *arg) {
  int fd;
  int ret;
  cxd5602pwbimu_data_t data;
  struct timespec current_time;
  double elapsed_time;
  char data_json[256];
  char config_json[128];

  fd = open(IMU_BOARD_PATH, O_RDONLY);
  if (fd < 0) {
    ret = -errno;
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to open device\", \"config\":{}}\n", ret);
    g_running = false;
    return NULL;
  }

  ret = start_sensing(fd, g_samplerate, g_accel_range, g_gyro_range, g_fifo);
  if (ret) {
    close(fd);
    g_running = false;
    return NULL;
  }

  snprintf(config_json, sizeof(config_json),
           "{\\\"samplerate\\\":%d,\\\"accel_range\\\":%d,\\\"gyro_range\\\":%d,\\\"fifo\\\":%d}",
           g_samplerate, g_accel_range, g_gyro_range, g_fifo);

  printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":0, \"data\":\"IMU logging started\", \"config\":%s}\n",
         config_json);

  clock_gettime(CLOCK_MONOTONIC, &g_start_time);

  while (g_running) {
    /* Read IMU data */
    ret = read(fd, &data, sizeof(data));
    if (ret != sizeof(data)) {
      printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to read data\", \"config\":{}}\n", -EIO);
      break;
    }

    /* Calculate elapsed time */
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    elapsed_time = (current_time.tv_sec - g_start_time.tv_sec) +
                   (current_time.tv_nsec - g_start_time.tv_nsec) / 1000000000.0;

    snprintf(data_json, sizeof(data_json),
             "{\\\"time\\\":%.3f,\\\"gx\\\":%.8f,\\\"gy\\\":%.8f,\\\"gz\\\":%.8f,\\\"ax\\\":%.8f,\\\"ay\\\":%.8f,\\\"az\\\":%.8f}",
             elapsed_time, data.gx, data.gy, data.gz, data.ax, data.ay, data.az);

    printf("{\"cmd\":\"imulogger\", \"type\":\"post\", \"status\":0, \"data\":%s, \"config\":{}}\n", data_json);

  }

  ioctl(fd, SNIOC_ENABLE, 0);
  close(fd);

  return NULL;
}

static int parse_options(int argc, char *argv[]) {
  int opt;
  int ret = 0;

  optind = 0;

  while ((opt = getopt(argc, argv, "r:a:g:f:h")) != -1) {
    switch (opt) {
      case 'r':
        g_samplerate = atoi(optarg);
        if (g_samplerate != 15 && g_samplerate != 30 && g_samplerate != 60 &&
            g_samplerate != 120 && g_samplerate != 240 && g_samplerate != 480 &&
            g_samplerate != 960 && g_samplerate != 1920) {
          printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Invalid sample rate\", \"config\":{}}\n", -EINVAL);
          ret = -EINVAL;
        }
        break;
      case 'a':
        g_accel_range = atoi(optarg);
        if (g_accel_range != 2 && g_accel_range != 4 && g_accel_range != 8 && g_accel_range != 16) {
          printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Invalid accelerometer range\", \"config\":{}}\n", -EINVAL);
          ret = -EINVAL;
        }
        break;
      case 'g':
        g_gyro_range = atoi(optarg);
        if (g_gyro_range != 125 && g_gyro_range != 250 && g_gyro_range != 500 &&
            g_gyro_range != 1000 && g_gyro_range != 2000 && g_gyro_range != 4000) {
          printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Invalid gyroscope range\", \"config\":{}}\n", -EINVAL);
          ret = -EINVAL;
        }
        break;
      case 'f':
        g_fifo = atoi(optarg);
        if (g_fifo < 1) {
          printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"FIFO threshold must be at least 1\", \"config\":{}}\n", -EINVAL);
          ret = -EINVAL;
        }
        break;
      case 'h':
        show_usage(argv[0]);
        ret = 1;  /* Exit but not as an error */
        break;
      default:
        show_usage(argv[0]);
        ret = -EINVAL;
        break;
    }
  }

  return ret;
}

static int cmd_imu_start(int argc, char *argv[]) {
  int ret;
  char config_json[256] = "{}";

  if (g_running) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"IMU logging already running\", \"config\":{}}\n", -EALREADY);
    return -1;
  }

  ret = parse_options(argc, argv);
  if (ret != 0) {
    return ret < 0 ? ret : 0;  /* Return 0 for help display */
  }

  if (argc > 2) {
    snprintf(config_json, sizeof(config_json),
             "{\"samplerate\":%d,\"accel_range\":%d,\"gyro_range\":%d,\"fifo\":%d}",
             g_samplerate, g_accel_range, g_gyro_range, g_fifo);
  }
  g_running = true;
  ret = pthread_create(&g_thread_id, NULL, imu_data_thread, NULL);

  if (ret != 0) {
    g_running = false;
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Failed to create thread\", \"config\":{}}\n", -ret);
    return -1;
  }

  return 0;
}

static int cmd_imu_stop(int argc, char *argv[]) {
  if (!g_running) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"IMU logging not running\", \"config\":{}}\n", -EINVAL);
    return -1;
  }
  g_running = false;
  pthread_join(g_thread_id, NULL);

  printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":0, \"data\":\"IMU logging stopped\", \"config\":{}}\n");
  return 0;
}

int main(int argc, char *argv[]) {
  /* Check for command argument */
  if (argc < 2) {
    show_usage(argv[0]);
    return -1;
  }

  if (strcmp(argv[1], "start") == 0) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":0, \"data\":\"start command received\", \"config\":{}}\n");
    return cmd_imu_start(argc, argv);
  }
  else if (strcmp(argv[1], "stop") == 0) {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":0, \"data\":\"stop command received\", \"config\":{}}\n");
    return cmd_imu_stop(argc, argv);
  }
  else {
    printf("{\"cmd\":\"imulogger\", \"type\":\"res\", \"status\":%d, \"data\":\"Unknown command: %s\", \"config\":{}}\n", -EINVAL, argv[1]);
    show_usage(argv[0]);
    return -1;
  }
}
