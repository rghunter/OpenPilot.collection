/*
 *  libplayerc : a Player client library
 *  Copyright (C) Andrew Howard 2002-2003
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) Andrew Howard 2003
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/***************************************************************************
 * Desc: Chemical sensor proxy.
 * Author: Elvina Motard
 * Date: 5 January 2009
 * CVS: $Id: dev_chemical.c 6566 2008-06-14 01:00:19Z thjc $
 **************************************************************************/
#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "playerc.h"
#include "error.h"

// Local declarations
void playerc_chemical_putmsg(playerc_chemical_t *device,
                           player_msghdr_t *header,
                           player_chemical_data_t *data,
                           size_t len);

// Create a new chemical sensor proxy
playerc_chemical_t *playerc_chemical_create(playerc_client_t *client, int index)
{
  playerc_chemical_t *device;

  device = malloc(sizeof(playerc_chemical_t));
  memset(device, 0, sizeof(playerc_chemical_t));
  playerc_device_init(&device->info, client, PLAYER_CHEMICAL_CODE, index,
                      (playerc_putmsg_fn_t) playerc_chemical_putmsg);
  return device;
}


// Destroy a chemical sensor proxy
void playerc_chemical_destroy(playerc_chemical_t *device)
{
  playerc_device_term(&device->info);
  free(device);
}


// Subscribe to the chemical sensor device
int playerc_chemical_subscribe(playerc_chemical_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}


// Un-subscribe from the chemical sensor device
int playerc_chemical_unsubscribe(playerc_chemical_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}


// Process incoming data
void playerc_chemical_putmsg(playerc_chemical_t *device, player_msghdr_t *header, player_chemical_data_t *data, size_t len)
{
  if((header->type == PLAYER_MSGTYPE_DATA) &&
     (header->subtype == PLAYER_CHEMICAL_DATA_STATE))
  {
    device->chemical_id        = data->chemical_id;
    device->concentration      = data->concentration;
    device->temperature        = data->temperature;
  }
    return;
}


