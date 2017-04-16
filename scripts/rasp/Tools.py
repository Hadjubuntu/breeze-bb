#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

# Retrieve input parameter
def retrieveInput(pArgvIdx, pDefaultValue):
	if len(sys.argv) > pArgvIdx + 1:
		return sys.argv[pArgvIdx]
	else:
		return pDefaultValue