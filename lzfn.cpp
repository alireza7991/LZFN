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
        cout << root->data << ": " << str << "\n";
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
    for(auto c : input) {
        out += hcodes[c];
    }
    return out;
}




void alirezaLZ() {
        string input = "abcde_bcdefgh_abcdefgh";
        const char *data = input.c_str();
        int ss = input.size();
        struct match {
            int distance,len,score;
        };
        vector<match> ms(input.size());
        for(int i = 0 ; i < input.size(); i++) {
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
                cout << "i = " << i << " found match dis : " << bestDis << " len : " << bestLen << " Score : " <<  bestScore << endl;
            } else {
                cout << "i = " << i << " NO MATCH" << endl;
            }
        }
        for(int i = ss-1 ; i >= 0;i--) {
            bool betterMatch = true;
            if(ms[i].distance != -1) {
                for(int j = i+1 ; j < (i+ms[i].len); j++) {
                    if(ms[i].score < ms[j].score) betterMatch=false;
                }
                if(betterMatch) {
                    cout << " i :" << i << " is better match" << endl;
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
            cout << "i = " << i << " c : " << data[i] << " score : " << ms[i].score << " dis : " << ms[i].distance << " len : " << ms[i].len << endl;
        }
        char *output = new char[(int)(ss*1.4)];
        int k = 0;
        for(int i = 0 ; i < ss; i++) {
            if(ms[i].distance > 0) {
                // a match
                int mlen = ms[i].len, mdis = ms[i].distance;
                output[k] = -1 * (mlen%128);
                if(mlen >= 128) {
                    mlen -= 127;
                    while(mlen > 0) {
                        char 
                    }
                }

            } else {
                //  a literal

            }
        }

}

int main() {
    int freq[256];
    alirezaLZ();
    return 0;
    string input;
    cin >> input;
    buildFreqTable(freq,input);
    generateHCodes(freq);

    string out = huffmanEncode(input);
    cout << "Encoded: " << out << endl;
    cout << "Len: " << out.length() << endl;
    cout << "Original Len " << input.length()*8;
}