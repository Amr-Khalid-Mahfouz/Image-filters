#include <iostream>
#include <fstream>
#include "Image_Class.h"
using namespace std;

// Function to convert image to black and white
void BlackAndWhite(Image& img, string& save) {
    int h = 128;
    int grayscale;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            // Convert to grayscale
            grayscale = (img(i, j, 0) + img(i, j, 1) + img(i, j, 2)) / 3;
            // Apply threshold
            if (grayscale >= h) {
                // Set pixel to white
                img(i, j, 0) = 255;
                img(i, j, 1) = 255;
                img(i, j, 2) = 255;
            } else {
                // Set pixel to black
                img(i, j, 0) = 0;
                img(i, j, 1) = 0;
                img(i, j, 2) = 0;
            }
        }
    }
    img.saveImage(save); // Saving the modified image with a unique name
}

// Function to flip image horizontally
void horizontalFlip(Image& img, string& save) {
    Image hf(img.width, img.height);
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < img.channels; k++) {
                // Copy pixel from the original image to the flipped image with reversed indices
                hf(i, j, k) = img(img.width - i - 1, j, k);
            }
        }
    }
    hf.saveImage(save); // Saving the flipped image with a unique name
}

// Function to flip image vertically
void verticalFlip(Image& img, string& save) {
    Image vf(img.width, img.height);
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < 3; k++) // Assuming 3 channels for RGB
                vf(i, j, k) = img(i, img.height - j - 1, k);
        }
    }
    vf.saveImage(save); // Saving the flipped image with a unique name
}

// Main blur function with padding fix and modifiable kernel
void blurAndSaveImage(Image& img, string& save) {
    // Main blur function with default box blur
    auto image_bluriation = [](Image& image, float kernel[] = nullptr) {
        // Declare variables used within the function
        int kernel_size = 38; // Increased size of the blur kernel for stronger blur
        int padd_ing = kernel_size / 2;
        int newwidth = image.width + 2 * padd_ing;
        int newheight = image.height + 2 * padd_ing;
        unsigned char* temporary_picture = new unsigned char[newwidth * newheight * image.channels];

        // Default box blur kernel (if no kernel is provided)
        float default_kern[38];
        for (int i = 0; i < 38; ++i)
            default_kern[i] = 1.0f / 38;
        // Use the provided kernel or the default kernel
        float* chosen_kernel = kernel != nullptr ? kernel : default_kern;

        // Copy image data with padding
        for (int y = 0; y < newheight; ++y) {
            for (int x = 0; x < newwidth; ++x) {
                for (int c = 0; c < image.channels; ++c) {
                    if (y >= padd_ing && y < image.height + padd_ing && x >= padd_ing && x < image.width + padd_ing) {
                        temporary_picture[y * newwidth * image.channels + x * image.channels + c] = image.getPixel(x - padd_ing, y - padd_ing, c);
                    } else {
                        temporary_picture[y * newwidth * image.channels + x * image.channels + c] = 0;
                    }
                }
            }
        }

        // Apply the blur to the padded image
        int y = padd_ing;
        for (; y < image.height + padd_ing; ++y) {
            int x = padd_ing;
            for (; x < image.width + padd_ing; ++x) {
                for (int c = 0; c < image.channels; ++c) {
                    float sum = 0.0f;
                    for (int kx = -padd_ing; kx <= padd_ing; ++kx) {
                        int index_x = x + kx;
                        if (index_x >= padd_ing && index_x < newwidth - padd_ing) {
                            sum += temporary_picture[y * newwidth * image.channels + index_x * image.channels + c] * chosen_kernel[kx + padd_ing];
                        }
                    }
                    // Assign the blurred value to the temporary image
                    temporary_picture[y * newwidth * image.channels + x * image.channels + c] = (unsigned char)sum;
                }
            }
        }

        // Second pass: vertical
        for (int y = padd_ing; y < image.height + padd_ing; ++y) {
            int x = padd_ing;
            for (; x < image.width + padd_ing; ++x) {
                int c = 0;
                for (; c < image.channels; ++c) {
                    float sum = 0.0f;
                    int ky = -padd_ing;
                    for (; ky <= padd_ing; ++ky) {
                        int index_y = y + ky;
                        if (index_y >= padd_ing && index_y < newheight - padd_ing) {
                            sum += temporary_picture[index_y * newwidth * image.channels + x * image.channels + c] * chosen_kernel[ky + padd_ing];
                        }
                    }
                    // Assign the blurred value to the original image
                    if (x - padd_ing >= 0 && x - padd_ing < image.width && y - padd_ing >= 0 && y - padd_ing < image.height) {
                        image.setPixel(x - padd_ing, y - padd_ing, c, (unsigned char)sum);
                    }
                }
            }
        }

        delete[] temporary_picture;  // Deallocate memory
    };

    // Apply blur twice
    image_bluriation(img);
    image_bluriation(img);

    // Save the blurred image
    img.saveImage(save);
}

// Function to invert colors of an image
void invertColors(Image& img, string& save) {
    int l = 0;
    for (; l < img.width; ++l) {
        for (int m = 0; m < img.height; ++m) {
            // Invert each color channel
            img(l, m, 0) = 255 - img(l, m, 0); // Red channel
            img(l, m, 1) = 255 - img
                                         (l, m, 1); // Green channel
            img(l, m, 2) = 255 - img(l, m, 2); // Blue channel
        }
    }
    img.saveImage(save); // Saving the inverted image with a unique name
}

// Function to make the image lighter
void makeImageLighter(Image& img, const string& save) {
    Image new_img(img.width, img.height);
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned int colors[3] = {0, 0, 0};
            for (int k = 0; k < 3; k++) {
                colors[k] = img(i, j, k);
                // Adjusting pixel color to make it lighter
                new_img.setPixel(i, j, k, (colors[k] + colors[k] / 2 >= 255 ? 255 : colors[k] + (colors[k]) / 2));
            }
        }
    }
    new_img.saveImage(save); // Saving the lighter image with a unique name
}

int main() {
    string filename, savename;
    int choice;
    //the main loop for the program allowing to multi-choose filters without exit unless user choosed to by entering '7'
    while (true) {
        cout << "[1] Black and white\n";
        cout << "[2] Horizontal flipping\n";
        cout << "[3] Vertical Flipping\n";
        cout << "[4] Blur\n";
        cout << "[5] Inverted colors\n";
        cout << "[6] Make image lighter\n";
        cout << "[7] Exit\n";
        cout << "Enter your choice: "; cin >> choice;

        if (choice == 7) //exiting program if user choosed to
            break;

        cout << "Enter file name: ";
        cin >> filename;

        ifstream file(filename); //checks if the image name the user inputed exists in the same folder or not
        if (!file.is_open()) {
            cout << "Image file '" << filename << "' not founded in your file.\n";
            cout << "Please try again.\n\n";
            continue; // go back to the begainning of the loop so that user can re-choose filter to use
        }
        //makes user choose the name of new filtered image and choose extention also
        cout << "Enter new name to save with extension for it (.jpg, .bmp, .png, .tga): "; cin >> savename;
        
        Image img(filename);

        // according to users choice it calls one function from the filters to start
        
        switch (choice) {
            case 1:
                BlackAndWhite(img, savename);
                break;
            case 2:
                horizontalFlip(img, savename);
                break;
            case 3:
                verticalFlip(img, savename);
                break;
            case 4:
                blurAndSaveImage(img, savename);
                break;
            case 5:
                invertColors(img, savename);
                break;
            case 6:
                makeImageLighter(img, savename);
                break;
            default:
                cout << "Invalid choice\n";
                break;
        }   
        //message to announce user that his image is saved successfully
        cout <<"Please check your saved items as the picture has been saved successfully.\n\n";
        cout <<"________________________________________________________________________________\n\n";
    }

    return 0;
}