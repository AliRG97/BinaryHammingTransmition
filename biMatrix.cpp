//
//  biMatrix.cpp
//  Binary Matrix
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include "biMatrix.h"

using namespace std;

//Constructors :

biMatrix::biMatrix ( int R , int C , bool *** V) {
    rows = R;
    columns = C;
    values = new bool**[rows];
    for (int i = 0; i < rows; i++) {
        values[i] = new bool*[columns];
        for (int j = 0; j < columns; j++) {
            values[i][j] = new bool(V ? *V[i][j]:false);
        }
    }
}

biMatrix::biMatrix ( int R , int C, char** Str ) {
    rows = R;
    columns = C;
    values = new bool**[rows];
    for (int i = 0; i < rows; i++) {
        values[i] = new bool*[columns];
        for (int j = 0; j < columns; j++) {
            values[i][j] = new bool((Str[i][j]-'0') % 2);
        }
    }
}

biMatrix::biMatrix ( const biMatrix& T ) {
    rows = T.rows;
    columns = T.columns;
    values = new bool**[rows];
    for (int i = 0; i < rows; i++) {
        values[i] = new bool*[columns];
        for (int j = 0; j < columns; j++) {
            values[i][j] = new bool(*(T.values[i][j]));
        }
    }
}

//Get Functions :

int biMatrix::getRows() const {
    return rows;
}

int biMatrix::getColumns() const {
    return columns;
}

bool biMatrix::getElement(int i, int j) const {
    return *values[i][j];
}

//Manipulative Functions :

biMatrix biMatrix::Transpose() const {
    biMatrix Tr (columns,rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            *(Tr.values[j][i]) = *(values[i][j]);
        }
    }
    return Tr;
}

//Operators :

biMatrix& biMatrix::operator=(const biMatrix& T) {
    if (&T == this) {
        return *this;
    }
    this->~biMatrix();
    rows = T.rows;
    columns = T.columns;
    values = new bool**[rows];
    for (int i = 0; i < rows; i++) {
        values[i] = new bool*[columns];
        for (int j = 0; j < columns; j++) {
            values[i][j] = new bool(*(T.values[i][j]));
        }
    }
    return *this;
}

bool biMatrix::operator== (const biMatrix& T) const {
    if (rows != T.rows || columns != T.columns) {
        return false;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (*(values[i][j]) != *(T.values[i][j])) {
                return false;
            }
        }
    }
    return true;
}

bool biMatrix::operator!= ( const biMatrix& T ) const {
    return not operator== (T);
}

biMatrix biMatrix::operator+(const biMatrix& T) const {
    biMatrix Temp (*this);
    for (int i = 0; i < rows; i++) {
        for (int j= 0; j < columns; j++) {
            *(Temp.values[i][j]) ^= *(T.values[i][j]);
        }
    }
    return Temp;
}

biMatrix biMatrix::operator*(const biMatrix& T) const {
    biMatrix M (rows,T.columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < T.columns; j++) {
            bool sum = false;
            for (int k = 0; k < columns; k++) {
                sum ^= (*(values[i][k]) & *(T.values[k][j]));
            }
            *(M.values[i][j]) = sum;
        }
    }
    return M;
}

biMatrix biMatrix::operator*(const bool B) const {
    biMatrix M(rows,columns);
    if (B) {
         M = (*this);
    }
    return M;
}

biMatrix& biMatrix::operator+=(const biMatrix& T) {
    return operator=(operator+(T));
}

biMatrix& biMatrix::operator*=(const biMatrix& T) {
    return operator=(operator*(T));
}

biMatrix& biMatrix::operator*=(const bool B) {
    return operator=(operator*(B));
}

//I/O Operators :

ostream& operator<< (ostream& OS, const biMatrix& T) {
    for (int i = 0; i < T.rows; i++) {
        for (int j = 0; j < T.columns; j++) {
            OS << (*(T.values[i][j]) ? '1' : '0');
        }
        OS << "\n";
    }
    return OS;
}

istream& operator>> (istream& IS, biMatrix& T) {
    char c;
    for (int i = 0; i < T.rows; i++) {
        for (int j = 0; j < T.columns; j++) {
            IS >> c;
            *(T.values[i][j]) = ( c - '0' ) % 2;
        }
    }
    return IS;
}

//Destructor :

biMatrix::~biMatrix () {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            delete values[i][j];
        }
        delete [] values[i];
    }
    delete [] values;
    rows = 0;
    columns = 0;
}




