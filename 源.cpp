#include <iostream>
#include <vector>
#include <cmath>
#include<map>
#include<Windows.h>
#include<fstream>
#include"MultiArray.h"
#include"FunctionWind.h"
#include"常用函数.hpp"

class FeedForwardNeuralNetwork {
public:
    FeedForwardNeuralNetwork(int numInputs, int numHidden, int numOutputs) :
        numInputs(numInputs), numHidden(numHidden), numOutputs(numOutputs)
    {
        InitializeWeights();
    }

    void InitializeWeights() {
        // Initialize weights randomly between -0.5 and 0.5
        hiddenLayerWeights.resize(numHidden);
        for (int i = 0; i < numHidden; ++i) {
            hiddenLayerWeights[i].resize(numInputs + 1); // +1 for the bias
            for (int j = 0; j <= numInputs; ++j) {
                hiddenLayerWeights[i][j] = RandomWeight();
            }
        }

        outputLayerWeights.resize(numOutputs);
        for (int i = 0; i < numOutputs; ++i) {
            outputLayerWeights[i].resize(numHidden + 1); // +1 for the bias
            for (int j = 0; j <= numHidden; ++j) {
                outputLayerWeights[i][j] = RandomWeight();
            }
        }
    }

    void Train(const std::vector<std::vector<double>>& trainingInputs,
        const std::vector<std::vector<double>>& trainingOutputs,
        int maxEpochs, double learningRate) {
        int numTrainingSamples = trainingInputs.size();

        for (int epoch = 1; epoch <= maxEpochs; ++epoch) {
            double totalError = 0.0;

            // Iterate through each training sample
            for (int sample = 0; sample < numTrainingSamples; ++sample) {
                // Forward propagation
                std::vector<double> hiddenOutputs(numHidden);
                for (int j = 0; j < numHidden; ++j) {
                    double sum = hiddenLayerWeights[j][0]; // Bias weight
                    for (int i = 0; i < numInputs; ++i) {
                        sum += trainingInputs[sample][i] * hiddenLayerWeights[j][i + 1];
                    }
                    hiddenOutputs[j] = Sigmoid(sum);
                }

                std::vector<double> outputs(numOutputs);
                for (int k = 0; k < numOutputs; ++k) {
                    double sum = outputLayerWeights[k][0]; // Bias weight
                    for (int j = 0; j < numHidden; ++j) {
                        sum += hiddenOutputs[j] * outputLayerWeights[k][j + 1];
                    }
                    outputs[k] = Sigmoid(sum);
                }

                // Backpropagation
                std::vector<double> outputErrors(numOutputs);
                for (int k = 0; k < numOutputs; ++k) {
                    double error = trainingOutputs[sample][k] - outputs[k];
                    outputErrors[k] = error * SigmoidDerivative(outputs[k]);
                    totalError += std::abs(error);
                }

                std::vector<double> hiddenErrors(numHidden);
                for (int j = 0; j < numHidden; ++j) {
                    double error = 0.0;
                    for (int k = 0; k < numOutputs; ++k) {
                        error += outputErrors[k] * outputLayerWeights[k][j + 1];
                    }
                    hiddenErrors[j] = error * SigmoidDerivative(hiddenOutputs[j]);
                }

                // Update weights
                for (int k = 0; k < numOutputs; ++k) {
                    outputLayerWeights[k][0] += learningRate * outputErrors[k]; // Update bias weight
                    for (int j = 0; j < numHidden; ++j) {
                        outputLayerWeights[k][j + 1] += learningRate * outputErrors[k] * hiddenOutputs[j];
                    }
                }

                for (int j = 0; j < numHidden; ++j) {
                    hiddenLayerWeights[j][0] += learningRate * hiddenErrors[j]; // Update bias weight
                    for (int i = 0; i < numInputs; ++i) {
                        hiddenLayerWeights[j][i + 1] += learningRate * hiddenErrors[j] * trainingInputs[sample][i];
                    }
                }
            }
            if (epoch % 20 == 0)
                std::cout << "Epoch: " << epoch << ", Error: " << totalError << std::endl;

            // Stop training if the total error is below a threshold
            if (totalError < 0.01) {
                break;
            }
        }
    }

    double Predict(const std::vector<double>& inputs) {
        std::vector<double> hiddenOutputs(numHidden);
        for (int j = 0; j < numHidden; ++j) {
            double sum = hiddenLayerWeights[j][0]; // Bias weight
            for (int i = 0; i < numInputs; ++i) {
                sum += inputs[i] * hiddenLayerWeights[j][i + 1];
            }
            hiddenOutputs[j] = Sigmoid(sum);
        }

        double output = 0.0;
        for (int k = 0; k < numOutputs; ++k) {
            double sum = outputLayerWeights[k][0]; // Bias weight
            for (int j = 0; j < numHidden; ++j) {
                sum += hiddenOutputs[j] * outputLayerWeights[k][j + 1];
            }
            output = Sigmoid(sum);
        }

        return output;
    }

private:
    int numInputs;
    int numHidden;
    int numOutputs;
    std::vector<std::vector<double>> hiddenLayerWeights;
    std::vector<std::vector<double>> outputLayerWeights;

    double RandomWeight() {
        return static_cast<double>(rand()) / RAND_MAX - 0.5;
    }

    double Sigmoid(double x) {
        return 1.0 / (1.0 + exp(-x));
    }

    double SigmoidDerivative(double x) {
        return x * (1.0 - x);
    }
};

enum ImageType {
    airplane,
    automobile,
    bird,
    cat,
    deer,
    dog,
    frog,
    horse,
    ship,
    truck,
};
HBITMAP ConvertLineRGBImageToHBITMAP(int w,int h,const std::vector<BYTE>& image) {
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;  // 负值表示从顶部开始扫描
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;      // 每个像素 24 位（RGB）
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    HDC hdc = GetDC(NULL);
    
    HBITMAP hbitmap_ = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);

    if (hbitmap_) {
        BYTE* ptr = reinterpret_cast<BYTE*>(bits);
        size_t imageColorDataSize = h * w;
        size_t colorLong = h * w-1;
        // 将图像数据复制到位图中
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {

                int idx = y * w + x;
                ptr[3 * idx+2] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 1] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 0] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                colorLong--;
            }
        }
    }
    else
    {
        CMath::GetSystemLastErrorClass Error;
        std::cout << "创建位图失败" << std::endl;
        std::cout << "错误代码：" << Error.GetErrorCode() << std::endl;
        std::cout << Error.GetErrorMsg() << std::endl;
    }
    ReleaseDC(nullptr, hdc);
    return hbitmap_;
}
//颜色通道顺序
enum class ColorChannelOrder
{
    RGB,
    RBG,
    GRB,
    GBR,
    BRG,
    BGR,
};
std::vector<unsigned char> ConvertLineRGB_ColorImageToRGBimage(int w, int h, const std::vector<BYTE>& image, ColorChannelOrder cco = ColorChannelOrder::RBG) {
    std::vector<unsigned char> out(image.size());
    BYTE* ptr = reinterpret_cast<BYTE*>(out.data());
    size_t imageColorDataSize = h * w;
    size_t colorLong = h * w - 1;
    // 将图像数据复制到位图中
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {

            int idx = y * w + x;
            switch (cco)
            {
            case ColorChannelOrder::RGB:
                ptr[3 * idx + 0] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 1] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 2] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            case ColorChannelOrder::RBG:
                ptr[3 * idx + 0] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 2] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 1] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            case ColorChannelOrder::GRB:
                ptr[3 * idx + 1] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 0] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 2] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            case ColorChannelOrder::GBR:
                ptr[3 * idx + 2] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 0] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 1] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            case ColorChannelOrder::BRG:
                ptr[3 * idx + 1] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 2] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 0] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            case ColorChannelOrder::BGR:
                ptr[3 * idx + 2] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 1] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 0] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            default:
                ptr[3 * idx + 0] = image[colorLong];                 // 红色通道
                ptr[3 * idx + 1] = image[colorLong + imageColorDataSize];      // 绿色通道
                ptr[3 * idx + 2] = image[colorLong + imageColorDataSize * 2];    // 蓝色通道
                break;
            }

            colorLong--;
        }
    }
    return out;
}
void LoadCifar_10Image_oneImage(const std::vector<unsigned char>& fileData, size_t imageID, 
    int imageTypeCharSize, int imageWidth, int imageHeight, std::vector<unsigned char>& RGB_Line_imageData,int& Imagetype)
{
    if (fileData.empty())
        return;
    UINT OneImageDataCount = imageHeight * imageWidth * 3 + imageTypeCharSize;
    size_t DataPtr = imageID * OneImageDataCount;
    std::vector<char>OneImageData(OneImageDataCount);
    auto ptr = fileData.begin() + DataPtr;
    OneImageData.assign(ptr, ptr + OneImageDataCount);
    for (int i = 0; i < imageTypeCharSize; ++i)
    {
        Imagetype <<= 8;
        Imagetype |= OneImageData[i];
    }
    RGB_Line_imageData.assign((OneImageData.begin() + imageTypeCharSize), OneImageData.end());
}
void LoadCifar_10Image(const std::string& filename, std::vector < std::pair<CMath::Image, ImageType> >& Out)
{
    std::ifstream file(filename, std::ios::binary); // 打开二进制文件
    if (!file)
        return;
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> fileData(fileSize);
    // 读取文件内容到缓冲区
    file.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    if (!file)
        return;
    int imagetype;
    std::vector<unsigned char>imageData;
    for (int i = 0; i < 10000; ++i)
    {
        LoadCifar_10Image_oneImage(fileData, i, 1, 32, 32, imageData, imagetype);
        Out.push_back(std::make_pair(CMath::Image(imageData, 32,32, CMath::Image::ColorChannelCount::RGB_24,
            CMath::Image::ColorChannelOrder::RGB,true), (ImageType)imagetype));
    }

}


int main() {
    int x = 3;
    int y = 4;
    int z = 4;


    CMath::ImageWind iw;
    std::vector < std::pair<CMath::Image, ImageType>>images;
    LoadCifar_10Image("D:\\测试数据\\cifar-10-batches-bin\\data_batch_1.bin", images);
    std::vector<CMath::Image>bitmaps(images.size());
    for (int i = 0; i < images.size(); ++i)
    {
        bitmaps[i] = images[i].first;
    }

    iw.SetImage(bitmaps);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    //// Create an instance of the FeedForwardNeuralNetwork class
    //FeedForwardNeuralNetwork nn(2, 2, 1);

    //// Define the training data
    //std::vector<std::vector<double>> trainingInputs = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
    //std::vector<std::vector<double>> trainingOutputs = { {0}, {1}, {1}, {0} };

    //// Train the neural network
    //nn.Train(trainingInputs, trainingOutputs, 20000, 0.1);

    //// Test the trained network
    //std::cout << "0 XOR 0 = " << nn.Predict({ 0, 0 }) << std::endl;
    //std::cout << "0 XOR 1 = " << nn.Predict({ 0, 1 }) << std::endl;
    //std::cout << "1 XOR 0 = " << nn.Predict({ 1, 0 }) << std::endl;
    //std::cout << "1 XOR 1 = " << nn.Predict({ 1, 1 }) << std::endl;

    return 0;
}
