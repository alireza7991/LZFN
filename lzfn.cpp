#include <bits/stdc++.h>
using namespace std;

string hcodes[256];

struct Node {
    unsigned char data;
    unsigned freq;
    Node *left, *right;
    Node(unsigned char data, unsigned freq) {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

struct compare { bool operator()(Node* l, Node* r) { return (l->freq > r->freq); } };

void buildHCodes(struct Node* root, string str) {
    if (!root)
        return;
    if (root->data != '$') {
       // cout << root->data << ": " << str << "\n";
        hcodes[root->data]=str;
    }
    buildHCodes(root->left, str + "0");
    buildHCodes(root->right, str + "1");
}

void generateHCodes(int freq[]) {
    struct Node *left, *right, *top;
    priority_queue<Node*, vector<Node*>, compare> q;
    for (unsigned char i = 0; i < 255; i++)
        q.push(new Node(i, freq[i]));
    while (q.size() != 1) {
        left = q.top();
        q.pop();
        right = q.top();
        q.pop();
        top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        q.push(top);
    }
    buildHCodes(q.top(), "");

}

void buildFreqTable(int *freqTable,string input) {
    for(int i = 0 ; i < 256; i++) freqTable[i] = 0;
    for(unsigned char a : input) {
        freqTable[a]++;
    }
}

string huffmanEncode(string input) {
    string out;
    for(int i = 0 ; i < input.size(); i++) {
        out += hcodes[(unsigned char)input[i]];
    }
    return out;
}




char *alirezaLZ(const char *data, int ss,int &ksize) {
        //string input = "abcde_bcdefgh_abcdefgh";
       // const char *data = input.c_str();
        //int ss = input.size();
        struct match {
            int distance,len,score;
        };
        vector<match> ms(ss);
        for(int i = 0 ; i < ss; i++) {
            int k = 0;
            int matchLen = 0;
            vector<match> matches;
            while(k < i) {
                if(data[i+matchLen] == data[k]) matchLen++; else if(matchLen >= 4) {
                    match m;
                    m.distance = i-(k-matchLen);
                    m.len = matchLen;
                    m.score = matchLen - (m.len/256 + m.distance/256);
                    matchLen = 0;
                    matches.push_back(m);
                }
                k++;
            }
            // select best match
            int bestDis=-1,bestLen=-1,bestScore = -1;
            match best;
            if(matches.size()) {
                std::sort(matches.begin(),matches.end(),[&](match a,match b) { return a.score > b.score; });
                bestDis = matches[0].distance;
                bestLen = matches[0].len;
                bestScore = matches[0].score;
                best = matches[0];

            } else {
                best.score=-1;
                best.len=-1;
                best.distance=-1;
            }
            ms[i] = best;
            if(bestDis != -1) {
               // cout << "i = " << i << " found match dis : " << bestDis << " len : " << bestLen << " Score : " <<  bestScore << endl;
            } else {
               // cout << "i = " << i << " NO MATCH" << endl;
            }
        }
        for(int i = ss-1 ; i >= 0;i--) {
            bool betterMatch = true;
            if(ms[i].distance != -1) {
                for(int j = i+1 ; j < (i+ms[i].len); j++) {
                    if(ms[i].score < ms[j].score) betterMatch=false;
                }
                if(betterMatch) {
                    //cout << " i :" << i << " is better match" << endl;
                    for(int j = i+1; j < (i+ms[i].len); j++){
                        ms[j].score = ms[i].score;
                        ms[j].len = -2;
                        ms[j].distance = -2;
                    }
                }
                else {
                    ms[i].score=-1;
                    ms[i].distance=-1;
                    ms[i].len=-1;
                }
            }
        }
        for(int i = 0 ; i < ss; i++) {
           // cout << "i = " << i << " c : " << data[i] << " score : " << ms[i].score << " dis : " << ms[i].distance << " len : " << ms[i].len << endl;
        }
        char *output = new char[(int)(ss*1.4)];
        int k = 0;
        for(int i = 0 ; i < ss;) {
           // cout << "i = " << i << " k = " << k << endl;
            if(i==10){
                int yyyyy =9;
            }
            if (ms[i].distance > 0) {

              //  cout << "i = " << i << " match " << endl;
                // a match
                int mlen = ms[i].len, mdis = ms[i].distance;
                // encode len
                output[k] = -1 * (mlen % 128);
                k++;
                if (mlen >= 128) {
                    mlen -= 127;

                    while (mlen > 0) {
                        output[k] = mlen % 256;
                        mlen -= 256;
                        k++;
                    }
                }
                // encode offset
                while (mdis > 0) {
                    output[k] = mdis % 256;
                    mdis -= 256;
                    k++;
                }
                i+=mlen;
            } else {
                //cout << "i = " << i << " literal " << endl;
                //  a literal
                int litLen = 0;
                for (int j = i; ms[j].distance < 0; j++) {
                    litLen++;
                }
                // encode len
                output[k] = -1 * (litLen % 128);
                k++;
                if (litLen >= 128) {
                    litLen -= 127;
                    while (litLen > 0) {
                        output[k] = litLen % 256;
                        litLen -= 256;
                        k++;
                    }
                }

                for (int j = 0; j < litLen; j++) {
                    output[k] = data[i];
                    i++;
                    k++;
                }
            }
        }
        cout << "Step 1 => Base size : " << ss << " - Compressed : " << k << endl;
    ksize = k;
    return output;
}

int main() {
    int freq[256];
    ///string in;
    //getline(cin,in);
    std::ifstream ifs("test.txt");
    std::string in( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    int ks;
    char *s = alirezaLZ(in.c_str(),in.length(),ks);
    //return 0;
    //string input;
   // cin >> input;
    string hh(s,ks);
    buildFreqTable(freq,hh);
    generateHCodes(freq);
    string out = huffmanEncode(hh);
   // cout << "Encoded: " << out << endl;

    cout << "Step 2 => Base size :  " << hh.length();
    cout << " - Compressed : " << out.length()/8 << endl;
}
