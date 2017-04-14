/****************************************************************************
 *
 *   Copyright (c) 2012-2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file sbus.c
 *
 * Serial protocol decoder for the Futaba S.bus protocol.
 */

#include <stdio.h>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <sstream>
//#include <termios.h>

#include <asm/termios.h> // custom baud rate
#include <stropts.h>

#define RC_INPUT_CHANNELS 18


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

/* pre-calculate the floating point stuff as far as possible at compile time */
#define SBUS_SCALE_FACTOR ((SBUS_TARGET_MAX - SBUS_TARGET_MIN) / (SBUS_RANGE_MAX - SBUS_RANGE_MIN))
#define SBUS_SCALE_OFFSET (int)(SBUS_TARGET_MIN - (SBUS_SCALE_FACTOR * SBUS_RANGE_MIN + 0.5f))

static int sbus_fd = -1;

static long last_rx_time;
static long last_frame_time;
static long last_txframe_time = 0;

static uint8_t	frame[SBUS_FRAME_SIZE];

static unsigned partial_frame_count;

unsigned sbus_frame_drops;

static bool sbus_decode(long frame_time, uint16_t *values, uint16_t *num_values, bool *sbus_failsafe,
		bool *sbus_frame_drop, uint16_t max_channels);



std::chrono::time_point<std::chrono::system_clock> dateOrigin = std::chrono::system_clock::now();


long micros()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

	long micros = std::chrono::duration_cast<std::chrono::microseconds>
	(now-dateOrigin).count();

	return micros;
}

#define PKT_MAX_REGS	32 // by agreement w/FMU

/* array of raw RC input values, microseconds */
#define PX4IO_PAGE_RAW_RC_INPUT		4
#define PX4IO_P_RAW_RC_COUNT			0	/* number of valid channels */
#define PX4IO_P_RAW_RC_FLAGS			1	/* RC detail status flags */
#define PX4IO_P_RAW_RC_FLAGS_FRAME_DROP		(1 << 0) /* single frame drop */
#define PX4IO_P_RAW_RC_FLAGS_FAILSAFE		(1 << 1) /* receiver is in failsafe mode */
#define PX4IO_P_RAW_RC_FLAGS_RC_DSM11		(1 << 2) /* DSM decoding is 11 bit mode */
#define PX4IO_P_RAW_RC_FLAGS_MAPPING_OK		(1 << 3) /* Channel mapping is ok */
#define PX4IO_P_RAW_RC_FLAGS_RC_OK		(1 << 4) /* RC reception ok */

#define PX4IO_P_RAW_RC_NRSSI			2	/* [2] Normalized RSSI value, 0: no reception, 255: perfect reception */
#define PX4IO_P_RAW_RC_DATA			3	/* [1] + [2] Details about the RC source (PPM frame length, Spektrum protocol type) */
#define PX4IO_P_RAW_FRAME_COUNT			4	/* Number of total received frames (wrapping counter) */
#define PX4IO_P_RAW_LOST_FRAME_COUNT		5	/* Number of total dropped frames (wrapping counter) */
#define PX4IO_P_RAW_RC_BASE			6	/* CONFIG_RC_INPUT_COUNT channels from here */

/* array of scaled RC input values, -10000..10000 */
#define PX4IO_PAGE_RC_INPUT		5
#define PX4IO_P_RC_VALID			0	/* bitmask of valid controls */
#define PX4IO_P_RC_BASE				1	/* CONFIG_RC_INPUT_COUNT controls from here */




int
sbus_init(const char *device)
{
	if (sbus_fd < 0) {
		sbus_fd = open(device, O_RDWR | O_NONBLOCK);
	}

	if (sbus_fd >= 0) {
		// Following code is desactivated since we need specific baud rate
		//		struct termios t;
		//
		//		/* 100000bps, even parity, two stop bits */
		//		tcgetattr(sbus_fd, &t);
		//		cfsetspeed(&t, 57600); // 100000
		//		t.c_cflag |= (CSTOPB | PARENB);
		//		tcsetattr(sbus_fd, TCSANOW, &t);

		// start: custom baud rate
		struct termios2 tio;
		ioctl(sbus_fd, TCGETS2, &tio);
		tio.c_cflag &= ~CBAUD;
		tio.c_cflag |= (BOTHER | CSTOPB | PARENB);
		tio.c_ispeed = 100000;
		tio.c_ospeed = 100000;
		/* do other miscellaneous setup options with the flags here */
		int retSpeed = ioctl(sbus_fd, TCSETS2, &tio);
		printf("custom baud rate = %d\n", retSpeed);
		// end: custom baud rate


		/* initialise the decoder */
		partial_frame_count = 0;
		last_rx_time = micros();

		printf("S.Bus: ready");

	} else {
		printf("S.Bus: open failed");
	}

	return sbus_fd;
}

void
sbus1_output(uint16_t *values, uint16_t num_values)
{
	uint8_t byteindex = 1; /*Data starts one byte into the sbus frame. */
	uint8_t offset = 0;
	uint16_t value;
	long now;

	now = micros();

	if ((now - last_txframe_time) > SBUS1_FRAME_DELAY) {
		last_txframe_time = now;
		uint8_t	oframe[SBUS_FRAME_SIZE] = { 0x0f };

		/* 16 is sbus number of servos/channels minus 2 single bit channels.
		 * currently ignoring single bit channels.  */

		for (unsigned i = 0; (i < num_values) && (i < 16); ++i) {
			value = (uint16_t)(((values[i] - SBUS_SCALE_OFFSET) / SBUS_SCALE_FACTOR) + .5f);

			/*protect from out of bounds values and limit to 11 bits*/
			if (value > 0x07ff) {
				value = 0x07ff;
			}

			while (offset >= 8) {
				++byteindex;
				offset -= 8;
			}

			oframe[byteindex] |= (value << (offset)) & 0xff;
			oframe[byteindex + 1] |= (value >> (8 - offset)) & 0xff;
			oframe[byteindex + 2] |= (value >> (16 - offset)) & 0xff;
			offset += 11;
		}

		write(sbus_fd, oframe, SBUS_FRAME_SIZE);
	}
}
void
sbus2_output(uint16_t *values, uint16_t num_values)
{
	char b = 'B';
	write(sbus_fd, &b, 1);
}

bool
sbus_input(uint16_t *values, uint16_t *num_values, bool *sbus_failsafe, bool *sbus_frame_drop, uint16_t max_channels)
{
	ssize_t		ret;
	long	now;

	/*
	 * The S.bus protocol doesn't provide reliable framing,
	 * so we detect frame boundaries by the inter-frame delay.
	 *
	 * The minimum frame spacing is 7ms; with 25 bytes at 100000bps
	 * frame transmission time is ~2ms.
	 *
	 * We expect to only be called when bytes arrive for processing,
	 * and if an interval of more than 3ms passes between calls,
	 * the first byte we read will be the first byte of a frame.
	 *
	 * In the case where byte(s) are dropped from a frame, this also
	 * provides a degree of protection. Of course, it would be better
	 * if we didn't drop bytes...
	 */
	now = micros();

	if ((now - last_rx_time) > 3000) {
		if (partial_frame_count > 0) {
			sbus_frame_drops++;
			partial_frame_count = 0;
		}
	}

	//	printf("Diff = %d\n", (int)(now - last_rx_time), now, last_rx_time);
	/*
	 * Fetch bytes, but no more than we would need to complete
	 * the current frame.
	 */
	ret = read(sbus_fd, &frame[partial_frame_count], SBUS_FRAME_SIZE - partial_frame_count);

	/* if the read failed for any reason, just give up here */
	if (ret < 1) {
		return false;
	}
	else {
		//		 printf("ret ! partial_frame_count=%d\n", partial_frame_count);
	}

	last_rx_time = now;

	/*
	 * Add bytes to the current frame
	 */
	partial_frame_count += ret;

	/*
	 * If we don't have a full frame, return
	 */
	if (partial_frame_count < SBUS_FRAME_SIZE) {
		return false;
	}

	/*
	 * Great, it looks like we might have a frame.  Go ahead and
	 * decode it.
	 */
	partial_frame_count = 0;
	return sbus_decode(now, values, num_values, sbus_failsafe, sbus_frame_drop, max_channels);
}

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

static bool
sbus_decode(long frame_time, uint16_t *values, uint16_t *num_values, bool *sbus_failsafe, bool *sbus_frame_drop,
		uint16_t max_values)
{
	/* check frame boundary markers to avoid out-of-sync cases */
	if ((frame[0] != 0x0f)) {
		sbus_frame_drops++;
		return false;
	}


	switch (frame[24]) {
	case 0x00:
		/* this is S.BUS 1 */
		break;

	case 0x03:
		/* S.BUS 2 SLOT0: RX battery and external voltage */
		break;

	case 0x83:
		/* S.BUS 2 SLOT1 */
		break;

	case 0x43:
	case 0xC3:
	case 0x23:
	case 0xA3:
	case 0x63:
	case 0xE3:
		break;

	default:
		/* we expect one of the bits above, but there are some we don't know yet */
		break;
	}

	/* we have received something we think is a frame */
	last_frame_time = frame_time;

	unsigned chancount = (max_values > SBUS_INPUT_CHANNELS) ?
			SBUS_INPUT_CHANNELS : max_values;

	/* use the decoder matrix to extract channel data */
	for (unsigned channel = 0; channel < chancount; channel++) {
		unsigned value = 0;

		for (unsigned pick = 0; pick < 3; pick++) {
			const struct sbus_bit_pick *decode = &sbus_decoder[channel][pick];

			if (decode->mask != 0) {
				unsigned piece = frame[1 + decode->byte];
				piece >>= decode->rshift;
				piece &= decode->mask;
				piece <<= decode->lshift;

				value |= piece;
			}
		}


		/* convert 0-2048 values to 1000-2000 ppm encoding in a not too sloppy fashion */
		values[channel] = (uint16_t)(value * SBUS_SCALE_FACTOR + .5f) + SBUS_SCALE_OFFSET;
	}

	/* decode switch channels if data fields are wide enough */
	if (RC_INPUT_CHANNELS > 17 && chancount > 15) {
		chancount = 18;

		/* channel 17 (index 16) */
		values[16] = (frame[SBUS_FLAGS_BYTE] & (1 << 0)) * 1000 + 998;
		/* channel 18 (index 17) */
		values[17] = (frame[SBUS_FLAGS_BYTE] & (1 << 1)) * 1000 + 998;
	}

	/* note the number of channels decoded */
	*num_values = chancount;

	/* decode and handle failsafe and frame-lost flags */
	if (frame[SBUS_FLAGS_BYTE] & (1 << SBUS_FAILSAFE_BIT)) { /* failsafe */
		/* report that we failed to read anything valid off the receiver */
		*sbus_failsafe = true;
		*sbus_frame_drop = true;

	} else if (frame[SBUS_FLAGS_BYTE] & (1 << SBUS_FRAMELOST_BIT)) { /* a frame was lost */
		/* set a special warning flag
		 *
		 * Attention! This flag indicates a skipped frame only, not a total link loss! Handling this
		 * condition as fail-safe greatly reduces the reliability and range of the radio link,
		 * e.g. by prematurely issueing return-to-launch!!! */

		*sbus_failsafe = false;
		*sbus_frame_drop = true;

	} else {
		*sbus_failsafe = false;
		*sbus_frame_drop = false;
	}

	return true;
}

long lastIter = micros();
long subiter = 0;

/**
int main() {

	sbus_init("/dev/ttyO1");

	int iter = 0;
	while (iter < 1000000)
	{
		long now = micros();

		bool sbus_failsafe, sbus_frame_drop;

		uint16_t r_raw_rc_values[16];
		uint16_t r_raw_rc_count[16];

		bool sbus_updated = sbus_input(r_raw_rc_values, r_raw_rc_count, &sbus_failsafe, &sbus_frame_drop, RC_INPUT_CHANNELS);

		if (sbus_updated) {
			printf("sbus update | ch_0=%d | ch_1=%d | ch_2=%d\n", r_raw_rc_values[0], r_raw_rc_values[1], r_raw_rc_values[2]);

			if (subiter % 2 == 0)
			{
				char parameters[100];
				sprintf(parameters, "%d %d", 3, 0);



				std::string paramStr = "";
				paramStr.append(parameters);

				long periodNs = 20000000l;
				std::stringstream periodStr;
				periodStr << periodNs;

				std::stringstream dutyCycleStr;
				dutyCycleStr << ((long)r_raw_rc_values[0]) * 1000l;

				paramStr.append(" " + periodStr.str() + " " + dutyCycleStr.str());

				std::string scriptSetup = "sudo ../../scripts/bb/pwm-setup.sh " + paramStr;

				// Execute script to setup pwm pin
				int result = system(scriptSetup.c_str());
//				printf("Duty cycle %s\n", dutyCycleStr.c_str());
				printf("Executed command setup pwm %s\n", scriptSetup.c_str());
			}

			subiter ++;
		}
		else {
			if (iter % 50000 == 0)
			{
				printf("No sbus\n");
			}
		}

		//		printf("dt_loop=%lu | dt_inside_loop=%lu\n", (now - lastIter), (micros()-now));
		lastIter = now;

		iter ++;
	}

	return 0;
}
*/
