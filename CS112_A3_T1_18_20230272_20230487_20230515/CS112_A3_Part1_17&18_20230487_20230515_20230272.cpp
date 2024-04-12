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
*/


/* ♣ The explanation for the code:
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

bool isValidFilename(const std::string& filename) {
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
            int threshold = 185;
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

int main() {
    //the main loop for the program allowing to multi-choose filters without exit unless user chose to by entering '6'
    while (true) {
        string filename, new_image_name;
        string choice;
        cout << "choose filter : \n";
        cout << "[1] grayscale\n"<< "[2] Black and white\n" << "[3] Flip\n" << "[4] Blur\n" << "[5] Inverted colors\n" << "[6] Make image lighter or darker\n" << "[7] detect edges\n" << "bonus :\n" << "[13] infra_red\n" << "[16] Exit\n";
        cout << "Enter your choice: "; //take the user's choice to decide which filter to apply
        getline(cin,choice);
        bool check_input = check_user_input(choice);
        while(!check_input){ //checks if user entered an invalid option and if true it asks for another entry
            cin.clear();fflush(stdin); 
            cout << "Invalid Input\n\n";
            cout << "choose filter : \n";
            cout << "[1] grayscale\n"<< "[2] Black and white\n" << "[3] Flip\n" << "[4] Blur\n" << "[5] Inverted colors\n" << "[6] Make image lighter or darker\n" << "[7] detect edges\n" << "bonus : \n" << "[13] infra_red\n" << "[16] Exit\n";
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
            else if(choice == "13"){
                infra_red_filter(Original_image,new_image_name);
            }    


        //message to announce user that his image is saved successfully.
        cout <<"Please check your saved items as the picture has been saved successfully.\n";
        cout <<"________________________________________________________________________________\n\n";
        cin.clear();fflush(stdin);
    }
    return 0;
}
