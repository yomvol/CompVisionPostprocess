#include "WebcamCapture.h"

namespace gl_cv_app {

    WebcamCapture::WebcamCapture()
    {
        m_capture = cv::VideoCapture(0, cv::CAP_DSHOW);
        if (!m_capture.isOpened())
        {
            std::cerr << "Error: Could not open web camera" << std::endl;
            return;
        }

        int width, height;
        width = m_capture.get(cv::CAP_PROP_FRAME_WIDTH);
        height = m_capture.get(cv::CAP_PROP_FRAME_HEIGHT);
        std::cout << "\n\nWebcam resolution: " << width << "x" << height << std::endl;
    }

    WebcamCapture::~WebcamCapture()
    {
        m_capture.release();
    }

    cv::Mat WebcamCapture::getFrame()
    {
        cv::Mat frame;
        m_capture >> frame;

        if (frame.empty())
        {
            std::cerr << "Error: Could not get frame" << std::endl;
            return cv::Mat();
        }
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        //cv::imshow("Webcam", frame);
        cv::flip(frame, frame, 0);
        return frame;
    }
}