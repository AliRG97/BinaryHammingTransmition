//
//  HammCoder.h
//  Binary Hamming Coder
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#ifndef HammCoder_h
#define HammCoder_h

#include "biMatrix.h"
#include "biVector.h"
#include <iostream>
#include <cstring>

class Encoder {
    
    int r;
    int n;
    
    biMatrix Generator;
    biMatrix ParCheck;
    bool ParCheckBit;
    
    biVector Message;
    biVector Code;
    
public:
    
    static bool HammCheck ( const biMatrix& );
    static biMatrix ParCheckReduce ( const biMatrix& );
    
    Encoder(const int ,const biMatrix& , bool = false);
    
    Encoder& SetMessage (const biVector&);
    Encoder& Encode ( void );
    biMatrix PrepGen ( const biMatrix& ) const;
    biMatrix getParCheck ( void ) const;
    
    std::string Output ( void );

};

class Decoder {
    
    int r;
    int n;
    biMatrix ParCheck;
    bool ParCheckBit;
    biVector Code;
    biMatrix Message;
    bool* reSend;
    int reSendNum;
    
    bool DecodePart ( biVector& , const biVector , const biVector , bool = false);
    
public:
    
    Decoder (const int , const biMatrix& , bool = false );
    
    Decoder& setCode ( const biVector& );
    bool Decode ( void );
    
    std::string Output ( void ) const;
    bool* resend ( void ) const;
    
};

void Swap (biVector& , biVector& );

#endif