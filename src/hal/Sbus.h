/*
 * Sbus.h
 *
 *  Created on: Aug 21, 2016
 *      Author: adrien
 */

#ifndef SRC_HAL_SBUS_H_
#define SRC_HAL_SBUS_H_

#include "../math/time/Date.h"

#define PX4IO_RC_INPUT_CHANNELS 18


#define SBUS_FRAME_SIZE		25
#define SBUS_INPUT_CHANNELS	16
#define SBUS_FLAGS_BYTE		23
#define SBUS_FAILSAFE_BIT	3
#define SBUS_FRAMELOST_BIT	2
#define SBUS1_FRAME_DELAY	14000

/*
  Measured values with Futaba FX-30/R6108SB:
    -+100% on TX:  PCM 1.100/1.520/1.950ms -> SBus raw values: 350/1024/1700  (100% ATV)
    -+140% on TX:  PCM 0.930/1.520/2.112ms -> SBus raw values:  78/1024/1964  (140% ATV)
    -+152% on TX:  PCM 0.884/1.520/2.160ms -> SBus raw values:   1/1024/2047  (140% ATV plus dirty tricks)
 */

/* define range mapping here, -+100% -> 1000..2000 */
#define SBUS_RANGE_MIN 200.0f
#define SBUS_RANGE_MAX 1800.0f

#define SBUS_TARGET_MIN 1000.0f
#define SBUS_TARGET_MAX 2000.0f

#define NB_CHANNELS_OPERATIONNAL 8
#define RADIO_OFFSET 1023.0f
#define RADIO_VAR 680.0f

/* pre-calculate the floating point stuff as far as possible at compile time */
#define SBUS_SCALE_FACTOR ((SBUS_TARGET_MAX - SBUS_TARGET_MIN) / (SBUS_RANGE_MAX - SBUS_RANGE_MIN))
#define SBUS_SCALE_OFFSET (int)(SBUS_TARGET_MIN - (SBUS_SCALE_FACTOR * SBUS_RANGE_MIN + 0.5f))


/*
 * S.bus decoder matrix.
 *
 * Each channel value can come from up to 3 input bytes. Each row in the
 * matrix describes up to three bytes, and each entry gives:
 *
 * - byte offset in the data portion of the frame
 * - right shift applied to the data byte
 * - mask for the data byte
 * - left shift applied to the result into the channel value
 */
struct sbus_bit_pick {
	uint8_t byte;
	uint8_t rshift;
	uint8_t mask;
	uint8_t lshift;
};
static const struct sbus_bit_pick sbus_decoder[SBUS_INPUT_CHANNELS][3] = {
		/*  0 */ { { 0, 0, 0xff, 0}, { 1, 0, 0x07, 8}, { 0, 0, 0x00,  0} },
		/*  1 */ { { 1, 3, 0x1f, 0}, { 2, 0, 0x3f, 5}, { 0, 0, 0x00,  0} },
		/*  2 */ { { 2, 6, 0x03, 0}, { 3, 0, 0xff, 2}, { 4, 0, 0x01, 10} },
		/*  3 */ { { 4, 1, 0x7f, 0}, { 5, 0, 0x0f, 7}, { 0, 0, 0x00,  0} },
		/*  4 */ { { 5, 4, 0x0f, 0}, { 6, 0, 0x7f, 4}, { 0, 0, 0x00,  0} },
		/*  5 */ { { 6, 7, 0x01, 0}, { 7, 0, 0xff, 1}, { 8, 0, 0x03,  9} },
		/*  6 */ { { 8, 2, 0x3f, 0}, { 9, 0, 0x1f, 6}, { 0, 0, 0x00,  0} },
		/*  7 */ { { 9, 5, 0x07, 0}, {10, 0, 0xff, 3}, { 0, 0, 0x00,  0} },
		/*  8 */ { {11, 0, 0xff, 0}, {12, 0, 0x07, 8}, { 0, 0, 0x00,  0} },
		/*  9 */ { {12, 3, 0x1f, 0}, {13, 0, 0x3f, 5}, { 0, 0, 0x00,  0} },
		/* 10 */ { {13, 6, 0x03, 0}, {14, 0, 0xff, 2}, {15, 0, 0x01, 10} },
		/* 11 */ { {15, 1, 0x7f, 0}, {16, 0, 0x0f, 7}, { 0, 0, 0x00,  0} },
		/* 12 */ { {16, 4, 0x0f, 0}, {17, 0, 0x7f, 4}, { 0, 0, 0x00,  0} },
		/* 13 */ { {17, 7, 0x01, 0}, {18, 0, 0xff, 1}, {19, 0, 0x03,  9} },
		/* 14 */ { {19, 2, 0x3f, 0}, {20, 0, 0x1f, 6}, { 0, 0, 0x00,  0} },
		/* 15 */ { {20, 5, 0x07, 0}, {21, 0, 0xff, 3}, { 0, 0, 0x00,  0} }
};


class Sbus {
private:
	int sbus_fd;
	unsigned partial_frame_count;
	uint8_t	frame[SBUS_FRAME_SIZE];


	Date last_rx_time;
	Date last_frame_time;
	Date last_txframe_time;

	unsigned sbus_frame_drops;

	bool sbus_failsafe, sbus_frame_drop;
	bool defaultValueInitialized;


public:

	uint16_t channels[16];
	uint16_t channelsCalib[16];

	Sbus();
	virtual ~Sbus();

	int		init(const char *device);
	bool 	input(uint16_t *values, uint16_t *num_values, bool *sbus_failsafe, bool *sbus_frame_drop, uint16_t max_channels);
	bool	decode(Date frame_time, uint16_t *values, uint16_t *num_values, bool *sbus_failsafe, bool *sbus_frame_drop,
			uint16_t max_values);

	void 	fastLoop();


	float getChannelNormed(uint8_t ch)
	{
		return (channels[ch] - channelsCalib[ch-1]) / RADIO_VAR;
	}

	bool isFailsafe() {
		return sbus_failsafe;
	}
};

#endif /* SRC_HAL_SBUS_H_ */
