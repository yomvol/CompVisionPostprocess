#pragma once
#include "Effect.h"

namespace gl_cv_app
{
    class TriangulationEffect : public Effect
    {
#define PI 3.14159265358979323846

    public:
        TriangulationEffect(bool isDelaunay, cv::Scalar color, int threshold, bool isDrawingCenters, uint priority = 0) : m_is_delaunay(isDelaunay),
           m_color(color), m_is_drawing_centers(isDrawingCenters), Effect(priority)
        {
            m_detector = cv::FastFeatureDetector::create(threshold);
            srand(time(0));
        };
        ~TriangulationEffect() {};
        inline static uint getID() { return m_id; };
        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat result = frame, gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            auto keyPoints = extractKeyPoints(gray);

            cv::Rect rect(0, 0, frame.cols, frame.rows);
            cv::Subdiv2D subdiv(rect);

            for (auto& p : keyPoints)
            {
                if (rect.contains(p)) subdiv.insert(p);
            }

            if (m_is_delaunay)
            {
                auto triangles = performDelaunay(subdiv, rect);

                for (auto& triangle : triangles)
                {
                    cv::polylines(result, triangle, true, m_color, 1, cv::LINE_AA);
                }
            }
            else
            {
                drawVoronoi(result, subdiv);
            }

            return result;
        }

        bool isUsingCustomShader() override { return false; };
        void setColor(cv::Scalar color) { m_color = color; };
        void setDelaunay(bool isDelaunay) { m_is_delaunay = isDelaunay; };
        void setThreshold(int th) { m_detector->setThreshold(th); };
        void setDrawingCenters(bool isDrawingCenters) { m_is_drawing_centers = isDrawingCenters; };

    private:
        static uint m_id;
        bool m_is_delaunay = true;
        cv::Scalar m_color = cv::Scalar(0, 255, 0);
        bool m_is_drawing_centers = true;
        cv::Ptr<cv::FastFeatureDetector> m_detector;

        std::vector<cv::Point2f> extractKeyPoints(const cv::Mat& frame)
        {
            std::vector<cv::KeyPoint> key_points;
            m_detector->detect(frame, key_points);

            std::vector<cv::Point2f> points;
            for (auto& kp : key_points)
            {
                points.push_back(kp.pt);
            }
            return points;
        }

        std::vector<std::vector<cv::Point>> performDelaunay(const cv::Subdiv2D& divide, const cv::Rect& rect)
        {
            std::vector<cv::Vec6f> triangleList;
            divide.getTriangleList(triangleList);

            std::vector<std::vector<cv::Point>> triangles;
            for (auto& t : triangleList)
            {
                std::vector<cv::Point> triangle;
                triangle.push_back(cv::Point(cvRound(t[0]), cvRound(t[1])));
                triangle.push_back(cv::Point(cvRound(t[2]), cvRound(t[3])));
                triangle.push_back(cv::Point(cvRound(t[4]), cvRound(t[5])));

                if (rect.contains(triangle[0]) && rect.contains(triangle[1]) && rect.contains(triangle[2]))
                {
                    triangles.push_back(triangle);
                }
            }

            return triangles;
        }

        // Function to derive a stable pseudo-random color from a point
        cv::Scalar generateColorFromPoint(const cv::Point2f& point, const cv::Size& frameSize) {
            // Normalize coordinates to [0, 1]
            float normX = point.x / frameSize.width;
            float normY = point.y / frameSize.height;

            // Generate pseudo-random RGB values based on normalized coordinates
            int r = static_cast<int>(std::abs(std::sin(normX * PI)) * 255);
            int g = static_cast<int>(std::abs(std::sin(normY * PI)) * 255);
            int b = static_cast<int>(std::abs(std::sin((normX + normY) * PI)) * 255);

            return cv::Scalar(b, g, r); // OpenCV uses BGR format
        }

        void drawVoronoi(cv::Mat& img, cv::Subdiv2D& divide)
        {
            std::vector<std::vector<cv::Point2f>> facets;
            std::vector<cv::Point2f> centers;
            divide.getVoronoiFacetList({}, facets, centers);

            // Draw each facet
            for (size_t i = 0; i < facets.size(); i++) {
                std::vector<cv::Point> facet_points;
                for (cv::Point2f pt : facets[i])
                    facet_points.push_back(pt);
                
                // Fill the facet with a random color   
                cv::Scalar color = generateColorFromPoint(centers[i], img.size());
                cv::fillPoly(img, std::vector<std::vector<cv::Point>>{facet_points}, color);

                // Draw the site point
                if (m_is_drawing_centers) cv::circle(img, centers[i], 3, cv::Scalar(0, 0, 0), 1);
            }

            // Draw edges
            for (size_t i = 0; i < facets.size(); i++) {
                for (size_t j = 0; j < facets[i].size(); j++) {
                    cv::line(img, facets[i][j], facets[i][(j + 1) % facets[i].size()], cv::Scalar(0, 0, 0), 1);
                }
            }
        }
    };
}