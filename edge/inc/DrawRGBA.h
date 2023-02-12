/*
 * @Description: Draw bboxes and class labels on RGBA format frame data.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-11 18:40:55
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 17:37:59
 */

#pragma once

#include "Common.h"

static void drawRGBA(u_char* frame_data,
    uint32_t frame_width,
    uint32_t frame_height,
    uint32_t frame_pitch,
    std::shared_ptr<std::vector<yolov5::Detection>>& results,
    std::tuple<int, int, int>& bbox_rgb,
    std::tuple<int, int, int>& text_rgb,
    uint32_t bbox_thickness,
    uint32_t text_thickness)
{
    auto [b_r, b_g, b_b] = bbox_rgb;
    auto [t_r, t_g, t_b] = text_rgb;
    cv::Mat frame(frame_height, frame_width, CV_8UC4, frame_data, frame_pitch);
    const cv::Scalar color(b_r, b_g, b_b);
    for (auto& result : *results.get()) {
        const cv::Rect& bbox = result.boundingBox();

        cv::rectangle(frame, bbox, color, bbox_thickness);
        std::string className = result.className();
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << result.score();
        const std::string label = className + ": " + ss.str();

        /*  Draw a rectangle above the bounding box, in which the
            label will be written   */
        int baseline = 0;
        const cv::Size textSize = cv::getTextSize(label, 
                                        cv::FONT_HERSHEY_PLAIN,
                                        1.0, text_thickness, &baseline);
        const cv::Point tl(bbox.x - bbox_thickness / 2.0, bbox.y - textSize.height);
        const cv::Rect labelRect(tl, textSize);
        cv::rectangle(frame, labelRect, color, -1);  /*  filled rectangle */

        /*  white text on top of the previously drawn rectangle */
        const cv::Point bl(tl.x, bbox.y - 2/2.0);
        cv::putText(frame, label, bl, cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(255, 255, 255), text_thickness);
    }
}