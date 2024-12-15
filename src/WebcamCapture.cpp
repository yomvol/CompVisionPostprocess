#include "WebcamCapture.h"

namespace gl_cv_app {

    WebcamCapture::WebcamCapture() : m_texture(0)
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

    GLuint WebcamCapture::getTexture(cv::Mat frame)
    {
        if (frame.empty())
            return 0;

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        // Setup texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Upload the frame to the texture frame.data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

        return m_texture;
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