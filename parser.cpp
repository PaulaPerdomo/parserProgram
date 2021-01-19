#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here
bool invalidArgument(stringstream & stream) {
  //if((stream.fail() && !stream.eof()) || stream.peek() == '.') {
  if(stream.fail() && !stream.eof()) {
    return true;
  }
  else {
    for(char c = '!'; c <= '/'; c++) {
      if(stream.peek() == c) {
        return true;
      }
    }
    for(int c = ':'; c <= '~'; c++) {
      if(stream.peek() == c) {
        return true;
      }
    }
  }
  return false;
}

bool invalidShapeName(string n) {
  for(int i = 0; i < NUM_KEYWORDS; i++) {
      if(n == keyWordsList[i]) {
        return true;
      }
    }
    for(int i = 0; i < NUM_TYPES; i++) {
      if(n == shapeTypesList[i]) {
        return true;
      }
    }
  return false;
}

bool shapeNameExists(string n) {
  for(int i = 0; i < shapeCount; i++) {
    if(n == shapesArray[i]->getName()) {
      return true;
    }
  }
  return false;
}

bool shapeNameNotFound(string n) {
  for(int i = 0; i < shapeCount; i++) {
    if(n == shapesArray[i]->getName()) {
      return false;
    }
  }
  return true;
}

bool invalidShapeType(string t) {
  bool flag = true;
  for(int i = 0; i < NUM_TYPES; i++) {
    if(t == shapeTypesList[i]) {
      flag = false;
    }
  }
  if(flag) {
    return true;
  }
  return false;
}

bool invalidValueforCreate(stringstream & stream, string t, int x_loc, int y_loc, int x_sz, int y_sz) {
    if(x_loc < 0 || y_loc < 0) {
     return true;
    }
    if(t == shapeTypesList[0] && x_sz != y_sz) {
      return true;
    }
  return false;
}

bool invalidValueforRotate(stringstream & stream, int angle) {
  if(!stream.fail()) {
   if(angle < 0 || angle > 360) {
      return true;
    }
  }
  return false;
}

bool invalidValueforMove(stringstream & stream, int x_loc, int y_loc) {
  if(!stream.fail()) {
    if(x_loc < 0 || y_loc < 0) {
     return true;
    }
  }
  return false;
}

bool tooFewArguments(stringstream & stream) {
  if(stream.fail() && stream.eof()) {
    return true;
  }
  return false;
}

bool tooManyArguments(stringstream & stream) {
  if(!stream.fail() && !stream.eof()) { 
    return true;
  }
  return false;
}

bool shapeArrayFull() {
  if(shapeCount == max_shapes) {
    return true;
  }
  return false;
}
  
int main() {

  string line;
  string command;

  cout << "> ";         // Prompt for input
  getline(cin, line);    // Get a line from standard input

  bool first = true;

  while (!cin.eof ()) {
  // Put the line in a linestream for parsing
  // Making a new sstream for each line so flags etc. are cleared
  //cout << line << endl;
  stringstream lineStream (line);
  
  // Read from string stream into the command
  // The only way this can fail is if the eof is encountered
  lineStream >> command;

  bool flag = true;
  for(int i = 0; i < NUM_KEYWORDS; i++) {
    if(command == keyWordsList[i]) {
      flag = false; 
    }
  }
  if(flag) {
    cout << "Error: invalid command" << endl;
  }

  // Check for the command and act accordingly
  // ECE244 Student: Insert your code here
  string n, t;
  int x_loc = 0, x_sz = 0, y_loc = 0, y_sz = 0, angle = 0;

  //MaxShapes command
  if(command == keyWordsList[1]) {
    max_shapes = 0;
    lineStream >> max_shapes;
    if(invalidArgument(lineStream)) {
      cout << "Error: invalid argument" << endl;
    }
    else if(tooFewArguments(lineStream) && max_shapes == 0) {
      cout << "Error: too few arguments" << endl;
    }
    else if(tooManyArguments(lineStream)) {
      cout << "Error: too many arguments" << endl;
    }
    else {
      if(!first) {
        for(int i = 0; i < shapeCount; i++) {
          delete shapesArray[i];
          shapesArray[i] = nullptr;
        }
        delete [] shapesArray;
        shapesArray = nullptr;
        shapeCount = 0;
      }
      shapesArray = new Shape*[max_shapes];
      for(int i = 0; i < max_shapes; i++) {
        shapesArray[i] = nullptr;
      }
      first = false;
      cout << "New database: max shapes is " << max_shapes << endl;
    }
    
  }

  //Create command
  if(command == keyWordsList[2]) {
    lineStream >> n >> t >> x_loc >> y_loc >> x_sz >> y_sz;
    if(invalidArgument(lineStream)) {
      cout << "Error: invalid argument" << endl;
    }
    else if(invalidShapeName(n)) {
      cout << "Error: invalid shape name" << endl;
    }
    else if(shapeNameExists(n)) {
      cout << "Error: shape " << n << " exists" << endl;
    }
    //shape name not found error is missing, but this is create.
    else if(invalidShapeType(t) && t != "") {
      cout << "Error: invalid shape type" << endl;
    }
    else if(invalidValueforCreate(lineStream, t, x_loc, y_loc, x_sz, y_sz)) {
      cout << "Error: invalid value" << endl;
    }
    else if(tooFewArguments(lineStream) && y_sz == 0) {
      cout << "Error: too few arguments" << endl;
    }
    else if(tooManyArguments(lineStream)) {
      cout << "Error: too many arguments" << endl;
    }
    else if(shapeArrayFull()) {
      cout << "Error: shape array is full" << endl;
    }
    else {
      shapesArray[shapeCount] = new Shape(n, t, x_loc, x_sz, y_loc, y_sz);
      cout << "Created " << n << ": " << t << " " << x_loc << " " << y_loc << " " << x_sz << " " << y_sz << endl;
      shapeCount++; 
    }
  }

  //Move command
  if(command == keyWordsList[3]) {
    lineStream >> n >> x_loc >> y_loc;
    if(invalidArgument(lineStream)) {
      cout << "Error: invalid argument" << endl;
    }
    else if(invalidShapeName(n)) { //just added
      cout << "Error: invalid shape name" << endl;
    }
    else if(shapeNameNotFound(n) && n != "" && n != keyWordsList[0]) {
      cout << "Error: shape " << n << " not found" << endl;
    }
    //just added invalid shape type!!!!!!
    else if(invalidShapeType(t) && t != "") {
      cout << "Error: invalid shape type" << endl;
    }
    else if(invalidValueforMove(lineStream, x_loc, y_loc)) {
      cout << "Error: invalid value" << endl;
    }
    else if(tooFewArguments(lineStream) && y_loc == 0) {
      cout << "Error: too few arguments" << endl;
    }
    else if(tooManyArguments(lineStream)) {
      cout << "Error: too many arguments" << endl;
    }
    else {
      for(int i = 0; i < shapeCount; i++) {
        if(shapesArray[i] != nullptr) {
          if(n == shapesArray[i]->getName()) {
            shapesArray[i]->setXlocation(x_loc);
            shapesArray[i]->setYlocation(y_loc);
            cout << "Moved " << n << " to " << x_loc << " " << y_loc << endl;
          }
        }  
      }
    }
  }

  //Rotate command
  if(command == keyWordsList[4]) {
    lineStream >> n >> angle;
    if(invalidArgument(lineStream)) {
      cout << "Error: invalid argument" << endl;
    }
    else if(invalidShapeName(n)) {
      cout << "Error: invalid shape name" << endl;
    }
    //changed order.
    else if(shapeNameNotFound(n) && n != "" && n != keyWordsList[0]) {
      cout << "Error: shape " << n << " not found" << endl;
    }
    else if(invalidShapeType(t) && t != "") {
      cout << "Error: invalid shape type" << endl;
    }
    else if(invalidValueforRotate(lineStream, angle)) {
      cout << "Error: invalid value" << endl;
    }
    else if(tooFewArguments(lineStream) && angle == 0) {
      cout << "Error: too few arguments" << endl;
    }
    else if(tooManyArguments(lineStream)) {
      cout << "Error: too many arguments" << endl;
    }
    else {
      for(int i = 0; i < shapeCount; i++) {
        if(shapesArray[i] != nullptr) {
          if(n == shapesArray[i]->getName()) {
            shapesArray[i]->setRotate(angle);
            cout << "Rotated " << n << " by " << angle << " degrees" << endl;
          }
        }
      }
    }
  }

  //Draw command
  if(command == keyWordsList[5]) {
    lineStream >> n;
    if(invalidArgument(lineStream)) {
      cout << "Error: invalid argument" << endl;
    }
    else if(invalidShapeName(n)) {
      cout << "Error: invalid shape name" << endl;
    }
    else if(shapeNameNotFound(n) && n != "" && n != keyWordsList[0]) {
      cout << "Error: shape " << n << " not found" << endl;
    }
    else if(tooFewArguments(lineStream) && n == "") {
      cout << "Error: too few arguments" << endl;
    }
    else if(tooManyArguments(lineStream)) {
      cout << "Error: too many arguments" << endl;
    }
    else {
      if(n == keyWordsList[0]) {
        cout << "Drew all shapes" << endl;
        for(int i = 0; i < shapeCount; i++) {
          if(shapesArray[i] != nullptr) {
            shapesArray[i]->draw();
          }
        }
      }
      else {
        for(int i = 0; i < shapeCount; i++) {
          if(shapesArray[i] != nullptr) {
            if(n == shapesArray[i]->getName()) {
              cout << "Drew ";
              shapesArray[i]->draw();
            }
          }
        }
      }
    }
  }

  //Delete command
  if(command == keyWordsList[6]) {
    lineStream >> n;
    if(invalidArgument(lineStream)) {
      cout << "Error: invalid argument" << endl;
    }
    //just added invalid shapeName!!!!!!
    else if(invalidShapeName(n)) {
      cout << "Error: invalid shape name" << endl;
    }
    else if(shapeNameNotFound(n) && n != "" && n != keyWordsList[0]){
      cout << "Error: shape " << n << " not found" << endl;
    }
     else if(tooFewArguments(lineStream) && n == "") {
      cout << "Error: too few arguments" << endl;
    }
    else if(tooManyArguments(lineStream)) {
      cout << "Error: too many arguments" << endl;
    }
    else {
      if(n == keyWordsList[0]) {
        cout << "Deleted: all shapes" << endl;
        for(int i = 0; i < shapeCount; i++) {
          if(shapesArray[i] != nullptr) {
            delete shapesArray[i];
            shapesArray[i] = nullptr;
          }
        }
        shapeCount = 0;
      }

      else {
        for(int i = 0; i < shapeCount+1; i++) {
          if(shapesArray[i] != nullptr) {
            if(n == shapesArray[i]->getName()) {
              cout << "Deleted shape " << n << endl;
              delete shapesArray[i];
              shapesArray[i] = nullptr;
              shapeCount--;
              for(int j = i; j <= shapeCount; j++) {
                shapesArray[j] = shapesArray[j+1];
              }
              shapesArray[shapeCount+1] = nullptr;
            }
          }
        }
      }
    }
  }

  /*for(int i = 0; i < max_shapes; i++) {
    cout << shapesArray[i] << endl;
  }*/

  // Once the command has been processed, prompt for the
  // next command
  cout << "> ";          // Prompt for input
  getline(cin, line);
    
  }  // End input loop until EOF.
  for(int i = 0; i < shapeCount; i++) {
    if(shapesArray[i] != nullptr) {
      delete shapesArray[i];
      shapesArray[i] = nullptr;
    }
  }
  delete [] shapesArray;
  shapesArray = nullptr;

  return 0;
}

