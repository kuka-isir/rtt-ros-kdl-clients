/*! \file       example-client.cpp
 *  \brief
 *  \details
 *  \author     [Ryan Lober](http://www.ryanlober.com)
 *  \author     [Antoine Hoarau](http://ahoarau.com)
 *  \date       Feb 2016
 *  \copyright  GNU General Public License.
 */
/*
 *  This file is part of ocra-yarp.
 *  Copyright (C) 2016 Institut des Systèmes Intelligents et de Robotique (ISIR)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Network.h>
#include <yarp/os/Log.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Time.h>

#include "test-client/TestClient.h"
#include <ocra-recipes/ControllerClient.h>
#include <ocra-recipes/ClientManager.h>
#include <OcraKDLModel/OcraKDLModel.h>
#include <ros/ros.h>

int main (int argc, char * argv[])
{
    ros::init(argc, argv, "ocra_lwr_client_example");
    yarp::os::Log yLog;
    yarp::os::Network yarp;

    double network_timeout = 10.0;
    if (!yarp.checkNetwork(network_timeout))
    {
        yLog.fatal() << "YARP network is not available";
        return -1;
    }

    int loopPeriod = 10;



    std::cout << "Making robot model" << std::endl;
    std::shared_ptr<ocra::Model> model = std::make_shared<OcraKDLModel>("arm",true);

    std::cout << "Making controller client" << std::endl;
    std::shared_ptr<ocra_recipes::ControllerClient> ctrlClient
                        = std::make_shared<TestClient>(model, loopPeriod);

    std::cout << "Making client manager" << std::endl;
    std::shared_ptr<ocra_recipes::ClientManager> clientManager
                    = std::make_shared<ocra_recipes::ClientManager>(ctrlClient);

    std::cout << "Resource finder stuff" << std::endl;
    yarp::os::ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("test-client"); //when no parameters are given to the module this is the default context
    rf.configure(argc,argv);

    if (rf.check("help"))
    {
        clientManager->printHelp();
        return 0;
    }

    std::cout << "Configuring" << std::endl;
    clientManager->configure(rf);

    std::cout << "Launching client" << std::endl;
    return clientManager->launchClient();

    // return clientManager->runModule(rf);
}
