/************************************************************************************
 *  (c) Copyright 2014-2015 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#include "ap_int.h"
#include<assert.h>
#include<stdlib.h>
#define LARGE_BUS 512
#define MARS_WIDE_BUS_TYPE ap_uint<LARGE_BUS>
#define BUS_WIDTH LARGE_BUS / 8
#define cpp_get_range(tmp, x, y) tmp(x, y)
#define c_get_range(tmp, x, y) apint_get_range(tmp, x, y)
#define cpp_set_range(tmp, x, y, val) tmp(x, y) = val
#define c_set_range(tmp, x, y, val) tmp = apint_set_range(tmp, x, y, val)
#ifdef __cplusplus
#define tmp2(x, y) cpp_get_range(tmp, x, y)
#define tmp3(x, y, val) cpp_set_range(tmp, x, y, val)
#else
#define tmp2(x, y) c_get_range(tmp, x, y)
#define tmp3(x, y, val) c_set_range(tmp, x, y, val)
#endif
static char memcpy_wide_bus_single_read_char(MARS_WIDE_BUS_TYPE *a,
                                                  size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(char);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = a[start];
#endif
  char ret = tmp2(((head_align + 1) * data_width * 8 - 1),
                 (head_align * data_width * 8));
  return ret;
}

static void memcpy_wide_bus_single_write_char(MARS_WIDE_BUS_TYPE *c,
                                                    char c_buf,
                                                    size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(char);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(c[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = c[start];
#endif
  tmp3(((head_align + 1) * data_width * 8 - 1), (head_align * data_width * 8),
       c_buf);
  c[start] = tmp;
  return;
}

static int memcpy_wide_bus_single_read_int(MARS_WIDE_BUS_TYPE *a,
                                                  size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(int);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = a[start];
#endif
  int ret = tmp2(((head_align + 1) * data_width * 8 - 1),
                 (head_align * data_width * 8));
  return ret;
}

static void memcpy_wide_bus_single_write_int(MARS_WIDE_BUS_TYPE *c,
                                                    int c_buf,
                                                    size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(int);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(c[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = c[start];
#endif
  tmp3(((head_align + 1) * data_width * 8 - 1), (head_align * data_width * 8),
       c_buf);
  c[start] = tmp;
  return;
}

static float
memcpy_wide_bus_single_read_float(MARS_WIDE_BUS_TYPE *a,
                                  size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(float);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = a[start];
#endif

  int raw_bits = tmp2(((head_align + 1) * data_width * 8 - 1),
                      (head_align * data_width * 8));
  float ret = *(float *)(&raw_bits);
  return ret;
}

static void
memcpy_wide_bus_single_write_float(MARS_WIDE_BUS_TYPE *c, float c_buf,
                                   size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(float);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(c[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = c[start];
#endif
  float buf_tmp = c_buf;
  long long raw_bits = *(long long *)&buf_tmp;
  tmp3(((head_align + 1) * data_width * 8 - 1), (head_align * data_width * 8),
       raw_bits);
  c[start] = tmp;
  return;
}

static double
memcpy_wide_bus_single_read_double(MARS_WIDE_BUS_TYPE *a,
                                   size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(double);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = a[start];
#endif

  long long raw_bits = tmp2(((head_align + 1) * data_width * 8 - 1),
                            (head_align * data_width * 8));
  double ret = *(double *)(&raw_bits);
  return ret;
}

static void
memcpy_wide_bus_single_write_double(MARS_WIDE_BUS_TYPE *c, double c_buf,
                                    size_t offset_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(double);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t start = offset / num_elements;
#ifdef __cplusplus
  MARS_WIDE_BUS_TYPE tmp(c[start]);
#else
  MARS_WIDE_BUS_TYPE tmp = c[start];
#endif
  double buf_tmp = c_buf;
  long long raw_bits = *(long long *)&buf_tmp;
  tmp3(((head_align + 1) * data_width * 8 - 1), (head_align * data_width * 8),
       raw_bits);
  c[start] = tmp;
  return;
}

static void memcpy_wide_bus_read_int(int *a_buf, MARS_WIDE_BUS_TYPE *a,
                                            size_t offset_byte,
                                            size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(int);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  //MARS_WIDE_BUS_TYPE *a_offset = a + start;
  size_t i, j;
  int len = end - start;
  assert(len <= buf_size / num_elements + 2);
  assert(len >= buf_size / num_elements);
  if (1 == len) {
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[start];
#endif
    for (j = 0; j < num_elements; ++j) {
       if (j < head_align || j > tail_align)
         continue;
        a_buf[j - head_align] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
    }
    return;
  }

  for (i = 0; i < len; ++i) {
#pragma HLS pipeline
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[i + start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[i + start];
#endif
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 0] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 1)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 1] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 2)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 2] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 3)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 3] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 4)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 4] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 5)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 5] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 6)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 6] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 7) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 7)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 7] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 8) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 8)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 8] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 9) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 9)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 9] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 10) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 10)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 10] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 11) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 11)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 11] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 12) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 12)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 12] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 13) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 13)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 13] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 14) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 14)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 14] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 15)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 15] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
  }
}

static void memcpy_wide_bus_write_int(MARS_WIDE_BUS_TYPE *c, int *c_buf,
                                             size_t offset_byte,
                                             size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(int);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  size_t len = end - start;
  size_t i, j;
  if (head_align == 0)
    len = (buf_size + num_elements - 1) / num_elements;
  size_t align = 0;
  if (len == 1) {
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align != 0 || tail_align != (num_elements - 1))
      tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      if (j > tail_align)
        continue;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
           c_buf[j - head_align]);
    }
    c[start] = tmp;
    return;
  }
  if (head_align != 0) {
    MARS_WIDE_BUS_TYPE tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
           c_buf[j - head_align]);
    }
    c[start] = tmp;
    start++;
    align++;
  }
  if (tail_align != (num_elements - 1))
    align++;
  int burst_len = len - align;
  assert(burst_len <= buf_size / num_elements);
  for (i = 0; i < burst_len; ++i) {
#pragma HLS pipeline
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j - 0]);
      }

    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 15]);
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 14]);
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 13]);
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 12]);
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 11]);
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 10]);
      }
    }

    else if (head_align == 7) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 9]);
      }
    }

    else if (head_align == 8) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 8]);
      }
    }

    else if (head_align == 9) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 7]);
      }
    }

    else if (head_align == 10) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 6]);
      }
    }

    else if (head_align == 11) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 5]);
      }
    }

    else if (head_align == 12) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 4]);
      }
    }

    else if (head_align == 13) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 3]);
      }
    }

    else if (head_align == 14) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 2]);
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 1]);
      }
    }

    c[i + start] = tmp;
  }
  if (tail_align != num_elements - 1) {
    MARS_WIDE_BUS_TYPE tmp = c[end - 1];
    size_t pos = (len - align) * num_elements;
    pos += (num_elements - head_align) % num_elements;
    for (j = 0; j < num_elements; ++j) {
      if (j > tail_align)
        continue;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
           c_buf[pos + j]);
    }
    c[end - 1] = tmp;
  }
}

static void memcpy_wide_bus_read_char(char *a_buf, MARS_WIDE_BUS_TYPE *a,
                                            size_t offset_byte,
                                            size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(char);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  //MARS_WIDE_BUS_TYPE *a_offset = a + start;
  size_t i, j;
  int len = end - start;
  assert(len <= buf_size / num_elements + 2);
  assert(len >= buf_size / num_elements);
  if (1 == len) {
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[start];
#endif
    for (j = 0; j < num_elements; ++j) {
       if (j < head_align || j > tail_align)
         continue;
        a_buf[j - head_align] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
    }
    return;
  }

  for (i = 0; i < len; ++i) {
#pragma HLS pipeline
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[i + start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[i + start];
#endif
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 0] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 1)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 1] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 2)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 2] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 3)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 3] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 4)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 4] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 5)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 5] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 6)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 6] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 7) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 7)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 7] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 8) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 8)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 8] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 9) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 9)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 9] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 10) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 10)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 10] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 11) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 11)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 11] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 12) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 12)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 12] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 13) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 13)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 13] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }

    else if (head_align == 14) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 14)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 14] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 15) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 15)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 15] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 16) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 16)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 16] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 17) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 17)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 17] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 18) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 18)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 18] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 19) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 19)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 19] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 20) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 20)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 20] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 21) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 21)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 21] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 22) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 22)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 22] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 23) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 23)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 23] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 24) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 24)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 24] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 25) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 25)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 25] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 26) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 26)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 26] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 27) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 27)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 27] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 28) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 28)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 28] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 29) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 29)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 29] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 30) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 30)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 30] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 31) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 31)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 31] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 32) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 32)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 32] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 33) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 33)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 33] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 34) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 34)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 34] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 35) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 35)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 35] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 36) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 36)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 36] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 37) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 37)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 37] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 38) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 38)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 38] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 39) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 39)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 39] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 40) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 40)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 40] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 41) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 41)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 41] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 42) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 42)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 42] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 43) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 43)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 43] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 44) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 44)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 44] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 45) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 45)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 45] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 46) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 46)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 46] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 47) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 47)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 47] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 48) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 48)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 48] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 49) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 49)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 49] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 50) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 50)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 50] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 51) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 51)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 51] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 52) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 52)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 52] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 53) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 53)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 53] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 54) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 54)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 54] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 55) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 55)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 55] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 56) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 56)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 56] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 57) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 57)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 57] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 58) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 58)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 58] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 59) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 59)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 59] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 60) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 60)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 60] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 61) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 61)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 61] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else if (head_align == 62) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 62)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 62] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
    else {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 63)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        a_buf[i * num_elements + j - 63] =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
      }
    }
  }
}

static void memcpy_wide_bus_write_char(MARS_WIDE_BUS_TYPE *c, char *c_buf,
                                             size_t offset_byte,
                                             size_t size_byte) {
#pragma HLS inline self 
  const size_t data_width = sizeof(char);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  size_t len = end - start;
  size_t i, j;
  if (head_align == 0)
    len = (buf_size + num_elements - 1) / num_elements;
  size_t align = 0;
  if (len == 1) {
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align != 0 || tail_align != (num_elements - 1))
      tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      if (j > tail_align)
        continue;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
           c_buf[j - head_align]);
    }
    c[start] = tmp;
    return;
  }
  if (head_align != 0) {
    MARS_WIDE_BUS_TYPE tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
           c_buf[j - head_align]);
    }
    c[start] = tmp;
    start++;
    align++;
  }
  if (tail_align != (num_elements - 1))
    align++;
  int burst_len = len - align;
  assert(burst_len <= buf_size / num_elements);
  for (i = 0; i < burst_len; ++i) {
#pragma HLS pipeline
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j - 0]);
      }

    }
    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 63]);
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 62]);
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 61]);
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 60]);
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 59]);
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 58]);
      }
    }

    else if (head_align == 7) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 57]);
      }
    }

    else if (head_align == 8) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 56]);
      }
    }

    else if (head_align == 9) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 55]);
      }
    }

    else if (head_align == 10) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 54]);
      }
    }

    else if (head_align == 11) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 53]);
      }
    }

    else if (head_align == 12) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 52]);
      }
    }

    else if (head_align == 13) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 51]);
      }
    }

    else if (head_align == 14) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 50]);
      }
    }

    else if (head_align == 15) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 49]);
      }
    }

    else if (head_align == 16) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 48]);
      }
    }

    else if (head_align == 17) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 47]);
      }
    }

    else if (head_align == 18) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 46]);
      }
    }

    else if (head_align == 19) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 45]);
      }
    }

    else if (head_align == 20) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 44]);
      }
    }

    else if (head_align == 21) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 43]);
      }
    }

    else if (head_align == 22) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 42]);
      }
    }

    else if (head_align == 23) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 41]);
      }
    }

    else if (head_align == 24) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 40]);
      }
    }

    else if (head_align == 25) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 39]);
      }
    }

    else if (head_align == 26) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 38]);
      }
    }

    else if (head_align == 27) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 37]);
      }
    }

    else if (head_align == 28) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 36]);
      }
    }

    else if (head_align == 29) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 35]);
      }
    }

    else if (head_align == 30) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 34]);
      }
    }

    else if (head_align == 31) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 33]);
      }
    }

    else if (head_align == 32) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 32]);
      }
    }

    else if (head_align == 33) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 31]);
      }
    }

    else if (head_align == 34) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 30]);
      }
    }

    else if (head_align == 35) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 29]);
      }
    }

    else if (head_align == 36) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 28]);
      }
    }

    else if (head_align == 37) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 27]);
      }
    }

    else if (head_align == 38) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 26]);
      }
    }

    else if (head_align == 39) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 25]);
      }
    }

    else if (head_align == 40) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 24]);
      }
    }

    else if (head_align == 41) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 23]);
      }
    }

    else if (head_align == 42) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 22]);
      }
    }

    else if (head_align == 43) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 21]);
      }
    }

    else if (head_align == 44) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 20]);
      }
    }

    else if (head_align == 45) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 19]);
      }
    }

    else if (head_align == 46) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 18]);
      }
    }

    else if (head_align == 47) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 17]);
      }
    }

    else if (head_align == 48) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 16]);
      }
    }

    else if (head_align == 49) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 15]);
      }
    }

    else if (head_align == 50) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 14]);
      }
    }

    else if (head_align == 51) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 13]);
      }
    }

    else if (head_align == 52) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 12]);
      }
    }

    else if (head_align == 53) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 11]);
      }
    }

    else if (head_align == 54) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 10]);
      }
    }

    else if (head_align == 55) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 9]);
      }
    }

    else if (head_align == 56) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 8]);
      }
    }

    else if (head_align == 57) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 7]);
      }
    }

    else if (head_align == 58) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 6]);
      }
    }

    else if (head_align == 59) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 5]);
      }
    }

    else if (head_align == 60) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 4]);
      }
    }

    else if (head_align == 61) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 3]);
      }
    }

    else if (head_align == 62) {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 2]);
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
             c_buf[i * num_elements + j + 1]);
      }
    }

    c[i + start] = tmp;
  }
  if (tail_align != num_elements - 1) {
    MARS_WIDE_BUS_TYPE tmp = c[end - 1];
    size_t pos = (len - align) * num_elements;
    pos += (num_elements - head_align) % num_elements;
    for (j = 0; j < num_elements; ++j) {
      if (j > tail_align)
        continue;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8),
           c_buf[pos + j]);
    }
    c[end - 1] = tmp;
  }
}

static void memcpy_wide_bus_read_float(float *a_buf,
                                              MARS_WIDE_BUS_TYPE *a,
                                              size_t offset_byte,
                                              size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(float);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  //MARS_WIDE_BUS_TYPE *a_offset = a + start;
  size_t i, j;
  int len = end - start;
  assert(len <= buf_size / num_elements + 2);
  assert(len >= buf_size / num_elements);
  if (1 == len) {
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[start];
#endif
    for (j = 0; j < num_elements; ++j) {
       if (j < head_align || j > tail_align)
         continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[j - head_align] = *(float *)(&raw_bits);
    }
    return;
  }

  for (i = 0; i < len; ++i) {
#pragma HLS pipeline
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[i + start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[i + start];
#endif

    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 0] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 1)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 1] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 2)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 2] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 3)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 3] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 4)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 4] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 5)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 5] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 6)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 6] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 7) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 7)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 7] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 8) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 8)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 8] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 9) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 9)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 9] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 10) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 10)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 10] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 11) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 11)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 11] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 12) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 12)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 12] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 13) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 13)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 13] = *(float *)(&raw_bits);
      }
    }

    else if (head_align == 14) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 14)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 14] = *(float *)(&raw_bits);
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 15)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        int raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 15] = *(float *)(&raw_bits);
      }
    }
  }
}

static void memcpy_wide_bus_write_float(MARS_WIDE_BUS_TYPE *c,
                                               float *c_buf,
                                               size_t offset_byte,
                                               size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(float);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  size_t len = end - start;
  size_t i, j;
  if (head_align == 0)
    len = (buf_size + num_elements - 1) / num_elements;
  if (len == 1) {
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align != 0 || tail_align != (num_elements - 1))
      tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      if (j > tail_align)
        continue;
      float buf_tmp = c_buf[j - head_align];
      int raw_bits = *(int *)&buf_tmp;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
    }
    c[start] = tmp;
    return;
  }
  unsigned align = 0;
  if (head_align != 0) {
    MARS_WIDE_BUS_TYPE tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      float buf_tmp = c_buf[j - head_align];
      int raw_bits = *(int *)&buf_tmp;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
    }
    c[start] = tmp;
    start++;
    align++;
  }
  if (tail_align != (num_elements - 1))
    align++;
  int burst_len = len - align;
  assert(burst_len <= buf_size / num_elements);
  for (i = 0; i < burst_len; ++i) {
#pragma HLS pipeline
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j - 0];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }

    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 15];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 14];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 13];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 12];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 11];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 10];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 7) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 9];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 8) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 8];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 9) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 7];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 10) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 6];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 11) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 5];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 12) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 4];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 13) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 3];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 14) {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 2];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        float buf_tmp = c_buf[i * num_elements + j + 1];
        int raw_bits = *(int *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    c[i + start] = tmp;
  }
  if (tail_align != num_elements - 1) {
    MARS_WIDE_BUS_TYPE tmp = c[end - 1];
    size_t pos = (len - align) * num_elements;
    pos += (num_elements - head_align) % num_elements;
    for (j = 0; j < num_elements; ++j) {
      if (j > tail_align)
        continue;
      float buf_tmp = c_buf[pos + j];
      int raw_bits = *(int *)&buf_tmp;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
    }
    c[end - 1] = tmp;
  }
}

static void memcpy_wide_bus_read_double(double *a_buf,
                                               MARS_WIDE_BUS_TYPE *a,
                                               size_t offset_byte,
                                               size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(double);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  //MARS_WIDE_BUS_TYPE *a_offset = a + start;
  size_t i, j;
  int len = end - start;
  assert(len <= buf_size / num_elements + 2);
  assert(len >= buf_size / num_elements);
  if (1 == len) {
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[start];
#endif
    for (j = 0; j < num_elements; ++j) {
       if (j < head_align || j > tail_align)
         continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[j - head_align] = *(double *)(&raw_bits);
    }
    return;
  }

  for (i = 0; i < len; ++i) {
#pragma HLS pipeline
#ifdef __cplusplus
    MARS_WIDE_BUS_TYPE tmp(a[i + start]);
#else
    MARS_WIDE_BUS_TYPE tmp = a[i + start];
#endif
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 0] = *(double *)(&raw_bits);
      }
    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 1)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 1] = *(double *)(&raw_bits);
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 2)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 2] = *(double *)(&raw_bits);
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 3)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 3] = *(double *)(&raw_bits);
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 4)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 4] = *(double *)(&raw_bits);
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 5)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 5] = *(double *)(&raw_bits);
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 6)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 6] = *(double *)(&raw_bits);
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        if (i == 0 && j < 7)
          continue;
        if (i == end - start - 1 && j > tail_align)
          continue;
        long long raw_bits =
            tmp2(((j + 1) * data_width * 8 - 1), (j * data_width * 8));
        a_buf[i * num_elements + j - 7] = *(double *)(&raw_bits);
      }
    }
  }
}

static void memcpy_wide_bus_write_double(MARS_WIDE_BUS_TYPE *c,
                                                double *c_buf,
                                                size_t offset_byte,
                                                size_t size_byte) {
#pragma HLS inline self
  const size_t data_width = sizeof(double);
  const size_t bus_width = BUS_WIDTH;
  const size_t num_elements = bus_width / data_width;
  size_t buf_size = size_byte / data_width;
  size_t offset = offset_byte / data_width;
  size_t head_align = offset & (num_elements - 1);
  size_t new_offset = offset + buf_size;
  size_t tail_align = (new_offset - 1) & (num_elements - 1);
  size_t start = offset / num_elements;
  size_t end = (offset + buf_size + num_elements - 1) / num_elements;
  size_t len = end - start;
  size_t i, j;
  if (head_align == 0)
    len = (buf_size + num_elements - 1) / num_elements;
  if (len == 1) {
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align != 0 || tail_align != (num_elements - 1))
      tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      if (j > tail_align)
        continue;
      double buf_tmp = c_buf[j - head_align];
      long long raw_bits = *(long long *)&buf_tmp;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
    }
    c[start] = tmp;
    return;
  }
  size_t align = 0;
  if (head_align != 0) {
    MARS_WIDE_BUS_TYPE tmp = c[start];
    for (j = 0; j < num_elements; ++j) {
      if (j < head_align)
        continue;
      double buf_tmp = c_buf[j - head_align];
      long long raw_bits = *(long long *)&buf_tmp;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
    }
    c[start] = tmp;
    start++;
    align++;
  }
  if (tail_align != (num_elements - 1))
    align++;
  int burst_len = len - align;
  assert(burst_len <= buf_size / num_elements);
  for (i = 0; i < burst_len; ++i) {
#pragma HLS pipeline
    MARS_WIDE_BUS_TYPE tmp;
    if (head_align == 0) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j - 0];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }

    }

    else if (head_align == 1) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 7];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 2) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 6];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 3) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 5];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 4) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 4];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 5) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 3];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else if (head_align == 6) {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 2];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    else {
      for (j = 0; j < num_elements; ++j) {
        double buf_tmp = c_buf[i * num_elements + j + 1];
        long long raw_bits = *(long long *)&buf_tmp;
        tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
      }
    }

    c[i + start] = tmp;
  }
  if (tail_align != num_elements - 1) {
    MARS_WIDE_BUS_TYPE tmp = c[end - 1];
    size_t pos = (len - align) * num_elements;
    pos += (num_elements - head_align) % num_elements;
    for (j = 0; j < num_elements; ++j) {
      if (j > tail_align)
        continue;
      double buf_tmp = c_buf[pos + j];
      long long raw_bits = *(long long *)&buf_tmp;
      tmp3(((j + 1) * data_width * 8 - 1), (j * data_width * 8), raw_bits);
    }
    c[end - 1] = tmp;
  }
}


//#undef LARGE_BUS
//#undef BUS_WIDTH
//#undef cpp_get_range
//#undef c_get_range
//#undef cpp_set_range
//#undef c_set_range
//#undef tmp2
//#undef tmp3
