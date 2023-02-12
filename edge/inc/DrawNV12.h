/*
 * @Description: Draw bboxes and class labels on NV12 format frame data.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-09 20:35:23
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 17:38:29
 */

#pragma once

#include "Common.h"
#include "CharacterBitmaps.h"

static void drawLine (unsigned char* py,
    unsigned char* pu,
    unsigned char* pv,
    int   width,
    bool  vertical,
    int   length,
    unsigned char color_y,
    unsigned char color_u,
    unsigned char color_v,
    int thick)
{

    if (!vertical) {
        for (int i = 0; i < length / 2; i++) {
            for (int j = 0; j < thick; j++) {
                *(py + width * j + i * 2 + 0) = color_y;
                *(py + width * j + i * 2 + 1) = color_y;
            }
        }
        for (int i = 0; i < length / 2; i++) {
            for (int j = 0; j < thick / 2; j++) {
                *(pu + width * j + i * 2) = color_u;
                *(pv + width * j + i * 2) = color_v;
            }
        }
    } else{
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < thick; j++) {
                *(py + i * width + j) = color_y;
            }
        }
        for (int i=0; i<length / 2; i++) {
            for (int j = 0; j < thick / 2; j++) {
                *(pu + width * i + j * 2) = color_u;
                *(pv + width * i + j * 2) = color_v;
            }
        }
    }
}


static void drawRect(unsigned char* data,
    int   width,
    int   height,
    int   rect_x,
    int   rect_y,
    int   rect_w,
    int   rect_h,
    unsigned char color_y,
    unsigned char color_u,
    unsigned char color_v,
    int   thick)
{
    rect_x = ((rect_x / 2) * 2);
    rect_y = ((rect_y / 2) * 2);
    rect_w = ((rect_w / 2) * 2);
    rect_h = ((rect_h / 2) * 2);
    thick  = ((thick + 1) / 2) * 2;

    if (rect_x + rect_w + thick >= width) {
        rect_w = width  - rect_x - thick - 1;
        rect_w = (rect_w / 2) * 2;
    }
    if (rect_y + rect_h + thick >= height) {
        rect_h = height - rect_y - thick - 1;
        rect_h = (rect_h / 2) * 2;
    }
    if (rect_x < 0 || rect_y < 0 || rect_w < 0 || rect_h < 0) {
        return;
    }

    unsigned char* py;
    unsigned char* pu;
    unsigned char* pv;
    int offset;

    unsigned char* y = data;
    unsigned char* u = data + width * height;
    unsigned char* v = u + 1;

    offset = width * rect_y;
    py = y + offset     + rect_x;
    pu = u + offset / 2 + rect_x;
    pv = v + offset / 2 + rect_x;

    drawLine (py, pu, pv, width, false, rect_w, color_y, color_u, color_v, thick);

    offset = width * (rect_y + rect_h);
    py = y + offset     + rect_x;
    pu = u + offset / 2 + rect_x;
    pv = v + offset / 2 + rect_x;

    drawLine (py, pu, pv, width, false, rect_w, color_y, color_u, color_v, thick);

    offset = width * (rect_y + thick);
    py = y + offset     + rect_x;
    pu = u + offset / 2 + rect_x;
    pv = v + offset / 2 + rect_x;

    drawLine (py, pu, pv, width, true, rect_h - thick, color_y, color_u, color_v, thick);

    offset = width      *  rect_y;
    py = y + offset     + (rect_x + rect_w);
    pu = u + offset / 2 + (rect_x + rect_w);
    pv = v + offset / 2 + (rect_x + rect_w);

    drawLine (py, pu, pv, width, true, rect_h + thick, color_y, color_u, color_v, thick);
}

static void drawChar(unsigned char* data,
    int   width,
    int   height,
    int   pos_x,
    int   pos_y,
    const char ch,
    unsigned char color_y,
    unsigned char color_u,
    unsigned char color_v,
    int thick)
{
    int offset = ch - CHAR_BASE_OFFSET;
    if (offset < 0 || offset >= CHAR_TOTAL_COUNT) {
        return;
    }

    const unsigned char* bitmap = char_bitmaps_[offset];
    int index = 0;

    for (int i = 0; i < CHAR_BITMAP_HEIGHT; i++) {
        for (int j = 0; j < CHAR_BITMAP_WIDTH; j++, index++) {
            if (0 != (bitmap[index / 8] & (0x80u >> (index % 8)))) {
                drawRect(data, width, height, pos_x + j, pos_y + i,
                    0, 0, color_y, color_u, color_v, thick);
            }
        }
    }
}

static void drawString(unsigned char* data,
    int   width,
    int   height,
    int   pos_x,
    int   pos_y,
    const std::string& str,
    unsigned char color_y,
    unsigned char color_u,
    unsigned char color_v,
    int thick)
{
    const char* chars = str.c_str();

    for (int i = 0, n_space = 0; i < str.length (); i++) {
        if (chars[i] != ' ') {
            drawChar (data, width, height, pos_x + CHAR_BITMAP_WIDTH * (i - n_space) + CHAR_SPACING_WIDTH * n_space,
                pos_y, chars[i], color_y, color_u, color_v, thick);
        } else {
            n_space++;
        }
    }
}

static void drawNV12(u_char* frame_data,
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
    unsigned char b_y =  0.257 * b_r + 0.504 * b_g + 0.098 * b_b +  16;
    unsigned char b_u = -0.148 * b_r - 0.291 * b_g + 0.439 * b_b + 128;
    unsigned char b_v =  0.439 * b_r - 0.368 * b_g - 0.071 * b_b + 128;
    unsigned char t_y =  0.257 * t_r + 0.504 * t_g + 0.098 * t_b +  16;
    unsigned char t_u = -0.148 * t_r - 0.291 * t_g + 0.439 * t_b + 128;
    unsigned char t_v =  0.439 * t_r - 0.368 * t_g - 0.071 * t_b + 128;

    for (auto& result : *results.get()) {
        const cv::Rect& bbox = result.boundingBox();

        drawRect(frame_data, frame_pitch, frame_height, bbox.x, bbox.y,
            bbox.width, bbox.height, b_y, b_u, b_v, bbox_thickness);
        std::string className = result.className();
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << result.score();
        const std::string label = className + ": " + ss.str();

        drawString(frame_data, frame_pitch, frame_height, bbox.x,
            bbox.y - CHAR_BITMAP_HEIGHT, label,  t_y, t_u, t_v, text_thickness / 2);
    }
}