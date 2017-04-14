/*
 * Frame.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: adrien-i3
 */

#include "Frame.h"



Frame::Frame(Frame *pParent, Quaternion pRot, Vect3D pTranslate) : parent(pParent), rotation(pRot), translation(pTranslate)
{

}

