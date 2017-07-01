//
//  biVector.h
//  Binary Vector
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#ifndef biVector_h
#define biVector_h

#include "biMatrix.h"

class biVector : public biMatrix {
    
    friend std::istream& operator>> (std::istream& IS, biVector& );
    friend std::ostream& operator<< ( std::ostream& , const biVector& );
    
    bool mirror;
    int lenght;
    
public:
    
    biVector ( const int = 1 );
    biVector ( const std::string& );
    biVector ( const bool **, const int );
    biVector ( const biMatrix& , const int , const char);
    biVector ( const biVector& );
    
    long getValue ( void ) const;
    int getLenght( void ) const;
    bool getElement ( int ) const;
    biVector& setElements (const biVector& );
    
    biVector operator+ ( const biVector& ) const;
    biVector operator* (const biMatrix& ) const;
    biVector operator* (const bool ) const;
    bool operator== ( const biVector& ) const;
    bool operator!= ( const biVector& ) const;
    bool& operator[] ( int );
    biVector& operator= ( const biVector& );
    biVector& operator+= ( const biVector& );
    biVector& operator*=( const biMatrix& );
    biVector& operator*= (const bool );
    
    std::string Output ( void ) const;
    
    ~biVector( void );
    
};

std::ostream& operator<< ( std::ostream& , const biVector& );
std::istream& operator>> (std::istream& IS, biVector& );

void Swap (biVector& , biVector& );

#endif