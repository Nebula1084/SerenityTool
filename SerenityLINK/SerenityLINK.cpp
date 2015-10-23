#include "fstream"
#include "iostream"
#include "queue"
#include "cstdlib"
using namespace std;
int getOrigin(ifstream& fin){
    if (fin.is_open()){
        char buf[4];
        int origin = 0;
        fin.seekg(0);
        fin.read(buf,4);
        if(fin.gcount() != 4){
            cout << "No head!!!" << endl;
            exit(1);
        }
        for (int i = 0; i < sizeof(buf)/sizeof(*buf); ++i){
            origin |= (unsigned int)(unsigned char)buf[i] << (3-i)*8;
        }
        // cout << fileName << ":" << origin << endl;
        return origin;
    }
    else{
        cout << "Not opened file!!!" << endl;
        exit(1);
    }
}
int getOrigin(string& fileName){
    ifstream fin(fileName.c_str(), ios_base::in|ios_base::binary);
    if (!fin.is_open()){
        cout << "Cann't open file \"" << fileName << "\"" << endl;
        exit(1);
    }
    return getOrigin(fin);
}

class Comparor
{
public:
    bool operator() (string& lhs, string& rhs) const{
        return getOrigin(lhs) > getOrigin(rhs);
    }
};

void append(ofstream& fout, string fileName){
    ifstream fin(fileName.c_str(), ios_base::in|ios_base::binary);
    if (!fin.is_open()){
        cout << "Cann't open file \"" << fileName << "\"" << endl;
        exit(1);
    }
    static int foutOrigin;
    if (fout.tellp() == 0){
        char tmp;
        fin.read(&tmp, 1);
        while(!fin.eof()){
            fout.write(&tmp, 1);
            fin.read(&tmp, 1);
        }
        foutOrigin = getOrigin(fileName);
    }
    else{
        int origin = getOrigin(fileName);
        // cout << "fout.tellp()=" << fout.tellp() << endl;
        // cout << fileName << ":" << origin << endl;
        if (foutOrigin*2 + fout.tellp() > origin*2){
            cout << "Overlap occurs!!!" << endl;
            exit(1);
        }
        else{
            char tmp = 0;
            while(foutOrigin*2 + fout.tellp() < origin*2){
                fout.write(&tmp, 1);
            }
            fin.seekg(4);
            fin.read(&tmp, 1);
            while(!fin.eof()){
                fout.write(&tmp, 1);
                fin.read(&tmp, 1);
            }
        }
    }
}
int main(int argc, char const *argv[]){
    ofstream fout;
    priority_queue<string, vector<string>, Comparor> fins;
    for (int i = 1; i < argc; ++i){
        if (string(argv[i]) == string("-o")){
            ++i;
            if(i < argc){
                fout.open(argv[i], ios_base::out|ios_base::binary);
                if (!fout.is_open()){
                    cout << "Cann't open file \"" << argv[i] << "\"" << endl;
                    return 1;
                }
            }
            else{
                cout << "No output file!!!" << endl;
                return 1;
            }
        }
        else{
            fins.push(string(argv[i]));
        }
    }
    if (!fout.is_open()){
        cout << "No output file!!!" << endl;
    }

    while(fins.size()){
        // cout << fins.top() << endl;
        append(fout, fins.top());
        fins.pop();
    }
    return 0;
}