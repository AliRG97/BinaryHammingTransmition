//
//  biMatrix.h
//  Binary Matrixes
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#ifndef biMatrix_h
#define biMatrix_h

class biMatrix {
    
protected:
    
    bool *** values;
    int rows;
    int columns;
    
    friend std::ostream& operator<< ( std::ostream& , const biMatrix& );
    friend std::istream& operator>> ( std::istream& , biMatrix& );
    friend class biVector;
    
public:
    
    biMatrix ( int , int , bool*** = nullptr);
    biMatrix ( int , int, char** );
    biMatrix ( const biMatrix& );
    
    int getRows ( void ) const;
    int getColumns ( void ) const;
    bool getElement ( int , int ) const;
    
    biMatrix Transpose ( void ) const;
    
    bool operator== ( const biMatrix& ) const;
    bool operator!= ( const biMatrix& ) const;
    biMatrix operator* ( const biMatrix& ) const;
    biMatrix operator* ( const bool ) const;
    biMatrix operator+ ( const biMatrix& ) const;
    biMatrix& operator*= ( const biMatrix& );
    biMatrix& operator*= ( const bool );
    biMatrix& operator= ( const biMatrix& );
    biMatrix& operator+= ( const biMatrix& );
    
    ~biMatrix( void );
    
};

std::ostream& operator<< ( std::ostream& , const biMatrix& );
std::istream& operator>> ( std::istream& , biMatrix& );

#endif