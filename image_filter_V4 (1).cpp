/*
File name: CS112_A3_Part1_17&18_20230487_20230515_20230272.cpp
Authors:

Author(1): Amr Khalid Mahfouz Mohammed
Author(2): Youssef Bahaa Abdel Wahab
Author(3): Youssef Hesham Ali Ali Zayan
IDS:

ID(1): 20230272 --> Filter 7: Darken and Lighten & menu of main function.
ID(2): 20230487 --> Filter 2: Black and White & Filter 5: Flip Image.
ID(3): 20230515 --> Filter 3: Invert Image & Filter 12: Blur Images.
All of the team handled errors and exceptions with distributing the work fairly between us.



 ♣ The explanation for the code:
• Header Files:
    ○ iostream: Provides input/output functionalities for user interaction.
    ○ fstream: Enables working with files for image reading and saving.
    ○ Image_Class.h: definition for representing and manipulating images.

• Functions:
    ○ BlackAndWhite: Converts an image to black and white using a threshold.
    ○ horizontalFlip: Flips an image horizontally.
    ○ verticalFlip: Flips an image vertically.
    ○ blurImage: Blurs an image using a box blur kernel.
    ○ invertColors: Inverts the colors of an image.
    ○ Lighter_or_darker: Adjusts the brightness of an image (lightens or darkens).
    ○ check_user_input: Validates user input for filter choices.
    ○ isValidFilename: Checks for valid image filenames with extensions.
    ○ main: The main function that drives the program's flow.

• Program Flow:
    ○ Enters a loop that continues until the user chooses to exit:
        ♦ The program displays a menu listing available filters (Black and White, Flip, Blur, etc.).
        ♦ The user selects a filter from the list.
        ♦ The program validates the user's choice to ensure they selected a valid filter option.

    ○ Prompts the user for the input filename and validates its existence:
        ♦ The program asks the user to enter the filename of the image they want to process.
        ♦ The program checks if the file exists and can be loaded successfully.

    ○ Prompts the user for a new filename to save the modified image with an extension:
        ♦ The program asks the user to choose a new name for the processed image,
          including a valid image file extension (e.g., .jpg, .png, etc.).

    ○ Calls the appropriate function based on the chosen filter to apply the effect:
        ♦ Depending on the user's filter selection, the program calls the corresponding
          function (e.g., BlackAndWhite for black and white conversion, horizontalFlip or verticalFlip
          for flipping, and so on).
        ♦ The chosen function applies the filter effect to the loaded image.

    ○ Saves the modified image with the new filename:
        ♦ The program saves the processed image with the user-provided filename.

    ○ Informs the user that the image has been saved successfully:
        ♦ The program displays a message confirming that the image has been processed and saved successfully.

    ○ The loop repeats, prompting the user for another filter and image until they choose to exit.
*/


#include <iostream>
#include <fstream>
#include "Image_Class.h"
#include <cmath>
using namespace std;

//  Function to convert an image into grayscale.
void gray_scale(Image img, string &save){
    int grayscale = 0;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            // Convert to grayscale
            grayscale = 0.299*(img(i,j,0) + 0.587*(img(i,j,1) + 0.114*(img(i,j,2))));
            img(i,j,0) = grayscale; img(i,j,1) = grayscale; img(i,j,2) = grayscale;}}
    img.saveImage(save);
}

// Function to convert image to black and white
void BlackAndWhite(Image& img, string& save) {
    int h = 128;
    int grayscale;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            // Convert to grayscale
            grayscale = 0.299*img.getPixel(i, j, 0) + 0.587*img.getPixel(i, j, 1) + 0.114*img.getPixel(i, j, 2);
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

// Function to flip image horizontally.
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

// Function to flip image vertically.
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

// Function to blur image using kernel.
void blurImage(Image& img, string& save) {
    // Main blur function with default box blur
    auto image_blurring = [](Image& image, float kernel[] = nullptr) {
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
    image_blurring(img);
    image_blurring(img);

    // Save the blurred image
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
    string valid_options[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17"};
    int size = sizeof(valid_options)/sizeof(valid_options[0]);
    for (int i = 0 ; i < size ; i++){
        if(choice == valid_options[i]){
            return true;}}
    return false;
}

bool isValidFilename(string& filename) {
    const char* extension = strrchr(filename.c_str(), '.');
    if (extension == nullptr) {
        std::cerr << "Invalid filename: " << filename << std::endl;
        return false;
    }
    return true;}

//  Function to detect the edges of images.
void edge_detection(Image img, string save,Image new_image){
/*  horizontal_sobel =  {-1, 0, 1}
                        {-2, 0, 2}
                        {-1, 0, 1}

    vertical_sobel =  {1,   2,   1}
                      {0,   0,   0}
                      {-1 , -2, -1} */

    for(int i = 1 ; i < img.width - 1 ; i++){
        for(int j = 1 ; j < img.height - 1 ; j++){ // pixel = 0 since it is in grey scale so it does not matter.
            int horizontal_gradient = (-img(i-1,j-1,0) + img(i+1,j-1,0) - img(i-1,j,0)*2 + img(i+1,j,0)*2 - img(i-1,j+1,0) + img(i+1,j+1,0));
            int vertical_gradient = (img(i-1,j-1,0) + img(i,j-1,0)*2 + img(i+1,j-1,0) - img(i-1,j+1,0) - img(i,j+1,0)*2 - img(i+1,j+1,0));
            int total_gradient = ceil(sqrt(pow(horizontal_gradient,2) + pow(vertical_gradient,2)));
            int threshold = 170;
            for(int k = 0 ; k < img.channels ; k++){
                if(total_gradient >= threshold){
                    new_image.setPixel(i,j,k,0);}
                else{new_image.setPixel(i,j,k,255);}
            }
        }}
    new_image.saveImage(save);
}

// Function to give an image an infra red filter.
void infra_red_filter(Image img, string save){
    for(int i = 0 ; i < img.width ; i++){
        for(int j = 0 ; j < img.height ; j++){
            unsigned int gray = 0;
            gray = 0.299*(img.getPixel(i, j, 0)) + 0.587 * (img.getPixel(i, j, 1)) + 0.114*(img.getPixel(i, j, 2));

            img.setPixel(i,j,0,255);
            img.setPixel(i, j, 1, (255 - gray) );
            img.setPixel(i, j, 2, (255 - gray) );
        }
    }
    img.saveImage(save);
}

void addFrame(Image& img, string& save, unsigned char R, unsigned char G, unsigned char B) {
    int w = img.width;
    int h = img.height;
    int fwidth = img.width / 100;
    int fspace = img.width / 50;

    // Calculate dimensions of the white area within the frame
    int innerw = w - 2 * (fwidth + fspace); // Account for frame, space on both sides
    int innerh = h - 2 * (fwidth + fspace);

    Image fimg(w, h);
    // Fill the entire image area with the original image content
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            fimg(i, j, 0) = img(i, j, 0);
            fimg(i, j, 1) = img(i, j, 1);
            fimg(i, j, 2) = img(i, j, 2);
        }
    }
    // Fill the white frame area within the image
    for (int i = fspace; i < w - fspace; i++) {
        for (int j = fspace; j < h - fspace; j++) {
            if (i < fspace + fwidth || i >= w - fspace - fwidth || j < fspace + fwidth || j >= h - fspace - fwidth) {
                fimg(i, j, 0) = R;
                fimg(i, j, 1) = G;
                fimg(i, j, 2) = B;
            }
        }
    }
    fimg.saveImage(save);
}

void addDoubleFrame(Image& img, string& save, unsigned char R, unsigned char G, unsigned char B) {
    int w = img.width;
    int h = img.height;
    int fwidth = img.width / 100;
    int fspace = img.width / 25;

    // Calculate dimensions of the white area within the frame
    int innerw = w - 2 * (fwidth + fspace); // Account for frame, space on both sides
    int innerh = h - 4 * (fwidth + fspace);

    Image fimg(w, h);
    // Fill the entire image area with the original image content
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            fimg(i, j, 0) = img(i, j, 0);
            fimg(i, j, 1) = img(i, j, 1);
            fimg(i, j, 2) = img(i, j, 2);
        }
    }
    // Fill the white frame area within the image
    for (int i = fspace; i < w - fspace; i++) {
        for (int j = fspace; j < h - fspace; j++) {
            if (i < fspace + fwidth || i >= w - fspace - fwidth || j < fspace + fwidth || j >= h - fspace - fwidth) {
                fimg(i, j, 0) = R;
                fimg(i, j, 1) = G;
                fimg(i, j, 2) = B;
            }
        }
    }
    fimg.saveImage(save);
}

void addCornerSquares(Image& img, unsigned char R, unsigned char G, unsigned char B) {
    int w = img.width;
    int h = img.height;
    int squareSize = w/17;

    // Ensure square size is valid relative to image size
    if (squareSize * 2 >= w || squareSize * 2 >= h) {
        throw runtime_error("Specified square size is too large for the image");
    }

    // Top left corner
    for (int i = 0; i < squareSize; i++) {
        for (int j = 0; j < squareSize; j++) {
            img(i, j, 0) = R;
            img(i, j, 1) = G;
            img(i, j, 2) = B;
        }
    }

    // Top right corner
    for (int i = w - squareSize; i < w; i++) {
        for (int j = 0; j < squareSize; j++) {
            img(i, j, 0) = R;
            img(i, j, 1) = G;
            img(i, j, 2) = B;
        }
    }

    // Bottom left corner
    for (int i = 0; i < squareSize; i++) {
        for (int j = h - squareSize; j < h; j++) {
            img(i, j, 0) = R;
            img(i, j, 1) = G;
            img(i, j, 2) = B;
        }
    }

    // Bottom right corner
    for (int i = w - squareSize; i < w; i++) {
        for (int j = h - squareSize; j < h; j++) {
            img(i, j, 0) = R;
            img(i, j, 1) = G;
            img(i, j, 2) = B;
        }
    }
}

bool deleteTempFile(const string& filename) {
    // Function to delete temporary file (optional error handling)
    if (remove(filename.c_str()) != 0) {
        cerr << "Error deleting temporary file: " << filename << endl;
        return false;
    }
    return true;
}

void filterofpurple(Image& img, string new_image_name) {
    int h = img.height;
    int w = img.width;

    Image new_img(w,h);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            new_img(i, j, 0) = 0.8* img(i, j, 0);
            new_img(i, j, 1) = 0.00005*img(i, j, 2) ; // Green channel
            new_img(i, j, 2) = 0.99*img(i, j, 2); // Blue channel
        }
    }
    new_img.saveImage(new_image_name);
}

void rotateImageClockwise90(Image& img, string new_image_name) {
    int h = img.height;
    int w = img.width;

    Image rotate_90(h,w);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < 3; k++) {
                int h_rot= h-j-1;
                int w_rot= i;
                rotate_90(h_rot, w_rot, k) =img(i, j, k);
            }
        }
    }
    rotate_90.saveImage(new_image_name);
}

void the180(Image& img, string& save) {
    int h = img.height;
    int w = img.width;
    Image temp(w, h);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < 3; k++){
                temp(i, j, k) = img(i, h - 1 - j, k);
            }
        }
    }
    Image rot_180 (w,h);
    for (int i=0; i <w ; i++){
        for (int j=0; j<h;j++){
            for (int k = 0; k < 3; k++){
                rot_180(i , j , k) = temp (w-1-i , j ,k );
            }
        }
    }
    rot_180.saveImage(save);
}

void rotateImageClockwise270(Image& img, string new_image_name) {
    int h = img.height;
    int w = img.width;

    Image rotate_270(h, w); // New image
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (w - i - 1 < 0) {
                continue;
            } else {
                rotate_270(j, w - 1 - i, 0) = img(i, j, 0);
                rotate_270(j, w - 1 - i, 1) = img(i, j, 1);
                rotate_270(j, w - 1 - i, 2) = img(i, j, 2);
            }
        }
    }
    rotate_270.saveImage(new_image_name);
}

void crop( Image& img,string new_image_name) {
    int width, height, startx, starty;
    cout << "Enter width, height, and starting points: ";
    cin >> width >> height >> startx >> starty;
    Image crop(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                crop(j, i, k) = img(startx + j, starty + i, k);
            }
        }
    }
    crop.saveImage( new_image_name);
}

void resize(Image& img,string new_image_name, int new_width, int new_height){
    Image res(new_width, new_height);
    float hrat = (float) (img.height) / (new_height);
    float wrat = (float) (img.width) / (new_width);
    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            float origx = j * wrat;
            float origy = i * hrat;
            for (int c = 0; c < 3; ++c) {
                res(j, i, c) = img.getPixel(origx, origy, c);
            }
        }
    }
    res.saveImage(new_image_name);
}

Image resize_for_merge(Image img, int new_width, int new_height){
    Image res(new_width, new_height);
    float hrat = (float) (img.height) / (new_height);
    float wrat = (float) (img.width) / (new_width);
    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            float origx = j * wrat;
            float origy = i * hrat;
            for (int c = 0; c < 3; ++c) {
                res(j, i, c) = img.getPixel(origx, origy, c);
            }
        }}
    return res;
}

void merge(Image img1, Image img2, string save, int min_width, int min_height){
    Image new_image(min_width, min_height);
    for(int i = 0 ; i < min_width ; i++){
        for(int j = 0; j < min_height ; j++){
            int avg = 0;
            for(int k = 0 ; k < 3 ; k++){
                avg = float(img1(i, j, k) + img2(i, j, k))/2;
                new_image.setPixel(i, j, k, avg);
            }
        }
    }
    new_image.saveImage(save);
}

void sun_light(Image& img,const string& new_image_name) {
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            unsigned int red = img(x, y, 0);
            unsigned int green = img(x, y, 1);
            unsigned int blue = img(x, y, 2);

            // Adjust RGB values
            red += 15;
            green += 40;
            blue -= 25;

            // Clamp values
            if (red > 255) red = 255;
            if (green > 255) green = 255;
            if (blue < 0) blue = 0;

            // Update image with clamped values
            img(x, y, 0) = red;
            img(x, y, 1) = green;
            img(x, y, 2) = blue;
        }
    }
    img.saveImage(new_image_name);
}

int main() {
    using namespace std;
    //the main loop for the program allowing to multi-choose filters without exit unless user chose to by entering '6'
    while (true) {
        string filename, new_image_name,final_framed_name;
        string temp_framed_name = filename + "_temp_framed.jpg"; // Temporary filename
        string final_temp_name = filename + "_final_framed.jpg";
        string choice;
        cout << "choose filter : \n";
        cout << "[1] grayscale\n"<< "[2] Black and white\n" << "[3] Flip\n" << "[4] Blur\n" << "[5] Inverted colors\n" << "[6] Make image lighter or darker\n" << "[7] detect edges\n" << "[8] Rotate Image(90° , 180° , 270°)\n" << "[9] Adding a Frame to the Picture\n" <<"[10] Crop\n"<<"[11] Resize\n"<<"[12] merge 2 photos"<< "Bonus : \n"<<"[13] The Land of Wano\n"<< "[14] infra_red\n" << "[15] purple at night\n"<< "[16] Exit\n";
        cout << "Enter your choice: "; //take the user's choice to decide which filter to apply
        getline(cin,choice);
        bool check_input = check_user_input(choice);
        while(!check_input){ //checks if user entered an invalid option and if true it asks for another entry
            cin.clear();fflush(stdin);
            cout << "Invalid Input\n\n";
            cout << "choose filter : \n";
            cout << "[1] grayscale\n"<< "[2] Black and white\n" << "[3] Flip\n" << "[4] Blur\n" << "[5] Inverted colors\n" << "[6] Make image lighter or darker\n" << "[7] detect edges\n" << "[8] Rotate Image(90° , 180° , 270°)\n" << "[9] Adding a Frame to the Picture\n" <<"[10] crop"<<"[11]Resize\n" << "[12] merge 2 photos" << "Bonus : \n" << "[13] The Land of Wano\n"<< "[14] infra_red\n" << "[15] purple at night\n"<< "[16] Exit\n";
            cout << "Enter your choice: ";
            getline(cin,choice);
            check_input = check_user_input(choice);}

        if (choice == "16"){ //exiting program if user chose to
            break;}
        while(true){
            try{
                cout << "Enter file name : "; //get the file name of photo user want to edit on and must be included in the same folder
                getline(cin,filename);
                Image _(filename);
                break;
            }
            catch(...){;}

        }

        //makes user choose the name of new filtered image and choose extention also
        cout << "Enter new name to save with extension for it (.jpg, .bmp, .png, .tga): "; cin >> new_image_name;
        bool valid = isValidFilename(new_image_name);
        //if user didn't provide a proper new image name , it will re-ask to enter a proper / valid one
        while(!valid){cout << "Enter new name to save with extension for it (.jpg, .bmp, .png, .tga): ";
            cin >> new_image_name;
            valid = isValidFilename(new_image_name);}

        Image Original_image(filename);

        // according to users choice it calls one function from the filters to start
        if(choice == "1"){
            gray_scale(Original_image, new_image_name);
        }
        else if(choice == "2"){
            BlackAndWhite(Original_image, new_image_name);}

        else if(choice == "3"){
            cout<<"do you want a [1] horizontal flip or a [2] vertical flip : ";
            string flip_choice;
            cin.clear();fflush(stdin);
            getline(cin,flip_choice);
            while(flip_choice != "1" && flip_choice != "2"){ //checks if users input is valid or not and if not it re-ask for a correct input
                cout<<"invalid input\n\n";
                cout<<"do you want a [1] horizontal flip or a [2] vertical flip : ";
                flip_choice;cin.clear();fflush(stdin);
                getline(cin,flip_choice);} //user choose between horizontal and vertical flipping
            if(flip_choice == "1"){horizontalFlip(Original_image,new_image_name);}
            else if(flip_choice == "2"){verticalFlip(Original_image,new_image_name);}
        }

        else if(choice == "4"){
            blurImage(Original_image, new_image_name);}

        else if(choice == "5"){
            invertColors(Original_image, new_image_name);}

        else if(choice == "6"){
            Lighter_or_darker(Original_image, new_image_name);}

        else if(choice == "7"){
            gray_scale(Original_image, new_image_name);
            Image gray_image(new_image_name);
            Image new_image(gray_image.width, gray_image.height);
            edge_detection(gray_image,new_image_name,new_image);
        }
        else if (choice == "8"){
            string choice;
            char choice2;
            cout << "Enter 'a' to start or 'e' to exit: ";
            cin >> choice2;

            while (choice2 == 'a' || choice2 == 'A') {
                cout << "Choose from the following:\n [1] 90° Rotation.\n [2] 180° Rotation.\n [3] 270° Rotation.\n [4] To Return.\n--> ";
                cin >> choice;

                if (choice == "4") {
                    break; // Exit the loop
                }
                Image image(filename);

                if (choice == "1") {
                    rotateImageClockwise90(image, new_image_name);
                } else if (choice == "2") {
                    the180(image, new_image_name);
                } else if (choice == "3") {
                    rotateImageClockwise270(image, new_image_name);
                } else {
                    cout << "Invalid input! Please try again." << endl;
                    continue;
                }
                cout << "New image has been saved successfully as \"" << new_image_name << "\".\n";
                cout << "Enter 'a' to continue or 'e' to exit: ";
                cin >> choice2;
            }
        }
        else if (choice == "9") {
            string frame_choice;
            while (true) {
                cout
                        << "Choose from the following:\n [1] Add simple frame.\n [2] Add fancy frame.\n [3] To Exit.\n--> ";
                cin >> frame_choice;
                if (frame_choice == "3") {
                    break; // Exit the loop
                }
                if (frame_choice == "1") {
                    addFrame(Original_image, new_image_name, 255, 255, 255);
                } else if (frame_choice == "2") {
                    addFrame(Original_image, temp_framed_name, 255, 255, 255);
                    // Load the temporary framed image
                    Image framed_image(temp_framed_name);
                    final_framed_name = new_image_name;

                    // Call addDoubleFrame on the loaded framed image
                    // Create a second temporary filename for the final result with both frames
                    string final_temp_name = filename + "_final_framed.jpg";
                    addDoubleFrame(framed_image, final_temp_name, 0, 120, 255);

                    // Load the final image with both frames
                    Image finalImage(final_temp_name);

                    // Add small white squares to the final image
                    addCornerSquares(finalImage, 255, 255, 255);

                    // Save the final image with squares
                    finalImage.saveImage(final_framed_name);

                    // Delete temporary images (optional error handling)
                    if (!deleteTempFile(temp_framed_name) || !deleteTempFile(final_temp_name)) {
                        return 1; // Indicate error during deletion
                    }

                } else {
                    cout << "Invalid input! Please try again." << endl;
                    continue;
                }
                cout << "New image has been saved successfully.\n\n";
            }
        }

        else if (choice == "10") {
            crop(Original_image,new_image_name);
        }

        else if(choice == "11"){
            int new_width, new_height;
            cout<<"Enter the width and height : ";
            cin >> new_width >> new_height;
            resize(Original_image, new_image_name,new_width,new_height);
        }

        else if (choice == "12"){
            string filename2;
            while(true){
            cin.clear();fflush(stdin);    
            try{
                cout << "Enter 2nd file name : "; //get the file name of photo user want to edit on and must be included in the same folder
                getline(cin,filename2);
                Image _(filename2);
                break;
            }
            catch(...){;}}
            Image second_image(filename2);
            string choice3;
            cout << "crop to smaller image [1] or resize to bigger image [2] : ";
            cin >> choice3;
            while(true){
                if(choice3 != "1" && choice3 != "2"){
                    cout << "invalid input\n";
                    cout << "crop to smaller image [1] or resize to bigger image [2] : ";
                    cin.clear();fflush(stdin);    
                    cin >> choice3;
                }
            }
            if(choice3 == "1"){
                int min_height = min(Original_image.height, second_image.height);
                int min_width = min(Original_image.width, second_image.width);
                merge(Original_image, second_image, new_image_name, min_width, min_height);
            }
            else if(choice3 == "2"){
                int max_height = max(Original_image.height, second_image.height);
                int max_width = max(Original_image.width, second_image.width);
                Image img1 = resize_for_merge(Original_image, max_width, max_height);
                Image img2 = resize_for_merge(second_image, max_width, max_height);
                merge(img1,img2,new_image_name, max_width, max_height);
            }
        }

        else if(choice=="13"){
            sun_light(Original_image, new_image_name);
        }

        else if(choice == "14"){
            infra_red_filter(Original_image,new_image_name);
        }

        else if (choice == "15"){
            filterofpurple(Original_image, new_image_name);
        }

        //message to announce user that his image is saved successfully.
        cout <<"Please check your saved items as the picture has been saved successfully.\n";
        cout <<"________________________________________________________________________________\n\n";
        cin.clear();fflush(stdin);
    }
    return 0;
}