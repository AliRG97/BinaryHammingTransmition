//
//  biVector.cpp
//  Binary Vector
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <cstring>
#include "biMatrix.h"
#include "biVector.h"

using namespace std;

//Constructors

biVector::biVector (const int n) : biMatrix(1,n) , mirror(false) , lenght(n) {}

biVector::biVector ( const string& Str ) : biMatrix(1,(int)Str.length()) , mirror(false) {
    lenght = columns;
    for (int i = 0; i < columns; i++) {
        *(values[0][i]) = (Str[i] - '0') % 2;
    }
}

biVector::biVector ( const bool ** V , const int n) : biMatrix(1, n) , mirror(false) {
    lenght = columns;
    for (int j = 0; j < columns; j++) {
        *(values[0][j]) = *(V[j]);
    }
}

biVector::biVector ( const biVector& T ) : biMatrix(1, T.columns) , mirror(false) {
    lenght = columns;
    for (int j = 0; j < columns; j++) {
        *(values[0][j]) = *(T.values[0][j]);
    }
}

biVector::biVector ( const biMatrix& T , const int n , const char c) : biMatrix(1, c == 'r' ? T.getColumns() : T.getRows()) , mirror(true) {
    lenght = columns;
    if (c == 'r') {
        for (int j = 0; j < columns; j++) {
            values[0][j] = (T.values[n][j]);
        }
    }
    else {
        for (int j = 0; j < columns; j++) {
            values[0][j] = (T.values[j][n]);
        }
    }
}

//Modifying Functions :

long biVector::getValue() const {
    long sum = 0;
    for (int j = 0; j < columns; j++) {
        sum += (pow(2, columns-j) * (*(values[0][j])));
    }
    return sum;
}

int biVector::getLenght() const {
    return lenght;
}

bool biVector::getElement( int i) const {
    return *values[0][i];
}

biVector& biVector::setElements(const biVector& T) {
    int min = lenght <= T.getLenght() ? lenght : getLenght();
    for (int i = 0; i < min; i++) {
        *(values[0][i]) = T.getElement(i);
    }
    return *this;
}

//Operators :

bool biVector::operator==(const biVector& T) const {
    return (biMatrix::operator==(T));
}

bool biVector::operator!=(const biVector& T) const {
    return !operator==(T);
}

biVector biVector::operator*(const biMatrix& T) const {
    biMatrix S (1, lenght , values);
    biMatrix B (S * T);
    biVector X(B , 0 , 'r');
    biVector M = X;
    return M;
}

biVector biVector::operator* (const bool B) const {
    biMatrix S = biMatrix::operator*(B);
    biVector X (S , 0, 'r');
    biVector M = X;
    return M;
}

biVector& biVector::operator*=(const biMatrix& T) {
    return operator=(operator*(T));
}

biVector& biVector::operator*= (const bool B) {
    return operator=(operator*(B));
}

bool& biVector::operator[] ( const int n ) {
    return *values[0][n];
}

biVector& biVector::operator=(const biVector& T) {
    this->~biVector();
    lenght = T.getLenght();
    rows = 1;
    columns = T.columns;
    values = new bool **;
    values[0] = new bool*[columns];
    for (int j = 0; j < columns; j++) {
        values[0][j] = new bool(*(T.values[0][j]));
    }
    return *this;
}

biVector biVector::operator+ ( const biVector& T) const{
    int min = lenght;
    min = min >= T.getLenght() ? T.getLenght() : min;
    biVector Sum (min);
    for (int i = 0; i < min; i++) {
        Sum[i] = getElement(i) ^ T.getElement(i);
    }
    return Sum;
}

biVector& biVector::operator+= ( const biVector& T) {
    return operator=(operator+(T));
}

//I/O Operators :

string biVector::Output() const {
    string s = "";
    for (int i = 0; i < lenght; i++) {
        s += (*(values[0][i])) ? '1': '0';
    }
    return s;
}

ostream& operator<< ( ostream& OS, const biVector& V)  {
    for (int j = 0; j < V.getLenght(); j++) {
        OS << V.getElement(j);
    }
    return OS;
}

istream& operator>> ( istream& IS , biVector& V) {
    string s;
    getline(IS, s);
    V = biVector(s);
    return IS;
}

//Destructor

biVector::~biVector() {
    if (mirror) {
        for (int j = 0; j < columns; j++) {
            values[0][j] = new bool(false);
        }
    }
}

//Other Functions:

void Swap (biVector& A , biVector& B) {
    biVector Temp = biVector(A);
    A.setElements(B);
    B.setElements(Temp);
}


