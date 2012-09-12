/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  debugutilities.h
 *
 *  This file is part of StandaloneNodeWithHAL.
 *
 *  StandaloneNodeWithHAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  StandaloneNodeWithHAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with StandaloneNodeWithHAL. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 12.09.2012
 */

#ifndef DEBUGUTILITIES_H_
#define DEBUGUTILITIES_H_

/**
 * This is just a debug utility to verify buffer length.
 * It prints the buffer according to the length information in len.
 * @param buffer the buffer
 * @param len the length of the string inside
 */
void printbuffer(char* buffer, unsigned int len);

#endif /* DEBUGUTILITIES_H_ */
