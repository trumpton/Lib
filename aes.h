//
// AES
//
//
//
//

class AES {

private:
    int Nr ;  // Number of ROunds
    int Nk ;  // Number of 32-bit words in the key
    unsigned char RoundKey[240];  // Round Keys
    unsigned char state[4][4];    // Intermediate State

private:
    int getSBoxValue(int num) ;
    int getSBoxInvert(int num) ;
    int getRConValue(int num) ;
    void KeyExpansion(unsigned char *Key) ;
    void AddRoundKey(int round)  ;
    void SubBytes() ;
    void ShiftRows() ;
    void MixColumns() ;
    void InvSubBytes() ;
    void InvShiftRows() ;
    void InvMixColumns() ;





public:
    AES(int keylen) ;
    ~AES() ;

    void encrypt(unsigned char *key, unsigned char *in, unsigned char *out) ;
    void decrypt(unsigned char *key, unsigned char *in, unsigned char *out) ;

} ;
