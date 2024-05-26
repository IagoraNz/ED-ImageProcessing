# 📄 Implementation of Image Manipulation Operations with Rollback History
Software development project for the second grade of the Data Structure I discipline. The general objective is to apply knowledge in linked lists in image processing through the C programming language

## 🔗 Development environment
🔧 GCC
```
gcc.exe (tdm64-1) 10.3.0
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
🔧 Python
```
Python 3.11.9
```

🔧 Visual Studio Code
```
1.89.1
```

## 🔗 **Objective**
The objective of this project is to develop a solution for image manipulation that allows the application of various operations, such as rotation, inversion and filters, on grayscale and color images. Furthermore, the system must provide a reversible history of the operations carried out, allowing the user to go back to previous versions of the image, or if necessary, go back to the later version, after all it is a double list.

**Implementation:**
1. **Data Structure:**
   - Use of a doubly linked list to store image history. Each node in the list will contain a reference to the image resulting from an operation and pointers to the next and previous nodes in the list.

2. **Image Manipulation Operations:**
   - Implementation of a variety of operations such as rotation, flipping (vertical and horizontal), applying filters (such as the average filter), histogram equalization and other image processing techniques.
   - Each operation must return a new processed image, preserving the original image in the history, this will always be the initial image in the list.

3. **User Interface:**
   - Development of an interface with menu options indicated for each type of image, i.e., RBG or Gray, 
   - View the history of operations (use python codes to generate the images).
   - The interface must provide controls for applying new operations, undoing operations and navigating through history (i.e., list positions).

4. **Random operations:**
   - In this approach, the user selects an image type to which they want to apply processing operations. The system then randomly chooses which operations will be applied to that image. After the operations are completed, the final result is presented in a simple linked list. Each entry in the list corresponds to an operation performed, allowing the user to visualize the step-by-step process and the final result of image manipulation.
  
## 🔗 Enjoy the work
1. Clone this repository
```
git clone https://github.com/IagoraNz/ED-ImageProcessing
```
2. Navigate to directory
```
cd ED-ImageProcessing
```
3. Open in VSCode in the directory and in windows (or command from your preferred IDE) and run
```
code .
```
