#ifndef OPNECVDEBUGIMAGE_H
#define OPNECVDEBUGIMAGE_H

#include "debugimage.h"

#include <opencv2/highgui/highgui.hpp>

class openCVDebugImage;

class openCVDebugImageDestroyer
{
private:
    openCVDebugImage* p_instance;
public:
    ~openCVDebugImageDestroyer();
    void initialize( openCVDebugImage* p );
};

class openCVDebugImage : public debugImage
{
    // The structure that describes image
    struct DisplayImageObject
    {
        cv::Mat         img;            // OpenCV image presentation
        std::string     name;           // The name of the image
        bool            autoSize;       // Flag for autosize
    };

protected:
    openCVDebugImage();
    openCVDebugImage( const openCVDebugImage& );
    openCVDebugImage& operator=( openCVDebugImage& );

    virtual ~openCVDebugImage();

    friend class openCVDebugImageDestroyer;

private:
    static openCVDebugImage*            p_instatnceThis;
    static openCVDebugImageDestroyer    m_oDestroyer;

public:
    static openCVDebugImage* getInstance();
    virtual int waitKey(  int milliseconds );

protected:
    virtual void addNewImageInstance( const char *windowName, const cv::Mat &image, bool autoSize);

    virtual void showImage(const char* windowName, const cv::Mat& image, bool autoSize);

    virtual void loopIteration();

    virtual void destroyAllwindows();

private:
    // Indigested list of all opened windows described as std::string
    std::unordered_set<std::string> openWindows;
    // Array of images
    std::vector<DisplayImageObject>  displayQueue;
};

#endif // OPNECVDEBUGIMAGE_H

