#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

// Load a grayscale image
cv::Mat loadImage(const std::string& imageFile) {
    cv::Mat img = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Couldn't open image " << imageFile << ".\n";
        exit(-1);
    }
    return img;
}

// Generate grayscale noise frames
std::vector<cv::Mat> generateNoiseFrames(int width, int height, int numFrames) {
    std::vector<cv::Mat> noiseFrames;
    for (int i = 0; i < numFrames; ++i) {
        cv::Mat noise(height, width, CV_8UC1);
        cv::randu(noise, cv::Scalar(0), cv::Scalar(255));
        noiseFrames.push_back(noise);
    }
    return noiseFrames;
}

int main() {
    std::string imageFile = "/home/jim/Desktop/PiTests/images/image.jpg"; // Path to your image file

    cv::Mat img = loadImage(imageFile);

    const int numNoiseFrames = 30;
    std::vector<cv::Mat> noiseFrames = generateNoiseFrames(img.cols, img.rows, numNoiseFrames);

    cv::namedWindow("Blended Image Playback", cv::WINDOW_AUTOSIZE);
    int noiseFrameIndex = 0;

    while (true) {
        auto loopStartTime = std::chrono::steady_clock::now();

        // Use pre-generated noise frame
        cv::Mat noise = noiseFrames[noiseFrameIndex];
        noiseFrameIndex = (noiseFrameIndex + 1) % numNoiseFrames;

        // Blend image with noise (50% opacity)
        cv::Mat blendedImg;
        cv::addWeighted(img, 0.5, noise, 0.5, 0.0, blendedImg);
        cv::imshow("Blended Image Playback", blendedImg);

        int key = cv::waitKey(30);
        if (key == 27)  // ASCII code for the escape key
            break;

        auto loopEndTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = loopEndTime - loopStartTime;
        std::cout << "Loop duration: " << elapsed_seconds.count() << "s\n";
    }

    return 0;
}
