//
//  main.cpp
//  BinaryHammingTransmition
//
//  Created by Ali RG on 6/16/17.
//  Copyright (c) 2017 Ali RG. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <cstring>
#include <random>
#include "biVector.h"
#include "biMatrix.h"
#include "HammCoder.h"

using namespace std;


biVector RandErr (int n , double p) {
    biVector Error(n);
    bernoulli_distribution berndist (p);
    mt19937 generator(time(0) % 10000);
    for (int i = 0; i < n; i++) {
        Error[i] = berndist(generator);
    }
    return Error;
}

biMatrix ParCheckGen (int r) {
    biMatrix M (r , pow(2, r) - 1);
    biVector A (M , 0 , 'c');
    A[r-1]= 1;
    for (int j = 1; j < M.getColumns(); j++) {
        biVector S ( M, j , 'c');
        int temp = 1;
        for (int i = r - 1; i >= 0; i--) {
            if (!temp) {
                S[i] = M.getElement(i, j-1);
            }
            else {
                if (M.getElement(i, j-1)) {
                    S[i] = 0;
                }
                else {
                    temp = 0;
                    S[i] = 1;
                }
            }
        }
    }
    return M;
}

int main() {
    int r;
    int n;
    bool parcheck = false;      //Parity-Check Bit
    bool def = true;            //Default Parity-Check Matrix
    bool run;
    while (run) {
        cout << " Welcome to Binary Hamming Code Transmition Simulator :\n Please Enter Your Intended Rank : \n ";
        cin >> r;
        while (r <= 1) {
            cout << " Invalid rank!\n ";
            cin >> r;
        }
        n = pow(2, r) - 1;
        biMatrix M(r, n);
        biMatrix defM = ParCheckGen(r);
        M = defM;
        bool ready = false;
        while (!ready) {
            cout << " 1)Start Process\t2)Options\n ";
            char c;
            cin >> c;
            switch (c) {
                case '2':
                    cout << " 1)Parity-Check Bit : " << (parcheck ? "On" : "Off") << " --> " << (!parcheck ? "On" : "Off") << "\n 2)Hamming Code : " << ( def ? "Default" : "Manual") << " --> " << (def ? "Manual" : "Default") << "\n ";
                    cin >> c;
                    switch (c) {
                        case '1':
                            parcheck = !parcheck;
                            cout << " Done!\n";
                            break;
                        case '2':
                            def = !def;
                            if (def) {
                                M = defM;
                                break;
                            }
                            cout << " Please Enter a [" << r << "x" << n << "] Matrix \n";
                            cin >> M;
                            if (Encoder::HammCheck(M)) {
                                cout << " Done!\n";
                            }
                            else
                            {
                                cout << " Matrix is invalid!\n";
                                def = !def;
                            }
                            break;
                        default:
                            cout << " Bad Input!\n";
                            break;
                    }
                    break;
                case '1':
                    ready = true;
                    break;
                default:
                    cout << " Bad Input!\n";
                    break;
            }
        }
        bool send = true;
        while (send) {
            cout << " Please Enter The Message : \n ";
            biVector Message;
            getc(stdin);
            cin >> Message;
            Encoder enc (r , M, parcheck); //Initializing Encoder
            cout << " Encoding Message... \n";
            string Code = enc.SetMessage(Message).Encode().Output(); //Encoding Massage
            cout << " Encoded Message :\n " << Code << "\n ";
            cout << "Please Enter Symbol Error Probability of The Channel : \n ";
            double p;
            cin >> p;
            cout << " Transmitting Data ....\n ";
            biVector recVector (Code);
            cout << "Error :\n ";
            biVector Err = RandErr((int)Code.length(), p);
            cout << Err.Output() << "\n ";
            recVector += Err;
            string recieved = recVector.Output();   //Applying Error
            cout << "Recieved Data :\n " << recieved << "\n ";
            Decoder dec (r ,M ,parcheck );
            dec.setCode(biVector(recieved));
            bool resend = dec.Decode();
            cout << "Decoded Message is :\n " << dec.Output() << "\n";
            if (resend) {
                cout << " The Following Pieces Needs to be Resent : ";
                for (int i = 0; i < Message.getLenght() / (n-r); i++) {
                    if (dec.resend()[i]) {
                        cout << i+1 << " ";
                    }
                }
                cout << "\n ";
            }
            bool pass = false;
            while (!pass) {
                cout << "\n 1)Next Message\t2)Main Menu\t0)Exit\n ";
                char c;
                cin >> c;
                switch (c) {
                    case '1':
                        pass = true;
                        send = true;
                        break;
                    case '2':
                        pass = true;
                        send = false;
                        run = true;
                        break;
                    case '0':
                        send = run = false;
                        pass = true;
                        break;
                    default:
                        cout << " Bad Input!\n ";
                        break;
                }
            }
        }
    }
    return 0;
}
