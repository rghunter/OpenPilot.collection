/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000
 *     Brian Gerkey, Kasper Stoy, Richard Vaughan, & Andrew Howard
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/*
 Desc: Driver for the SHU chemical sensor
 Author: 
 Date: 8 Dec 2008
*/

/** @ingroup drivers Drivers */
/** @{ */
/** @defgroup driver_chemical chemical
 * @brief SHU chemical sensor

*/
/** @} */


#include <assert.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <libplayercore/playercore.h>
#include <libplayerxdr/playerxdr.h>

class Chemical : public Driver
{
  public:

    // Constructor
    Chemical(ConfigFile* cf, int section);

    int Setup();
    int Shutdown();

    // MessageHandler
    int ProcessMessage(QueuePointer & resp_queue,
		       player_msghdr * hdr,
		       void * data);
  private:
    // serial port file handle
    int file;

    // string name of serial port to use
    const char *port;

    // serial port baudrate
    int baudrate;

    // Main function for device thread.
    virtual void Main();

    // Read range data from laser and fill the player_chemical_data_t structure
    int ReadSensorData();

  protected:

    player_chemical_data_t data;
};


Driver* Chemical_Init(ConfigFile* cf, int section)
{
 	return (Driver*)(new Chemical(cf,section));
}



void Chemical_Register(DriverTable* table)
{
 	table->AddDriver("chemical", Chemical_Init);
}



////////////////////////////////////////////////////////////////////////////////
// Constructor
Chemical::Chemical(ConfigFile* cf, int section)
    : Driver(cf, section, true, PLAYER_MSGQUEUE_DEFAULT_MAXLEN, PLAYER_CHEMICAL_CODE)
{

  this->port = cf->ReadString(section, "port", "/dev/ttyUSB0");

  switch(cf->ReadInt(section, "baudrate", 9600))
  {
    case 9600:
      this->baudrate = B9600;
      break;

    case 19200:
      this->baudrate = 19200;
      break;

    case 38400:
      this->baudrate = B38400;
      break;

    case 57600:
      this->baudrate = B57600;
      break;

    case 115200:
      this->baudrate = B115200;
      break;

#ifdef B230400
    case 230400:
      this->baudrate = B230400;
      break;
#endif

    default:
      this->baudrate = B9600;
      break;
  }

  return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device
int Chemical::Setup()
{
  struct termios term;

  PLAYER_MSG0(2, "Chemical sensor initialising");

  // setup communication
  this->file = open(this->port, O_RDWR | O_NOCTTY);

  if (this->file < 0)
  {
    PLAYER_ERROR1("Unable to open serial port %s", this->port);
    return -1;
  }

  tcgetattr(this->file, &term);

  cfsetispeed(&term, baudrate);
  cfsetospeed(&term, baudrate);

  term.c_cflag |= (CLOCAL | CREAD);
  term.c_cflag &= ~(CSIZE|PARENB);
  term.c_cflag |= CS8;
  term.c_cflag &= ~CRTSCTS;
  term.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN);
  term.c_lflag |= ICANON;
  term.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
  term.c_oflag &= ~OPOST;

  tcflush(this->file, TCIFLUSH);

  tcsetattr(this->file, TCSANOW, &term);

  PLAYER_MSG0(2, "chemical sensor ready");

  // Start the device thread
  StartThread();

  return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
int Chemical::Shutdown()
{
  // shutdown chemical sensor device
  StopThread();

  close(this->file);

  PLAYER_MSG0(2, "chemical sensor shutdown");

  return(0);
}


int Chemical::ProcessMessage(QueuePointer & resp_queue,
                           player_msghdr * hdr,
                           void * data)
{
 // We don't have any message to proccess, do we ?

  // Don't know how to handle this message.
  return(-1);
}
////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void Chemical::Main()
{

  while(true)
  {
    // test if we are supposed to cancel
    pthread_testcancel();

    ProcessMessages();

    // Process incoming data
    if (!ReadSensorData())
    {
      // Make data available
      this->Publish(this->device_addr,
                    PLAYER_MSGTYPE_DATA, PLAYER_CHEMICAL_DATA_STATE,
                    (void*)&this->data, 0);
    }
  }
}


int Chemical::ReadSensorData()
{
  int n;
  char tmp[13];
  tmp[12] = '\0';

  n = read(this->file, tmp, 12);

  if (n != 12 && tmp[3] != ',' && tmp[7] != ',' && tmp[11] != '\n')
    return -1;

  tmp[3] = '\0';
  this->data.chemical_id = atoi(&tmp[0]);

  tmp[7] = '\0';
  this->data.concentration = (float) atoi(&tmp[4]) / 10;

  tmp[11] = '\0';
  this->data.temperature = (float) atoi(&tmp[8]) / 10;

  return 0;
}
