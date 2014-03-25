/*
 * CDS55.hh
 *
 *  Created on: Jun 20, 2013
 *      Author: keevi7c3l
 */

#ifndef CDS55_HH_
#define CDS55_HH_


#include <pthread.h>
#include <libplayercore/playercore.h>
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class CDS55 : public ThreadedDriver
{
  public:

    // Constructor; need that
    CDS55(ConfigFile* cf, int section);

    // This method will be invoked on each incoming message
    virtual int ProcessMessage(QueuePointer &resp_queue,
                               player_msghdr * hdr,
                               void * data);

  private:

    // Main function for device thread.
    virtual void Main();
    virtual int MainSetup();
    virtual void MainQuit();

    void SetSpeedCmd(player_position2d_cmd_vel_t cmd);

    // My position interface
    player_devaddr_t m_position_addr;

};


#endif /* CDS55_HH_ */
