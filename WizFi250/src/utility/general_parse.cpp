/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#include "general_parse.h"

bool WXParse_Ip(uint8_t *p, IPAddress &ip)
{
	uint32_t val;
	uint32_t digits;
	int32_t quad;
//
//	Serial.println("ZZZZZZZZ");
//	Serial.println(ptr_ip[0],DEC);
//	Serial.println(ptr_ip[1],DEC);
//	Serial.println(ptr_ip[2],DEC);
//	Serial.println(ptr_ip[3],DEC);
//	Serial.println("ZZZZZZZZ");
//
//	memset(ptr_ip, 0, 4);


	while (isspace(*p))
	{
		p++;
	}

	for (quad = 0; quad < 4; quad++)
	{
		val = 0;
		digits = 0;

		while (*p)
		{
			if (!isdigit(*p))
			{
				break;
			}

			val = val * 10 + (*p - '0');

			p++;
			digits++;
		}

		if (!digits || val > 255 || (quad < 3 && *p != '.'))
		{
			return false;
		}

		ip[quad] = val;

		if (quad < 3)
		{
			p++;
		}
	}

	while (isspace(*p))
	{
		p++;
	}

	if (*p)
	{
		return false;
	}

	return true;
}
