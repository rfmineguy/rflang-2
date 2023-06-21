#ifndef LOGGER_CHANNELS_H
#define LOGGER_CHANNELS_H
#include <stdint.h>
#include <stdio.h>
#include "color.h"

// PARSE CHANNELS
#define WARNING_CHANNEL 0
#define ERROR_CHANNEL 1
#define PARSE_STATUS_CHANNEL 2

// FREE CHANNELS
#define FREE_STATUS_CHANNEL 3

typedef struct {
  uint64_t    channel_map;
  const char* channel_color[64];
  const char* channel_prefix[64];
  FILE*       channel_streams[64];
} logger_t;

void      logger_reset(logger_t*);

void      logger_channel_set_stream(logger_t*, int, FILE*);
void      logger_channel_enable(logger_t*, int, const char*, const char*);
void      logger_channel_disable(logger_t*, int);

void      logger_log(logger_t*, int, const char*, ...);

logger_t* logger_get_global();

#endif
