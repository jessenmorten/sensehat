#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

#define FRAMEBUFFER_FILE "/dev/fb0"
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define DEFAULT_COLOR rgb(0, 0, 0)

struct Color {
    int red;
    int green;
    int blue;
};

Color rgb(int r, int g, int b) {
    return Color{
        .red = r,
        .green = g,
        .blue = b,
    };
}

void write_matrix(std::vector<std::vector<Color>> matrix) {
    auto fb = std::ofstream(FRAMEBUFFER_FILE, std::ios::out | std::ios::binary);
    if (!fb.is_open()) {
        std::cerr << "Failed to open framebuffer device" << std::endl;
        return;
    }

    auto width = matrix.size() > MATRIX_WIDTH ? MATRIX_WIDTH : matrix.size();
    auto height = matrix[0].size() > MATRIX_HEIGHT ? MATRIX_HEIGHT : matrix[0].size();

    for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            auto color = i < width && j < height ? matrix[i][j] : DEFAULT_COLOR;
            unsigned char color_bytes[2];
            color_bytes[0] = ((color.green & 0x07) << 5) | (color.blue >> 3);
            color_bytes[1] = (color.red & 0xF8) | (color.green >> 5);
            fb.write(reinterpret_cast<char*>(color_bytes), 2);
        }
    }

    fb.close();
}

void fill(Color color) {
    auto matrix = std::vector<std::vector<Color>>(MATRIX_WIDTH, std::vector<Color>(MATRIX_HEIGHT, color));
    write_matrix(matrix);
}

std::vector<std::vector<Color>> create_colered_matrix(std::vector<std::vector<bool>> matrix, Color color, Color default_color = DEFAULT_COLOR) {
    auto colored_matrix = std::vector<std::vector<Color>>(matrix.size(), std::vector<Color>(matrix[0].size(), default_color));

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            colored_matrix[i][j] = matrix[i][j] ? color : default_color;
        }
    }

    return colored_matrix;
}

int main() {
    auto red = rgb(255, 0, 0);
    fill(red);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto green = rgb(0, 255, 0);
    fill(green);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto blue = rgb(0, 0, 255);
    fill(blue);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    fill(DEFAULT_COLOR);
    return 0;
}

