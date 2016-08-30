/*
 * Sbus.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: adrien
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
#include "Sbus.h"

Sbus::Sbus() : last_rx_time(Date::zero()), last_frame_time(Date::zero()), last_txframe_time(Date::zero()), lastUpdate(Date::zero())
{
	sbus_fd = -1;
	last_frame_time = Date::now();
	last_rx_time = Date::now();
	partial_frame_count = 0;
	sbus_frame_drops = 0;
	lastUpdate = Date::now();
}

Sbus::~Sbus()
{
}

bool Sbus::isComLost()
{
	return (Date::now().durationFrom(lastUpdate) > DateUtils::secondsToMicros(5.0));
}

int Sbus::init(const char *device)
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
		last_rx_time = Date::now();

		printf("S.Bus: ready");

	} else {
		printf("S.Bus: open failed");
	}

	// Initialize channel
	for (int i = 0; i < NB_CHANNELS_OPERATIONNAL; i ++)
	{
		channels[i] = 1000;
	}

	return sbus_fd;
}

bool Sbus::input(uint16_t *values, uint16_t *num_values, bool *sbus_failsafe, bool *sbus_frame_drop, uint16_t max_channels)
{
	ssize_t		ret;

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

	if ((Date::now().durationFrom(last_rx_time)) > DateUtils::microToSeconds(3000l))
	{
		if (partial_frame_count > 0)
		{
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

	Date now = Date::now();
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
	return decode(now, values, num_values, sbus_failsafe, sbus_frame_drop, max_channels);
}


bool Sbus::decode(Date frame_time, uint16_t *values, uint16_t *num_values, bool *sbus_failsafe, bool *sbus_frame_drop,
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
	if (PX4IO_RC_INPUT_CHANNELS > 17 && chancount > 15) {
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

Date lastCall = Date::now();

// Must be called at least at 400Hz
void Sbus::fastLoop()
{
	float duration = Date::now().durationFrom(lastCall);
	lastCall = Date::now();
	printf("Delta call in ms= %.2f\n", duration);

	// Prepare input variables
	bool sbus_failsafe, sbus_frame_drop;
	uint16_t r_raw_rc_values[16];
	uint16_t r_raw_rc_count[16];

	// Call input function
	bool sbus_updated = input(r_raw_rc_values, r_raw_rc_count, &sbus_failsafe, &sbus_frame_drop, PX4IO_RC_INPUT_CHANNELS);

	// If sbus data are updated
	if (sbus_updated)
	{
		// Initialize channel (TODO check nb channels < 16 !!)
		for (int i = 0; i < NB_CHANNELS_OPERATIONNAL; i ++)
		{
			channels[i] = r_raw_rc_values[i];
		}

		lastUpdate = Date::now();
	}
}
