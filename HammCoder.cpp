//
//  HammCoder.cpp
//  Binary Hamming Coder
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <cmath>
#include <cstring>
#include "biMatrix.h"
#include "biVector.h"
#include "HammCoder.h"

using namespace std;

// Encoding :

//Static Function :

bool Encoder::HammCheck(const biMatrix& T)  {
    biVector Zero (T.getRows());
    biVector *S = new biVector[T.getColumns()];
    for (int j = 0; j < T.getColumns(); j++) {
        S[j] = biVector(T, j, 'c');
        if (S[j] == Zero) {
            return false;
        }
        for (int k = 0; k < j; k++) {
            if (S[j] == S[k]) {
                return false;
            }
        }
    }
    delete [] S;
    return true;
}

//Constructor :

Encoder::Encoder (const int rank ,const biMatrix& M , bool ParBit) : r(rank) , ParCheckBit(ParBit) , n (pow(2, rank)-1) , ParCheck(M) , Generator(n - rank , n) {
    Generator = PrepGen(ParCheck);
}

//Modifying Functions :

biMatrix Encoder::ParCheckReduce(const biMatrix& T) {
    int length = T.getColumns();
    biMatrix Res = T;
    int k = T.getRows();
    for (int i = k-1; i >= 0; i--) {
        biVector S1 (Res, length-k+i, 'c');
        biVector R1(Res, i , 'r');
        if (not S1[i]) {
            int j;
            for (j = 0; j < length-k+i; j++) {
                biVector S2(Res,j,'c');
                if (S2[i]) {
                    break;
                }
            }
            biVector S2(Res,j,'c');
            Swap(S1, S2);
        }
        for (int j = 0; j < T.getRows(); j++) {
            if (j == i) {
                continue;
            }
            biVector R2 (Res,j,'r');
            if (R2[length-k+i]) {
                R2.setElements(R2+R1);
            }
        }
    }
    return Res;
}

biMatrix Encoder::PrepGen(const biMatrix& T) const {
    int k = n-r;
    biMatrix res = ParCheckReduce(T).Transpose();
    biMatrix Gen(k,n);
    for (int i = 0; i < k; i++) {
        biVector S (Gen, i, 'r');
        S[i] = true;
        for (int j = k; j < n; j++) {
            S[j] = res.getElement( i, j - k);
        }
    }
    return Gen;
}

Encoder& Encoder::SetMessage(const biVector& V) {
    Message = V;
    return *this;
}

biMatrix Encoder::getParCheck() const{
    return ParCheck;
}

Encoder& Encoder::Encode() {
    int k = n-r;
    int d = Message.getLenght() / k;
    string mStr = Message.Output();
    string cStr = "";
    string mPart;
    string cPart;
    for (int i = 0; i < d; i++) {
        bool Check = false;
        mPart = "";
        cPart = "";
        for (int j = 0; j < k; j++) {
            mPart += mStr[i*k + j];
        }
        mPart = (biVector(mPart) *= Generator).Output();
        cPart += mPart;
        for (int j = 0; j < n; j++) {
            Check ^= (cPart[j] - '0') % 2;
        }
        if (ParCheckBit) {
            cPart += (Check + '0');
        }
        cStr += cPart;
    }
    Code = biVector(cStr);
    return *this;
}

string Encoder::Output() {
    return Code.Output();
}

// Decoding :

// Constructor :

Decoder::Decoder (const int rank , const biMatrix& T, bool ParBit) : r(rank) , reSendNum(0) , n(pow(2, rank)-1),  ParCheck(T) , Message(1,n) {
    reSend = nullptr;
    ParCheck = Encoder::ParCheckReduce(ParCheck);
    ParCheckBit = ParBit;
}

Decoder& Decoder::setCode(const biVector& T) {
    Code = T;
    return *this;
}

bool Decoder::Decode() {
    reSendNum = 0;
    int d = Code.getLenght() / (ParCheckBit ?  n+1 : n);
    reSend = new bool[d];
    Message = biMatrix(d, n-r);
    biVector Error(n);
    string cStr = Code.Output();
    string cPart;
    string mStr;
    bool Check = false;
    int j = 0;
    for (int i = 0; i < d; i++) {
        cPart = "";
        mStr = "";
        for (int k = 0; k < n; k++) {
            cPart += cStr[j];
            j++;
        }
        if (ParCheckBit) {
            Check = (cStr[j] - '0') % 2;
            j++;
        }
        else {
            Check = true;
        }
        biVector CodePiece(cPart);
        Error = CodePiece*(ParCheck.Transpose());
        biVector msg (n-r);
        if (not DecodePart(msg, cPart,Error,Check) ) {
            reSend[i] = true;
            reSendNum++;
        }
        else {
            reSend[i] = false;
            mStr += msg.Output();
        }
        if (mStr.length()) {
            biVector S(Message, i, 'r');
            S.setElements(biVector(mStr));
        }
    }
    return (reSendNum);
}

bool Decoder::DecodePart(biVector& res, const biVector Code, const biVector Error , bool CheckBit) {
    bool check = false;
    if (ParCheckBit) {
        for (int j = 0; j < n; j++) {
            check ^= Code.getElement(j);
        }
        check ^= CheckBit;
    }
    else {
        check = true;
    }
    res = biVector(n-r);
    for (int j = 0; j < n-r; j++) {
        res[j] = Code.getElement(j);
    }
    if (not Error.getValue()) {
        return true;
    }
    if (check) {
        int i = 0;
        for (; i < n-r; i++) {
            biVector S(ParCheck , i , 'c');
            if (S == Error) {
                break;
            }
        }
        if (i < n-r) {
            res[i] ^= 1;
        }
        return true;
    }
    return false;
}

// Output :

string Decoder::Output() const {
    string s = "";
    for (int j = 0; j < Message.getRows(); j++) {
        if (j > 0) {
            s+= "/";
        }
        if (reSend[j]) {
            s += "-";
        }
        else {
            biVector S(Message, j, 'r');
            s += S.Output();
        }
    }
    return s;
}

bool* Decoder::resend() const {
    return reSend;
}
