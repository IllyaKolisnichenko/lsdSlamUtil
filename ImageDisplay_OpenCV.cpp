/**
* This file is part of LSD-SLAM.
*
* Copyright 2013 Jakob Engel <engelj at in dot tum dot de> (Technical University of Munich)
* For more information see <http://vision.in.tum.de/lsdslam> 
*
* LSD-SLAM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LSD-SLAM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with LSD-SLAM. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ImageDisplay.h"

#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <unordered_set>

#include <boost/thread.hpp>

namespace lsd_slam
{

namespace Util
{
    // Flag of thread usage
    const bool useImageDisplayThread = true;

    // Indigested list of all opened windows described as std::string
    std::unordered_set<std::string> openWindows;

    // Provide data thread safety
    boost::mutex                    openCVdisplayMutex;

    // Conditional variable (probably it generates the signal)
    boost::condition_variable       openCVdisplaySignal;

    // Pointer to the thread
    boost::thread*                  imageDisplayThread = 0;

    // Array of images
    std::vector<DisplayImageObect>  displayQueue;

    // Changing of value causes thread stop and all windows closing
    bool imageThreadKeepRunning     = true;

// Function is called in the thread loop
void displayThreadLoop()
{
    // Notify that thread is started
	printf("started image display thread!\n");

    // Set the mutex for the thread ???
    boost::unique_lock<boost::mutex> lock( openCVdisplayMutex );

    // While we need threads of images
	while(imageThreadKeepRunning)
	{
        // Stop the thread until we receive new data
        openCVdisplaySignal.wait( lock );

        // If we need to pause the thread
		if(!imageThreadKeepRunning)
            // Break the loop
			break;

        // If the queue is not empty
		while(displayQueue.size() > 0)
		{
            // If last element doesn't require autoSize
            if( !displayQueue.back().autoSize )
			{
                // If the name is still not in the list
                if( openWindows.find( displayQueue.back().name ) == openWindows.end() )
				{
                    // Create the window ??
                    cv::namedWindow ( displayQueue.back().name, cv::WINDOW_NORMAL );

                    cv::resizeWindow( displayQueue.back().name,
                                      displayQueue.back().img.cols,
                                      displayQueue.back().img.rows  );

                    // Add to the vector
                    openWindows.insert(displayQueue.back().name);
				}
			}

            // Show the window
            cv::imshow( displayQueue.back().name, displayQueue.back().img );

            // Delete the strcture from the queue
			displayQueue.pop_back();
		}
	}

    // Delete all the windows
	cv::destroyAllWindows();

    // Clear the vector
	openWindows.clear();

    // Notify that thread is finished
	printf("ended image display thread!\n");
}

// Create the image thread
void makeDisplayThread()
{
    // Allow the thread of images
    imageThreadKeepRunning  = true;

    // Run function in the thread
    imageDisplayThread      = new boost::thread(&displayThreadLoop);
}

void displayImage(const char* windowName, const cv::Mat& image, bool autoSize)
{
	if(useImageDisplayThread)
	{
        // If thread wasn't started yet
		if(imageDisplayThread == 0)
            // Run the thread
			makeDisplayThread();

        // Lock an access to the data
		boost::unique_lock<boost::mutex> lock(openCVdisplayMutex);

        // Add new instance of the image object
		displayQueue.push_back(DisplayImageObect());

        // Install parameters of the new object
        displayQueue.back().autoSize    = autoSize;
        displayQueue.back().img         = image.clone();
        displayQueue.back().name        = windowName;

        // If there are thread that are waiting for being unlocked than one of them will be unlocked
		openCVdisplaySignal.notify_one();
	}
	else
	{
        //  If parameter is not set
		if(!autoSize)
		{
			if(openWindows.find(windowName) == openWindows.end())
			{
                cv::namedWindow (windowName, cv::WINDOW_NORMAL);
				cv::resizeWindow(windowName, image.cols, image.rows);

				openWindows.insert(windowName);
			}
		}
		cv::imshow(windowName, image);
	}
	//cv::waitKey(1);
}

int waitKey(int milliseconds)
{
    // Waiting for pressing buttons
	return cv::waitKey(milliseconds);
}

int waitKeyNoConsume(int milliseconds)
{
	// Cannot implement this with OpenCV functions.
	return cv::waitKey(milliseconds);
}

void closeAllWindows()
{
    // Lock an access to the data
	boost::unique_lock<boost::mutex> lock(openCVdisplayMutex);

	if(useImageDisplayThread)
	{
        // If thread is running
		if(imageDisplayThread != 0)
		{
            // Stop the thread
			imageThreadKeepRunning = false;

            // Notify all
			openCVdisplaySignal.notify_all();

            // Message "Thread finish"
			printf("waiting for image display thread to end!\n");

            // Unlock the mutex
			lock.unlock();

            // ???
			imageDisplayThread->join();

            // Notify that thread is finished
			printf("done waiting for image display thread to end!\n");

            // Null the pointer
			imageDisplayThread = 0;
		}
	}
	else
	{
        // Destroy all the windows
		cv::destroyAllWindows();

        // Clear the queue
		openWindows.clear();
	}
}
}

}
