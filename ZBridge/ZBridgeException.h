/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: ZBridgeException.h
  Developers: eak

  Revision History:
  27-jul-2013 eak: Original

  Abstract: ZBridge exceptions.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration/definition of the ZBridge exceptions.
 */

#ifndef ZBRIDGEEXCEPTION_H
#define ZBRIDGEEXCEPTION_H

#include <string>
#include <stdexcept>

using namespace std;


/** \brief ZBridge exceptions.
 *
 * ZBridge exceptions groups the application exceptions.
 *
 */
class ZBridgeException : public runtime_error
{
public:
    /** \brief Constructor for ZBridge exception with cause. */
    explicit ZBridgeException(const string& msg) : runtime_error(msg)
    {}
};

/** \brief Fatal exceptions.
 *
 * Exceptions considered fatal errors.
 *
 */
class FatalException : public ZBridgeException
{
public:
    explicit FatalException(const string& msg) : ZBridgeException(msg)
    {}
};

/** \brief Net protocol exceptions.
 *
 * Exceptions considered non fatal errors.
 *
 */
class NetProtocolException : public ZBridgeException
{
public:
    explicit NetProtocolException(const string& msg) : ZBridgeException(msg)
    {}
};

/** \brief Play exceptions.
 *
 * Exceptions considered non fatal errors.
 *
 */
class PlayException : public ZBridgeException
{
public:
    explicit PlayException(const string& msg) : ZBridgeException(msg)
    {}
};

#endif // ZBRIDGEEXCEPTION_H
