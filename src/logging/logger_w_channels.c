#include "logger_w_channels.h"
#include <stdarg.h>

logger_t g_rfc_logger = {0};

void logger_reset(logger_t* logger) {
  logger->channel_map = 0;
  for (int i = 0; i < 64; i++) {
    logger->channel_color[i] = ANSI_RESET;
    logger->channel_streams[i] = NULL;
  }
}

void logger_channel_set_stream(logger_t* logger, int channel, FILE* stream) {
  logger->channel_streams[channel] = stream;
}

void logger_channel_enable(logger_t* logger, int channel, const char* prefix, const char* ansi_color) {
  if (channel < 0 || channel >= 64) {
    fprintf(stderr, "%s ERROR: channel #%d out of bounds\n", __func__, channel);
    return;
  }
  logger->channel_map = logger->channel_map | (0x01 << channel);
  logger->channel_prefix[channel] = prefix;
  logger->channel_color[channel] = ansi_color;
}

void logger_channel_disable(logger_t* logger, int channel) {
  if (channel < 0 || channel >= 64) {
    fprintf(stderr, "%s ERROR: channel #%d out of bounds\n", __func__, channel);
    return;
  }
  logger->channel_map = logger->channel_map & ~(0x01 << channel);
  logger->channel_streams[channel] = NULL;
  logger->channel_color[channel] = NULL;
}

void logger_log(logger_t* logger, int channel, const char* fmt, ...) {
  if ((logger->channel_map & (0x1 << channel)) == 0 || logger->channel_streams[channel] == NULL) {
    // fprintf(stderr, ANSI_RESET "logger_log:: something wrong with channel(%d)\n", channel);
    return;
  }
  if (logger->channel_prefix[channel]) {
    printf(ANSI_RESET "[CHANNEL %d, %s] ", channel, logger->channel_prefix[channel]);
  }
  else {
    printf(ANSI_RESET "[CHANNEL %d] ", channel);
  }
  fprintf(logger->channel_streams[channel], "%s", logger->channel_color[channel]);

  va_list args;
  va_start(args, fmt);
  vfprintf(logger->channel_streams[channel], fmt, args);
  va_end(args);
}

logger_t* logger_get_global() {
  return &g_rfc_logger;
}
