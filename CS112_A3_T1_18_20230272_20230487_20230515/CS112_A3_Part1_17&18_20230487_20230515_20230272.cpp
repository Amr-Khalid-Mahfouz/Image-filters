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
            grayscale = (img.getPixel(i, j, 0) + img.getPixel(i, j, 1) + img.getPixel(i, j, 2))/3;
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

// Function to blur image using kernel
void blurImage(Image& img, string& save) {
    // Main blur function with default box blur
    auto image_blurring = [](Image& image, float kernel[] = nullptr) {
        // Declare variables used within the function
        int kernel_size = 38; // Increased size of the blur kernel for stronger blur
        int padding = kernel_size / 2;
        int new_width = image.width + 2 * padding;
        int new_height = image.height + 2 * padding;
        unsigned char* temporary_picture = new unsigned char[new_width * new_height * image.channels];

        // Default box blur kernel (if no kernel is provided)
        float default_kern[38];
        for (int i = 0; i < 38; ++i)
            default_kern[i] = 1.0f / 38;
        // Use the provided kernel or the default kernel
        float* chosen_kernel = kernel != nullptr ? kernel : default_kern;

        // Copy image data with padding
        for (int y = 0; y < new_height; ++y) {
            for (int x = 0; x < new_width; ++x) {
                for (int c = 0; c < image.channels; ++c) {
                    if (y >= padding && y < image.height + padding && x >= padding && x < image.width + padding) {
                        temporary_picture[y * new_width * image.channels + x * image.channels + c] = image.getPixel(x - padding, y - padding, c);
                    } else {
                        temporary_picture[y * new_width * image.channels + x * image.channels + c] = 0;
                    }
                }
            }
        }

        // Apply the blur to the padded image
        int y = padding;
        for (; y < image.height + padding; ++y) {
            int x = padding;
            for (; x < image.width + padding; ++x) {
                for (int c = 0; c < image.channels; ++c) {
                    float sum = 0.0f;
                    for (int kx = -padding; kx <= padding; ++kx) {
                        int index_x = x + kx;
                        if (index_x >= padding && index_x < new_width - padding) {
                            sum += temporary_picture[y * new_width * image.channels + index_x * image.channels + c] * chosen_kernel[kx + padding];
                        }
                    }
                    // Assign the blurred value to the temporary image
                    temporary_picture[y * new_width * image.channels + x * image.channels + c] = (unsigned char)sum;
                }
            }
        }

        // Second pass: vertical
        for (int y = padding; y < image.height + padding; ++y) {
            int x = padding;
            for (; x < image.width + padding; ++x) {
                int c = 0;
                for (; c < image.channels; ++c) {
                    float sum = 0.0f;
                    int ky = -padding;
                    for (; ky <= padding; ++ky) {
                        int index_y = y + ky;
                        if (index_y >= padding && index_y < new_height - padding) {
                            sum += temporary_picture[index_y * new_width * image.channels + x * image.channels + c] * chosen_kernel[ky + padding];
                        }
                    }
                    // Assign the blurred value to the original image
                    if (x - padding >= 0 && x - padding < image.width && y - padding >= 0 && y - padding < image.height) {
                        image.setPixel(x - padding, y - padding, c, (unsigned char)sum);
                    }
                }
            }
        }

        delete[] temporary_picture;  // Deallocate memory
    };

    // Apply blur twice.
    image_blurring(img);
    image_blurring(img);

    // Save the blurred image.
    img.saveImage(save);
}

// Function to invert colors of an image.
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

// Function to make the image lighter.
void Lighter_or_darker(Image& img, const string& save) {
    while (true) {
        cin.clear();fflush(stdin);
        Image new_img(img.width, img.height);
        string light_or_dark_choice;
        cout << "choose whether to make the image (1) darker or (2) lighter : ";
        getline(cin,light_or_dark_choice);

        // Check for valid choices (1 or 2 only)
        while(light_or_dark_choice != "1" && light_or_dark_choice != "2") {
            cout << "Invalid input\n";
            cout << "\n";
            cout << "choose whether to make the image (1) darker or (2) lighter : ";
            getline(cin,light_or_dark_choice);}

        // Process image based on the choice
        if (light_or_dark_choice == "1") {
            // Darken the image
            for (int i = 0; i < img.width; ++i) {
                for (int j = 0; j < img.height; ++j) {
                    unsigned int colors[3] = {0, 0, 0};
                    for (int k = 0; k < 3; k++) {
                        colors[k] = img(i, j, k);
                        new_img.setPixel(i, j, k, colors[k] - colors[k] / 2);
                    }
                }
            }
        } else if (light_or_dark_choice == "2") {
            // Lighten the image
            for (int i = 0; i < img.width; ++i) {
                for (int j = 0; j < img.height; ++j) {
                    unsigned int colors[3] = {0, 0, 0};
                    for (int k = 0; k < 3; k++) {
                        colors[k] = img(i, j, k);
                        new_img.setPixel(i, j, k, (colors[k] + colors[k] / 2 >= 255 ? 255 : colors[k] + (colors[k]) / 2));
                    }
                }
            }
        }

        // Save the modified image regardless of choice
        new_img.saveImage(save);

        break; // Exit the loop after a valid choice is made
    }
}

// Function to check if the user chose a valid input for the filters.
bool check_user_input(string choice){
    string valid_options[] = {"1","2","3","4","5","6"};
    int size = sizeof(valid_options)/sizeof(valid_options[0]);
    for (int i = 0 ; i < size ; i++){
        if(choice == valid_options[i]){
            return true;}
    }
            return false;}

bool isValidFilename(const std::string& filename) {
    const char* extension = strrchr(filename.c_str(), '.');
    if (extension == nullptr) {
        std::cerr << "Invalid filename: " << filename << std::endl;
        return false;
    }
    return true;}

int main() {
    //the main loop for the program allowing to multi-choose filters without exit unless user chose to by entering '6'
    while (true) {
        string filename, new_image_name;
        string choice;
        cout << "choose filter : \n";
        cout << "[1] Black and white\n" << "[2] Flip\n" << "[3] Blur\n" << "[4] Inverted colors\n" << "[5] Make image lighter or darker\n" << "[6] Exit\n";
        cout << "Enter your choice: "; 
        getline(cin,choice);
        bool check_input = check_user_input(choice);
        while(!check_input){
            cin.clear();fflush(stdin);
            cout << "Invalid Input\n";
            cout << "choose filter : \n";
            cout << "[1] Black and white\n" << "[2] Flip\n" << "[3] Blur\n" << "[4] Inverted colors\n" << "[5] Make image lighter or darker\n" << "[6] Exit\n";
            cout << "Enter your choice: "; 
            getline(cin,choice);
            check_input = check_user_input(choice);}
        
        if (choice == "6"){ //exiting program if user chose to
            break;}
        
        cout << "Enter file name: ";
        getline(cin,filename);

        ifstream file(filename); //checks if the image name the user inputted exists in the same folder or not
        while (!file.is_open()) {
            cin.clear();fflush(stdin);            
            cout << "Image file '" << filename << "' not founded in your file.\n";
            cout << "Please try again.\n\n";
            cout << "Enter file name: ";
            getline(cin,filename);
        }

        //makes user choose the name of new filtered image and choose extention also
        cout << "Enter new name to save with extension for it (.jpg, .bmp, .png, .tga): "; cin >> new_image_name;
        bool valid = isValidFilename(new_image_name);

        while(!valid){cout << "Enter new name to save with extension for it (.jpg, .bmp, .png, .tga): ";
        cin >> new_image_name;
        valid = isValidFilename(new_image_name);}

        Image Original_image(filename);

        // according to users choice it calls one function from the filters to start
            if(choice == "1"){
                BlackAndWhite(Original_image, new_image_name);}

            else if(choice == "2"){
                cout<<"do you want a [1] horizontal flip or a [2] vertical flip : ";
                string flip_choice;
                do{
                    getline(cin,flip_choice);
                    if(flip_choice == "1")
                        horizontalFlip(Original_image, new_image_name);
                    else if (flip_choice == "2")
                        verticalFlip(Original_image, new_image_name);
                    else
                        cout<<"invalid input\n";}while(flip_choice != "1" && flip_choice != "2");}

            else if(choice == "3"){
                blurImage(Original_image, new_image_name);}

            else if(choice == "4"){
                invertColors(Original_image, new_image_name);}

            else if(choice == "5"){
                Lighter_or_darker(Original_image, new_image_name);}


        //message to announce user that his image is saved successfully
        cout <<"Please check your saved items as the picture has been saved successfully.\n\n";
        cout <<"________________________________________________________________________________\n\n";
        cin.clear();fflush(stdin);
    }

    return 0;
}
